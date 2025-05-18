#pragma once

#include <QMessageBox>
#include <QPushButton>

class MessageHelper
{
public:
    MessageHelper();

    static void showCustomMessageBox(QWidget* parent, const QString& title, const QString& text, QMessageBox::Icon icon);
    static bool showConfirmationDialog(QWidget *parent, const QString &title, const QString &text);
    static QMessageBox::StandardButton showCustomConfirmationDialog(QWidget *parent, const QString &title, const QString &text, const QString &informativeText, const QString &btnOne, const QString &btnTwo);
};

