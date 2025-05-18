#include "discovery_manager.h"
#include "helpers/message_enum/host_binding_mode.h"

#include <QTimer>
#include <QJsonDocument>
#include <QJsonObject>
constexpr const char* DISCOVERY_REQUEST = "SERVER_DISCOVERY";

DiscoveryManager::DiscoveryManager(QObject *parent)
    : QObject{parent}
{

}

void DiscoveryManager::discover(const QNetworkAddressEntry &entry, quint16 port, HostBindingMode mode)
{
    if (m_discoveryActive) {
        qDebug() << "Search already active, callback rejected";
        return;
    }

    m_entry = entry;
    m_host = entry.ip();
    m_port = port;
    m_mode = mode;
    m_discoveryActive = true;

    if (mode == HostBindingMode::FixedPort) {
        emit serverFound(m_entry, m_port, mode);
        return;
    }

    if (m_udpSendTimer && m_udpSendTimer->isActive()) {
        return;
    }

    startUdp();
    QTimer::singleShot(2000, this, &DiscoveryManager::onDiscoveryTimeout);
}

void DiscoveryManager::startUdp()
{
    cleanupUdpSocket();

    m_udpSocket = new QUdpSocket(this);
    connect(m_udpSocket, &QUdpSocket::readyRead, this, &DiscoveryManager::readPendingDatagrams);
    m_udpSocket->bind(QHostAddress::AnyIPv4, 0, QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);
    m_udpSocket->setSocketOption(QAbstractSocket::MulticastTtlOption, 1);

    if (!m_udpSendTimer) {
        m_udpSendTimer = new QTimer(this);
        connect(m_udpSendTimer, &QTimer::timeout, this, [this]() {;
            sendDiscoveryRequest();
        });
    }

    // Start periodic sending
    m_udpSendTimer->start(500);
    sendDiscoveryRequest();
}

void DiscoveryManager::readPendingDatagrams()
{
    qDebug() << "UDP found the server";
    while (m_udpSocket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(m_udpSocket->pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort;

        m_udpSocket->readDatagram(datagram.data(), datagram.size(), &sender, &senderPort);

        QJsonDocument doc = QJsonDocument::fromJson(datagram);
        if (doc.isNull() || !doc.isObject())
        {
            qDebug() << "JSON parsing error";
            return;
        }

        QJsonObject jsonObj = doc.object();
        if (!jsonObj.contains("message") || !jsonObj.contains("host_address") || !jsonObj.contains("port")) {
            qDebug() << "Incorrect JSON, missing fields";
            return;
        }

        QString message = jsonObj.value("message").toString();
        QString netmask = jsonObj.value("netmask").toString();
        QString broadcast = jsonObj.value("broadcast").toString();
        QString messageIp = jsonObj.value("host_address").toString();
        QString messagePort = jsonObj.value("port").toString();

        QHostAddress netmaskAddr(netmask);
        QHostAddress broadcastAddr(broadcast);
        QHostAddress ip(messageIp);
        quint16 port = messagePort.toInt();

        QNetworkAddressEntry entry;
        entry.setIp(ip);
        entry.setNetmask(netmaskAddr);
        entry.setBroadcast(broadcastAddr);

        m_entry = entry;
        m_host = ip;
        m_port = port;

        emit serverFound(m_entry, m_port, HostBindingMode::DynamicPort);
        m_discoveryActive = false;
        //qDebug() << "Connecting to server at" << m_host.toString() << ":" << port;

    }
}

void DiscoveryManager::sendDiscoveryRequest()
{
    if (!m_udpSocket)
        return;

    QByteArray datagram = DISCOVERY_REQUEST;
    qint64 bytesSent = m_udpSocket->writeDatagram(datagram, QHostAddress("239.255.43.21"), 50501);
    if (bytesSent == -1) {
        qWarning() << "Error when sending UDP: " << m_udpSocket->errorString();
    } else {
        qDebug() << "Broadcasting server discovery request";
    }
}

void DiscoveryManager::onDiscoveryTimeout()
{
    if (!m_discoveryActive)
        return;

    emit discoveryFailed(m_entry, m_port);
}

void DiscoveryManager::cleanupUdpSocket()
{
    if (m_udpSendTimer && m_udpSendTimer->isActive()) {
        m_udpSendTimer->stop();
        qDebug() << "Stopped UDP send timer";

        // m_udpSendTimer->deleteLater();
        // m_udpSendTimer = nullptr;
    }

    if (!m_udpSocket)
    {
        qDebug() << "The UDP socket has already been cleared";
        return;
    }

    if (m_udpSocket) {
        qDebug() << "Cleaning up UDP socket";
        m_udpSocket->disconnect();
        m_udpSocket->close();

        QUdpSocket* socketToDelete = m_udpSocket;
        m_udpSocket = nullptr;

        socketToDelete->deleteLater();
    } else {
        qDebug() << "m_udpSocket is already nullptr";
    }
}

void DiscoveryManager::stopDiscovery()
{
    m_discoveryActive = false;
    QTimer::singleShot(0, this, &DiscoveryManager::cleanupUdpSocket);
    qDebug() << "UDP discovery stopped manually";
}

QHostAddress DiscoveryManager::getHost() const
{
    return m_host;
}

quint16 DiscoveryManager::getPort() const
{
    return m_port;
}

HostBindingMode DiscoveryManager::getHostingMode() const
{
    return m_mode;
}

QNetworkAddressEntry DiscoveryManager::getEntry() const
{
    return m_entry;
}
