#ifndef CALCULATIONSTRATEGY_H
#define CALCULATIONSTRATEGY_H
#include "iostream"
#include "QFileInfo"
#include "QDir"


class CalculationStrategy {
public:
    virtual ~CalculationStrategy() {} // Виртуальный деструктор

    // Функция для подсчета размеров файлов и папок
    virtual std::map<QString, std::pair<int, double>> calculate(const QString& path) = 0;
};

#endif // CALCULATIONSTRATEGY_H
