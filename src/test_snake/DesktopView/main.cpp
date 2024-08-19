#include <QApplication>
#include "desktopview.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    s21::DesktopView app;
    app.setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
    app.setFixedSize(800, 600);
    app.show();
    return a.exec();
}

