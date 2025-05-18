#pragma once

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QPixmap>
#include <QPushButton>
#include <QLineEdit>
#include <QSettings>
#include <QFileDialog>
#include <QAbstractButton>
#include <QToolButton>
#include <QPushButton>
#include "helpers/dictionary/server_message_dictionary.h"
#include <QGraphicsDropShadowEffect>

class PageBase : public QWidget
{
    Q_OBJECT

public:
    explicit PageBase(QWidget *parent = nullptr) : QWidget(parent) {}
    virtual ~PageBase();

protected:
    // Function for adding an image
    QLabel* createImageLabel(const QString &pageName);

    // Function for activating/deactivating the button depending on the text
    void onTextChanged(const QString &text, QPushButton *button);

    // Connects textChanged and enables/disables the button automatically
    void bindLineEditToButton(QLineEdit *lineEdit, QAbstractButton *button);

    // Safely calls click() only if the field is not empty
    void onReturnPressed(QLineEdit *lineEdit, QAbstractButton *button);

    template<typename ButtonType, typename Receiver, typename Slot>
    void setupInputAction(QLineEdit *lineEdit, ButtonType *button, Receiver *receiver, Slot slot);

    ServerMessageDictionary messageDict;
    void updateImageSize();

private:
    void resizeEvent(QResizeEvent *event);
    QLabel *m_imageLabel = nullptr;
    QPixmap m_originalPixmap;

    double m_aspectRatio = 1.0;
};


template<typename ButtonType, typename Receiver, typename Slot>
void PageBase::setupInputAction(QLineEdit *lineEdit, ButtonType *button, Receiver *receiver, Slot slot)
{
    // Connects textChanged and enables/disables the button automatically
    bindLineEditToButton(lineEdit, button);

    // Safely calls click() only if the field is not empty
    connect(lineEdit, &QLineEdit::returnPressed, this, [=]() {
        onReturnPressed(lineEdit, button);
    });

    connect(button, &QAbstractButton::clicked, receiver, slot);
}
