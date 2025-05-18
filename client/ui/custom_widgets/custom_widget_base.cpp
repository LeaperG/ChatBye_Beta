#include "custom_widget_base.h"
#include <QGraphicsDropShadowEffect>


CustomWidgetBase::CustomWidgetBase(QWidget *parent)
    : QWidget{parent}
{

}


void CustomWidgetBase::applyStyle(QLabel *label, const QString &style)
{
    QString finalStyle = defaultLabelStyle();
    if (!style.isEmpty())
        finalStyle += "\n" + style;

    label->setStyleSheet(finalStyle);
}

void CustomWidgetBase::applyStyle(QLineEdit *lineEdit, const QString &style)
{
    QString finalStyle = defaultLineEditStyle();
    if (!style.isEmpty())
        finalStyle += "\n" + style;

    lineEdit->setStyleSheet(finalStyle);
}

void CustomWidgetBase::applyStyle(QPushButton *button, const QString &style)
{
    QString finalStyle = defaultButtonStyle();
    if (!style.isEmpty())
        finalStyle += "\n" + style;

    button->setStyleSheet(finalStyle);
}

QString CustomWidgetBase::defaultLabelStyle() const
{
    return "QWidget { font-size: 14px; color: #222; font-family: Comic Sans MS; margin: 0px; }";
}

QString CustomWidgetBase::defaultLineEditStyle() const
{
    return "QWidget { background-color: white; border-radius: 5px; font-size: 14px; font-family: Comic Sans MS; margin: 0px; }";
}

QString CustomWidgetBase::defaultButtonStyle() const
{
    return "QPushButton { background-color: #1E88E5; border-radius: 5px; font-size: 14px; font-family: Comic Sans MS; margin: 0px; }";
}
