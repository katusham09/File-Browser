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

        for (const QFileInfo& fileInfo : fileList) {
            totalSize += fileInfo.size();
        }

        for (const QFileInfo& fileInfo : fileList) {
            QString fileType = fileInfo.completeSuffix(); // Получаем расширение файла
            int fileSize = fileInfo.size();
            if (!fileType.isEmpty()) {
                fileSizes[fileType].first += fileSize; // Добавляем размер файла к общему размеру типа файла
            }
        }

        for (auto& file : fileSizes) {
            file.second.second = calculatePercentage(file.second.first, totalSize); // Вычисляем процентный размер
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
