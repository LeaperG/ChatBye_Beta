#pragma once

#include <QDialog>
#include <QObject>
#include <QLabel>
#include <QProgressDialog>

class ProgressDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ProgressDialog(QWidget *parent = nullptr);
    void setMessage(const QString &text);
    void setGif(const QString &gifPath);
    void setProgress(int current, int max);

private:
    QLabel *m_gifLabel;
    QLabel *m_textLabel;
    QMovie *m_movie;
    QProgressBar *m_progressBar;
    QPushButton *m_cancelButton;

signals:
    void canceled();
};

