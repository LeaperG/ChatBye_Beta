#pragma once

#include <QObject>
#include <QGraphicsDropShadowEffect>
#include <QWidget>

class ShadowHelper
{
public:

    static void applyShadow(QWidget* widget,
                            const QColor& color = QColor(0, 0, 0, 160),
                            int blurRadius = 15,
                            QPointF offset = QPointF(0, 4));


    static void applyShadowToSpecificWidgets(QWidget* parent);

};

