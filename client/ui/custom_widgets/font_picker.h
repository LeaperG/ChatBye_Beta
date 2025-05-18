#pragma once

#include "client/ui/custom_widgets/custom_widget_base.h"
#include <QWidget>
#include <QFont>
class QFontDialog;
class QHBoxLayout;

class FontPickerWidget : public CustomWidgetBase
{
    Q_OBJECT
public:
    explicit FontPickerWidget(QWidget *parent = nullptr);

    QFont selectedFont() const;
    void setLabelText(const QString &text);
    void setInitialFont(const QFont &font);
    QString text();
    void setText(const QString &text);
    QFont getFontFamily();


private:
    QHBoxLayout *layout;
    QLabel *m_label;
    QLineEdit *m_lineEdit;
    QPushButton *m_fontButton;
    QFont m_currentFont;

    void onFontEdited();
    void setupStyle();
    void setupCompleterForLineEdit();
    void validateFontInput();

public slots:
    void setDefaultFont();

private slots:
    void onPickFontClicked();

};

