#include "name_page.h"
#include "client/ui/helpers/message_helper.h"
#include "client/pages/image_settings/image_settings_manager.h"
#include "client/pages/settings_page/settings_page.h"
#include "client/ui/styles/name_page_style.h"


NamePage::NamePage(QWidget *parent)
    : PageBase(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    imageLayout = new QVBoxLayout;
    imageLayout->setAlignment(Qt::AlignCenter);
    updateImage("NamePage");

    mainLayout->addLayout(imageLayout);
    QHBoxLayout *inputLayout = new QHBoxLayout;

    m_hintLabel = new QLabel("Введите имя:");
    inputLayout->addWidget(m_hintLabel);

    m_nameLineEdit = new QLineEdit;
    m_nameLineEdit->setPlaceholderText("Ваше имя");
    m_nameLineEdit->setMinimumSize(130, 35);
    m_nameLineEdit->setFocus();
    inputLayout->addWidget(m_nameLineEdit);

    m_checkNameButton = new QPushButton("Далее");
    m_checkNameButton->setEnabled(false);
    m_checkNameButton->setMinimumSize(130, 35);
    inputLayout->addWidget(m_checkNameButton);

    mainLayout->addLayout(inputLayout);

    //updateStyleSheet();

    setupInputAction(m_nameLineEdit, m_checkNameButton, this,  &NamePage::isNameTaken);
    //connect(SettingsPage::instance(), &SettingsPage::colorChanged, this, &NamePage::updateStyleSheet);
    connect(ImageSettingsManager::instance(), &ImageSettingsManager::imageUpdated, this, &NamePage::updateImage);

    QTimer::singleShot(100, m_nameLineEdit, SLOT(setFocus()));

}

QPushButton *NamePage::getCheckNameButton() const
{
    return m_checkNameButton;
}

QLineEdit *NamePage::getNameLineEdit() const
{
    return m_nameLineEdit;
}

QLabel *NamePage::getHintLabel() const
{
    return m_hintLabel;
}

void NamePage::isNameTaken()
{
    QString name = m_nameLineEdit->text().trimmed();

    if (!name.isEmpty() && m_clientName != name && name.length() <= 25) {
        emit nameEntered(name);
    }
    else {
        if (m_clientName == name) {
            bool isUserConfirmed  = MessageHelper::showConfirmationDialog(this, "Подтверждение", "Вы не изменили имя. Вы хотите продолжить без изменений?");
            if (isUserConfirmed) {
                emit nameEntered("");
            }
            else {
                return;
            }
        }
        else if (name.length() >= 25) {
            MessageHelper::showCustomMessageBox(this, "Ошибка", "Имя слишком длинное", QMessageBox::Warning);
            m_nameLineEdit->clear();
            return;
        }
        else {
            MessageHelper::showCustomMessageBox(this, "Ошибка", "Неверное имя", QMessageBox::Warning);
            m_nameLineEdit->clear();
            return;
        }
    }
}

void NamePage::updateStyleSheet()
{
    NamePageStyle style;
    //style.setStyle(this);
}

void NamePage::handleNameAccepted(const QString &name)
{
    m_clientName = name;
}

void NamePage::handleNameRejected(const QString &name)
{
    MessageHelper::showCustomMessageBox(this, "Ошибка", "Не удалось установить имя\nИмя: " + name + " занято", QMessageBox::Warning);
    m_clientName.clear();
}

void NamePage::updateImage(const QString &pageName)
{
    if (pageName != "NamePage") {
        return;
    }

    QLabel *newImageLabel = createImageLabel(pageName);

    if (!newImageLabel) {
        qWarning() << "Ошибка загрузки нового изображения!";
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
