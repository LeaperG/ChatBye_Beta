#include "json_message_builder.h"
#include "helpers/message_enum/message_source.h"
#include "helpers/message_enum/message_type.h"
#include <qnetworkinterface.h>

JsonMessageBuilder::JsonMessageBuilder() {}

QByteArray JsonMessageBuilder::createMessage(const MessageSource source, const QString &from, const QString &message)
{
    QJsonObject messageObj;
    messageObj["type"] = messageTypeToString(MessageType::Message);
    messageObj["date"] = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    messageObj["from"] = from;
    messageObj["source"] = messageSourceToString(source);
    messageObj["message"] = message;

    QJsonDocument doc(messageObj);
    return doc.toJson();
}

QByteArray JsonMessageBuilder::createRequestName()
{
    QJsonObject messageObj;
    messageObj["type"] = messageTypeToString(MessageType::RequestName);
    messageObj["date"] = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    messageObj["source"] = messageSourceToString(MessageSource::System);

    QJsonDocument doc(messageObj);
    return doc.toJson();
}

QByteArray JsonMessageBuilder::createNicknameChange(const MessageType type, const QString &newName, const QString &oldName)
{
    QJsonObject messageObj;
    messageObj["type"] = messageTypeToString(type);
    messageObj["date"] = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    messageObj["source"] = messageSourceToString(MessageSource::Server);

    if (!oldName.isEmpty()) {  // Add only if oldName is not empty
        messageObj["old_name"] = oldName;
    }
    messageObj["new_name"] = newName;

    QJsonDocument doc(messageObj);
    return doc.toJson();
}


QByteArray JsonMessageBuilder::createSystemMessage(const QString &message)
{
    QJsonObject messageObj;
    messageObj["type"] = messageTypeToString(MessageType::Message);
    messageObj["date"] = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    messageObj["source"] = messageSourceToString(MessageSource::Server);
    messageObj["message"] = message;

    QJsonDocument doc(messageObj);
    return doc.toJson();
}

QByteArray JsonMessageBuilder::createServerShuttingDown(const QHostAddress &host, const QString &message, const QString &newHostName, QString &clientNames)
{
    QJsonObject messageObj;
    messageObj["type"] = messageTypeToString(MessageType::ServerShuttingDown);
    messageObj["date"] = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    messageObj["source"] = messageSourceToString(MessageSource::Server);
    messageObj["client_names"] = clientNames;
    messageObj["host"] = host.toString();
    messageObj["host_name"] = newHostName;
    messageObj["message"] = message;

    QJsonDocument doc(messageObj);
    return doc.toJson();
}

QByteArray JsonMessageBuilder::sendServerDetails(const QString &message, const QNetworkAddressEntry &entry, const quint16 port)
{
    QJsonObject messageObj;
    messageObj["message"] = message;
    messageObj["host_address"] = entry.ip().toString();
    messageObj["netmask"] = entry.netmask().toString();
    messageObj["broadcast"] = entry.broadcast().toString();
    messageObj["port"] = QString::number(port);

    QJsonDocument doc(messageObj);
    return doc.toJson();
}

QByteArray JsonMessageBuilder::sendClientListName(const QString &message)
{
    QJsonObject messageObj;
    messageObj["type"] = messageTypeToString(MessageType::ClientList);
    messageObj["date"] = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    messageObj["source"] = messageSourceToString(MessageSource::System);
    messageObj["message"] = message;

    QJsonDocument doc(messageObj);
    return doc.toJson();
}

QByteArray JsonMessageBuilder::createHostStatusMessage(bool isHost)
{
    QJsonObject messageObj;
    messageObj["type"] = messageTypeToString(MessageType::HostStatus);
    messageObj["source"] = messageSourceToString(MessageSource::System);
    messageObj["isHost"] = isHost;
    return QJsonDocument(messageObj).toJson(QJsonDocument::Compact);
}







