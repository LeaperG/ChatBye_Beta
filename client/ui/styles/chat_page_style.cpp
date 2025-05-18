#include "chat_page_style.h"

#include <QSettings>

ChatPageStyle::ChatPageStyle()
    :   settings("ChatBye", "ColorSettings")
{

}

void ChatPageStyle::setStyle(ChatPage *page)
{
    //QString fontFamily = settings.value("fontFamily").toString();
    int radiusValue = settings.value("radiusSlider").toInt();

    if (settings.contains("clientList")) {
        QString color = settings.value("clientList").toString();
        QString style = QString("QListWidget { background-color: %1; border-radius: %2px; font-size: 18px; color: black; }"
                                " QScrollBar:vertical { border: none; background: transparent; width: 6px; margin: 0; }"
                                " QScrollBar::handle:vertical { background: #a4aae5; min-height: 30px; border-radius: 3px; }"
                                " QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical { background: transparent; height: 15px; }"

                                " QScrollBar:horizontal { border: none; background: transparent; height: 8px; margin: 0; }"
                                " QScrollBar::handle:horizontal { background: #a4aae5; min-width: 30px; border-radius: 3px; }"
                                " QScrollBar::add-line:horizontal, QScrollBar::sub-line:horizontal { background: transparent; width: 15px; }"
                                ).arg(color).arg(radiusValue);
        QListWidget *usersList = page->getUsersList();
        if (usersList) usersList->setStyleSheet(style);
    }


    // if (settings.contains("button")) {
    //     QString color = settings.value("button").toString();
    //     QString colorText = settings.value("buttonText").toString();
    //     QString style = QString("background-color: %1; color: %2; border-radius: %3px; font-size: 14px; ").arg(color).arg(colorText).arg(radiusValue);
    //     QPushButton *changeNameButton = page->getChangeNameButton();
    //     if (changeNameButton) changeNameButton->setStyleSheet(style);
    // }

    // if (settings.contains("lineEdit")) {
    //     QString colorLineEdit = settings.value("lineEdit").toString();
    //     QString colorText = settings.value("Text").toString();
    //     QString style = QString("QLineEdit { background-color: %1; color: %2; font-size: 14px; border-radius: %3px; ").arg(colorLineEdit).arg(colorText).arg(radiusValue);
    //     QLineEdit *messageInput = page->getMessageInput();
    //     if (messageInput) messageInput->setStyleSheet(style);
    // }
}

