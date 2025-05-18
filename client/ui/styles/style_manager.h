#pragma once

#include <QObject>
#include <QWidget>
#include <QSettings>

class QSettings;

class StyleManager : public QObject
{
    Q_OBJECT
public:
    explicit StyleManager(QObject *parent = nullptr);
    void setDefaultStyle(QWidget* widget);

private:
    QSettings settings;

signals:
};

