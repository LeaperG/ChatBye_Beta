#include "image_settings_manager.h"
#include "client/ui/helpers/message_helper.h"

#include <QSettings>
#include <QFile>
#include <QStandardPaths>
#include <QDir>
#include <QMessageBox>

ImageSettingsManager::ImageSettingsManager(QObject *parent)
    : QObject{parent}
{}

QString ImageSettingsManager::loadImage(const QString &pageName)
{
    QSettings settings("ChatBye", "ImageSettings");
    QString imagePath = settings.value(QString("imagePath%1").arg(pageName)).toString();

    if (!imagePath.isEmpty() && QFile::exists(imagePath)) {
        return imagePath;
    }

    if (imagePath.isEmpty() && !QFile::exists(imagePath)) {
        imagePath = getDefaultImagePath(pageName);
        return imagePath;
    }

    return QString();
}

void ImageSettingsManager::saveImage(const QString &sourcePath, const QString &pageName)
{
    if (sourcePath.isEmpty() || !QFile::exists(sourcePath)) {
        MessageHelper::showCustomMessageBox(nullptr, "Ошибка", "Исходный файл не найден.", QMessageBox::Critical);
        return;
    }

    QString appDataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir().mkpath(appDataPath);

    QFileInfo fileInfo(sourcePath);
    QString newExtension = fileInfo.suffix();


    // Delete all old files with this pageName
    QDir dir(appDataPath);
    QStringList filters = {QString("image%1.*").arg(pageName)};
    QStringList oldFiles = dir.entryList(filters, QDir::Files);

    for (const QString &oldFile : oldFiles) {
        QFile::remove(dir.filePath(oldFile));
    }

    // Form the path for the new image
    QString destPath = appDataPath + QString("/image%1.%2").arg(pageName).arg(newExtension);

    if (QFile::copy(sourcePath, destPath)) {
        QSettings settings("ChatBye", "ImageSettings");
        settings.setValue(QString("imagePath%1").arg(pageName), destPath);

        qDebug() <<sourcePath;
        notifyImageChanged(pageName);

    }
    else
    {
        MessageHelper::showCustomMessageBox(nullptr, "Ошибка", "Не удалось скопировать изображение в папку приложения.", QMessageBox::Critical);
    }
}

void ImageSettingsManager::resetImage(const QString &pageName)
{
    QSettings settings("ChatBye", "ImageSettings");
    QString key = QString("imagePath%1").arg(pageName);

    if (settings.contains(key)) {
        QString imagePath = settings.value(key).toString();

        if (!imagePath.isEmpty() && QFile::exists(imagePath)) {
            QFile::remove(imagePath);
        }

        settings.remove(key);
        settings.sync();
    }
}

QString ImageSettingsManager::getDefaultImagePath(const QString &pageName)
{
    QSettings settings("ChatBye", "ColorSettings");
    QString chatColor = settings.value("chat").toString();

    if (pageName == "PortPage") return ":/images/portPageImg.png";
    if (pageName == "NamePage") return ":/images/namePageImg.png";
    if (pageName == "ChatPage") return chatColor;
    return QString();
}

void ImageSettingsManager::notifyImageChanged(const QString &pageName)
{
    emit imageUpdated(pageName);
}

bool ImageSettingsManager::isImageAvailable(const QString &pageName)
{
    QSettings settings("ChatBye", "ImageSettings");
    QString imagePath = settings.value(QString("imagePath%1").arg(pageName)).toString();

    // Check if the image exists and the path is not empty
    return !imagePath.isEmpty() && QFile::exists(imagePath);
}
