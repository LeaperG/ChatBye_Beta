#pragma once

#include "helpers/message_enum/message_source.h"
#include "helpers/dictionary/server_message_dictionary.h"

#include <QSettings>
#include <QObject>
#include <QMessageBox>
#include <QTimer>
#include <QListWidget>
#include <QTime>
#include <QGridLayout>
#include <QLabel>

class MessageManager : public QObject
{
    Q_OBJECT
public:
    explicit MessageManager(QObject *parent = nullptr);
    void addMessageToChat(MessageSource source, const QString &myName, const QString &sender, const QString &message, Qt::Alignment alignment, const QDateTime &dateTime, const QString lastSender);

private:
    QSettings settings;
    QString m_fontFamily;

    QString m_clientName;
    QLabel *m_messageLabel;
    MessageSource m_currentSource;
    ServerMessageDictionary messageDict;

    // Adding a message
    QWidget* createMainWidget();
    QWidget* createMessageWidget(QWidget *parent, const QString &sender, bool isSenderLabel, const QString &message, const QDateTime &dateTime);
    QLabel* createSenderLabel(const QString &sender);
    QLabel* createMessageLabel(const QString &message, const QString &sender);
    QLabel* createTimeLabel(const QDateTime &dateTime);
    void alignMessage(Qt::Alignment alignment, QGridLayout *layout, QWidget *messageWidget);
    QString formatMessage(const QString &message, int maxLength);
    QString formatNickChangeMessage(const QString &message);

signals:
    void messageAdded(QWidget *mainWidget);
};

