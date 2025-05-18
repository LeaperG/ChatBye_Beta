#pragma once

#include <QObject>
#include <qtcpsocket.h>

class TcpSender : public QObject
{
    Q_OBJECT
public:
    explicit TcpSender(QObject *parent = nullptr);
    void sendToClient(const QByteArray jsonData);
    void addClientSocket(QTcpSocket *socket);
    bool removeClientSocket(QTcpSocket *socket);
    void removeAllClientSocket();
    void shutdownServerAndNotifyClients(const QByteArray jsonData);

private:
    void checkAllDataSent(qint64 bytes);

    QVector<QTcpSocket*> m_sockets;
    QByteArray Data;
    int m_pendingClients = 0;

signals:
};

