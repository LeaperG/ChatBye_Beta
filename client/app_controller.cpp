#include "app_controller.h"

#include "helpers/message_enum/interface_filter.h"
#include "helpers/json/json_message_builder.h"
#include <qtimer.h>

AppController::AppController(QObject *parent)
    : QObject{parent}
{}

void AppController::setDependencies(NetworkManager *net, DiscoveryManager *disc, ServerLauncher *serv, MessageHandler *msg, HostInfo *host)
{
    networkManager = net;
    discoveryManager = disc;
    serverLauncher = serv;
    messageHandler = msg;
    hostInfo = host;


    connect(hostInfo, &HostInfo::hostEntriesReady, this, &AppController::onHostEntriesReady);
    connect(discoveryManager, &DiscoveryManager::serverFound, this, &AppController::onServerFound);
    connect(discoveryManager, &DiscoveryManager::discoveryFailed, this, &AppController::onDiscoveryFailed);
    connect(networkManager, &NetworkManager::portAvailable, this, &AppController::onPortAvailable);
    connect(networkManager, &NetworkManager::noAvailablePortFound, this, &AppController::onNoAvailablePortFound);
    connect(networkManager, &NetworkManager::connected, this, &AppController::onConnected);
    connect(networkManager, &NetworkManager::disconnected, this, &AppController::onDisconnected);
    connect(networkManager, &NetworkManager::messageReceived, this, &AppController::onMessageReceived);
    connect(messageHandler, &MessageHandler::serverShuttingDown, this, &AppController::onServerShuttingDown);
    connect(messageHandler, &MessageHandler::chatMessageReceived, this, &AppController::onChatMessageReceived);
    connect(messageHandler, &MessageHandler::clientListReceived, this, &AppController::onClientListReceived);

    connect(networkManager, &NetworkManager::errorOccurred, this, &AppController::onErrorOccurred);
    connect(serverLauncher, &ServerLauncher::errorOccurred, this, &AppController::onErrorOccurred);

    connect(serverLauncher, &ServerLauncher::requestInterface, this, &AppController::onRequestInterface);
    connect(hostInfo, &HostInfo::interfaceReady, this, &AppController::onInterfaceReady);

    connect(messageHandler, &MessageHandler::requestClientNameForHost, this, &AppController::onRequestClientName);
    connect(networkManager, &NetworkManager::scanProgressUpdated, this, &AppController::onScanProgressUpdated);

}

bool AppController::isHost() const
{
    return m_isHost;
}

void AppController::onRequestHost(InterfaceFilter filter)
{
    hostInfo->getNetworkEntries(filter);
}

void AppController::onHostEntriesReady(const QList<QNetworkAddressEntry> &entryList)
{
    emit hostEntriesReadyAvailable(entryList);
}

void AppController::onHostAndPortEntered(const QNetworkAddressEntry &entry, quint16 port, HostBindingMode mode)
{
    m_entry = entry;
    m_port = port;
    m_mode = mode;

    emit scanProgressStarted();

    if (!networkManager->isConnected()) {
        discoveryManager->discover(entry, port, mode);
    }
}

void AppController::onServerFound(const QNetworkAddressEntry &entry, quint16 port, HostBindingMode mode)
{
    networkManager->stopScan();
    qDebug() << "UDP has found a server specify onServerFound";
    discoveryManager->stopDiscovery();


    if (!networkManager->isConnected()) {

        networkManager->tryConnectToHost(entry, port);

        if (mode == HostBindingMode::FixedPort) {
            if (!networkManager->isConnected()) {
                if (serverLauncher->startServerOnFixedPort(entry, port, mode)) {
                    if (!m_isHost) {
                        m_isHost = true;
                        qDebug() << "I'm a host now!";
                    }
                    networkManager->tryConnectToHost(entry, port);
                }
            }
        }
    }
}

void AppController::onDiscoveryFailed(const QNetworkAddressEntry &entry, quint16 port)
{
    discoveryManager->stopDiscovery();

    if (!networkManager->isConnected()) {
        if (!serverLauncher->isRunning()) {
            networkManager->scanAvailablePortsAsync(entry, port, ScanMode::FullScan, 1);
        }
        else {
            networkManager->tryConnectToHost(entry, port);
        }
    }
}

void AppController::onPortAvailable(const QNetworkAddressEntry &entry, quint16 port, HostBindingMode mode)
{
    discoveryManager->stopDiscovery();

    if (m_isHost) {
        m_isHost = false;
        qDebug() << "Now I'm not a host!";
    }

    networkManager->tryConnectToHost(entry, port);
}

