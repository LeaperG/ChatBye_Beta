#include "page_base.h"
#include "client/pages/image_settings/image_settings_manager.h"

PageBase::~PageBase()
{

}

QLabel *PageBase::createImageLabel(const QString &pageName)
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


void PageBase::bindLineEditToButton(QLineEdit *lineEdit, QAbstractButton *button)
{
    // Set initial state
    button->setEnabled(!lineEdit->text().isEmpty());

    // Update the button when the text changes
    connect(lineEdit, &QLineEdit::textChanged, button, [lineEdit, button]() {
        button->setEnabled(!lineEdit->text().isEmpty());
    });
}

void PageBase::onReturnPressed(QLineEdit *lineEdit, QAbstractButton *button)
{
    if (!lineEdit->text().isEmpty()) {
        button->click();
    }
}

void PageBase::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    updateImageSize();
}

void PageBase::updateImageSize()
{
    if (!m_imageLabel || m_originalPixmap.isNull())
        return;

    m_imageLabel->setPixmap(m_originalPixmap.scaled(m_imageLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
}
