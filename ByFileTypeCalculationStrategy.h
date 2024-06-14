#ifndef BYFILETYPECALCULATIONSTRATEGY_H
#define BYFILETYPECALCULATIONSTRATEGY_H
#include "calculationstrategy.h"

class ByFileTypeCalculationStrategy : public CalculationStrategy {
public:
    std::map<QString, std::pair<int, double>> calculate(const QString& path) override {
        std::map<QString, std::pair<int, double>> fileSizes;
        calculateFileSizes(QDir(path), fileSizes);
        return fileSizes;
    }

private:
    void calculateFileSizes(const QDir& dir, std::map<QString, std::pair<int, double>>& fileSizes) {
        QFileInfoList fileList = dir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot);
        qint64 totalSize = 0;

        // Считаем размеры файлов в текущей директории
        for (const QFileInfo& fileInfo : fileList) {
            totalSize += fileInfo.size();
            QString fileType = fileInfo.completeSuffix();
            int fileSize = fileInfo.size();
            if (!fileType.isEmpty()) {
                fileSizes[fileType].first += fileSize;
            }
        }

        // Считаем размеры файлов в поддиректориях первого уровня
        QFileInfoList dirList = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);
        for (const QFileInfo& dirInfo : dirList) {
            QDir subDir(dirInfo.absoluteFilePath());
            QFileInfoList subFileList = subDir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot);
            for (const QFileInfo& fileInfo : subFileList) {
                totalSize += fileInfo.size();
                QString fileType = fileInfo.completeSuffix();
                int fileSize = fileInfo.size();
                if (!fileType.isEmpty()) {
                    fileSizes[fileType].first += fileSize;
                }
            }
        }

        // Вычисляем процентный размер для каждого типа файла
        for (auto& file : fileSizes) {
            file.second.second = calculatePercentage(file.second.first, totalSize);
        }
    }

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
