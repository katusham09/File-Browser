#ifndef BYFOLDERCALCULATIONSTRATEGY_H
#define BYFOLDERCALCULATIONSTRATEGY_H
#include "calculationstrategy.h"

class ByFolderCalculationStrategy : public CalculationStrategy {
public:
    std::map<QString, std::pair<int, double>> calculate(const QString& path) override {
        std::map<QString, std::pair<int, double>> folderSizes;
        calculateFolderSize(QDir(path), folderSizes);
        return folderSizes;
    }

private:
    void calculateFolderSize(const QDir& dir, std::map<QString, std::pair<int, double>>& folderSizes) {
        QFileInfoList fileList = dir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot);
        qint64 totalSize = 0;

        for (const QFileInfo& fileInfo : fileList) {
            totalSize += fileInfo.size();
        }

        for (const QFileInfo& subDirInfo : dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot)) {
            QString subDirName = subDirInfo.fileName();
            QDir subDir(subDirInfo.absoluteFilePath());
            qint64 subDirSize = calculateDirectorySize(subDir);
            folderSizes[subDirName] = { static_cast<int>(subDirSize), calculatePercentage(subDirSize, totalSize) };
        }
    }

    qint64 calculateDirectorySize(const QDir& dir) {
        qint64 size = 0;
        for (const QFileInfo& fileInfo : dir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot)) {
            size += fileInfo.size();
        }
        return size;
    }

    double calculatePercentage(qint64 value, qint64 total) {
        if (total == 0) return 0.0;
        double percentage = static_cast<double>(value) / total * 100;
        if (percentage < 0.01 && value > 0)
        {
            return 0.01; // Если процент < 0.01%, вернуть 0.01%
        }
        return percentage;
    }
};

#endif // BYFOLDERCALCULATIONSTRATEGY_H
