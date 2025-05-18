#pragma once

#include "helpers/message_enum/host_binding_mode.h"
#include "server/server.h"
#include <QObject>
#include <QHostAddress>
#include <QMessageBox>
#include <QNetworkAddressEntry>

class ServerLauncher : public QObject
{
    Q_OBJECT
public:
    explicit ServerLauncher(QObject *parent = nullptr);
    bool startServer(const QNetworkAddressEntry &entry, quint16 &outPort);
    bool startServerOnFixedPort(const QNetworkAddressEntry &entry, quint16 &outPort, HostBindingMode mode);
    void stopServer();
    bool isRunning() const;

private:
    Server *m_server = nullptr;
    QNetworkAddressEntry m_entry = QNetworkAddressEntry();
    QHostAddress m_host = QHostAddress::Any;
    quint16 m_port;

private slots:
    void onRequestInterface(const QHostAddress& host);
public slots:
    void onInterfaceAvailable(const QNetworkInterface& interface);

signals:
    void errorOccurred(const QString& title, const QString& message, QMessageBox::Icon icon);
    void requestInterface(const QHostAddress& host);
    void interfaceAvailable(const QNetworkInterface& interface);
};

