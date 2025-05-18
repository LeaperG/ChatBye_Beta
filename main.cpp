#include "client/main_window.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // QPixmap m_pixmap = QPixmap("");
    // QPixmap scaledPixmap = m_pixmap.scaled(15, 15, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    // QCursor m_cursor = QCursor(scaledPixmap);
    // a.setOverrideCursor(m_cursor);

    MainWindow w;
    w.resize(800, 600);
    w.show();
    return a.exec();
}
