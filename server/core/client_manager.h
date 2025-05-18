#pragma once

#include <QObject>
#include "helpers/message_enum/message_type.h"
#include <QTcpSocket>
#include <QHostAddress>
#include <QMap>
#include <QStringList>

class ClientManager : public QObject
{
    Q_OBJECT
public:
    explicit ClientManager(QObject *parent = nullptr);
    void addClient(QTcpSocket* socket, const QString& name, const QHostAddress& ip);
    void removeClient(QTcpSocket* socket);

    QString getNewHostName() const;

    void setClientName(MessageType type, const QDateTime &dateTime, const QString &newName, const QString &oldName, QTcpSocket *socket);
    QString getClientName(QTcpSocket* socket) const;
    QHostAddress getClientIp(const QString& name) const;

    QStringList getAllClientNames() const;
    void broadcastClientList();

    QTcpSocket* getHostSocket() const;
    void setHostSocket(QTcpSocket* socket);
    void clear();
    bool hasClient(QTcpSocket* socket) const;

    QString preserveClientListBeforeHostChange();

private:
    QMap<QTcpSocket*, QString> m_clientNames;
    QMap<QString, QHostAddress> m_clientNameToIp;

    QTcpSocket *m_hostSocket = nullptr;

    QString m_clientListMessage;
    QString m_clientListKnown;

signals:
    void sendToClient(const QByteArray& jsonData);
};

