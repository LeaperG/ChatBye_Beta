#include "message_manager.h"

MessageManager::MessageManager(QObject *parent)
    : QObject(parent), settings("ChatBye", "ColorSettings")
{
    messageDict = ServerMessageDictionary();
    m_fontFamily = settings.value("fontFamily").toString();
}

void MessageManager::addMessageToChat(MessageSource source, const QString &myName, const QString &sender, const QString &message, Qt::Alignment alignment, const QDateTime &dateTime, const QString lastSender)
{
    m_clientName = myName;
    m_currentSource = source;

    QString formattedMessage = formatMessage(message, 50);

    QWidget *mainWidget = createMainWidget();
    QGridLayout *mainLayout = new QGridLayout(mainWidget);
    bool isSenderLabel = true;

    QString nameSender = sender;

    if (MessageSource::Client == source)
    {
        mainLayout->setContentsMargins(9, 2, 9, 2);
    }
    else {
        //nameSender = messageSourceToString(source);
        nameSender = "";
        isSenderLabel = false;
        mainLayout->setAlignment(Qt::AlignCenter);
    }

    if (lastSender == sender) {
        nameSender = "";
        isSenderLabel = false;
    }

    if (m_clientName == "Вы")
    {
        //nameSender = "";
        isSenderLabel = false;
    }



    QWidget *messageWidget = createMessageWidget(mainWidget, sender, isSenderLabel, formattedMessage, dateTime);

    if (MessageSource::Server == source)
    {
        QString colorSystemMessageBack = settings.value("systemMessageBack").toString();
        messageWidget->setStyleSheet(QString("background-color: %1; padding: 0px; margin: 0px; "
                                             "border-top-left-radius: 15px; border-bottom-left-radius: 15px; "
                                             "border-top-right-radius: 15px; border-bottom-right-radius: 15px;").arg(colorSystemMessageBack));
    }
    else if (m_clientName == "Вы")
    {
        QString colorSendMessageBack = settings.value("sendMessageBack").toString();
        messageWidget->setStyleSheet(QString("background-color: %1; padding: 0px; margin: 0px; "
                                             "border-top-left-radius: 15px; border-bottom-left-radius: 15px; "
                                             "border-top-right-radius: 7px; border-bottom-right-radius: 7px;").arg(colorSendMessageBack));
    }
    else {
        QString colorGetMessageBack = settings.value("getMessageBack").toString();
        messageWidget->setStyleSheet(QString("background-color: %1; padding: 0px; margin: 0px; "
                                             "border-top-left-radius: 7px; border-bottom-left-radius: 7px; "
                                             "border-top-right-radius: 15px; border-bottom-right-radius: 15px;").arg(colorGetMessageBack));
    }

    // Select the alignment for the message.
    alignMessage(alignment, mainLayout, messageWidget);

    // Create a new list item and set its size by content.
    //addItemToChatList(mainWidget);

    if (mainWidget != nullptr) {
        emit messageAdded(mainWidget);
    } else {
        qWarning() << "mainWidget is null!";
    }
}

QWidget *MessageManager::createMainWidget()
{
    QWidget *widget = new QWidget();
    widget->setStyleSheet("background-color: transparent; border-radius: 10px; padding: 0px; margin: 0px;");
    return widget;
}

QWidget *MessageManager::createMessageWidget(QWidget *parent, const QString &sender, bool isSenderLabel, const QString &message, const QDateTime &dateTime)
{
    QWidget *messageWidget = new QWidget(parent);
    QGridLayout *layout = new QGridLayout(messageWidget);
    layout->setContentsMargins(10, 7, 5, 5);
    layout->setSpacing(0);

    // Create and add sender, message and time labels
    if (isSenderLabel) {
        QLabel *senderLabel = createSenderLabel(sender);
        layout->addWidget(senderLabel, 0, 0, Qt::AlignLeft);
    }

    m_messageLabel = createMessageLabel(message, sender);
    QLabel *timeLabel = createTimeLabel(dateTime);

    layout->addWidget(m_messageLabel, 1, 0, Qt::AlignCenter);
    layout->addWidget(timeLabel, 1, 1, Qt::AlignRight);

    //messageWidget->setStyleSheet(QString("background-color: %1; border-radius: 10px; padding: 0px; margin: 0px; ").arg(bgColor));
    messageWidget->setLayout(layout);

    return messageWidget;
}

