#include "server_controller.h"
#include "helpers/json/json_message_builder.h"
#include "helpers/dictionary/server_message_dictionary.h"

ServerController::ServerController(QObject *parent)
    : QObject{parent}
{}

void ServerController::setDependencies(UdpService *udp, MessageDispatcher *msg, ClientManager *client, TcpSender *tcp)
{
    udpService = udp;
    messageDispatcher = msg;
    clientManager = client;
    tcpSender = tcp;

    connect(clientManager, &ClientManager::sendToClient, this, &ServerController::sendToClient);
    connect(messageDispatcher, &MessageDispatcher::hostStatusReceived, this, &ServerController::onHostStatusReceived);
    connect(messageDispatcher, &MessageDispatcher::changeNameRequested, this, &ServerController::onChangeNameRequested);
    connect(messageDispatcher, &MessageDispatcher::clientSentMessage, this, &ServerController::onClientSentMessage);
    connect(messageDispatcher, &MessageDispatcher::unnamedClientTriedToSend, this, &ServerController::onUnnamedClientTriedToSend);
}

void ServerController::shutdown()
{
    if (!clientManager || !tcpSender) {
        qWarning() << "No dependencies for ServerController";
        return;
    }

    QString newHostName = clientManager->getNewHostName();
    QHostAddress newHostIp = clientManager->getClientIp(newHostName);
    QString clientListKnown = clientManager->preserveClientListBeforeHostChange();

    clientManager->clear();
    clientManager->broadcastClientList();

    qDebug() << "The server shuts down...";

    QByteArray jsonData = JsonMessageBuilder::createServerShuttingDown(newHostIp, messageDict.getMessage(1300), newHostName, clientListKnown);
    m_serverShuttingDown = true;
    tcpSender->shutdownServerAndNotifyClients(jsonData);

    qDebug() << "The name of the new host: " << newHostName;
    qDebug() << "Server successfully shut down";
}

void ServerController::handleClientDisconnected(QTcpSocket *socket)
{
    if (!socket)
        return;

    if (m_serverShuttingDown) {
        qDebug() << "Disabling the client when the server is shut down - no message is sent.";
        return;
    }

    if (clientManager->hasClient(socket)) {
        QString name = clientManager->getClientName(socket);
        QByteArray jsonData = JsonMessageBuilder::createSystemMessage(messageDict.getMessage(800) + ": " + name);
        tcpSender->sendToClient(jsonData);
    }

    if (socket->state() == QAbstractSocket::UnconnectedState) {
        if (tcpSender->removeClientSocket(socket)) {
            clientManager->removeClient(socket);
            socket->deleteLater();
            qDebug() << "Client disconnected";
        } else {
            qDebug() << "Error removing socket from TcpSender";
        }

        clientManager->broadcastClientList();
    }
}

void ServerController::sendToClient(const QByteArray jsonData)
{
    tcpSender->sendToClient(jsonData);
}

void ServerController::handleNewClientConnection(QTcpSocket *socket)
{
    tcpSender->addClientSocket(socket);
    clientManager->broadcastClientList();
}

void ServerController::onHostStatusReceived(QTcpSocket *socket)
{
    clientManager->setHostSocket(socket);
    QByteArray response = JsonMessageBuilder::createRequestName();
    tcpSender->sendToClient(response);
}

void ServerController::onChangeNameRequested(MessageType type, const QDateTime &dateTime, const QString &newName, const QString &oldName, QTcpSocket *socket)
{
    clientManager->setClientName(type, dateTime, newName, oldName, socket);
}

void ServerController::onClientSentMessage(const QString &message, QTcpSocket *socket)
{
    if (clientManager->hasClient(socket)) {
        QString name = clientManager->getClientName(socket);
        QByteArray response = JsonMessageBuilder::createMessage(MessageSource::Client, name, message);
        tcpSender->sendToClient(response);
    }
}

void ServerController::onUnnamedClientTriedToSend()
{
    QByteArray response = JsonMessageBuilder::createSystemMessage(messageDict.getMessage(1200));
    tcpSender->sendToClient(response);
}
