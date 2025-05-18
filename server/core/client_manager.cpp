#include "client_manager.h"
#include "helpers/json/json_message_builder.h"

ClientManager::ClientManager(QObject *parent)
    : QObject{parent}
{}

void ClientManager::addClient(QTcpSocket *socket, const QString &name, const QHostAddress &ip)
{
    m_clientNames[socket] = name;
    m_clientNameToIp[name] = ip;
}

void ClientManager::removeClient(QTcpSocket *socket)
{
    if (m_clientNames.contains(socket)) {
        QString name = m_clientNames.value(socket);
        m_clientNameToIp.remove(name);
        m_clientNames.remove(socket);
    }
}

QString ClientManager::getNewHostName() const
{
    // Notify clients of closing
    QString newHostName = "";
    QString currentHostName = m_clientNames.value(m_hostSocket);
    for (auto it = m_clientNames.begin(); it != m_clientNames.end(); ++it) {
        if (it.value() != currentHostName) {
            newHostName = it.value();
            break;
        }
    }

    if (newHostName.isEmpty()) {
        qDebug() << "Could not find a new host, all clients have the same name.";
    } else {
        qDebug() << "New host: " << newHostName;
    }

    return newHostName;
}

void ClientManager::setClientName(MessageType type, const QDateTime &dateTime, const QString &newName, const QString &oldName, QTcpSocket *socket)
{
    if (!socket) return;
    m_clientNames[socket] = newName;
    m_clientNameToIp[newName] = socket->peerAddress();
    qDebug() << "Client set name to:" << newName;

    if (MessageType::SetName == type) {
        QByteArray jsonData = JsonMessageBuilder::createNicknameChange(MessageType::SetName, newName);
        emit sendToClient(jsonData);
    }
    else if(MessageType::ChangeName == type) {
        QByteArray jsonData = JsonMessageBuilder::createNicknameChange(MessageType::ChangeName, oldName, newName);
        emit sendToClient(jsonData);
    }
    broadcastClientList();
}

void ClientManager::broadcastClientList()
{
    QStringList names = m_clientNames.values();
    m_clientListMessage = names.join(", ");

    QByteArray jsonData = JsonMessageBuilder::sendClientListName(m_clientListMessage);
    emit sendToClient(jsonData);
}

QString ClientManager::getClientName(QTcpSocket *socket) const
{
    return m_clientNames.value(socket, QString());
}

QHostAddress ClientManager::getClientIp(const QString &name) const
{
    return m_clientNameToIp.value(name, QHostAddress());
}

QStringList ClientManager::getAllClientNames() const
{
    return m_clientNames.values();
}

QTcpSocket *ClientManager::getHostSocket() const
{
    return m_hostSocket;
}

void ClientManager::setHostSocket(QTcpSocket *socket)
{
    m_hostSocket = socket;
}

void ClientManager::clear()
{
    m_clientNames.clear();
    m_clientNameToIp.clear();
}

bool ClientManager::hasClient(QTcpSocket *socket) const
{
    return m_clientNames.contains(socket);
}

QString ClientManager::preserveClientListBeforeHostChange()
{
    if (m_clientNames.contains(m_hostSocket)) {
        m_clientNames.remove(m_hostSocket);
    }
    QStringList names = m_clientNames.values();
    m_clientListKnown = names.join(", ");
    return m_clientListKnown;
}
