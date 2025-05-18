#include "main_window.h"
#include "client/app_controller.h"
#include "helpers/message_enum/message_type.h"
#include "helpers/json/json_message_builder.h"
#include "client/ui/helpers/message_helper.h"
#include "client/ui/helpers/shadow_helper.h"
#include <QListWidget>
#include <QtConcurrent/QtConcurrent>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("Сетевой чат");
    QMenu *fileMenu = menuBar()->addMenu("Меню");
    QAction *settingsAction = new QAction("Настройки", this);
    fileMenu->addAction(settingsAction);
    connect(settingsAction, &QAction::triggered, this, &MainWindow::showSettingsPage);
    m_stackedWidget = new QStackedWidget(this);
    setCentralWidget(m_stackedWidget);
    m_stackedWidget->setWindowFlags(Qt::Widget | Qt::MSWindowsFixedSizeDialogHint);

    settingPage = SettingsPage::instance();
    portPage = new PortPage(this);
    m_stackedWidget->addWidget(portPage);
    namePage = new NamePage(this);
    m_stackedWidget->addWidget(namePage);
    chatPage = new ChatPage(this);
    m_stackedWidget->addWidget(chatPage);
    m_stackedWidget->setMinimumSize(800,600);
    m_stackedWidget->setCurrentWidget(portPage);

    connect(namePage, &NamePage::nameEntered, this, &MainWindow::isNameTakenCheck);
    connect(this, &MainWindow::nameAccepted, namePage, &NamePage::handleNameAccepted);
    connect(this, &MainWindow::nameRejected, namePage, &NamePage::handleNameRejected);
    connect(chatPage, &ChatPage::changeName, this, &MainWindow::changeName);
    connect(this, &MainWindow::nameEntered, chatPage, &ChatPage::getClientName);
    connect(settingPage, &SettingsPage::exitPage, this, &MainWindow::exitSettingPage);
    connect(settingPage, &SettingsPage::colorChanged, this, &MainWindow::updateStyleSheet);
    messageDict = ServerMessageDictionary();
    updateStyleSheet();


    networkManager = new NetworkManager(this);
    messageHandler = new MessageHandler(this);
    serverLauncher = new ServerLauncher(this);
    discoveryManager = new DiscoveryManager(this);
    hostInfo = new HostInfo(this);

    controller = new AppController(this);
    controller->setDependencies(networkManager, discoveryManager, serverLauncher, messageHandler, hostInfo);


    connect(controller, &AppController::showMessage, this, &MainWindow::showMessageBox);
    connect(portPage, &PortPage::requestHost, controller, &AppController::onRequestHost);
    connect(controller, &AppController::hostEntriesReadyAvailable, portPage, &PortPage::setHostAddresses);
    connect(portPage, &PortPage::hostAndPortEntered, controller, &AppController::onHostAndPortEntered);
    connect(chatPage, &ChatPage::sendMessage, controller, &AppController::onSendMessage);
    connect(controller, &AppController::chatMessageReceived, chatPage, &ChatPage::receiveMessageFromServer);
    connect(controller, &AppController::clientListReceived, chatPage, &ChatPage::updateClientList);
    connect(controller, &AppController::requestShowNamePage, this, [this]() {
        m_stackedWidget->setCurrentWidget(namePage);
    });

    connect(controller, &AppController::scanProgressStarted, portPage, &PortPage::showProgressDialog);
    connect(controller, &AppController::scanProgressChanged, portPage, &PortPage::updateProgress);
    connect(controller, &AppController::scanProgressCanceled, portPage, &PortPage::closeProgressDialog);
    connect(portPage, &PortPage::scanProgressCanceled, controller, &AppController::stopScan);
}

MainWindow::~MainWindow()
{
    networkManager->disconnectFromServer();
    delete m_stackedWidget;
    this->close();
}


void MainWindow::showMessageBox(const QString &title, const QString &message, QMessageBox::Icon icon)
{
    MessageHelper::showCustomMessageBox(this, title, message, icon);
}

void MainWindow::showSettingsPage()
{
    if (!settingPage) {
        settingPage = SettingsPage::instance();
    }
    settingPage->show();
    settingPage->raise();
    settingPage->activateWindow();
}

void MainWindow::updateStyleSheet()
{
    styleManager = new StyleManager(this);
    styleManager->setDefaultStyle(this);
    ShadowHelper::applyShadowToSpecificWidgets(this);
}

void MainWindow::isNameTakenCheck(QString name)
{
    if (name.trimmed().isEmpty()) {
        m_stackedWidget->setCurrentWidget(chatPage);
        return;
    }

    const QStringList names = messageHandler->getNames();
    for (const QString &existingName : names) {
        if (existingName.compare(name, Qt::CaseInsensitive) == 0) {
            emit nameRejected(name);
            return;
        }
    }

    emit nameAccepted(name);

    QString oldName = messageHandler->getClientName();
    messageHandler->setClientName(name);

    QByteArray jsonData;
    if (oldName.isEmpty()) {
        jsonData = JsonMessageBuilder::createNicknameChange(MessageType::SetName, name);
    } else {
        jsonData = JsonMessageBuilder::createNicknameChange(MessageType::ChangeName, name, oldName);
    }

    networkManager->sendMessageToServer(jsonData);

    emit nameEntered(name);
    m_stackedWidget->setCurrentWidget(chatPage);
}

void MainWindow::changeName()
{
    QString clientName = messageHandler->getClientName();
    QStringList names = messageHandler->getNames();
    messageHandler->removeName(clientName);
    m_stackedWidget->setCurrentWidget(namePage);
}

void MainWindow::exitSettingPage()
{
    settingPage->close();
}

