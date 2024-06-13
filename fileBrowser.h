#ifndef FILEBROWSER_H
#define FILEBROWSER_H

#include "calculationstrategy.h"
#include <memory>
#include <iostream>

class FileBrowser {
public:
    explicit FileBrowser(std::unique_ptr<CalculationStrategy> strategy)
        : strategy_(std::move(strategy)) {}

    void setStrategy(std::unique_ptr<CalculationStrategy> strategy) {
        strategy_ = std::move(strategy);
    }

    void calculate(const QString& path) const {
        auto results = strategy_->calculate(path);
        for (const auto& entry : results) {
            QString item = entry.first;
            int size = entry.second.first;
            double percentage = entry.second.second;
            std::cout << item.toStdString() << ": " << size << " bytes, " << QString::number(percentage, 'f', 2).toStdString() << "%" << std::endl;
        }
    }

private:
    std::unique_ptr<CalculationStrategy> strategy_;
};

#endif // FILEBROWSER_H
