#pragma once

#include "helpers/message_enum/interface_filter.h"
#include "helpers/message_enum/host_binding_mode.h"

#include "client/core/discovery_manager.h"
#include "client/core/network_manager.h"
#include "client/core/server_launcher.h"
#include "client/core/message_handler.h"
#include "client/core/host_info.h"

#include <QObject>

class AppController : public QObject
{
    Q_OBJECT
public:
    explicit AppController(QObject *parent = nullptr);
    void setDependencies(NetworkManager *net, DiscoveryManager *disc, ServerLauncher *serv, MessageHandler *msg, HostInfo *host);
    bool isHost() const;

private:
    NetworkManager* networkManager = nullptr;
    DiscoveryManager* discoveryManager = nullptr;
    ServerLauncher* serverLauncher = nullptr;
    MessageHandler* messageHandler = nullptr;
    HostInfo* hostInfo = nullptr;

    bool m_isHost = false;
    bool isDiscDisconnected = false;
    QNetworkAddressEntry m_entry;
    quint16 m_port;
    HostBindingMode m_mode;

public slots:
    void onRequestHost(InterfaceFilter filter);
    void onHostEntriesReady(const QList<QNetworkAddressEntry>& entryList);

    void onHostAndPortEntered(const QNetworkAddressEntry &entry, quint16 port, HostBindingMode mode);

    void onServerFound(const QNetworkAddressEntry &entry, quint16 port, HostBindingMode mode);
    void onDiscoveryFailed(const QNetworkAddressEntry &entry, quint16 port);
    void onPortAvailable(const QNetworkAddressEntry &entry, quint16 port, HostBindingMode mode);
    void onNoAvailablePortFound(const QNetworkAddressEntry &entry, quint16 port, HostBindingMode mode);
    void onConnected();
    void onDisconnected();
    void onMessageReceived(const QByteArray &data);

    void onServerShuttingDown(const QHostAddress &host, const QString &newHostName, const QString &clientName);

    void onSendMessage(const QByteArray &jsonData);
    void onChatMessageReceived(MessageSource source, const QString &name, const QString &message, const QDateTime &dateTime);
    void onClientListReceived(const QStringList& names);

    void onErrorOccurred(const QString &title, const QString &message, QMessageBox::Icon icon);

    void onRequestInterface(const QHostAddress& host);
    void onInterfaceReady(const QNetworkInterface& interface);
    void onRequestClientName();

    void stopScan();


private slots:
    void onScanProgressUpdated(int current, int max);


signals:
    void hostEntriesReadyAvailable(const QList<QNetworkAddressEntry> &entryList);
    void requestShowNamePage();
    void showMessage(const QString &title, const QString &message, QMessageBox::Icon icon);
    void chatMessageReceived(MessageSource source, const QString &name, const QString &message, const QDateTime &dateTime);
    void clientListReceived(const QStringList& names);

    void scanProgressChanged(int current, int max);

    void scanProgressCanceled();
    void scanProgressStarted();

};