QLabel *MessageManager::createSenderLabel(const QString &sender)
{
    QString colorSender = settings.value("textSender").toString();
    QLabel *label = new QLabel(QString("<b><span style='color: %1; font-size: 10pt; font-family: %3;'>%2</span></b>").arg(colorSender).arg(sender).arg(m_fontFamily));
    label->setStyleSheet("margin: 0px; padding: 0px; background-color: transparent;");
    return label;
}

QLabel *MessageManager::createMessageLabel(const QString &message, const QString &sender)
{
    QString formattedMessage;
    if (m_currentSource == MessageSource::Server) {
        // Check if the nickname change should be formatted
        formattedMessage = formatNickChangeMessage(message);
    }
    else {
        formattedMessage = message;
    }

    // Create a QLabel to display the message
    QString colorTextMessage = settings.value("textMessage").toString();
    QLabel *label = new QLabel(QString("<span style='color: %1; font-size: 10pt; font-family: %3;'>%2</span>").arg(colorTextMessage).arg(formattedMessage).arg(m_fontFamily));
    label->setStyleSheet("margin: 0px; padding: 0px;");
    return label;
}

QLabel *MessageManager::createTimeLabel(const QDateTime &dateTime)
{
    QString colorTime = settings.value("textTime").toString();
    QLabel *label = new QLabel(QString("<span style='color: %1; font-size: 8pt; font-family: %3; '>%2</span>").arg(colorTime).arg(dateTime.toString("hh:mm")).arg(m_fontFamily));
    int messageLabelHeight = m_messageLabel->sizeHint().height();
    QString styleSheet = QString("margin-left: 5px; padding-top: %1px; padding-top: %1px;").arg(messageLabelHeight - 10);
    label->setStyleSheet(styleSheet);
    return label;
}

void MessageManager::alignMessage(Qt::Alignment alignment, QGridLayout *layout, QWidget *messageWidget)
{
    if (alignment == Qt::AlignRight) {
        layout->addWidget(messageWidget, 0, 0, Qt::AlignRight);
    } else {
        layout->addWidget(messageWidget, 0, 0, Qt::AlignLeft);
    }
}

QString MessageManager::formatMessage(const QString &message, int maxLength)
{
    QString formattedMessage;
    int start = 0;

    while (start < message.length()) {
        int end = start + maxLength;
        if (end >= message.length()) {
            end = message.length();
        } else {
            // Check if we're breaking a word.
            while (end > start && !message[end].isSpace()) {
                end--;
            }
            if (end == start) {
                // If the word is longer than maxLength, just cut it off
                end = start + maxLength;
            }
        }

        formattedMessage += message.mid(start, end - start);

        start = end;
        while (start < message.length() && message[start].isSpace()) {
            start++;
        }

        // Add a line break only if there is still some text left.
        if (start < message.length()) {
            formattedMessage += "<br>";
        }
    }
    return formattedMessage;
}

QString MessageManager::formatNickChangeMessage(const QString &message)
{
    //qDebug() << "Message received: [" << message << "]";

    // Clear the message of extra spaces and hidden characters
    QString cleanedMessage = message.trimmed().replace("\n", "").replace("\r", "");

    // Разбиваем строку по ":"
    QStringList parts = cleanedMessage.split(messageDict.getMessage(1400));

    if (parts.size() < 2) {
        return message;  // If the structure does not match, return the usual message
    }

    // Remove square brackets and spaces
    QString oldNick = parts[0].trimmed();
    QString newNick = parts[1].trimmed();

    qDebug() << "Обнаружена смена ника: " << oldNick << " -> " << newNick;

    if (oldNick.isEmpty())
    {
        return QString("<b>Подключился к чату: <span style='color: blue; font-family: %2;'>[%1]</span></b>").arg(newNick).arg(m_fontFamily);
    }


    // If the current user has changed the name
    if (newNick == m_clientName) {
        return QString("<b>Вы сменили имя на <span style='color: blue; font-family: %2;'>[%1]</span></b>")
            .arg(newNick).arg(m_fontFamily);
    }

    // If someone else changed their nickname
    return QString("Пользователь <b><span style='color: red;'>[%1]</span></b> "
                   "теперь известен как "
                   "<b><span style='color: green; font-family: %3; '>[%2]</span></b>")
        .arg(oldNick, newNick).arg(m_fontFamily);
}
