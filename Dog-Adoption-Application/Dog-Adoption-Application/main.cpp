#include "QTA12.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    std::string fileName = "dogs.txt";
    Repository repo(fileName);
    repo.loadEntitiesFromFile();
    Service serv{ repo };
    UserService userServ;

    QTA12 w{serv, userServ};
    w.show();
    return a.exec();
}
