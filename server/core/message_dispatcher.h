#pragma once

#include "helpers/message_enum/message_type.h"
#include <QObject>
#include <qtcpsocket.h>

class MessageDispatcher : public QObject
{
    Q_OBJECT
public:
    explicit MessageDispatcher(QObject *parent = nullptr);
    void processMessage(const QByteArray& jsonData, QTcpSocket* socket);

private:

signals:
    void hostStatusReceived(QTcpSocket *socket);
    void changeNameRequested(MessageType type, const QDateTime &dateTime, const QString &newName, const QString &oldName, QTcpSocket *socket);
    void clientSentMessage(const QString &message, QTcpSocket *socket);
    void unnamedClientTriedToSend();
};

