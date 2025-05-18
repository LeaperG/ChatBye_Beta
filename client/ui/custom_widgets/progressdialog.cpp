#include "progress_dialog.h"

#include <QVBoxLayout>
#include <QMovie>
#include <QProgressBar>
#include <qpushbutton.h>

ProgressDialog::ProgressDialog(QWidget *parent)
    : QDialog{parent},
    m_gifLabel(new QLabel(this)),
    m_textLabel(new QLabel(this)),
    m_movie(new QMovie(this)),
    m_progressBar(new QProgressBar(this)),
    m_cancelButton(new QPushButton("Отмена", this))
{
    setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
    setModal(true);
    setFixedSize(200, 200);

    m_gifLabel->setAlignment(Qt::AlignCenter);
    m_textLabel->setAlignment(Qt::AlignCenter);
    m_textLabel->setStyleSheet("font-size: 14px; color: #333;");
    m_progressBar->setRange(0, 100);
    m_progressBar->setValue(0);
    m_cancelButton->setFixedHeight(30);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(m_gifLabel);
    layout->addWidget(m_textLabel);
    layout->addWidget(m_progressBar);
    layout->addWidget(m_cancelButton);

    connect(m_cancelButton, &QPushButton::clicked, this, &ProgressDialog::canceled);
}

void ProgressDialog::setMessage(const QString &text)
{
    m_textLabel->setText(text);
}

void ProgressDialog::setGif(const QString &gifPath)
{
    m_movie->setFileName(gifPath);
    m_gifLabel->setMovie(m_movie);
    m_movie->start();
}

void ProgressDialog::setProgress(int current, int max)
{
    if (max > 0) {
        m_progressBar->setRange(0, max);
        m_progressBar->setValue(current);
    }
}
