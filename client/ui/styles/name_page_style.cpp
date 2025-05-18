#include "name_page_style.h"

NamePageStyle::NamePageStyle()
    : settings("ChatBye", "ColorSettings")
{

}

void NamePageStyle::setStyle(NamePage *page)
{
    QString fontFamily = settings.value("fontFamily").toString();
    int radiusValue = settings.value("radiusSlider").toInt();

    if (settings.contains("button")) {
        QString color = settings.value("button").toString();
        QString colorText = settings.value("buttonText").toString();
        QString style = QString("background-color: %1; color: %2; font-size: 14px; border-radius: %3px; font-family: %4;").arg(color).arg(colorText).arg(radiusValue).arg(fontFamily);
        QPushButton *checkNameButton = page->getCheckNameButton();
        if (checkNameButton) checkNameButton->setStyleSheet(style);
    }

    if (settings.contains("lineEdit")) {
        QString colorLineEdit = settings.value("lineEdit").toString();
        QString colorText = settings.value("Text").toString();
        QString style = QString("background-color: %1; color: %2; font-size: 14px; border-radius: %3px; font-family: %4;").arg(colorLineEdit).arg(colorText).arg(radiusValue).arg(fontFamily);
        QLineEdit *nameLineEdit = page->getNameLineEdit();
        if (nameLineEdit) nameLineEdit->setStyleSheet(style);
    }

    if (settings.contains("text")) {
        QString color = settings.value("text").toString();
        QString style = QString("color: %1; font-size: 24px; font-family: %2;").arg(color).arg(fontFamily);
        QLabel *hintLabel = page->getHintLabel();
        if (hintLabel) hintLabel->setStyleSheet(style);
    }
}
