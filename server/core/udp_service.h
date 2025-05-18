#pragma once

#include "helpers/message_enum/host_binding_mode.h"
#include <QObject>
#include <qnetworkinterface.h>
#include <qudpsocket.h>

class UdpService : public QObject
{
    Q_OBJECT
public:
    explicit UdpService(QObject *parent = nullptr);
    void start(const QNetworkAddressEntry &entry, const quint16 port, const QNetworkInterface &interface, HostBindingMode mode);

private:
    QUdpSocket *m_udpSocket;
    QNetworkAddressEntry m_entry;
    QHostAddress m_host;
    quint16 m_port;

private slots:
    void readPendingDatagrams();

signals:
};

