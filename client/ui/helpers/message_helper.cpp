#include "message_helper.h"

MessageHelper::MessageHelper() {}

void MessageHelper::showCustomMessageBox(QWidget *parent, const QString &title, const QString &text, QMessageBox::Icon icon)
{
    QMessageBox msgBox(parent);
    msgBox.setWindowTitle(title);
    msgBox.setText(text);
    msgBox.setIcon(icon);
    msgBox.setStyleSheet(
        "QMessageBox {"
        "    background-color: #2B2B2B;"
        "    color: white;"
        "    border-radius: 10px;"
        "    padding: 15px;"
        "}"
        "QMessageBox QLabel {"
        "    font-size: 18px;"
        "    font-weight: bold;"
        "    color: #E57373;"
        "    background-color: #2B2B2B;"
        "}"
        "QPushButton {"
        "    background-color: #D9534F;"
        "    color: white;"
        "    border-radius: 8px;"
        "    padding: 8px 15px;"
        "    font-size: 16px;"
        "    font-weight: bold;"
        "    border: 1px solid #B52B27;"
        "}"
        "QPushButton:hover {"
        "    background-color: #E57373;"
        "}"
        "QPushButton:pressed {"
        "    background-color: #B52B27;"
        "}"
        "QPushButton:focus {"
        "    outline: none;"
        "    border: 2px solid #FFCDD2;"
        "}"
        );
    msgBox.exec();
}

bool MessageHelper::showConfirmationDialog(QWidget *parent, const QString &title, const QString &text)
{
    QMessageBox msgBox(parent);
    msgBox.setWindowTitle(title);
    msgBox.setText(text);
    msgBox.setIcon(QMessageBox::Question);

    // Apply window styling
    msgBox.setStyleSheet(
        "QMessageBox {"
        "    background-color: #2B2B2B;"
        "    color: white;"
        "    border-radius: 10px;"
        "    padding: 15px;"
        "}"
        "QMessageBox QLabel {"
        "    font-size: 18px;"
        "    font-weight: bold;"
        "    color: #E0E0E0;"
        "    background-color: #2B2B2B;"
        "}"
        "QPushButton {"
        "    background-color: #3A6EA5;"
        "    color: white;"
        "    border-radius: 8px;"
        "    padding: 8px 15px;"
        "    font-size: 16px;"
        "    font-weight: bold;"
        "    border: 1px solid #1E4C7F;"
        "}"
        "QPushButton:hover {"
        "    background-color: #4A86C5;"
        "}"
        "QPushButton:pressed {"
        "    background-color: #1E4C7F;"
        "}"
        "QPushButton:focus {"
        "    outline: none;"
        "    border: 2px solid #87CEFA;"
        "}"
        );

    QPushButton *btnYes = msgBox.addButton("Да", QMessageBox::YesRole);
    QPushButton *btnNo = msgBox.addButton("Нет", QMessageBox::NoRole);

    msgBox.setDefaultButton(btnYes);
    msgBox.exec();

    return msgBox.clickedButton() == btnYes;
}

QMessageBox::StandardButton MessageHelper::showCustomConfirmationDialog(QWidget *parent, const QString &title, const QString &text, const QString &informativeText, const QString &btnOne, const QString &btnTwo)
{
    // Creating a message window
    QMessageBox msgBox(parent);
    msgBox.setWindowTitle(title);
    msgBox.setText(text);
    msgBox.setInformativeText(informativeText);
    msgBox.setIcon(QMessageBox::Question);

    // Apply window styling
    msgBox.setStyleSheet(
        "QMessageBox {"
        "    background-color: #2B2B2B;"
        "    color: white;"
        "    border-radius: 10px;"
        "    padding: 15px;"
        "}"
        "QMessageBox QLabel {"
        "    font-size: 18px;"
        "    font-weight: bold;"
        "    color: #E0E0E0;"
        "    background-color: #2B2B2B;"
        "}"
        "QPushButton {"
        "    background-color: #3A6EA5;"
        "    color: white;"
        "    border-radius: 8px;"
        "    padding: 8px 15px;"
        "    font-size: 16px;"
        "    font-weight: bold;"
        "    border: 1px solid #1E4C7F;"
        "}"
        "QPushButton:hover {"
        "    background-color: #4A86C5;"
        "}"
        "QPushButton:pressed {"
        "    background-color: #1E4C7F;"
        "}"
        "QPushButton:focus {"
        "    outline: none;"
        "    border: 2px solid #87CEFA;"
        "}"
        );



    // Добавляем кнопки
    QPushButton *buttonOne = msgBox.addButton(btnOne, QMessageBox::YesRole);
    QPushButton *buttonTwo = msgBox.addButton(btnTwo, QMessageBox::NoRole);
    QPushButton *cancelButton = msgBox.addButton(QMessageBox::Cancel);
    cancelButton->setText("Отмена");


    msgBox.exec();

    // Check which button was pressed
    if (msgBox.clickedButton() == buttonOne) {
        return QMessageBox::Yes;
    } else if (msgBox.clickedButton() == buttonTwo) {
        return QMessageBox::No;
    } else {
        return QMessageBox::Cancel;
    }
}
