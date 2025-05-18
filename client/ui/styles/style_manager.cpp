#include "style_manager.h"
#include "client/pages/settings_page/settings_page.h"

#include "client/ui/helpers/shadow_helper.h"

#include <QSettings>
#include <QFont>
#include <QApplication>
#include <QWidget>

StyleManager::StyleManager(QObject *parent)
    : QObject{parent}, settings("ChatBye", "ColorSettings")
{

}

void StyleManager::setDefaultStyle(QWidget *widget)
{
    QString fullStyle;
    //QSettings settings("ChatBye", "ColorSettings");
    int radiusValue = settings.value("radiusSlider").toInt();


    QFont font;
    QString fontStr = settings.value("fontFamily").toString();
    font.fromString(fontStr);
    qApp->setFont(font);

    if (settings.contains("main")) {
        QString color = settings.value("main").toString();
        fullStyle += QString("QWidget { background-color: %1;}").arg(color);
    }

    if (settings.contains("button")) {
        QString colorButton = settings.value("button").toString();
        QString colorText = settings.value("buttonText").toString();
        QString borderColor = "transparent";
        fullStyle += QString("QPushButton { background-color: %1; color: %2; font-size: 14px; border-radius: %3px; border: 0.5px solid %5; padding: 0px; margin: 0px; }"
                             " QPushButton:hover { background-color: #1f6db3; }"
                             " QPushButton:pressed { background-color: #1a5a99; }")
                         .arg(colorButton)
                         .arg(colorText)
                         .arg(radiusValue)
                         .arg(borderColor);
    }

    if (settings.contains("lineEdit")) {
        QString colorLineEdit = settings.value("lineEdit").toString();
        QString colorText = settings.value("Text").toString();
        QString borderColor = "lightGray";
        fullStyle += QString("QLineEdit { background-color: %1; color: %2; font-size: 14px; border-radius: %3px; border: 1px solid %5; }")
                         .arg(colorLineEdit)
                         .arg(colorText)
                         .arg(radiusValue)
                         .arg(borderColor);


        fullStyle += QString(
                         "QComboBox {"
                         "  background-color: %1;"
                         "  color: %2;"
                         "  font-size: 14px;"
                         "  border: 1px solid %3;"
                         "  border-radius: %4px;"
                         "  padding: 4px 8px;"
                         "}"
                         "QComboBox::drop-down {"
                         "  subcontrol-origin: padding;"
                         "  subcontrol-position: top right;"
                         "  width: 24px;"
                         "  border-left: 1px solid %3;"
                         "  border-top-right-radius: %4px;"
                         "  border-bottom-right-radius: %4px;"
                         "  background-color: %1;"
                         "}"
                         "QComboBox::down-arrow {"
                         "  image: url(:/icons/dropListIcon.png);"
                         "  width: 12px;"
                         "  height: 12px;"
                         "}"
                         "QComboBox QAbstractItemView {"
                         "  background-color: %1;"
                         "  color: %2;"
                         "  border: 1px solid %3;"
                         "  border-radius: %4px;"
                         "  selection-background-color: %3;"
                         "  padding: 4px;"
                         "}"
                         )
                         .arg(colorLineEdit)
                         .arg(colorText)
                         .arg(borderColor)
                         .arg(radiusValue);
    }

    if (settings.contains("text")) {
        QString color = settings.value("text").toString();
        fullStyle += QString("QLabel { color: %1; font-size: 24px; }")
                         .arg(color);
    }

    //default style
    fullStyle += QString("QGroupBox { font-size: 24px; font-weight: bold; padding-top: 40px; padding-bottom: 20px; font-family: Comic Sans MS;}");


    // Apply to the whole window
    widget->setStyleSheet(fullStyle);
    SettingsPage::instance()->setStyleSheet(fullStyle);
}

