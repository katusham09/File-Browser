#include <QCoreApplication>
#include "calculationstrategy.h"
#include "ByFileTypeCalculationStrategy.h"
#include "ByFolderCalculationStrategy.h"
#include "FileBrowser.h"

bool isValidDirectory(const QString& path) {
    QDir dir(path);
    return dir.exists();
}

bool isValidStrategy(const QString& strategy) {
    return strategy == "filetype" || strategy == "folder";
}

int main(int argc, char *argv[]) {
    QCoreApplication app(argc, argv);

    QString path;
    QString strategyName;

    if (argc == 3) {
        path = argv[1];
        strategyName = argv[2];
    } else {
        std::cout << "Enter the directory path: ";
        std::string inputPath;
        std::getline(std::cin, inputPath);
        path = QString::fromStdString(inputPath);

        std::cout << "Enter the strategy (filetype/folder): ";
        std::string inputStrategy;
        std::getline(std::cin, inputStrategy);
        strategyName = QString::fromStdString(inputStrategy);
    }

    if (!isValidDirectory(path)) {
        std::cerr << "Error: Invalid directory path." << std::endl;
        return 1;
    }

    if (!isValidStrategy(strategyName)) {
        std::cerr << "Error: Invalid strategy." << std::endl;
        return 1;
    }

    std::unique_ptr<CalculationStrategy> strategy;

    if (strategyName == "filetype") {
        strategy = std::make_unique<ByFileTypeCalculationStrategy>();
    } else if (strategyName == "folder") {
        strategy = std::make_unique<ByFolderCalculationStrategy>();
    }

    FileBrowser browser(std::move(strategy));
    browser.calculate(path);

    return 0;
}
