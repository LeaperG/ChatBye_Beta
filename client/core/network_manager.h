#pragma once

#include "helpers/message_enum/host_binding_mode.h"
#include "helpers/message_enum/scan_mode.h"
#include <QObject>
#include <QTcpSocket>
#include <QHostAddress>
#include <QMessageBox>
#include <qnetworkinterface.h>

class NetworkManager : public QObject
{
    Q_OBJECT
public:
    explicit NetworkManager(QObject *parent = nullptr);
    void sendMessageToServer(const QByteArray jsonData);
    void tryConnectToHost(const QNetworkAddressEntry &entry, quint16 port);
    void disconnectFromServer();
    void scanAvailablePortsAsync(const QNetworkAddressEntry &entry, quint16 port, ScanMode mode, int maxAttempts);
    void stopScan();
    bool isConnected() const;

    QTcpSocket* socket() const;
    QHostAddress getHost() const;
    quint16 getPort() const;
    QNetworkAddressEntry getEntry() const;

private:
    QTcpSocket *m_socket = nullptr;
    QByteArray m_data = nullptr;
    quint16 m_nextBlockSize = 0;

    QNetworkAddressEntry m_entry;
    QHostAddress m_host;
    quint16 m_port;
    ScanMode m_currentScanMode;
    int m_maxScanAttempts = 1;
    int m_currentScanAttempt = 0;
    int m_scanRetryDelayMs = 500;
    std::atomic_bool portFound = false;

    // To check for free ports
    void tryScanAgain();
    QList<QPair<QHostAddress, quint16>> generateTargets(const QNetworkAddressEntry &entry, ScanMode mode);
    bool testPortConnection(const QHostAddress &host, quint16 port);

    std::atomic<int> m_progressCounter = 0;
    std::atomic<int> m_progressMaxCounter = 0;

signals:
    void connected();
    void disconnected();
    void messageReceived(const QByteArray &data);
    void errorOccurred(const QString& title, const QString& message, QMessageBox::Icon icon);
    void portAvailable(const QNetworkAddressEntry &entry, quint16 port, HostBindingMode mode);
    void noAvailablePortFound(const QNetworkAddressEntry &entry, quint16 port, HostBindingMode mode);
    void reconnectSucceeded();
    void reconnectFailed();
    void scanProgressUpdated(int currentAttempt, int maxAttempts);

private slots:
    void onConnected();
    void onDisconnected();
    void onReadyRead();
    void onErrorOccurred(QAbstractSocket::SocketError socketError);

};
