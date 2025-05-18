#include "network_manager.h"
#include "client/ui/helpers/message_helper.h"
#include "helpers/message_enum/host_binding_mode.h"
#include "helpers/message_enum/scan_mode.h"

#include <QMessageBox>
#include <QtConcurrent>
#include <QFuture>
#include <QFutureWatcher>

NetworkManager::NetworkManager(QObject *parent)
    : QObject{parent}, m_socket(new QTcpSocket(this))
{

}

void NetworkManager::tryConnectToHost(const QNetworkAddressEntry &entry, quint16 port)
{
    if (isConnected()) return;

    m_entry = entry;
    m_host = entry.ip();
    m_port = port;

    if (m_socket->state() != QAbstractSocket::UnconnectedState) {
        m_socket->abort();
    }

    if (m_socket) {
        m_socket->deleteLater();
    }

    m_socket = new QTcpSocket(this);
    connect(m_socket, &QTcpSocket::connected, this, &NetworkManager::onConnected);
    connect(m_socket, &QTcpSocket::disconnected, this, &NetworkManager::onDisconnected);
    connect(m_socket, &QTcpSocket::readyRead, this, &NetworkManager::onReadyRead);
    connect(m_socket, &QTcpSocket::errorOccurred, this, &NetworkManager::onErrorOccurred);
    //qDebug() << "Trying to connect to the host now: " << m_host << "порт: " << m_port;
    m_socket->connectToHost(m_host, m_port);
}


void NetworkManager::sendMessageToServer(const QByteArray jsonData)
{
    if (m_socket->state() == QAbstractSocket::ConnectedState) {

        if (!jsonData.isEmpty())
        {
            m_data.clear();
            QDataStream out(&m_data, QIODevice::WriteOnly);
            out.setVersion(QDataStream::Qt_6_7);

            out << quint16(0);
            out.writeRawData(jsonData.constData(), jsonData.size());


            out.device()->seek(0);
            out << quint16(m_data.size() - sizeof(quint16));

            if (m_data != nullptr)
            {
                m_socket->write(m_data);
            }
            else
            {
                emit errorOccurred("Ошибка", "Пустое собщение", QMessageBox::Warning);
            }
        }
        else
        {
            emit errorOccurred("Ошибка", "Не правильное сообщение имя", QMessageBox::Warning);
        }
    }
}

QTcpSocket *NetworkManager::socket() const
{
    return m_socket;
}

void NetworkManager::disconnectFromServer()
{
    if (m_socket && m_socket->isOpen()) {
        m_socket->disconnectFromHost();

        if (m_socket->state() != QAbstractSocket::UnconnectedState) {
            if (m_socket->waitForDisconnected(3000)) {
                qDebug() << "Disconnected from the server";
            }
        }
    }
}

bool NetworkManager::isConnected() const
{
    return m_socket && m_socket->state() == QAbstractSocket::ConnectedState;
}

bool NetworkManager::testPortConnection(const QHostAddress &host, quint16 port)
{
    if (portFound.load(std::memory_order_acquire)) {
        //qDebug() << "exit since it is already true from tryToConnect. Port number: “ << port;
        return false;
    }

    QTcpSocket socket;
    //qDebug() << "Here we are trying to connect to the Host: “ << host.toString() << ”Port: " + QString::number(port);
    socket.connectToHost(host, port);


    if (socket.waitForConnected(30))
    {
        bool expected = false;
        if (portFound.compare_exchange_strong(expected, true, std::memory_order_release)) {
            qDebug() << "Port found: " << port;
            qDebug() << "Host =. " << host;
            m_host = host;
            m_entry.setIp(m_host);
            socket.disconnectFromHost();
            return true;
        }
        socket.abort();
        return false;
    }
    else
    {
        socket.abort();
        return false;
    }
}

QHostAddress NetworkManager::getHost() const
{
    return m_host;
}

quint16 NetworkManager::getPort() const
{
    return m_port;
}

QNetworkAddressEntry NetworkManager::getEntry() const
{
    return m_entry;
}

void NetworkManager::scanAvailablePortsAsync(const QNetworkAddressEntry &entry, quint16 port, ScanMode mode, int maxAttempts)
{
    // Reset portFound when starting a new scan
    portFound.store(false, std::memory_order_release);

    m_entry = entry;
    m_host = entry.ip();
    m_port = port;
    m_currentScanMode = mode;
    m_maxScanAttempts = maxAttempts;
    m_currentScanAttempt = 0;

    tryScanAgain();
}

void NetworkManager::stopScan()
{
    portFound.store(true, std::memory_order_release);
}

