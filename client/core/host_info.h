#pragma once

#include "helpers/message_enum/interface_Filter.h"
#include <QObject>
#include <QHostAddress>
#include <QNetworkAddressEntry>

class HostInfo : public QObject
{
    Q_OBJECT
public:
    explicit HostInfo(QObject *parent = nullptr);
    void getIpAddresses(InterfaceFilter filter);
    void getNetworkEntries(InterfaceFilter filter);
    void findInterfaceForHost(const QHostAddress& host);

signals:
    void hostAddressesReady(const QList<QHostAddress>& addresses);
    void hostEntriesReady(const QList<QNetworkAddressEntry>& entryList);
    void interfaceReady(const QNetworkInterface& interface);
    void interfaceFailed();
};

