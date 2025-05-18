#include "settings_page.h"
#include "client/ui/custom_widgets/color_selector.h"
#include "client/pages/image_settings/image_settings_manager.h"

SettingsPage::SettingsPage(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("Настройки");
    setModal(true);
    setFixedSize(800, 670);

    mainColorEdit = new ColorSelectorWidget("Основной цвет", QColor("#E3F2FD"));
    chatColorEdit = new ColorSelectorWidget("Фон чата", QColor("white"));
    chatColorEdit->setToolTip("Нужно сбросить фото чата");
    textColorEdit = new ColorSelectorWidget("Текст в интерфейсе", QColor("black"));
    lineEditColorEdit = new ColorSelectorWidget("Цвет поля ввода", QColor("white"));
    getMessageBackColorEdit = new ColorSelectorWidget("Фон входящих", QColor("#BBDEFB"));
    sendMessageBackColorEdit = new ColorSelectorWidget("Фон исходящих", QColor("#90CAF9"));
    systemMessageBackColorEdit = new ColorSelectorWidget("Фон системных", QColor("#E0E0E0"));
    clientListColorEdit = new ColorSelectorWidget("Фон списка клиентов", QColor("#BDBDBD"));
    clientListNameColorEdit = new ColorSelectorWidget("Имена клиентов", QColor("black"));
    buttonColorEdit = new ColorSelectorWidget("Цвет кнопок", QColor("#1E88E5"));
    buttonTextColorEdit = new ColorSelectorWidget("Текст на кнопках", QColor("black"));
    textMessageColorEdit = new ColorSelectorWidget("Текст сообщений", QColor("black"));
    senderColorEdit = new ColorSelectorWidget("Имя отправителя", QColor("RoyalBlue"));
    timeColorEdit = new ColorSelectorWidget("Время сообщений", QColor("gray"));
    fontFamilyEdit = new FontPickerWidget();

    m_colorWidgets = {
        mainColorEdit,
        chatColorEdit,
        textColorEdit,
        lineEditColorEdit,
        getMessageBackColorEdit,
        sendMessageBackColorEdit,
        systemMessageBackColorEdit,
        clientListColorEdit,
        clientListNameColorEdit,
        buttonColorEdit,
        buttonTextColorEdit,
        textMessageColorEdit,
        senderColorEdit,
        timeColorEdit,
    };


    QPushButton *changeImagePortPage = new QPushButton("Фон экрана входа");
    changeImagePortPage->setAutoDefault(true);
    changeImagePortPage->setEnabled(true);
    changeImagePortPage->setFixedSize(200, 35);

    QPushButton *changeImageNamePage = new QPushButton("Фон имени пользователя");
    changeImageNamePage->setAutoDefault(true);
    changeImageNamePage->setEnabled(true);
    changeImageNamePage->setFixedSize(200, 35);

    QPushButton *changeImageChatPage = new QPushButton("Фон окна чата");
    changeImageChatPage->setAutoDefault(true);
    changeImageChatPage->setEnabled(true);
    changeImageChatPage->setFixedSize(200, 35);


    QPushButton *applyButton = new QPushButton("Применить изменения");
    applyButton->setAutoDefault(true);
    applyButton->setEnabled(true);
    applyButton->setFixedSize(200, 35);

    QPushButton *saveButton = new QPushButton("Сохранить и выйти");
    saveButton->setAutoDefault(true);
    saveButton->setEnabled(true);
    saveButton->setFixedSize(200, 35);

    QPushButton *resetButton = new QPushButton("Сбросить цвета");
    resetButton->setAutoDefault(true);
    resetButton->setEnabled(true);
    resetButton->setFixedSize(200, 35);

    // Create widgets for settings
    m_radiusLabel = new QLabel("Радиус закругления: 0");
    m_radiusLabel->setFixedSize(175, 30);
    m_radiusLabel->setStyleSheet("QLabel {font-size: 14px; color: #222; font-family: Comic Sans MS; margin-left: 0px;}");
    m_radiusSlider = new QSlider(Qt::Horizontal);

    m_radiusSlider->setRange(0, 15);
    m_radiusSlider->setTickInterval(1);
    m_radiusSlider->setTickPosition(QSlider::TicksBelow);
    m_radiusSlider->setFixedSize(130, 30);


    QGridLayout *mainLayout = new QGridLayout(this);
    mainLayout->setAlignment(Qt::AlignCenter);

    // (●'◡'●) Group: Interface and chat (●'◡'●)
    QGroupBox *interfaceGroup = new QGroupBox("🎨 Цвета интерфейса");
    QVBoxLayout *interfaceLayout = new QVBoxLayout;
    interfaceLayout->addWidget(mainColorEdit);
    interfaceLayout->addWidget(textColorEdit);
    interfaceLayout->addWidget(lineEditColorEdit);
    interfaceLayout->addWidget(buttonColorEdit);
    interfaceLayout->addWidget(buttonTextColorEdit);
    interfaceLayout->addWidget(fontFamilyEdit);

    QHBoxLayout *radiusLayout = new QHBoxLayout;
    radiusLayout->setContentsMargins(0, 0, 0, 0);
    radiusLayout->setSpacing(10);
    radiusLayout->setAlignment(Qt::AlignLeft);
    radiusLayout->addWidget(m_radiusLabel);
    radiusLayout->addWidget(m_radiusSlider);
    interfaceLayout->addLayout(radiusLayout);
    interfaceGroup->setLayout(interfaceLayout);

    // Group: Chats and Messages
    QGroupBox *chatGroup = new QGroupBox("💬 Чат и сообщения");
    QVBoxLayout *chatLayout = new QVBoxLayout;
    chatLayout->addWidget(chatColorEdit);
    chatLayout->addWidget(getMessageBackColorEdit);
    chatLayout->addWidget(sendMessageBackColorEdit);
    chatLayout->addWidget(systemMessageBackColorEdit);
    chatLayout->addWidget(textMessageColorEdit);
    chatLayout->addWidget(senderColorEdit);
    chatLayout->addWidget(timeColorEdit);
    chatGroup->setLayout(chatLayout);

    // Group: Client List
    QGroupBox *clientListGroup = new QGroupBox("📋 Список клиентов");
    QVBoxLayout *clientListLayout = new QVBoxLayout;
    clientListLayout->addWidget(clientListColorEdit);
    clientListLayout->addWidget(clientListNameColorEdit);
    clientListGroup->setLayout(clientListLayout);

    // (●'◡'●) Group: Page Backgrounds (●'◡'●)
    QGroupBox *backgroundImagesGroup = new QGroupBox("🖼 Фоны страниц");
    QVBoxLayout *bgButtonLayout = new QVBoxLayout;
    bgButtonLayout->setAlignment(Qt::AlignCenter);
    bgButtonLayout->addWidget(changeImagePortPage);
    bgButtonLayout->addWidget(changeImageNamePage);
    bgButtonLayout->addWidget(changeImageChatPage);
    backgroundImagesGroup->setLayout(bgButtonLayout);

    // Control buttons
    QHBoxLayout *controlsLayout = new QHBoxLayout;
    controlsLayout->setContentsMargins(20, 20, 20, 20);
    controlsLayout->setSpacing(0);
    controlsLayout->addWidget(applyButton);
    controlsLayout->addWidget(saveButton);
    controlsLayout->addWidget(resetButton);

    // 1 row
    mainLayout->addWidget(interfaceGroup, 0, 0);
    mainLayout->addWidget(backgroundImagesGroup, 1, 0);
    // 2 row
    mainLayout->addWidget(chatGroup, 0, 1);
    mainLayout->addWidget(clientListGroup, 1, 1);
    // 3 row
    mainLayout->addLayout(controlsLayout, 2, 0, 1, 2);

    /*
    this->setAutoFillBackground(true);
    QPalette p = palette();
    p.setColor(QPalette::Window, QColor("#E3F2FD"));
    setPalette(p);


    this->setStyleSheet(R"(
    QPushButton {
        background-color: #1E88E5;
        color: white;
        border-radius: 6px;
        padding: 6px 12px;
        font-family: Comic Sans MS;
    }
    QPushButton:hover {
        background-color: #1f6db3;
    }
    QPushButton:pressed {
        background-color: #1a5a99;
    }
    )");

    changeImagePortPage->setStyleSheet("background-color: #1E88E5; border-radius: 5px;");
    changeImageNamePage->setStyleSheet("background-color: #1E88E5; border-radius: 5px;");
    changeImageChatPage->setStyleSheet("background-color: #1E88E5; border-radius: 5px;");
    applyButton->setStyleSheet("background-color: #1E88E5; border-radius: 5px;");
    saveButton->setStyleSheet("background-color: #1E88E5; border-radius: 5px;");
    resetButton->setStyleSheet("background-color: #1E88E5; border-radius: 5px;");
    */

    connect(changeImagePortPage, &QPushButton::clicked, this, [this] () { changeImage("PortPage"); });
    connect(changeImageNamePage, &QPushButton::clicked, this, [this] () { changeImage("NamePage"); });
    connect(changeImageChatPage, &QPushButton::clicked, this, [this] () { changeImage("ChatPage"); });

    connect(applyButton, &QPushButton::clicked, this, &SettingsPage::applyBtn);
    connect(saveButton, &QPushButton::clicked, this, &SettingsPage::saveAndExitBtn);
    connect(m_radiusSlider, &QSlider::valueChanged, this, &SettingsPage::sliderUpdated);
    connect(resetButton, &QPushButton::clicked, this, &SettingsPage::resetColors);

    loadColors();
    isImageAvailable("ChatPage");
}

