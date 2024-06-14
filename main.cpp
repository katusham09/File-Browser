#include <QCoreApplication>
#include <iostream>
#include <memory>
#include "calculationstrategy.h"
#include "ByFileTypeCalculationStrategy.h"
#include "ByFolderCalculationStrategy.h"
#include "FileBrowser.h"

// Функция для проверки существования директории
bool isValidDirectory(const QString& path) {
    QDir dir(path);
    return dir.exists();
}

// Функция для проверки допустимости выбранной стратегии
bool isValidStrategy(const QString& strategy) {
    return strategy == "filetype" || strategy == "folder";
}

// Функция для удаления кавычек из строки
QString removeQuotes(const QString& str) {
    QString result = str;
    if (result.startsWith('"') && result.endsWith('"')) {
        result = result.mid(1, result.length() - 2);
    }
    return result;
}

int main(int argc, char *argv[]) {
    QCoreApplication app(argc, argv);

    QString path; // Путь к директории
    QString strategyName; // Название стратегии

    std::cout << "Enter the directory path: ";
    std::string inputPath;
    std::getline(std::cin, inputPath);
    path = QString::fromStdString(inputPath);
    path = removeQuotes(path);

    // Проверяем, существует ли указанная директория
    if (!isValidDirectory(path)) {
        std::cerr << "Error: Invalid directory path." << std::endl;
        return 1; // Завершаем программу с ошибкой
    }

    std::cout << "Enter the strategy (filetype/folder): ";
    std::string inputStrategy;
    std::getline(std::cin, inputStrategy);
    strategyName = QString::fromStdString(inputStrategy);

    // Проверяем, является ли указанная стратегия допустимой
    if (!isValidStrategy(strategyName)) {
        std::cerr << "Error: Invalid strategy." << std::endl;
        return 1; // Завершаем программу с ошибкой
    }

    // Указатель на стратегию расчета
    std::unique_ptr<CalculationStrategy> strategy;

    // Создаем экземпляр выбранной стратегии
    if (strategyName == "filetype") {
        strategy = std::make_unique<ByFileTypeCalculationStrategy>();
    } else if (strategyName == "folder") {
        strategy = std::make_unique<ByFolderCalculationStrategy>();
    }

    // Создаем экземпляр FileBrowser с выбранной стратегией
    FileBrowser browser(std::move(strategy));
    // Запускаем расчет
    browser.calculate(path);

    return 0; // Завершаем программу успешно
}
