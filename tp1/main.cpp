#include "Tp1Application.h"

int main(int argc, char *argv[])
{
    Tp1Application a(argc, argv);
    a.addNewWorkspace();

    return a.exec();
}
