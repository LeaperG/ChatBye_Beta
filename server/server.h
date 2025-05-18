#pragma once

#include "helpers/message_enum/host_binding_mode.h"
#include "helpers/dictionary/server_message_dictionary.h"

#include "server/core/client_manager.h"
#include "server/core/message_dispatcher.h"
#include "server/server_controller.h"
#include "server/core/udp_service.h"
#include "server/core/tcp_sender.h"

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QByteArray>
#include <QUdpSocket>
#include <QNetworkInterface>

class Server : public QTcpServer
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = nullptr);
    ~Server();

    void initiateStartup(const QNetworkAddressEntry &entry, const quint16 port, HostBindingMode mode);
    void startServer(const QHostAddress &host, const quint16 port, HostBindingMode mode);
    QTcpSocket *socket;

private:
    UdpService *udpService = nullptr;
    ClientManager *clientManager = nullptr;
    TcpSender *tcpSender = nullptr;
    MessageDispatcher *messageDispatcher = nullptr;
    ServerController *serverController = nullptr;
    ServerMessageDictionary messageDict;

    QByteArray Data;

    QNetworkAddressEntry m_entry;
    QHostAddress m_host;
    quint16 m_port;
    HostBindingMode m_mode;
    QNetworkInterface m_interface;

    quint16 nextBlockSize = 0;

    void startUdpMulticast();
private slots:

public slots:
    void incomingConnection(qintptr socketDescriptor);
    void slotReadyRead();
    void clientDisconnected();
    void setInterface(const QNetworkInterface &interface);

signals:
    void requestInterface(const QHostAddress& host);
};