void NetworkManager::tryScanAgain()
{
    if (portFound.load(std::memory_order_acquire)) return;

    if (m_currentScanAttempt >= m_maxScanAttempts) {
        qWarning() << "The maximum number of scan attempts has been reached.";
        if (isConnected()) return;
        emit noAvailablePortFound(m_entry, m_port, HostBindingMode::DynamicPort);
        return;
    }

    m_currentScanAttempt++;
    //qDebug() << "Scan attempt #" << m_currentScanAttempt;

    // If we need to know the scanning time
/*  static QElapsedTimer timer;
    static bool timerStarted = false;

    if (!timerStarted) {
        timer.start();
        timerStarted = true;
    }
*/

    m_progressCounter = 0;

    QList<QPair<QHostAddress, quint16>> targets = generateTargets(m_entry, m_currentScanMode);
    m_progressMaxCounter = m_maxScanAttempts * targets.size();

    QFuture<QPair<QHostAddress, quint16>> future = QtConcurrent::mappedReduced(
        targets,
        [this](const QPair<QHostAddress, quint16>& target) -> QPair<QHostAddress, quint16> {
            int progress = ++m_progressCounter;
            QMetaObject::invokeMethod(this, [=]() {
                emit scanProgressUpdated(progress, m_progressMaxCounter);
            }, Qt::QueuedConnection);

            if (testPortConnection(target.first, target.second)) {
                return target;
            }
            return {};
        },
        [this](QPair<QHostAddress, quint16>& result, const QPair<QHostAddress, quint16>& intermediate) {
            if (!intermediate.first.isNull()) {
                result = intermediate;
            }
        },
        QtConcurrent::OrderedReduce
        );

    QFutureWatcher<QPair<QHostAddress, quint16>>* watcher = new QFutureWatcher<QPair<QHostAddress, quint16>>(this);
    connect(watcher, &QFutureWatcher<QPair<QHostAddress, quint16>>::finished, this, [=]() {
        QPair<QHostAddress, quint16> result = future.result();
        //qDebug() << "Total scanning time:" << timer.elapsed() << "мс";

        if (!result.first.isNull() && result.second != 0) {
            m_host = result.first;
            m_port = result.second;
            if (isConnected()) return;
            emit portAvailable(m_entry, m_port, HostBindingMode::DynamicPort);
        } else {
            QTimer::singleShot(m_scanRetryDelayMs, this, [this]() {
                tryScanAgain();
            });
        }
        //timerStarted = false;
        watcher->deleteLater();
    });

    watcher->setFuture(future);
}

QList<QPair<QHostAddress, quint16> > NetworkManager::generateTargets(const QNetworkAddressEntry &entry, ScanMode mode)
{
    QList<QPair<QHostAddress, quint16>> targets;

    if (entry.ip().protocol() != QAbstractSocket::IPv4Protocol)
        return targets;

    QList<quint16> ports;
    for (quint16 port = 50500; port < 50515; ++port) {
        ports.append(port);
    }

    if (mode == ScanMode::SingleIpOnly) {
        // Specified IP only
        for (quint16 port : ports) {
            targets.append({entry.ip(), port});
        }
        return targets;
    }

    quint32 ipInt = entry.ip().toIPv4Address();
    // Get the length of the mask prefix, e.g. 24 for /24
    int prefixLength = entry.prefixLength();

    if (prefixLength == -1) {
        qWarning() << "Invalid prefix length, defaulting to /24";
        prefixLength = 24;
    }

    // If the mask is too wide (less than /24), narrow it to /24
    if (prefixLength < 24) {
        qWarning() << "Prefix length too short (" << prefixLength << "), restricting to /24 range.";
        prefixLength = 24;

        quint32 mask24 = 0xFFFFFF00; // Mask /24 as a 32-bit value
        quint32 subnet24 = ipInt & mask24;
        quint32 broadcast24 = subnet24 | ~mask24;

        QHostAddress firstIp(subnet24);
        QHostAddress lastIp(broadcast24);

        qInfo() << "Operating within /24 range:" << firstIp.toString() << "-" << lastIp.toString();
    }

    quint32 maskInt = 0xFFFFFFFF << (32 - prefixLength);
    quint32 subnetInt = ipInt & maskInt;
    quint32 broadcastInt = subnetInt | ~maskInt;

    for (quint32 addr = subnetInt + 1; addr < broadcastInt; ++addr) {
        QHostAddress targetIp(addr);

        // Try multiple ports for each IP address
        for (quint16 port : ports) {
            targets.append({targetIp, port});
        }
    }

    return targets;
}

void NetworkManager::onConnected()
{
    emit connected();
}

void NetworkManager::onDisconnected()
{
    //qDebug() << "Failed to connect to the server";
    portFound.store(false, std::memory_order_relaxed);
    emit disconnected();
}

void NetworkManager::onReadyRead()
{
    QDataStream in(m_socket);
    in.setVersion(QDataStream::Qt_6_7);

    if (in.status() != QDataStream::Ok) {
        emit onErrorOccurred(QAbstractSocket::SocketAccessError);
        return;
    }

    if (in.status() == QDataStream::Ok)
    {

        for (;;) {
            if (m_nextBlockSize == 0) {
                if (m_socket->bytesAvailable() < 2) {
                    break;
                }
                in >> m_nextBlockSize;
            }
            if (m_socket->bytesAvailable() < m_nextBlockSize)
            {
                break;
            }

            QByteArray jsonData;
            jsonData.resize(m_nextBlockSize);
            in.readRawData(jsonData.data(), m_nextBlockSize);

            m_nextBlockSize = 0;

            emit messageReceived(jsonData);
        }
    }
    else {
        emit errorOccurred("Ошибка", "Не правильное сообщение полученное от сервера", QMessageBox::Warning);
    }
}

void NetworkManager::onErrorOccurred(QAbstractSocket::SocketError socketError)
{
    Q_UNUSED(socketError)
    emit errorOccurred( "Ошибка", m_socket->errorString(), QMessageBox::Critical);
}
