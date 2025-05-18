#pragma once

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>

class CustomWidgetBase : public QWidget
{
    Q_OBJECT
public:
    explicit CustomWidgetBase(QWidget *parent = nullptr);

protected:
    void applyStyle(QLabel *label, const QString &style = "");
    void applyStyle(QLineEdit *lineEdit, const QString &style = "");
    void applyStyle(QPushButton *button, const QString &style = "");

    QString defaultLabelStyle() const;
    QString defaultLineEditStyle() const;
    QString defaultButtonStyle() const;

signals:
};

