#pragma once

#include "helpers/message_enum/message_source.h"
#include "helpers/message_enum/message_type.h"
#include "helpers/dictionary/server_message_dictionary.h"

#include <QObject>
#include <QHostAddress>

class MessageHandler : public QObject
{
    Q_OBJECT
public:
    explicit MessageHandler(QObject *parent = nullptr);
    void process(const QByteArray &jsonData);

    QString getClientName() const;
    void setClientName(const QString &name);

    QStringList getNames() const;
    void setNames(const QStringList &names);

    void removeName(const QString &name);

private:
    QStringList m_names;
    QStringList m_knownNames;
    QString m_clientName;
    QString m_messageFromName;

    ServerMessageDictionary messageDict;

    bool parseJson(const QByteArray &data, QJsonObject &obj);
    bool handleServerShutdown(const QJsonObject &jsonObj, MessageType type);
    bool handleRequestName(const QJsonObject &jsonObj, MessageType type);
    void handleKnownClientList(const QString &message);
    bool handleClientList(MessageType type, const QString &message);
    bool handleNameChange(const QJsonObject &jsonObj, MessageType type, QDateTime &dateTime, QString &message);
    QString handleChangeName(MessageType type, const QDateTime &dateTime, const QString &newName, const QString &oldName = QString());
    void dispatchMessage(MessageSource source, const QDateTime &dateTime, const QString &message);

signals:
    void serverShuttingDown(const QHostAddress &host ,const QString &newHostName, const QString &clientName);
    void clientListReceived(const QStringList& names);
    void chatMessageReceived(MessageSource source, const QString &name, const QString &message, const QDateTime &dateTime);
    void requestClientNameForHost();
};
