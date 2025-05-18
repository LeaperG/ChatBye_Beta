#include "chat_page.h"
#include "helpers/json/json_message_builder.h"
#include "client/ui/helpers/message_helper.h"
#include "client/ui/helpers/shadow_helper.h"
#include "client/pages/image_settings/image_settings_manager.h"
#include "client/pages/settings_page/settings_page.h"

#include "QListWidget"
#include "client/ui/styles/chat_page_style.h"

ChatPage::ChatPage(QWidget *parent)
    : PageBase{parent}, messageManager(new MessageManager(this)), m_lastSender("Вы"), settings("ChatBye", "ColorSettings")
{
    QHBoxLayout *mainHLayout = new QHBoxLayout(this);


    chatLayout = new QVBoxLayout();
    m_chatList = new ChatListWidget("ChatPage", this);
    chatLayout->addWidget(m_chatList);
    inputLayout = new QHBoxLayout();

    int sizeWidget = 47;
    m_toggleUsersButton = new QPushButton;
    m_toggleUsersButton->setIcon(QIcon(":/icons/iconSidebar9.png"));
    m_toggleUsersButton->setIconSize(QSize(30, 30));
    m_toggleUsersButton->setFixedSize(sizeWidget, sizeWidget);
    inputLayout->addWidget(m_toggleUsersButton);

    m_messageInput = new QLineEdit;
    m_messageInput->setPlaceholderText("Введите сообщение...");
    m_messageInput->setMinimumSize(130,sizeWidget);
    inputLayout->addWidget(m_messageInput);

    m_sendButton = new QToolButton();
    m_sendButton->setIcon(QIcon(":/icons/iconSend3.png"));
    m_sendButton->setStyleSheet("QToolButton { background-color: transparent; padding: 0px; margin: 0px; }");
    m_sendButton->setFixedSize(sizeWidget, sizeWidget);
    m_sendButton->setIconSize(QSize(35, 35));


    QHBoxLayout *lineEditLayout = new QHBoxLayout(m_messageInput);
    lineEditLayout->setContentsMargins(0, 0, 0, 0);
    lineEditLayout->addStretch();
    lineEditLayout->addWidget(m_sendButton);
    m_messageInput->setLayout(lineEditLayout);


    // Use the universal method
    setupInputAction(m_messageInput, m_sendButton, this, &ChatPage::checkMessage);


    // left side - list of users
    usersLayout = new QVBoxLayout();
    m_usersList = new QListWidget;
    ShadowHelper::applyShadow(m_usersList, QColor(0, 0, 0, 160), 10, QPointF(2,2));
    m_usersList->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    m_usersList->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    m_usersList->setMaximumWidth(130);

    m_usersList->setWordWrap(true);
    usersLayout->addWidget(m_usersList);

    m_changeNameButton = new QPushButton("Изменить имя");
    m_changeNameButton->setEnabled(true);
    m_changeNameButton->setMinimumHeight(sizeWidget);
    m_changeNameButton->setMaximumWidth(130);
    usersLayout->addWidget(m_changeNameButton);


    mainHLayout->addLayout(usersLayout);

    QVBoxLayout *vLayaout = new QVBoxLayout;
    vLayaout->addLayout(chatLayout);
    vLayaout->addLayout(inputLayout);

    mainHLayout->addLayout(vLayaout);


    m_changeNameButton->setContentsMargins(0, 0, 0, 0);
    m_toggleUsersButton->setContentsMargins(0, 0, 0, 0);
    m_messageInput->setContentsMargins(0, 0, 0, 0);

    updateStyleSheet();

    connect(SettingsPage::instance(), &SettingsPage::colorChanged, this, &ChatPage::updateStyleSheet);
    connect(ImageSettingsManager::instance(), &ImageSettingsManager::imageUpdated, this, &ChatPage::updateImage);

    QTimer::singleShot(0, m_messageInput, SLOT(setFocus()));
    initConnect();
}

void ChatPage::toggleUsersList()
{
    bool isVisible = m_usersList->isVisible();
    m_usersList->setVisible(!isVisible);

    m_changeNameButton->setVisible(!isVisible);
    m_toggleUsersButton->focusWidget()->clearFocus();
}

