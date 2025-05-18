#include "message_dispatcher.h"
#include "helpers/message_enum/message_source.h"
#include "helpers/message_enum/message_type.h"
#include <qdatetime.h>
#include <qjsondocument.h>
#include <qjsonobject.h>
#include <qtcpsocket.h>

MessageDispatcher::MessageDispatcher(QObject *parent)
    : QObject{parent}
{}

void MessageDispatcher::processMessage(const QByteArray &jsonData, QTcpSocket *socket)
{
    QString message;
    MessageSource messageSource;
    MessageType messageType;
    QDateTime dateTime;

    // Парсим JSON
    QJsonDocument doc = QJsonDocument::fromJson(jsonData);
    if (!doc.isNull() && doc.isObject()) {
        QJsonObject jsonObj = doc.object();
        message = jsonObj.value("message").toString();
        QString sourceString = jsonObj.value("source").toString();
        messageSource = stringToMessageSource(sourceString);
        QString dateString  = jsonObj.value("date").toString();
        dateTime = QDateTime::fromString(dateString, "yyyy-MM-dd hh:mm:ss");
        QString typeString = jsonObj.value("type").toString();
        messageType = stringToMessageType(typeString);

        if (jsonObj.contains("from")) {
            //messageFromName = jsonObj.value("from").toString();
        }
    }
    else {
        qDebug() << "JSON parsing error";
    }

    if (messageType == MessageType::HostStatus) {
        emit hostStatusReceived(socket);
        return;
    }

    if (MessageType::ChangeName == messageType || MessageType::SetName == messageType) {
        QString oldName;
        QString newName;

        if (!doc.isNull() && doc.isObject()) {
            QJsonObject jsonObj = doc.object();
            oldName = jsonObj.value("old_name").toString();
            newName = jsonObj.value("new_name").toString();
        }
        emit changeNameRequested(messageType, dateTime, newName, oldName, socket);
    }
    else {
        if (!message.isEmpty()) {
            emit clientSentMessage(message, socket);
        }
        else {
            emit unnamedClientTriedToSend();
        }
    }
}

