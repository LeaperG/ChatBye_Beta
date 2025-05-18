#include "font_picker.h"
#include "client/ui/helpers/message_helper.h"
#include <QHBoxLayout>
#include <QFontDialog>
#include <QFontDatabase>
#include <QCompleter>
#include <QSettings>


FontPickerWidget::FontPickerWidget(QWidget *parent)
    : CustomWidgetBase(parent),
    m_label(new QLabel("Шрифт:", this)),
    m_lineEdit(new QLineEdit(this)) ,
    m_fontButton(new QPushButton("✏️", this)),
    m_currentFont(QFont("Comic Sans MS")),
    layout(new QHBoxLayout(this))
{
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addWidget(m_label);
    layout->addWidget(m_lineEdit);
    layout->addWidget(m_fontButton);

    connect(m_lineEdit, &QLineEdit::editingFinished, this, &FontPickerWidget::onFontEdited);
    connect(m_fontButton, &QPushButton::clicked, this, &FontPickerWidget::onPickFontClicked);
    connect(m_lineEdit, &QLineEdit::editingFinished, this, &FontPickerWidget::validateFontInput);

    QAction *action = m_lineEdit->addAction(QIcon(":/resources/icons/iconUpdate.png"), QLineEdit::TrailingPosition);
    connect(action, &QAction::triggered, this, &FontPickerWidget::setDefaultFont);

    setupStyle();
    setupCompleterForLineEdit();
}

QFont FontPickerWidget::selectedFont() const
{
    return m_currentFont;
}

void FontPickerWidget::setLabelText(const QString &text)
{
    m_label->setText(text);
}

void FontPickerWidget::setInitialFont(const QFont &font)
{
    m_currentFont = font;
    m_lineEdit->setText(m_currentFont.family());
}

QString FontPickerWidget::text()
{
    return m_lineEdit->text();
}

void FontPickerWidget::setText(const QString &text)
{
    m_lineEdit->setText(text);
}

QFont FontPickerWidget::getFontFamily()
{
    return m_currentFont.family();
}

void FontPickerWidget::onFontEdited()
{
    QFont font(m_lineEdit->text());
    setInitialFont(font);
}

void FontPickerWidget::setupStyle()
{
    applyStyle(m_label);
    applyStyle(m_lineEdit);
    //applyStyle(fontButton);

    m_label->setFixedSize(150, 30);
    m_lineEdit->setFixedSize(165, 30);
    m_fontButton->setFixedSize(30, 30);
}

void FontPickerWidget::setupCompleterForLineEdit()
{
    QStringList fontFamilies = QFontDatabase::families();

    QCompleter* completer = new QCompleter(fontFamilies, this);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    completer->setFilterMode(Qt::MatchContains); // You can enter part of the name, not just the beginning.
    m_lineEdit->setCompleter(completer);
}

void FontPickerWidget::validateFontInput()
{
    QString fontName = m_lineEdit->text();

    // Check if there is a font with this name
    if (!QFontDatabase::families().contains(fontName, Qt::CaseInsensitive)) {
        MessageHelper::showCustomMessageBox(this, "Ошибка", "Такого шрифта не существует в системе.", QMessageBox::Critical);
        m_lineEdit->clear();
    } else {
        // If the font exists, you can update the state, e.g. save
        m_currentFont.setFamily(fontName);
    }
}

void FontPickerWidget::onPickFontClicked()
{
    QFontDialog fontDialog(m_currentFont, this);
    fontDialog.setOption(QFontDialog::DontUseNativeDialog);

    // Remove padding from buttons inside the dialog box
    fontDialog.setStyleSheet(R"(
        QPushButton {
            padding: 2px 6px;
            min-height: 24px;
        }
    )");

    if (fontDialog.exec() == QDialog::Accepted) {
        QFont font = fontDialog.selectedFont();
        font.setPointSize(12); // Forcibly fix the size
        m_currentFont = font;


        QString fontFamilyName = QString("%1").arg(font.family());

        // Check if there is a font with this name
        if (!QFontDatabase::families().contains(fontFamilyName, Qt::CaseInsensitive)) {
            MessageHelper::showCustomMessageBox(this, "Ошибка", "Такого шрифта не существует в системе.", QMessageBox::Critical);
            m_lineEdit->clear();
            //setDefaultFont();
        } else {
            m_currentFont.setFamily(fontFamilyName);
        }



        QString displayText = QString("%1").arg(font.family());
        m_lineEdit->setText(displayText);
    }
}

void FontPickerWidget::setDefaultFont()
{
    QFont defaultFont("Comic Sans MS");
    QString displayText = QString("%1").arg(defaultFont.family());
    m_lineEdit->setText(displayText);
    m_currentFont = defaultFont;
}