void ChatPage::checkMessage()
{
    clearFocus();
    QString message = m_messageInput->text();

    // Output the length of the string to check
    //qDebug() << "Text length before cropping:" << message.length();


    // If the text length is more than 30,000 characters, cut it off
    if (message.length() > 30000) {
        message = message.left(30000);
        MessageHelper::showCustomMessageBox(this, "Ошибка", "Сообщение не может быть длиннее 30 000 символов. Мы обрезали ваш текст до максимального разрешённого размера.", QMessageBox::Warning);
    }

    if (!message.isEmpty()) {

        QStringList parts = splitMessage(message, 4096);

        for (const QString &part : parts)
        {
            // Создаём JSON-сообщение
            QByteArray jsonData = JsonMessageBuilder::createMessage(MessageSource::Client, m_clientName, message);
            emit sendMessage(jsonData);

            QDateTime dateTime = QDateTime::currentDateTime();
            messageManager->addMessageToChat(MessageSource::Client, "Вы", m_clientName, message, Qt::AlignRight, dateTime, m_lastSender);
            m_messageInput->clear();
        }
    } else {
        MessageHelper::showCustomMessageBox(this, "Ошибка", "Сообщение не может быть пустым", QMessageBox::Warning);
    }
    m_lastSender = m_clientName;  // Save the name of the last sender
}

void ChatPage::receiveMessageFromServer(MessageSource source, const QString &name, const QString &message, const QDateTime &dateTime)
{
    if (MessageSource::Server == source)
    {
        messageManager->addMessageToChat(source, m_clientName, name, message, Qt::AlignLeft, dateTime, m_lastSender);
    }
    else
    {
        messageManager->addMessageToChat(source, m_clientName, name, message, Qt::AlignLeft, dateTime, m_lastSender);
        m_lastSender = name;  // Save the name of the last sender
    }
}

void ChatPage::updateClientList(const QStringList &names)
{
    m_usersList->clear();
    for (const QString& name : names)
    {
        m_usersList->addItem(name);
    }
}

void ChatPage::getClientName(const QString &name)
{
    m_clientName = name;
}

void ChatPage::updateImage(const QString &pageName)
{
    m_chatList->updateBackground();
}

void ChatPage::sendName()
{
    emit changeName();
}

void ChatPage::initConnect()
{
    connect(m_changeNameButton, &QPushButton::clicked, this, &ChatPage::sendName);
    connect(messageManager, &MessageManager::messageAdded, this, &ChatPage::addItemToChatList);
    connect(m_toggleUsersButton, &QPushButton::clicked, this, &ChatPage::toggleUsersList);
}

QStringList ChatPage::splitMessage(const QString &message, int maxLength)
{
    QStringList result;
    QStringList sentences = message.split(QRegularExpression("(?<=[.!?])\\s+"), Qt::SkipEmptyParts);

    QString currentPart;
    for (const QString &sentence : sentences) {
        if (currentPart.length() + sentence.length() <= maxLength) {
            currentPart += (currentPart.isEmpty() ? "" : " ") + sentence;
        } else {
            if (!currentPart.isEmpty()) {
                result.append(currentPart);
                currentPart.clear();
            }

            if (sentence.length() > maxLength) {
                // Break a long sentence by force
                int pos = 0;
                while (pos < sentence.length()) {
                    result.append(sentence.mid(pos, maxLength));
                    pos += maxLength;
                }
            } else {
                currentPart = sentence;
            }
        }
    }

    if (!currentPart.isEmpty()) {
        result.append(currentPart);
    }

    return result;
}

void ChatPage::addItemToChatList(QWidget *mainWidget)
{
    QListWidgetItem *item = new QListWidgetItem(m_chatList->listWidget);
    mainWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    mainWidget->adjustSize();

    item->setSizeHint(mainWidget->sizeHint());
    m_chatList->listWidget->setItemWidget(item, mainWidget);
    m_chatList->listWidget->scrollToBottom();
}

void ChatPage::updateStyleSheet()
{
    ChatPageStyle style;
    style.setStyle(this);
    m_chatList->updateBackground();
}

QListWidget *ChatPage::getUsersList() const
{
    return m_usersList;
}

QLineEdit *ChatPage::getMessageInput() const
{
    return m_messageInput;
}

QPushButton *ChatPage::getChangeNameButton() const
{
    return m_changeNameButton;
}

ChatListWidget *ChatPage::getChatList() const
{
    return m_chatList;
}

