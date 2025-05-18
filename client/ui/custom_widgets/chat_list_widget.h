#pragma once

#include "client/ui/custom_widgets/custom_widget_base.h"
#include <QListWidget>
#include <QObject>
#include <QResizeEvent>

class QListWidget;
class QLabel;


class ChatListWidget : public CustomWidgetBase
{
    Q_OBJECT
public:
    explicit ChatListWidget(const QString &pageName, QWidget *parent = nullptr);
    QListWidget *listWidget;

    void updateBackground();

protected:
    void paintEvent(QPaintEvent *event);

private:
    QPixmap m_originalBackground;
    QString m_backgroundPath;
    QString m_pageName;
    int m_radiusValue;
    void setupStyle();
};

