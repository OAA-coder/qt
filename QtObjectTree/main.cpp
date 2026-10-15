#include <QApplication>
#include <QByteArray>
#include <QString>

#ifdef Q_OS_WIN
#  ifndef NOMINMAX
#    define NOMINMAX
#  endif
#  include <windows.h>
#endif

#include "mainwindow.h"

#ifdef Q_OS_WIN
static void utf8MessageHandler(QtMsgType type,
                               const QMessageLogContext &,
                               const QString &msg)
{
    QByteArray utf8 = msg.toUtf8();
    utf8.append('\n');

    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD written = 0;
    WriteConsoleA(h, utf8.constData(),
                  static_cast<DWORD>(utf8.size()), &written, nullptr);

    if (type == QtWarningMsg || type == QtCriticalMsg || type == QtFatalMsg) {
        OutputDebugStringA(utf8.constData());
    }
}
#endif

int main(int argc, char *argv[])
{
#ifdef Q_OS_WIN
    qInstallMessageHandler(utf8MessageHandler);

    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif

    QApplication app(argc, argv);
    MainWindow window;
    window.show();
    return app.exec();
}