#include "src/mainwindow.h"
#include "src/app.h"

int main(int argc, char *argv[])
{
    App a(argc, argv, "Tacher3000", "SAPR3000");
    MainWindow w;
    w.showMaximized();

    return a.exec();
}
