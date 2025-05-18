#pragma once

#include <QString>

enum class MessageType {
    Message,
    SetName,
    ChangeName,
    ServerShuttingDown,
    HostStatus,
    RequestName,
    ClientList,
    Image,
    File,
    System
};

QString messageTypeToString(MessageType Type);
MessageType stringToMessageType(const QString &str);

