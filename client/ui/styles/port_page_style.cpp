#include "port_page_style.h"

#include <QSettings>

PortPageStyle::PortPageStyle()
    :   settings("ChatBye", "ColorSettings")
{

}

void PortPageStyle::setStyle(PortPage *page)
{
    QString fontFamily = settings.value("fontFamily").toString();
    int radiusValue = settings.value("radiusSlider").toInt();

    if (settings.contains("button")) {
        QString colorButton = settings.value("button").toString();
        QString colorText = settings.value("buttonText").toString();
        QString style = QString("QPushButton { background-color: %1; color: %2; font-size: 14px; border-radius: %3px; font-family: %4; } QPushButton:hover { background-color: #1f6db3; } QPushButton:pressed { background-color: #1a5a99; }").arg(colorButton).arg(colorText).arg(radiusValue).arg(fontFamily);
        QList<QPushButton*> buttons = page->getButtons();
        for (QPushButton *button : buttons) {
            button->setStyleSheet(style);
        }
    }

    if (settings.contains("lineEdit")) {
        QString colorLineEdit = settings.value("lineEdit").toString();
        QString colorText = settings.value("Text").toString();
        QString style = QString("background-color: %1; color: %2; font-size: 14px; border-radius: %3px; font-family: %4;").arg(colorLineEdit).arg(colorText).arg(radiusValue).arg(fontFamily);
        QLineEdit *portLineEdit = page->getPortLineEdit();
        portLineEdit->setStyleSheet(style);
    }

    if (settings.contains("text")) {
        QString color = settings.value("text").toString();
        QString style = QString("color: %1; font-size: 24px; font-family: %2;").arg(color).arg(fontFamily);
        QList<QLabel*> labels = page->getLabels();
        for (QLabel *label : labels) {
            label->setStyleSheet(style);
        }
    }

    if (page->progressDialog()) {
        ProgressDialog * progressDialog = page->progressDialog();
        QString style = " QProgressBar { border: 2px solid #4A90E2; border-radius: 8px; background-color: #E0E0E0; text-align: center; height: 20px; color: #000000; font-weight: bold; }"
                        " QProgressBar::chunk { background: QLinearGradient(x1:0, y1:0, x2:1, y2:0, stop:0 #4A90E2, stop:1 #357ABD); border-radius: 8px; }";
        progressDialog->setStyleSheet(style);
    }

}
