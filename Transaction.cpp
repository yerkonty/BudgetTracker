#include "Transaction.h"
#include <sstream>
#include <iomanip>


Transaction::Transaction(TransactionType t, Category c, double amt, const std::string& desc)
    {   
        this->type = t;
        this->category = c;                //member initializer list
        this->amount = amt;
        this->description = desc;
        timestamp = time(nullptr);
    }


std::string Transaction::getCategoryString() const {
    switch (category) {
        case Category::FOOD : return "Food";
        case Category::TRANSPORT : return "Transport";
        case Category::ENTERTAINMENT : return "Entertainment";
        case Category::UTILITIES : return "Utilities";
        case Category::SALARY : return "Salary";
        case Category::SHOPPING : return "Shopping";
        case Category::OTHER : return "Other";
        default: return "Unknown";
    }
}


std::string Transaction::getTypeString() const {
    return (type == TransactionType::INCOME) ? "Income" : "Expense";
}

std::string Transaction::getDateString() const {
    struct tm* timeinfo = localtime(&timestamp);
    std::ostringstream oss;
    oss << std::put_time(timeinfo, "%Y-%m-%d %H-%M");
    return oss.str();
}