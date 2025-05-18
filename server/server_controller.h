#pragma once

#include "server/core/client_manager.h"
#include "server/core/message_dispatcher.h"
#include "server/core/tcp_sender.h"
#include "server/core/udp_service.h"
#include "helpers/dictionary/server_message_dictionary.h"
#include <QObject>

class ServerController : public QObject
{
    Q_OBJECT
public:
    explicit ServerController(QObject *parent = nullptr);
    void setDependencies(UdpService *udp, MessageDispatcher *msg, ClientManager *client, TcpSender *tcp);

    void shutdown();
    void handleClientDisconnected(QTcpSocket *socket);

    void sendToClient(const QByteArray jsonData);
    void handleNewClientConnection(QTcpSocket *socket);
    void onHostStatusReceived(QTcpSocket *socket);
    void onChangeNameRequested(MessageType type, const QDateTime &dateTime, const QString &newName, const QString &oldName, QTcpSocket *socket);
    void onClientSentMessage(const QString &message, QTcpSocket *socket);
    void onUnnamedClientTriedToSend();

    bool m_serverShuttingDown = false;

private:
    UdpService *udpService = nullptr;
    MessageDispatcher *messageDispatcher = nullptr;
    ClientManager *clientManager = nullptr;
    TcpSender *tcpSender = nullptr;
    ServerMessageDictionary messageDict;

signals:
};
