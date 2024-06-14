#ifndef BYFILETYPECALCULATIONSTRATEGY_H
#define BYFILETYPECALCULATIONSTRATEGY_H
#include "calculationstrategy.h"

// Класс для расчета размеров файлов по их типам (расширениям)
class ByFileTypeCalculationStrategy : public CalculationStrategy {
public:
    // Основная функция расчета, которая вызывается для заданного пути
    std::map<QString, std::pair<int, double>> calculate(const QString& path) override {
        std::map<QString, std::pair<int, double>> fileSizes;
        // Вызов вспомогательной функции для расчета размеров файлов
        calculateFileSizes(QDir(path), fileSizes);

        // Если нет файлов, выводим сообщение
        if (fileSizes.empty()) {
            std::cout << "No files found in the directory." << std::endl;
        }
        return fileSizes;
    }

private:
    // Вспомогательная функция для подсчета размеров файлов
    void calculateFileSizes(const QDir& dir, std::map<QString, std::pair<int, double>>& fileSizes) {
        // Получение списка файлов в текущей директории
        QFileInfoList fileList = dir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot);
        qint64 totalSize = 0;

        // Считаем размеры файлов в текущей директории
        for (const QFileInfo& fileInfo : fileList) {
            totalSize += fileInfo.size(); // Увеличиваем общий размер
            QString fileType = fileInfo.completeSuffix(); // Получаем расширение файла
            int fileSize = fileInfo.size(); // Размер текущего файла
            if (!fileType.isEmpty()) {
                fileSizes[fileType].first += fileSize; // Добавляем размер файла к соответствующему типу
            }
        }

        // Получение списка поддиректорий первого уровня (исключая . и ..)
        QFileInfoList dirList = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);
        for (const QFileInfo& dirInfo : dirList) {
            QDir subDir(dirInfo.absoluteFilePath()); // Переход в поддиректорию
            QFileInfoList subFileList = subDir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot);
            for (const QFileInfo& fileInfo : subFileList) {
                totalSize += fileInfo.size(); // Увеличиваем общий размер
                QString fileType = fileInfo.completeSuffix(); // Получаем расширение файла
                int fileSize = fileInfo.size(); // Размер текущего файла
                if (!fileType.isEmpty()) {
                    fileSizes[fileType].first += fileSize; // Добавляем размер файла к соответствующему типу
                }
            }
        }

        // Вычисляем процентный размер для каждого типа файла
        for (auto& file : fileSizes) {
            file.second.second = calculatePercentage(file.second.first, totalSize);
        }
    }

    // Функция для вычисления процентного размера
    double calculatePercentage(int value, qint64 total) {
        if (total == 0) return 0.0;
        double percentage = static_cast<double>(value) / total * 100;
        if (percentage < 0.01 && value > 0) {
            return 0.01; // Если процент < 0.01%, вернуть 0.01%
        }
        return percentage;
    }
};

#endif // BYFILETYPECALCULATIONSTRATEGY_H
