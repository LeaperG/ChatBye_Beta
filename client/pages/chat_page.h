#pragma once

#include "helpers/message_enum/message_source.h"
#include "client/pages/message_manager/message_manager.h"
#include "client/ui/custom_widgets/chat_list_widget.h"

#include "page_base.h"
#include <QWidget>
#include <QTextEdit>
#include <QMessageBox>
#include <QTimer>
#include <QListWidget>
#include <QTime>
#include <QToolButton>



class ChatPage : public PageBase
{
    Q_OBJECT
public:
    explicit ChatPage(QWidget *parent = nullptr);
    QListWidget* getUsersList() const;
    QLineEdit* getMessageInput() const;
    QPushButton* getChangeNameButton() const;
    ChatListWidget* getChatList() const;

private:
    MessageManager *messageManager;
    QSettings settings;

    QGridLayout *mainLayout;
    QVBoxLayout *chatLayout;
    QHBoxLayout *inputLayout;
    QVBoxLayout *usersLayout;

    ChatListWidget *m_chatList;
    QLineEdit *m_messageInput;
    QToolButton *m_sendButton;
    QPushButton *m_changeNameButton;
    QPushButton *m_toggleUsersButton;
    QListWidget *m_usersList;
    QString m_lastSender;
    QString m_clientName;

    void toggleUsersList();
    void checkMessage();
    void sendName();
    void initConnect();
    QStringList splitMessage(const QString &message, int maxLength = 4096);


public slots:
    void receiveMessageFromServer(MessageSource source, const QString &name, const QString &message, const QDateTime &dateTime); // Слот для получения строки
    void updateClientList(const QStringList& names);
    void getClientName(const QString &name);
    void updateImage(const QString &pageName);

private slots:
    void addItemToChatList(QWidget *mainWidget);
    void updateStyleSheet();

signals:
    void sendMessage(const QByteArray jsonData);
    void changeName();
};

