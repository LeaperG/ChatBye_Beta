#include "host_info.h"

#include <QNetworkInterface>

HostInfo::HostInfo(QObject *parent)
    : QObject{parent}
{}

void HostInfo::getIpAddresses(InterfaceFilter filter)
{
    QList<QHostAddress> ipList;

    const auto interfaces = QNetworkInterface::allInterfaces();

    for (const QNetworkInterface& interface : interfaces) {
        if (interface.flags().testFlag(QNetworkInterface::IsUp) &&
            interface.flags().testFlag(QNetworkInterface::IsRunning) &&
            !interface.flags().testFlag(QNetworkInterface::IsLoopBack)) {

            const QString name = interface.humanReadableName().toLower();
            const bool isWifi =
                interface.type() == QNetworkInterface::InterfaceType::Wifi ||
                name.contains("wlan") || name.contains("wi-fi") ||
                name.contains("wlp") || name.contains("wl");

            // Пропускаем интерфейс, если он не соответствует фильтру
            if ((filter == InterfaceFilter::OnlyWifi && !isWifi) ||
                (filter == InterfaceFilter::OnlyLan && isWifi)) {
                continue;
            }

            for (const QNetworkAddressEntry& entry : interface.addressEntries()) {
                if (entry.ip().protocol() == QAbstractSocket::IPv4Protocol) {
                    ipList << entry.ip();
                }
            }
        }
    }

    if (ipList.isEmpty()) {
        qDebug() << "IP addresses not found for filter:" << static_cast<int>(filter);
    }

    emit hostAddressesReady(ipList);

}

void HostInfo::getNetworkEntries(InterfaceFilter filter)
{
    QList<QNetworkAddressEntry> entryList;

    const auto interfaces = QNetworkInterface::allInterfaces();

    for (const QNetworkInterface& networkInterface : interfaces) {
        if (networkInterface.flags().testFlag(QNetworkInterface::IsUp) &&
            networkInterface.flags().testFlag(QNetworkInterface::IsRunning) &&
            !networkInterface.flags().testFlag(QNetworkInterface::IsLoopBack)) {

            const QString name = networkInterface.humanReadableName().toLower();
            const bool isWifi =
                networkInterface.type() == QNetworkInterface::InterfaceType::Wifi ||
                name.contains("wlan") || name.contains("wi-fi") ||
                name.contains("wlp") || name.contains("wl");

            // Пропускаем интерфейс, если он не соответствует фильтру
            if ((filter == InterfaceFilter::OnlyWifi && !isWifi) ||
                (filter == InterfaceFilter::OnlyLan && isWifi)) {
                continue;
            }

            // Добавляем записи из интерфейса
            for (const QNetworkAddressEntry& entry : networkInterface.addressEntries()) {
                if (entry.ip().protocol() == QAbstractSocket::IPv4Protocol) {
                    entryList << entry; // Собираем записи целиком
                }
            }
        }
    }

    if (entryList.isEmpty()) {
        qDebug() << "No network entries found for filter:" << static_cast<int>(filter);
    }

    // Вызываем сигнал с новым списком записей
    emit hostEntriesReady(entryList);
}

void HostInfo::findInterfaceForHost(const QHostAddress &host)
{
    const auto interfaces = QNetworkInterface::allInterfaces();
    for (const QNetworkInterface& iface : interfaces) {
        for (const QNetworkAddressEntry& entry : iface.addressEntries()) {
            if (entry.ip() == host) {
                emit interfaceReady(iface);
            }
        }
    }
    emit interfaceFailed();
}
