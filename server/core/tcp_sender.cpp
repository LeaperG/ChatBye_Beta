#include "tcp_sender.h"

#include <QDataStream>
#include <QByteArray>
#include <QTcpSocket>

TcpSender::TcpSender(QObject *parent)
    : QObject{parent}
{}

void TcpSender::sendToClient(const QByteArray jsonData)
{
    if (m_sockets.isEmpty()) return; // No clients - out

    Data.clear();
    QDataStream out(&Data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_7);

    out << quint16(0);
    out.writeRawData(jsonData.constData(), jsonData.size());


    out.device()->seek(0);
    out << quint16(Data.size() - sizeof(quint16));

    m_pendingClients = m_sockets.size(); // Set the number of active clients

    for (QTcpSocket* socket : m_sockets)
    {
        if (socket->state() == QAbstractSocket::ConnectedState) {
            socket->write(Data);
            socket->flush();

            // Connect the bytesWritten signal for tracking the send
            connect(socket, &QTcpSocket::bytesWritten, this, &TcpSender::checkAllDataSent);
        } else {
            m_pendingClients--; // Decrease the counter since the socket is inactive
        }
    }
}

void TcpSender::addClientSocket(QTcpSocket *socket)
{
    m_sockets.push_back(socket);
}

bool TcpSender::removeClientSocket(QTcpSocket *socket)
{
    return m_sockets.removeOne(socket);
}

void TcpSender::removeAllClientSocket()
{
    for (QTcpSocket* socket : m_sockets) {
        socket->close();
        socket->deleteLater();
    }

    m_sockets.clear();
}

void TcpSender::shutdownServerAndNotifyClients(const QByteArray jsonData)
{
    sendToClient(jsonData);
    for (QTcpSocket* clientSocket : m_sockets) {
        if (clientSocket->isOpen()) {
            clientSocket->disconnectFromHost();
            if (clientSocket->state() != QAbstractSocket::UnconnectedState) {
                clientSocket->waitForDisconnected(3000);
            }
        }
    }
    removeAllClientSocket();
}

void TcpSender::checkAllDataSent(qint64 bytes)
{
    QTcpSocket* senderSocket = qobject_cast<QTcpSocket*>(sender());
    if (senderSocket && senderSocket->bytesToWrite() == 0) {
        // The data has been completely sent to the client
        disconnect(senderSocket, &QTcpSocket::bytesWritten, this, &TcpSender::checkAllDataSent);

        m_pendingClients--;
        if (m_pendingClients == 0) {
            // qDebug() << "All messages have been successfully sent.";
        }
    }
}
