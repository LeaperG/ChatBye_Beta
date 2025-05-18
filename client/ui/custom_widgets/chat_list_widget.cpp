#include "chat_list_widget.h"
#include "client/ui/helpers/shadow_helper.h"

#include "client/pages/image_settings/image_settings_manager.h"

#include <QVBoxLayout>
#include <qpainter.h>
#include <qpainterpath.h>
#include <QSettings>

ChatListWidget::ChatListWidget(const QString &pageName, QWidget *parent)
    : CustomWidgetBase(parent)
{
    this->m_pageName = pageName;
    // Customize widget background transparency
    setAttribute(Qt::WA_TranslucentBackground);
    setAutoFillBackground(false);

    // Initializing the chat list
    listWidget = new QListWidget(this);
    listWidget->setWordWrap(true);
    listWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    listWidget->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->addWidget(listWidget);

    setupStyle();
    updateBackground();

}

void ChatListWidget::updateBackground()
{
    m_backgroundPath = ImageSettingsManager::instance()->loadImage(m_pageName);

    // Load the background
    m_originalBackground = QPixmap(m_backgroundPath);
}

void ChatListWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, true);

    QPainterPath path;
    qreal radius = m_radiusValue;
    path.addRoundedRect(rect(), radius, radius);
    painter.setClipPath(path); // sets the drawing area with rounded edges


    if (!m_originalBackground.isNull())
    {
        QPixmap scaled = m_originalBackground.scaled(size(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
        QPoint topLeft((width() - scaled.width()) / 2, (height() - scaled.height()) / 2);
        painter.drawPixmap(topLeft, scaled);

        QColor overlayColor(0, 0, 0, 80);
        painter.fillRect(rect(), overlayColor);
    }
    else
    {
        QColor BackgroundColor(m_backgroundPath);
        painter.fillRect(rect(), m_backgroundPath);
    }
}

void ChatListWidget::setupStyle()
{
    QSettings settings("ChatBye", "ColorSettings");
    m_radiusValue = settings.value("radiusSlider").toInt();

    ShadowHelper::applyShadow(listWidget, QColor(0, 0, 0, 160), 10, QPointF(2,2));
    ShadowHelper::applyShadow(this, QColor(0, 0, 0, 160), 10, QPointF(2,2));

    // Scrollbar and transparent background style
    listWidget->setStyleSheet(
        "QListWidget { background-color: transparent; border-radius: " + QString::number(m_radiusValue) + "px; }"
        "QScrollBar:vertical {"
        "   border: none;"
        "   background: transparent;"
        "   width: 6px;"
        "   margin: 0;"
        "}"
        "QScrollBar::handle:vertical {"
        "   background: #a4aae5;"
        "   min-height: 30px;"
        "   border-radius: 3px;"
        "}"
        "QScrollBar::add-line:vertical,"
        "QScrollBar::sub-line:vertical {"
        "   background: transparent;"
        "   height: 15px;"
        "}"
    );
}



