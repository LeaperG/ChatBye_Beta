#pragma once

#include "helpers/message_enum/host_binding_mode.h"
#include <QObject>
#include <QHostAddress>
#include <QUdpSocket>
#include <QNetworkInterface>
#include <qtimer.h>

class DiscoveryManager : public QObject
{
    Q_OBJECT
public:
    explicit DiscoveryManager(QObject *parent = nullptr);
    void discover(const QNetworkAddressEntry &entry, quint16 port, HostBindingMode mode);
    void startUdp();
    void stopDiscovery();

    HostBindingMode getHostingMode() const;
    QNetworkAddressEntry getEntry() const;
    QHostAddress getHost() const;
    quint16 getPort() const;

private:
    QUdpSocket *m_udpSocket = nullptr;
    QTimer* m_udpSendTimer = nullptr;
    HostBindingMode m_mode;
    QNetworkAddressEntry m_entry;
    QHostAddress m_host;
    quint16 m_port;
    bool m_discoveryActive = false;

    void sendDiscoveryRequest();
    void cleanupUdpSocket();

private slots:
    void readPendingDatagrams();
    void onDiscoveryTimeout();

signals:
    void serverFound(const QNetworkAddressEntry &entry, quint16 port, HostBindingMode mode);
    void discoveryFailed(const QNetworkAddressEntry &entry, quint16 port);
};

