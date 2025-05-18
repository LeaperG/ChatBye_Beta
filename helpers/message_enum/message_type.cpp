#include "message_type.h"

QString messageTypeToString(MessageType Type)
{
    switch (Type) {
    case MessageType::Message: return "message";
    case MessageType::SetName: return "set_name";
    case MessageType::ChangeName: return "change_name";
    case MessageType::ServerShuttingDown: return "Server_Shutting_Down";
    case MessageType::File: return "file";
    case MessageType::Image: return "image";
    case MessageType::System: return "System";
    case MessageType::HostStatus: return "host_status";
    case MessageType::RequestName: return "request_name";
    case MessageType::ClientList: return "client_list";
    default: return "Unknown";
    }
}

MessageType stringToMessageType(const QString &str)
{
    if (str == "message") return MessageType::Message;
    if (str == "set_name") return MessageType::SetName;
    if (str == "change_name") return MessageType::ChangeName;
    if (str == "Server_Shutting_Down") return MessageType::ServerShuttingDown;
    if (str == "file") return MessageType::File;
    if (str == "image") return MessageType::Image;
    if (str == "system") return MessageType::System;
    if (str == "host_status") return MessageType::HostStatus;
    if (str == "request_name") return MessageType::RequestName;
    if (str == "client_list") return MessageType::ClientList;
    return MessageType::Message;
}
