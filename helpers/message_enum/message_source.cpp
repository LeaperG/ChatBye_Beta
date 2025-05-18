#include "message_source.h"

QString messageSourceToString(MessageSource source)
{
    switch (source) {
    case MessageSource::Server: return "Server";
    case MessageSource::Client: return "Client";
    case MessageSource::System: return "System";
    case MessageSource::Bot: return "Bot";
    default: return "Unknown";
    }
}

MessageSource stringToMessageSource(const QString &str)
{
    if (str == "Server") return MessageSource::Server;
    if (str == "Client") return MessageSource::Client;
    if (str == "System") return MessageSource::System;
    if (str == "Bot") return MessageSource::Bot;
    return MessageSource::Server;
}