void SettingsPage::isImageAvailable(const QString &pageName)
{
    if (ImageSettingsManager::instance()->isImageAvailable(pageName)) {
        chatColorEdit->setEnabled(false);
    } else {
        chatColorEdit->setEnabled(true);
    }
}

QLabel *SettingsPage::createImageLabel(const QString &pageName)
{
    m_imageLabel = new QLabel(this);
    QString imagePath = ImageSettingsManager::instance()->loadImage(pageName);
    m_originalPixmap = QPixmap(imagePath);


    if (m_originalPixmap.isNull()) {
        return nullptr;
    }

    m_imageLabel->setPixmap(m_originalPixmap);
    m_imageLabel->setScaledContents(false);
    m_imageLabel->setAlignment(Qt::AlignCenter);
    m_imageLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    return m_imageLabel;
}

void SettingsPage::applyBtn()
{
    saveColors();
    emit colorChanged();
}

void SettingsPage::resetColors()
{
    for (ColorSelectorWidget *widget : m_colorWidgets) {
        widget->setDefaultColor();
    };

    fontFamilyEdit->setDefaultFont();
    m_radiusSlider->setValue(5);

    applyBtn();
}

void SettingsPage::saveAndExitBtn()
{
    applyBtn();
    emit exitPage();
}

void SettingsPage::saveColors()
{
    QSettings settings("ChatBye", "ColorSettings");

    settings.setValue("main", mainColorEdit->lineEdit->text());
    settings.setValue("clientList", clientListColorEdit->lineEdit->text());
    settings.setValue("button", buttonColorEdit->lineEdit->text());
    settings.setValue("text", textColorEdit->lineEdit->text());
    settings.setValue("clientListName", clientListNameColorEdit->lineEdit->text());
    settings.setValue("getMessageBack", getMessageBackColorEdit->lineEdit->text());
    settings.setValue("sendMessageBack", sendMessageBackColorEdit->lineEdit->text());
    settings.setValue("systemMessageBack", systemMessageBackColorEdit->lineEdit->text());
    settings.setValue("textMessage", textMessageColorEdit->lineEdit->text());
    settings.setValue("textSender", senderColorEdit->lineEdit->text());
    settings.setValue("textTime", timeColorEdit->lineEdit->text());
    settings.setValue("chat", chatColorEdit->lineEdit->text());
    settings.setValue("buttonText", buttonTextColorEdit->lineEdit->text());
    settings.setValue("lineEdit", lineEditColorEdit->lineEdit->text());
    settings.setValue("radiusSlider", m_radiusSlider->value());
    settings.setValue("fontFamily", fontFamilyEdit->selectedFont());
}

