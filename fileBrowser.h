#ifndef FILEBROWSER_H
#define FILEBROWSER_H

#include "calculationstrategy.h"
#include <memory>
#include <iostream>

// Класс FileBrowser для работы с файлами и папками в файловой системе
class FileBrowser {
public:
    // Конструктор, принимающий стратегию расчета
    explicit FileBrowser(std::unique_ptr<CalculationStrategy> strategy)
        : strategy_(std::move(strategy)) {}

    // Метод для выполнения расчета размеров файлов и папок по заданному пути
    void calculate(const QString& path) const {
        // Используем стратегию для расчета размеров и получаем результат
        auto results = strategy_->calculate(path);

        // Выводим результаты на консоль
        for (const auto& entry : results) {
            QString item = entry.first; // Название элемента (тип файла или имя папки)
            int size = entry.second.first; // Размер в байтах
            double percentage = entry.second.second; // Процентное соотношение
            // Выводим на консоль информацию о размере и процентном соотношении
            std::cout << item.toStdString() << ": " << size << " bytes, " << QString::number(percentage, 'f', 2).toStdString() << "%" << std::endl;
        }
    }

private:
    // Указатель на стратегию расчета, используется для вычисления размеров
    std::unique_ptr<CalculationStrategy> strategy_;
};

#endif // FILEBROWSER_H
