#ifndef BYFOLDERCALCULATIONSTRATEGY_H
#define BYFOLDERCALCULATIONSTRATEGY_H

#include "calculationstrategy.h"
#include <map>
#include <QString>
#include <QDir>
#include <QFileInfoList>

// Класс для расчета размеров папок
class ByFolderCalculationStrategy : public CalculationStrategy {
public:
    // Основная функция расчета, которая вызывается для заданного пути
    std::map<QString, std::pair<int, double>> calculate(const QString& path) override {
        std::map<QString, std::pair<int, double>> folderSizes;
        QDir dir(path);
        // Вычисляем общий размер всех файлов и папок в указанной директории
        qint64 totalSize = calculateDirectorySize(dir);
        // Вычисляем размеры для каждой папки и текущей директории
        calculateFolderSize(dir, folderSizes, totalSize);
        return folderSizes;
    }

private:
    // Вспомогательная функция для расчета размеров файлов в папке
    void calculateFolderSize(const QDir& dir, std::map<QString, std::pair<int, double>>& folderSizes, qint64 totalSize) {
        // Получаем список файлов в текущей директории
        QFileInfoList fileList = dir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot);

        qint64 currentDirSize = 0;
        // Считаем общий размер всех файлов в текущей директории
        for (const QFileInfo& fileInfo : fileList) {
            currentDirSize += fileInfo.size();
        }
        // Добавляем информацию о текущей директории
        folderSizes["currentDirectory"] = { static_cast<int>(currentDirSize), calculatePercentage(currentDirSize, totalSize) };

        // Проходим по каждой поддиректории
        for (const QFileInfo& subDirInfo : dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot)) {
            QString subDirName = subDirInfo.fileName(); // Имя поддиректории
            QDir subDir(subDirInfo.absoluteFilePath());
            // Вычисляем размер поддиректории
            qint64 subDirSize = calculateDirectorySize(subDir);
            // Добавляем информацию о поддиректории
            folderSizes[subDirName] = { static_cast<int>(subDirSize), calculatePercentage(subDirSize, totalSize) };
        }
    }

    // Вспомогательная функция для рекурсивного подсчета размеров файлов и папок
    qint64 calculateDirectorySize(const QDir& dir) {
        qint64 size = 0;
        // Получаем список всех файлов и поддиректорий
        for (const QFileInfo& fileInfo : dir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot | QDir::Dirs | QDir::NoDotAndDotDot)) {
            if (fileInfo.isDir()) {
                // Рекурсивный вызов для подсчета размера поддиректории
                QDir subDir(fileInfo.absoluteFilePath());
                size += calculateDirectorySize(subDir);
            } else {
                // Увеличиваем общий размер на размер текущего файла
                size += fileInfo.size();
            }
        }
        return size;
    }

    // Функция для вычисления процентного размера
    double calculatePercentage(qint64 value, qint64 total) {
        if (total == 0) return 0.0; // Если общий размер 0, возвращаем 0%
        double percentage = static_cast<double>(value) / total * 100;
        if (percentage < 0.01 && value > 0) {
            return 0.01; // Если процент < 0.01%, вернуть 0.01%
        }
        return percentage;
    }
};

#endif // BYFOLDERCALCULATIONSTRATEGY_H