void SettingsPage::loadColors()
{
    QSettings settings("ChatBye", "ColorSettings");

    mainColorEdit->setColorCode(settings.value("main", "#E3F2FD").toString());
    clientListColorEdit->setColorCode(settings.value("clientList", "#BDBDBD").toString());
    buttonColorEdit->setColorCode(settings.value("button", "#1E88E5").toString());
    textColorEdit->setColorCode(settings.value("text", "black").toString());
    clientListNameColorEdit->setColorCode(settings.value("clientListName", "black").toString());
    getMessageBackColorEdit->setColorCode(settings.value("getMessageBack", "#BBDEFB").toString());
    sendMessageBackColorEdit->setColorCode(settings.value("sendMessageBack", "#90CAF9").toString());
    systemMessageBackColorEdit->setColorCode(settings.value("systemMessageBack", "#E0E0E0").toString());
    textMessageColorEdit->setColorCode(settings.value("textMessage", "black").toString());
    senderColorEdit->setColorCode(settings.value("textSender", "RoyalBlue").toString());
    timeColorEdit->setColorCode(settings.value("textTime", "gray").toString());
    chatColorEdit->setColorCode(settings.value("chat", "white").toString());
    buttonTextColorEdit->setColorCode(settings.value("buttonText", "black").toString());
    lineEditColorEdit->setColorCode(settings.value("lineEdit", "white").toString());
    m_radiusSlider->setValue(settings.value("radiusSlider", "0").toInt());
    fontFamilyEdit->setInitialFont(settings.value("fontFamily", "Comic Sans MS").toString());
}

