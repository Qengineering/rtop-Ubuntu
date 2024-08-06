#include <QApplication>
#include <QTranslator>
#include <QLocale>
#include <QIcon>
#include <QStringList>
#include "dialog.h"
#include <cstdlib>

int main(int argc, char *argv[])
{
    // Fork the process
    pid_t pid = fork();
    if (pid > 0) {
        // Parent process exits
        exit(0);
    } else if (pid == 0) {
        // Child process runs the app
        QApplication a(argc, argv);

        // Set application icon
        QIcon appIcon("./rtop_icon.png");
        a.setWindowIcon(appIcon);

        Dialog w;
        w.show();
        return a.exec();
    }

    return 0;
}
