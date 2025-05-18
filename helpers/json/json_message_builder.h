#pragma once

#include "helpers/message_enum/message_source.h"
#include "helpers/message_enum/message_type.h"
#include <QByteArray>
#include <QString>
#include <QJsonDocument>
#include <QJsonObject>
#include <QHostAddress>
#include <qnetworkinterface.h>

class JsonMessageBuilder
{
public:
    JsonMessageBuilder();

    static QByteArray createMessage(const MessageSource source, const QString &from, const QString &message);

    static QByteArray createNicknameChange(const MessageType type, const QString &newName, const QString &oldName = QString());

    static QByteArray createSystemMessage(const QString &message);

    static QByteArray createServerShuttingDown(const QHostAddress &host, const QString &message,const QString &newHostName, QString &clientNames);

    static QByteArray sendServerDetails(const QString &message, const QNetworkAddressEntry &entry, const quint16 port);

    static QByteArray sendClientListName(const QString &message);

    static QByteArray createHostStatusMessage(bool isHost);

    static QByteArray createRequestName();


};
