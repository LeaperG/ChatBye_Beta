#include "port_page.h"
#include "client/ui/helpers/message_helper.h"
#include "client/pages/image_settings/image_settings_manager.h"
#include "helpers/message_enum/host_binding_mode.h"
#include "client/ui/styles/port_page_style.h"

#include "client/ui/custom_widgets/progress_dialog.h"


#include <QColor>
#include <QCompleter>
#include <QProgressDialog>


PortPage::PortPage(QWidget *parent)
    : PageBase(parent), m_filter(InterfaceFilter::AllInterfaces)
{
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->setAlignment(Qt::AlignCenter);
    QGridLayout *inputLayout = new QGridLayout;
    inputLayout->setAlignment(Qt::AlignCenter);
    imageLayout = new QVBoxLayout;
    imageLayout->setAlignment(Qt::AlignCenter);

    updateImage("PortPage");

    mainLayout->addLayout(imageLayout);
    mainLayout->addLayout(inputLayout);

    m_hostLabel = new QLabel("Хост:");
    m_hostLabel->setFixedSize(QSizePolicy::Label,30);
    m_comboBoxIp = new QComboBox;
    m_comboBoxIp->setEditable(true);
    m_comboBoxIp->setFixedSize(170, 35);

    m_portLabel = new QLabel("Порт:");
    m_portLabel->setMinimumSize(QSizePolicy::Label, 35);
    m_portLineEdit = new QLineEdit;
    m_portLineEdit->setMinimumSize(130, 35);
    QAction *myActionPort = m_portLineEdit->addAction(QIcon(":/icons/iconUpdate.png"), QLineEdit::TrailingPosition);
    connect(myActionPort, &QAction::triggered, this, &PortPage::updatePort);
    m_portLineEdit->setPlaceholderText("Например, 8080");

    m_connectButton = new QPushButton("Далее");
    m_connectButton->setAutoDefault(true);
    m_connectButton->setEnabled(true);
    m_connectButton->setMinimumSize(170, 35);

    m_quickSearchWifiBtn = new QPushButton("Быстрый поиск в Wifi");
    m_quickSearchWifiBtn->setMinimumSize(170, 35);

    m_quickSearchLANBtn = new QPushButton("Быстрый поиск в LAN");
    m_quickSearchLANBtn->setMinimumSize(170, 35);

    m_setValueBtn = new QPushButton("Вручную");
    m_setValueBtn->setMinimumSize(170, 35);

    m_buttons << m_connectButton << m_quickSearchWifiBtn << m_quickSearchLANBtn << m_setValueBtn;
    m_labels << m_hostLabel << m_portLabel;

    //setupInputAction(portLineEdit, connectButton, this, &PortPage::onCheckHostAndPort);
    connect(m_connectButton, &QPushButton::clicked, this, &PortPage::onCheckHostAndPort);

    connect(m_quickSearchWifiBtn, &QPushButton::clicked, this, [=] () {
        m_filter = InterfaceFilter::OnlyWifi;
        updateHostAddress(m_filter);
        onCheckHostAndPort();
    });

    connect(m_quickSearchLANBtn, &QPushButton::clicked, this, [=] () {
        m_filter = InterfaceFilter::OnlyLan;
        setVisibleUi();
        updateHostAddress(m_filter);
    });

    connect(m_setValueBtn, &QPushButton::clicked, this, [=] () {
        m_filter = InterfaceFilter::AllInterfaces;
        setVisibleUi();
        updateHostAddress(m_filter);
    });


    inputLayout->addWidget(m_quickSearchWifiBtn, 0, 1, Qt::AlignRight);
    inputLayout->addWidget(m_quickSearchLANBtn, 1, 1, Qt::AlignRight);
    inputLayout->addWidget(m_setValueBtn, 3,  1, Qt::AlignRight);


    inputLayout->addWidget(m_hostLabel, 0, 0, Qt::AlignRight);
    inputLayout->addWidget(m_comboBoxIp, 0,  1, Qt::AlignRight);
    inputLayout->addWidget(m_portLabel, 1, 0, Qt::AlignRight);
    inputLayout->addWidget(m_portLineEdit, 1, 1);
    inputLayout->addWidget(m_connectButton, 2, 0, 1, 2, Qt::AlignRight);


    inputLayout->setContentsMargins(0, 0, 0, 0);
    inputLayout->setSpacing(15);
    inputLayout->setColumnMinimumWidth(0, 100);
    inputLayout->setColumnMinimumWidth(1, 100);
    inputLayout->setColumnMinimumWidth(2, 100);

    setVisibleUi();


    connect(ImageSettingsManager::instance(), &ImageSettingsManager::imageUpdated, this, &PortPage::updateImage);

    //If a custom style is needed
    //connect(SettingsPage::instance(), &SettingsPage::colorChanged, this, &PortPage::updateStyleSheet);
    updateStyleSheet();


    updateHostAddress(InterfaceFilter::AllInterfaces);
    // Create a timer to update Wi-Fi IP address
    m_updateTimer = new QTimer(this);
    connect(m_updateTimer, &QTimer::timeout, this, [=] (){
        updateHostAddress(m_filter);
    });
    m_updateTimer->start(5000);
}

PortPage::~PortPage()
{

}


void PortPage::updateStyleSheet()
{
    PortPageStyle style;
    style.setStyle(this);
}

