#pragma once

#include "client/ui/custom_widgets/progress_dialog.h"
#include "helpers/message_enum/interface_filter.h"
#include "helpers/message_enum/host_binding_mode.h"

#include "page_base.h"
#include <QMessageBox>
#include <QTimer>
#include <QNetworkInterface>
#include <QComboBox>
#include <QProgressDialog>

class PortPage : public PageBase
{
    Q_OBJECT
public:
    explicit PortPage(QWidget *parent = nullptr);
    ~PortPage();
    QList<QPushButton*> getButtons() const;
    QList<QLabel*> getLabels() const;
    QLineEdit *getPortLineEdit() const;
    QComboBox* getComboBoxIp() const;
    ProgressDialog* progressDialog() const;

private:
    QVBoxLayout *imageLayout;
    QLabel *m_imageLabel = nullptr;
    QLabel *m_hostLabel = nullptr;
    QLabel *m_portLabel = nullptr;
    QLineEdit *m_portLineEdit;
    QPushButton *m_connectButton;
    QPushButton *m_quickSearchWifiBtn;
    QPushButton *m_quickSearchLANBtn;
    QPushButton *m_setValueBtn;
    QComboBox* m_comboBoxIp;

    QList<QPushButton*> m_buttons;
    QList<QLabel*> m_labels;

    QTimer *m_updateTimer;
    QList<QHostAddress> m_allHostIp;
    void updateHostAddress(InterfaceFilter filter);
    InterfaceFilter m_filter;
    QList<QNetworkAddressEntry> m_availableEntries;
    QNetworkAddressEntry m_selectedEntry;
    void updatePort();
    void initUi();

    // Variable for status tracking:
    bool advancedVisible = true;
    void setVisibleUi();
    QNetworkAddressEntry findEntryByIp(const QString &ipText);

    ProgressDialog* m_progressDialog = nullptr;

private slots:
    void onCheckHostAndPort();

public slots:
    void setHostAddresses(const QList<QNetworkAddressEntry> &entryList);
    void updateStyleSheet();
    void updateImage(const QString &pageName);

    void showProgressDialog();
    void updateProgress(int current, int max);
    void closeProgressDialog();


signals:
    void hostAndPortEntered(const QNetworkAddressEntry &entry, quint16 port, HostBindingMode mode);
    void requestHost(InterfaceFilter filter);
    void scanProgressCanceled();
};
