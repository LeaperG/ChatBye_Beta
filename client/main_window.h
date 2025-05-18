#pragma once

#include <QMainWindow>
#include <QStackedWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QTextEdit>
#include <QMessageBox>
#include <QTcpSocket>
#include <QTime>
#include <QStack>


#include "client/app_controller.h"
#include "client/core/discovery_manager.h"
#include "client/core/network_manager.h"
#include "client/core/host_info.h"
#include "helpers/dictionary/server_message_dictionary.h"
#include "helpers/message_enum/host_binding_mode.h"
#include "client/core/message_handler.h"
#include <QtConcurrent/QtConcurrent>

#include "client/pages/settings_page/settings_page.h"
#include "client/pages/port_page.h"
#include "client/pages/name_page.h"
#include "client/pages/chat_page.h"
#include "client/core/server_launcher.h"
#include "client/ui/styles/style_manager.h"

#include <QGuiApplication>
#include <QInputMethod>
#include <QScreen>
#include <QJsonObject>


#include <QMenuBar>  // Подключение для работы с меню

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void discoverServer(const QHostAddress &host, quint16 port, HostBindingMode mode); // Поиск сервера

private:
    StyleManager* styleManager;
    NetworkManager *networkManager;
    MessageHandler *messageHandler;
    ServerLauncher *serverLauncher;
    DiscoveryManager *discoveryManager;
    HostInfo *hostInfo;
    AppController *controller;

    QStackedWidget *m_stackedWidget;
    SettingsPage *settingPage;
    PortPage *portPage;
    NamePage *namePage;
    ChatPage *chatPage;

    ServerMessageDictionary messageDict;


    void showMessageBox(const QString& title, const QString& message, QMessageBox::Icon icon);
    void isNameTakenCheck(QString name);
    //void onErrorOccurred(QAbstractSocket::SocketError error);
    void showSettingsPage();
    bool isDiscDisconnected;

private slots:
    void changeName();
    void exitSettingPage();
    void updateStyleSheet();

public slots:


signals:
    void nameEntered(const QString &name);
    void nameAccepted(const QString &name);
    void nameRejected(const QString &error);
};
