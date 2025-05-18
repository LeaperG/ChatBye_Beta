#pragma once

#include "client/pages/page_base.h"
#include <QMessageBox>
#include <QTimer>

class NamePage : public PageBase
{
    Q_OBJECT
public:
    explicit NamePage(QWidget *parent = nullptr);
    QPushButton* getCheckNameButton() const;
    QLineEdit* getNameLineEdit() const;
    QLabel* getHintLabel() const;

private:
    QVBoxLayout *imageLayout;
    QLabel *m_imageLabel = nullptr;

    QLabel *m_hintLabel;
    QLineEdit *m_nameLineEdit;
    QPushButton *m_checkNameButton;
    QString m_clientName;

private slots:
    void isNameTaken();
    void updateStyleSheet();

public slots:
    void updateImage(const QString &pageName);
    void handleNameAccepted(const QString &name);
    void handleNameRejected(const QString &name);

signals:
    void nameEntered(const QString name);
};