void PortPage::updateImage(const QString &pageName)
{
    if (pageName != "PortPage") {
        return;
    }

    QLabel *newImageLabel = createImageLabel(pageName);

    if (!newImageLabel) {
        qWarning() << "Error loading a new image!";
        return;
    }

    if (m_imageLabel) {
        imageLayout->removeWidget(m_imageLabel);
        delete m_imageLabel;
    }

    m_imageLabel = newImageLabel;
    imageLayout->addWidget(m_imageLabel);

    // Forcibly update the layout
    imageLayout->invalidate();
    imageLayout->activate();

    adjustSize();
    updateImageSize();
}

void PortPage::showProgressDialog()
{
    if (m_progressDialog) {
        return;
        //delete m_progressDialog;
    }

    m_progressDialog = new ProgressDialog(this);
    m_progressDialog->setGif(":/gifs/download.gif");
    m_progressDialog->setMessage("Поиск сервера...");
    connect(m_progressDialog, &ProgressDialog::canceled, this, &PortPage::closeProgressDialog);
    updateStyleSheet();
    m_progressDialog->show();
}

void PortPage::updateProgress(int current, int max)
{
    if (!m_progressDialog)
        return;

    m_progressDialog->setProgress(current, max);

    if (current >= max) {
        m_progressDialog->close();
        delete m_progressDialog;
        m_progressDialog = nullptr;
    }
}

void PortPage::closeProgressDialog()
{
    emit scanProgressCanceled();

    if (m_progressDialog) {
        m_progressDialog->close();
        delete m_progressDialog;
        m_progressDialog = nullptr;
    }
}

void PortPage::updateHostAddress(InterfaceFilter filter)
{
    emit requestHost(filter);
}

void PortPage::updatePort()
{
    m_portLineEdit->setText("50500");
}

void PortPage::initUi()
{

}

void PortPage::setVisibleUi()
{
    advancedVisible = !advancedVisible;
    if (m_filter != InterfaceFilter::OnlyWifi) {
        m_hostLabel->setVisible(advancedVisible);

        if (m_filter != InterfaceFilter::OnlyLan) {
            m_portLabel->setVisible(advancedVisible);
            m_portLineEdit->setVisible(advancedVisible);
        }
        m_connectButton->setVisible(advancedVisible);
        m_comboBoxIp->setVisible(advancedVisible);
    }
    /// (❤ ω ❤) ///
    if (advancedVisible) m_setValueBtn->setText("Назад");
    else m_setValueBtn->setText("Вручную");
    m_quickSearchWifiBtn->setVisible(!advancedVisible);
    m_quickSearchLANBtn->setVisible(!advancedVisible);
}

QNetworkAddressEntry PortPage::findEntryByIp(const QString &ipText)
{
    QHostAddress targetIp(ipText);

    for (const QNetworkAddressEntry &entry : m_availableEntries) {
        if (entry.ip() == targetIp) {
            return entry;
        }
    }

    return QNetworkAddressEntry();
}



void PortPage::onCheckHostAndPort()
{
    m_updateTimer->stop(); // Stop the timer as soon as the address is updated
    QString ip = m_comboBoxIp->currentText();
    m_selectedEntry = findEntryByIp(ip);
    QHostAddress address(ip);

    if (m_filter == InterfaceFilter::OnlyWifi || m_filter == InterfaceFilter::OnlyLan) {
        if (ip.isNull() || ip.isEmpty() || m_comboBoxIp->currentText().isEmpty()) {
            MessageHelper::showCustomMessageBox(nullptr, "Ошибка", "Вы не подключены к Wi-Fi", QMessageBox::Critical);
            return;
        }
        emit hostAndPortEntered(m_selectedEntry, 50500, HostBindingMode::DynamicPort);
        return;
    }


    bool ok;
    int portCheck = m_portLineEdit->text().toInt(&ok);
    quint16 port;

    if (ok && portCheck > 0 && portCheck <= 65535) {
        port = portCheck;
        //qDebug() << "The port number is: " <<port;
    } else {
        MessageHelper::showCustomMessageBox(this, "Ошибка", "Неверный порт", QMessageBox::Warning);
        m_portLineEdit->clear();
        return;
    }



    if (!address.isNull()) {
        qDebug() << "Connected Wi-Fi IP entry:" << m_selectedEntry;
    } else {
        qDebug() << "No Wi-Fi connection found or no IP address assigned.";
        return;
    }

    if (m_selectedEntry == QNetworkAddressEntry()) {
        m_selectedEntry.setIp(address);
    }

    qDebug() << "Wi-Fi IP address found! Timer is stopped.";

    emit hostAndPortEntered(m_selectedEntry, port, HostBindingMode::FixedPort);
}

void PortPage::setHostAddresses(const QList<QNetworkAddressEntry> &entryList)
{
    m_availableEntries = entryList;
    QList<QHostAddress> ipList;
    for (const QNetworkAddressEntry &entry : entryList) {
        ipList.append(entry.ip());
    }

    if (ipList != m_allHostIp) {
        m_comboBoxIp->clear();
        for (const QHostAddress& ip : ipList) {
            m_comboBoxIp->addItem(ip.toString());
        }
        m_allHostIp = ipList;
        //MessageHelper::showCustomMessageBox(this, "Ошибка", "Добален новый порт", QMessageBox::Critical);
    }
}

QList<QPushButton *> PortPage::getButtons() const
{
    return m_buttons;
}

QList<QLabel *> PortPage::getLabels() const
{
    return m_labels;
}

QLineEdit *PortPage::getPortLineEdit() const
{
    return m_portLineEdit;
}

QComboBox *PortPage::getComboBoxIp() const
{
    return m_comboBoxIp;
}

ProgressDialog *PortPage::progressDialog() const
{
    return m_progressDialog;
}

