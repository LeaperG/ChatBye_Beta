#pragma once

#include "client/ui/custom_widgets/custom_widget_base.h"
#include <QWidget>
#include <QString>
#include <QColor>


class QLabel;
class QLineEdit;
class QPushButton;
class QHBoxLayout;

class ColorSelectorWidget : public CustomWidgetBase
{
    Q_OBJECT
public:
    explicit ColorSelectorWidget(const QString& labelText, const QColor &defaultColor, QWidget *parent = nullptr);

    QColor color() const;
    void setColorCode(const QString &code);
    void setDefaultColor();
    QLineEdit *lineEdit;

signals:
    //void colorChanged(const QColor& color); // Signal when the color changes

protected:
    // Поддержка drag-and-drop
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private slots:
    void onColorEdited();
    void onPickColorClicked();

private:
    void updateColorPreview();
    void setupStyle();
    void startColorDrag();

    QLabel *m_label;
    QPushButton *m_pickColorButton;
    QWidget *m_colorPreview;
    QHBoxLayout *layout;

    QColor m_currentColor;
    QColor m_defaultColor;
};
