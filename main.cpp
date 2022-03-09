#include <QApplication>
#include "solarped_control_interface.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    SolarpedControlInterface interface;
    interface.show();
    return app.exec();
}
