#include "shadow_helper.h"
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>

void ShadowHelper::applyShadow(QWidget *widget, const QColor &color, int blurRadius, QPointF offset)
{
    if (!widget) return;

    QGraphicsDropShadowEffect* shadow = new QGraphicsDropShadowEffect(widget);
    shadow->setBlurRadius(blurRadius);
    shadow->setColor(color);
    shadow->setOffset(offset);
    widget->setGraphicsEffect(shadow);
}

void ShadowHelper::applyShadowToSpecificWidgets(QWidget *parent)
{
    for (QObject* obj : parent->children()) {
        QWidget* child = qobject_cast<QWidget*>(obj);
        if (child) {
            if (qobject_cast<QLineEdit*>(child) ||
                qobject_cast<QPushButton*>(child) ||
                qobject_cast<QComboBox*>(child)){
                ShadowHelper::applyShadow(child, QColor(0, 0, 0, 160), 10, QPointF(2,2));
            }
            applyShadowToSpecificWidgets(child);
        }
    }
}