void SettingsPage::sliderUpdated(int value)
{
    m_radiusLabel->setText(QString("Радиус закругления: %1").arg(value));
}



// Methods for changing the image on pages
void SettingsPage::changeImage(const QString &pageName)
{
    if (pageName.isEmpty()) {
        MessageHelper::showCustomMessageBox(this, "Ошибка", "Неизвестная страница, сброс невозможен.", QMessageBox::Critical);
        return;
    }

    QMessageBox::StandardButton result = MessageHelper::showCustomConfirmationDialog(this, "Подтверждение", "Выберите действие", "Хотите изменить фото или сбросить его?", "Изменить фото", "Сбросить фото");

    if (result == QMessageBox::Yes)
    {
        QString desktopPath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
        QString filePath = QFileDialog::getOpenFileName(this, "Выберите изображение", desktopPath,
                                                        "Images (*.png *.jpg *.jpeg)");

        if (!filePath.isEmpty()) {
            QFileInfo fileInfo(filePath);
            QString extension = fileInfo.suffix().toLower();

            if (extension != "png" && extension != "jpg" && extension != "jpeg") {
                MessageHelper::showCustomMessageBox(this, "Ошибка", "Пожалуйста, выберите изображение в формате PNG или JPG.", QMessageBox::Critical);
                return;
            }


            ImageSettingsManager::instance()->saveImage(filePath, pageName);
        }

        chatColorEdit->setEnabled(false);
    }
    else if (result == QMessageBox::No)
    {
        QString destPath = ImageSettingsManager::instance()->getDefaultImagePath(pageName);

        if (destPath.isEmpty()) {
            MessageHelper::showCustomMessageBox(this, "Ошибка", "Неизвестная страница, сброс невозможен.", QMessageBox::Critical);
            return;
        }

        ImageSettingsManager::instance()->resetImage(pageName);

        ImageSettingsManager::instance()->notifyImageChanged(pageName);
        chatColorEdit->setEnabled(true);
        qDebug() << "No: ";
    }
    else if (result == QMessageBox::Cancel) {
        qDebug() <<"Cancel: " << result;
    }
}

