#include "client/core/server_launcher.h"
#include <qnetworkinterface.h>

ServerLauncher::ServerLauncher(QObject *parent)
    : QObject{parent}
{}

bool ServerLauncher::startServer(const QNetworkAddressEntry &entry, quint16 &outPort)
{
    m_entry = entry;
    m_host = entry.ip();
    //qDebug() << "The server will be started on the host:" << m_localHost;

    stopServer();

    const quint16 startPort = 50500;
    const quint16 endPort = 50515;
    bool serverStarted = false;

    for (quint16 port = startPort; port < endPort; ++port)
    {
        //Server* s = new Server(this, QHostAddress::Any, port);
        //Server* s = new Server(this, m_entry, port);
        Server* s = new Server(this);
        connect(s, &Server::requestInterface, this, &ServerLauncher::onRequestInterface);
        connect(this, &ServerLauncher::interfaceAvailable, s, &Server::setInterface);
        s->initiateStartup(m_entry, port, HostBindingMode::DynamicPort);

        if (s->isListening()) {

            //emit errorOccurred("Ошибка", "Сервер работает на m_localHost = " + m_localHost.toString() +"\tport = " + QString::number(port), QMessageBox::Warning);
            m_server = s;
            m_port = port;
            outPort = port;
            //qDebug() << "The server is running on port:" << port;
            serverStarted = true;
            break;
        } else {
            delete s;
        }
    }

    if (!serverStarted) {
        m_server = nullptr;
    }

    return serverStarted;
}

bool ServerLauncher::startServerOnFixedPort(const QNetworkAddressEntry &entry, quint16 &outPort, HostBindingMode mode)
{
    if (m_port == outPort) {
        emit errorOccurred("Ошибка", "Этот порт занят m_localPort = " + QString::number(m_port) +"\toutPort = " + QString::number(outPort), QMessageBox::Warning);
        return false;
    }

    m_entry = entry;
    m_host = entry.ip();
    m_port = outPort;

    Server* s = new Server(this);
    connect(s, &Server::requestInterface, this, &ServerLauncher::onRequestInterface);
    connect(this, &ServerLauncher::interfaceAvailable, s, &Server::setInterface);
    s->initiateStartup(m_entry, m_port, mode);

    if (s->isListening()) {
        m_server = s;
        //qDebug() << "The server is running on port:" << m_localPort;
        return true;
    } else {
        delete s;
    }

    return false;
}

void ServerLauncher::stopServer()
{
    if (m_server) {
        if (m_server->isListening()) {
            qDebug() << "The server is stopped on port:" << m_server->serverPort();
            m_server->close();
        }
        delete m_server;
        m_server = nullptr;
    }
}

bool ServerLauncher::isRunning() const
{
    return m_server && m_server->isListening();
}

void ServerLauncher::onRequestInterface(const QHostAddress &host)
{
    emit requestInterface(host);
}

void ServerLauncher::onInterfaceAvailable(const QNetworkInterface &interface)
{
    emit interfaceAvailable(interface);
}
