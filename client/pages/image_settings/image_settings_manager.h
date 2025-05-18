#pragma once

#include <QObject>
#include <QString>

class ImageSettingsManager : public QObject
{
    Q_OBJECT
public:

    static ImageSettingsManager* instance(QObject *parent = nullptr)
    {
        static ImageSettingsManager instance(parent);
        return &instance;
    }


    QString loadImage(const QString &pageName);
    void saveImage(const QString &sourcePath, const QString &pageName);
    void resetImage(const QString &pageName);
    QString getDefaultImagePath(const QString &pageName);
    void notifyImageChanged(const QString &pageName);
    bool isImageAvailable(const QString &pageName);

private:
    explicit ImageSettingsManager(QObject *parent = nullptr);

signals:
    void imageUpdated(const QString &pageName);
};

