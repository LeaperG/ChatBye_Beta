#pragma once

#include "client/ui/custom_widgets/color_selector.h"
#include "client/ui/custom_widgets/font_picker.h"
#include "client/ui/helpers/message_helper.h"
#include <QSettings>
#include <QFileDialog>
#include <QStandardPaths>
#include <QDir>
#include <QMessageBox>
#include <QGroupBox>
#include <QFormLayout>
#include <QSlider>
#include <QPushButton>

class SettingsPage : public QDialog
{
    Q_OBJECT
public:

    static SettingsPage *instance(QWidget *parent = nullptr)
    {
        static SettingsPage instance(parent);
        return &instance;
    }

    QLabel* createImageLabel(const QString &pageName);

private:
    explicit SettingsPage(QWidget *parent = nullptr);

    QLabel *m_imageLabel = nullptr;
    QPixmap m_originalPixmap;

    QList<ColorSelectorWidget*> m_colorWidgets;

    ColorSelectorWidget *mainColorEdit;
    ColorSelectorWidget *clientListColorEdit;
    ColorSelectorWidget *buttonColorEdit;
    ColorSelectorWidget *buttonTextColorEdit;
    ColorSelectorWidget *lineEditColorEdit;
    ColorSelectorWidget *textColorEdit;
    ColorSelectorWidget *clientListNameColorEdit;
    ColorSelectorWidget *getMessageBackColorEdit;
    ColorSelectorWidget *sendMessageBackColorEdit;
    ColorSelectorWidget *systemMessageBackColorEdit;
    ColorSelectorWidget *textMessageColorEdit;
    ColorSelectorWidget *senderColorEdit;
    ColorSelectorWidget *timeColorEdit;
    ColorSelectorWidget *chatColorEdit;
    FontPickerWidget *fontFamilyEdit;

    QLabel *m_radiusLabel;
    QSlider *m_radiusSlider;

    void isImageAvailable(const QString &pageName);

private slots:
    void applyBtn();
    void resetColors();
    void saveAndExitBtn();
    void saveColors();
    void loadColors();
    void sliderUpdated(int value);

    void changeImage(const QString &pageName);


signals:
    void exitPage();
    void colorChanged();
};

