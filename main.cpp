#include <QCoreApplication>
#include "calculationstrategy.h"
#include "ByFileTypeCalculationStrategy.h"
#include "ByFolderCalculationStrategy.h"

int main(int argc, char* argv[]) {
    QCoreApplication a(argc, argv);

    return a.exec();
}

