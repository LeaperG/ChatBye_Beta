#include "color_selector.h"
#include "client/ui/helpers/shadow_helper.h"

#include <QLabel>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QColor>
#include <QColorDialog>
#include <QPushButton>
#include <QColor>

#include <QMimeData>
#include <QDrag>
#include <QMouseEvent>
#include <QDebug>
#include <QGraphicsDropShadowEffect>


ColorSelectorWidget::ColorSelectorWidget(const QString& labelText, const QColor &defaultColor, QWidget *parent)
    : CustomWidgetBase{parent}, m_currentColor(Qt::white)
{

    this->m_defaultColor = defaultColor;

    // Create interface elements
    m_label = new QLabel(labelText);
    lineEdit = new QLineEdit();
    m_pickColorButton = new QPushButton("🎨");
    m_colorPreview = new QWidget();

    m_colorPreview->setAcceptDrops(true);
    m_colorPreview->setCursor(Qt::PointingHandCursor);

    m_pickColorButton->setCursor(Qt::PointingHandCursor);

    // Set layout
    layout = new QHBoxLayout(this);
    layout->addWidget(m_label);
    layout->addWidget(lineEdit);
    layout->addWidget(m_colorPreview);
    layout->addWidget(m_pickColorButton);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    // Signals and Slots
    connect(lineEdit, &QLineEdit::editingFinished, this, &ColorSelectorWidget::onColorEdited);
    connect(m_pickColorButton, &QPushButton::clicked, this, &ColorSelectorWidget::onPickColorClicked);

    QAction* action = lineEdit->addAction(QIcon(":/resources/icons/iconUpdate.png"), QLineEdit::TrailingPosition);
    connect(action, &QAction::triggered, this, &ColorSelectorWidget::setDefaultColor);


    setAcceptDrops(true);

    setupStyle();
    setColorCode("lime");
}

QColor ColorSelectorWidget::color() const
{
    return m_currentColor;
}

void ColorSelectorWidget::setColorCode(const QString &code)
{
    QColor color(code);
    if (color.isValid() && color != m_currentColor) {
        m_currentColor = color;
        lineEdit->setText(color.name(QColor::HexArgb));
        updateColorPreview();
        //emit colorChanged(currentColor);
    }
}


void ColorSelectorWidget::onColorEdited()
{
    setColorCode(lineEdit->text());
}

void ColorSelectorWidget::onPickColorClicked()
{
    QColorDialog dialog(m_currentColor, this);
    dialog.setOption(QColorDialog::ShowAlphaChannel);
    dialog.setOption(QColorDialog::DontUseNativeDialog);

    // Remove padding from buttons inside the dialog box
    dialog.setStyleSheet(R"(
        QPushButton {
            padding: 2px 6px;
            min-height: 24px;
        }
    )");


    if (dialog.exec() == QDialog::Accepted) {
        QColor chosenColor = dialog.selectedColor();
        setColorCode(chosenColor.name(QColor::HexArgb));
    }
}


void ColorSelectorWidget::updateColorPreview()
{
    if (!m_colorPreview) return;
    QString style = QString(
                        "background-color: %1;"
                        "border: 1px solid #444;"
                        "border-radius: 4px;")
                        .arg(m_currentColor.name(QColor::HexArgb));
    m_colorPreview->setStyleSheet(style);
}

void ColorSelectorWidget::setDefaultColor()
{
    lineEdit->setText(m_defaultColor.name(QColor::HexArgb));
    onColorEdited();
}

void ColorSelectorWidget::setupStyle()
{
    m_label->setFixedSize(150, 30);
    lineEdit->setFixedSize(130, 30);
    m_colorPreview->setFixedSize(30, 30);
    m_pickColorButton->setFixedSize(30, 30);


    //pickColorButton->setStyleSheet("background-color: #1E88E5; border-radius: 5px; font-size: 14px; font-family: Comic Sans MS;" );
    m_label->setStyleSheet(defaultLabelStyle());
    lineEdit->setStyleSheet(defaultLineEditStyle());
    m_colorPreview->setStyleSheet("QFrame#colorBox {border: 1px solid #333; border-radius: 3px; background-color: #ffffff;}");


    // // Тени
    ShadowHelper::applyShadow(m_colorPreview, QColor(0, 0, 0, 100), 10, QPointF(2,2));
    // applyShadow(pickColorButton);
    // applyShadow(lineEdit);

    updateColorPreview();
}


// DRAG & DROP

void ColorSelectorWidget::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasText()) {
        QColor color(event->mimeData()->text());
        if (color.isValid()) {
            event->acceptProposedAction();
        }
    }
}

void ColorSelectorWidget::dropEvent(QDropEvent *event)
{
    QColor droppedColor(event->mimeData()->text());
    if (droppedColor.isValid()) {
        setColorCode(droppedColor.name(QColor::HexArgb));
        event->acceptProposedAction();
    }
}

void ColorSelectorWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && m_colorPreview->geometry().contains(event->pos())) {
        startColorDrag();
    }
}


void ColorSelectorWidget::startColorDrag()
{
    QMimeData *mimeData = new QMimeData;
    mimeData->setText(m_currentColor.name(QColor::HexArgb));

    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimeData);
    drag->exec(Qt::CopyAction);
}

