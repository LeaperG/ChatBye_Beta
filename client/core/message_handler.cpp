#include "client/core/message_handler.h"
#include "helpers/message_enum/message_type.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QHostAddress>

MessageHandler::MessageHandler(QObject *parent)
    : QObject{parent}
{

}

void MessageHandler::process(const QByteArray &jsonData)
{
    QJsonObject jsonObj;
    if (!parseJson(jsonData, jsonObj)) return;

    QString message = jsonObj.value("message").toString();
    QString sourceString = jsonObj.value("source").toString();
    MessageSource messageSource = stringToMessageSource(sourceString);
    QString dateString  = jsonObj.value("date").toString();
    QDateTime dateTime = QDateTime::fromString(dateString, "yyyy-MM-dd hh:mm:ss");
    QString typeString = jsonObj.value("type").toString();
    MessageType messageType = stringToMessageType(typeString);
    if (jsonObj.contains("from"))
        m_messageFromName = jsonObj.value("from").toString();

    if (handleRequestName(jsonObj, messageType)) return;
    if (handleServerShutdown(jsonObj, messageType)) return;
    if (handleClientList(messageType, message)) return;
    if (handleNameChange(jsonObj, messageType, dateTime, message)) return;

    dispatchMessage(messageSource, dateTime, message);
}

bool MessageHandler::parseJson(const QByteArray &data, QJsonObject &obj)
{
    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (doc.isNull() || !doc.isObject()) {
        qDebug() << "Ошибка парсинга JSON";
        return false;
    }
    obj = doc.object();
    return true;
}

bool MessageHandler::handleServerShutdown(const QJsonObject &jsonObj, MessageType type)
{
    if (type != MessageType::ServerShuttingDown)
        return false;

    if (jsonObj.contains("client_names"))
        handleKnownClientList(jsonObj.value("client_names").toString());

    if (!jsonObj.contains("host"))
        return false;


    QString hostString = jsonObj.value("host").toString();
    QHostAddress host = QHostAddress(hostString);
    QString hostName = jsonObj.value("host_name").toString();
    QDateTime now = QDateTime::currentDateTime();
    emit chatMessageReceived(MessageSource::Server, "Сервер", messageDict.getMessage(1000), now);
    emit serverShuttingDown(host, hostName, m_clientName);
    return true;
}

bool MessageHandler::handleRequestName(const QJsonObject &jsonObj, MessageType type)
{
    if (type != MessageType::RequestName)
        return false;

    emit requestClientNameForHost();
    return true;
}

void MessageHandler::handleKnownClientList(const QString &message)
{
    QString clientList = message;
    m_knownNames = clientList.split(", ");
}

bool MessageHandler::handleClientList(MessageType type, const QString &message)
{
    if (type != MessageType::ClientList)
        return false;

    m_names = message.split(", ");
    emit clientListReceived(m_names);
    return true;
}

bool MessageHandler::handleNameChange(const QJsonObject &jsonObj, MessageType type, QDateTime &dateTime, QString &message)
{
    if (type != MessageType::SetName && type != MessageType::ChangeName)
        return false;

    QString oldName = jsonObj.value("old_name").toString();
    QString newName = jsonObj.value("new_name").toString();

    // Check for reconnection
    if (m_knownNames.contains(newName)) {
        m_knownNames.removeAll(newName);
        return true;
    }

    if (m_clientName == newName) return true;

    message = handleChangeName(type, dateTime, newName, oldName);
    m_messageFromName = oldName;
    return false;
}

QString MessageHandler::handleChangeName(MessageType type, const QDateTime &dateTime, const QString &newName, const QString &oldName)
{
    if (MessageType::SetName == type)
    {
        return oldName + messageDict.getMessage(1400) + newName;
    }
    else {
        return newName + messageDict.getMessage(1400) + oldName;
    }
}

void MessageHandler::dispatchMessage(MessageSource source, const QDateTime &dateTime, const QString &message)
{   // Check if the message should be sent to the UI
    if ((m_clientName != m_messageFromName && !m_clientName.isEmpty()) ||
        (source == MessageSource::Server && !m_clientName.isEmpty()))
    {
        emit chatMessageReceived(source, m_messageFromName.trimmed(), message.trimmed(), dateTime);
    }
}

void MessageHandler::setClientName(const QString &name)
{
    m_clientName = name;
    QDateTime dateTime = QDateTime::currentDateTime();
    emit chatMessageReceived(MessageSource::Server, m_clientName.trimmed(), messageDict.getMessage(600).trimmed(), dateTime);
}

QStringList MessageHandler::getNames() const
{
    return m_names;
}

void MessageHandler::setNames(const QStringList &names)
{
    m_names = names;
}

void MessageHandler::removeName(const QString &name)
{
    m_names.removeAll(name);
}

QString MessageHandler::getClientName() const
{
    return m_clientName;
}
