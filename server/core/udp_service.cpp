#include "udp_service.h"
#include "helpers/message_enum/host_binding_mode.h"
#include "helpers/json/json_message_builder.h"
#include <qnetworkinterface.h>

UdpService::UdpService(QObject *parent)
    : QObject{parent}
{}

void UdpService::start(const QNetworkAddressEntry &entry, const quint16 port, const QNetworkInterface &interface, HostBindingMode mode)
{
    m_entry = entry;
    m_host = entry.ip();
    m_port = port;

    if (mode != HostBindingMode::FixedPort) {
        m_udpSocket = new QUdpSocket(this);

        // Bind the socket to the desired address and port
        if (m_udpSocket->bind(QHostAddress::AnyIPv4, 50501,
                            QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint)) {

            // Join a multicast group through a specific interface
            if (m_udpSocket->joinMulticastGroup(QHostAddress("239.255.43.21"), interface)) {
                //qDebug() << "Multicast joined via:" << interface.humanReadableName();
            }

            connect(m_udpSocket, &QUdpSocket::readyRead, this, &UdpService::readPendingDatagrams);
        }
    }
}

void UdpService::readPendingDatagrams()
{
    while (m_udpSocket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(m_udpSocket->pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort;

        m_udpSocket->readDatagram(datagram.data(), datagram.size(), &sender, &senderPort);

        if (datagram == "SERVER_DISCOVERY")
        {
            QByteArray response = JsonMessageBuilder::sendServerDetails("We look forward to seeing you on our server)", m_entry, m_port);
            m_udpSocket->writeDatagram(response, sender, senderPort);
            //qDebug() << "Responded to discovery request from" << m_host.toString();
        }
    }
}
