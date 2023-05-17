#include <QApplication>
#include <QTextEdit>
#include "DropSiteWindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    DropSiteWindow window;
    window.show();
    return app.exec();
}
