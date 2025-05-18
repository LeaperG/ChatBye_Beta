#pragma once

#include <QString>
#include <QObject>

enum class MessageSource {
    Server,
    Client,
    System,
    Bot
};

QString messageSourceToString(MessageSource source);
MessageSource stringToMessageSource(const QString &str);

