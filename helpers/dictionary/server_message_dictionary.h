#pragma once

#include <QMap>
#include <QString>

class ServerMessageDictionary
{

public:
    ServerMessageDictionary();
    QString getMessage(int code) const;
    void addMessage(int code, const QString &message);

private:
    QMap<int, QString> messages;
    void initializeMessages();
};