void AppController::onNoAvailablePortFound(const QNetworkAddressEntry &entry, quint16 port, HostBindingMode mode)
{
    discoveryManager->stopDiscovery();

    if (!networkManager->isConnected()) {
        quint16 actualPort = port;
        if (serverLauncher->startServer(entry, actualPort)) {
            if (!m_isHost) {
                m_isHost = true;
                qDebug() << "I'm a host now!";
            }
            networkManager->tryConnectToHost(entry, actualPort);
        }
    }
}

void AppController::onConnected()
{
    emit scanProgressCanceled();

    if (m_isHost) {
        QByteArray hostAnnouncement = JsonMessageBuilder::createHostStatusMessage(m_isHost);
        networkManager->sendMessageToServer(hostAnnouncement);
        return;
    }
    onRequestClientName();
}

void AppController::onDisconnected()
{
    qDebug() << "Disconnected. Reconnecting...";
    isDiscDisconnected = true;
    m_isHost = false;
}

void AppController::onMessageReceived(const QByteArray &data)
{
    messageHandler->process(data);
}

void AppController::onServerShuttingDown(const QHostAddress &host, const QString &newHostName, const QString &clientName)
{
    QNetworkAddressEntry newEntry;
    newEntry.setIp(host);
    newEntry.setNetmask(m_entry.netmask());
    newEntry.setBroadcast(m_entry.broadcast());

    isDiscDisconnected = false;

    networkManager->disconnectFromServer();

    if (!isDiscDisconnected)
        return;
    qDebug() << "I'm a host or I'm not.";
    qDebug() << "newHostName = " << newHostName << "\t ClietName = " << clientName;

    if (newHostName.contains(clientName)){
        qDebug() << "I'm the host";
        //emit showMessage("Ошибка", "Мы сейчас перезапустим хоста на порту = " + QString::number(m_port), QMessageBox::Critical);
        if (serverLauncher->startServer(newEntry, m_port)) {
            if (!m_isHost) {
                m_isHost = true;
                qDebug() << "I'm the host";
            }
            networkManager->tryConnectToHost(newEntry, m_port);
        }else {
            emit showMessage("Ошибка", "Не удалось запустить сервер", QMessageBox::Critical);
        }
    }
    else {
        connect(networkManager, &NetworkManager::reconnectSucceeded, this, [this]() {
            qDebug() << "Connection to the new server is successful!";
        });

        connect(networkManager, &NetworkManager::reconnectFailed, this, [this]() {
            emit showMessage("Ошибка", "Не удалось подключиться к серверу", QMessageBox::Critical);
        });

        emit scanProgressStarted();
        discoveryManager->startUdp();
        networkManager->scanAvailablePortsAsync(newEntry, m_port, ScanMode::SingleIpOnly, 254);
    }
}

void AppController::onSendMessage(const QByteArray &jsonData)
{
    if (networkManager)
        networkManager->sendMessageToServer(jsonData);
}

void AppController::onChatMessageReceived(MessageSource source, const QString &name, const QString &message, const QDateTime &dateTime)
{
    emit chatMessageReceived(source, name, message, dateTime);
}

void AppController::onClientListReceived(const QStringList &names)
{
    emit clientListReceived(names);
}

void AppController::onErrorOccurred(const QString &title, const QString &message, QMessageBox::Icon icon)
{
    emit showMessage(title, message, icon);
}

void AppController::onRequestInterface(const QHostAddress &host)
{
    hostInfo->findInterfaceForHost(host);
}

void AppController::onInterfaceReady(const QNetworkInterface &interface)
{
    serverLauncher->onInterfaceAvailable(interface);
}

void AppController::onRequestClientName()
{
    QString clientName = messageHandler->getClientName();
    if (!clientName.isEmpty())
    {
        QByteArray jsonData = JsonMessageBuilder::createNicknameChange(MessageType::SetName, clientName);
        networkManager->sendMessageToServer(jsonData);
    }
    else
    {
        emit requestShowNamePage();
    }
}

void AppController::stopScan()
{
    qDebug() <<"Scan stoppped";
    networkManager->stopScan();
    onNoAvailablePortFound(m_entry, m_port, m_mode);
}

void AppController::onScanProgressUpdated(int current, int max)
{
    emit scanProgressChanged(current, max);
}
