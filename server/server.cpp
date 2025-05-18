#include "server.h"
#include "helpers/message_enum/host_binding_mode.h"

#include <QUdpSocket>
#include <QNetworkInterface>


Server::Server(QObject *parent)
    : QTcpServer{parent}
{
    nextBlockSize = 0;
    messageDict = ServerMessageDictionary();

    udpService = new UdpService(this);
    messageDispatcher = new MessageDispatcher(this);
    clientManager = new ClientManager(this);
    tcpSender = new TcpSender(this);

    serverController = new ServerController(this);
    serverController->setDependencies(udpService, messageDispatcher, clientManager, tcpSender);
}

Server::~Server(){

    if (this->isListening()) {
        serverController->shutdown();
        this->close();
    }
}

void Server::initiateStartup(const QNetworkAddressEntry &entry, const quint16 port, HostBindingMode mode)
{
    m_entry = entry;
    m_host = entry.ip();
    m_port = port;
    m_mode = mode;
    emit requestInterface(m_host);
}

void Server::startServer(const QHostAddress &host, const quint16 port, HostBindingMode mode)
{
    if (!m_interface.isValid()) {
        qDebug() << "Interface not specified, startup not possible";
        return;
    }

    if (this->listen(host, port)) {
        qDebug() << "The TCP server is running on" << host << "port" << port;
    } else {
        qDebug() << "TCP server is not running on" << host << "port" << port;
        return;
    }

    if (mode != HostBindingMode::FixedPort) {
        startUdpMulticast();
    }
}

void Server::incomingConnection(qintptr socketDescriptor)
{
    socket = new QTcpSocket;
    socket->setSocketDescriptor(socketDescriptor);
    //qDebug() << "New connection from:" << socket->peerAddress().toString();
    connect(socket, &QTcpSocket::readyRead, this, &Server::slotReadyRead);
    connect(socket, &QTcpSocket::disconnected, this, &Server::clientDisconnected);

    qDebug() << "Client connectes " << socketDescriptor;
    serverController->handleNewClientConnection(socket);
}

void Server::slotReadyRead()
{
    socket = (QTcpSocket*)sender();
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_6_7);

    if (in.status() == QDataStream::Ok)
    {
        for (;;) {
            if (nextBlockSize == 0) {
                //qDebug() << "nextBlockSize = 0";
                if (socket->bytesAvailable() < 2) {
                    //qDebug() << "Data < 2, break";
                    break;
                }
                in >> nextBlockSize;
                //qDebug() << "nextBlockSize = " << nextBlockSize;
            }
            if (socket->bytesAvailable() < nextBlockSize) {
                //qDebug() << "Data not full, break";
                break;
            }

            QByteArray jsonData;
            jsonData.resize(nextBlockSize);
            in.readRawData(jsonData.data(), nextBlockSize);

            nextBlockSize = 0;
            messageDispatcher->processMessage(jsonData, socket);

            break;
        }
    }
    else
    {
        qDebug() << "QDataStream error";
    }
}

void Server::clientDisconnected()
{
    QTcpSocket *clientSocket = qobject_cast<QTcpSocket*>(sender());
    if (clientSocket) {
        serverController->handleClientDisconnected(clientSocket);
    }
}

void Server::setInterface(const QNetworkInterface &interface)
{
    m_interface = interface;
    startServer(m_host, m_port, m_mode);
}

void Server::startUdpMulticast()
{
    udpService->start(m_entry, m_port, m_interface, m_mode);
}
