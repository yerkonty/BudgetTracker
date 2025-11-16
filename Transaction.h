#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <string>
#include <ctime>

enum class TransactionType {
    INCOME,
    EXPENSE
};

enum class Category {
    FOOD,
    TRANSPORT,
    ENTERTAINMENT,
    UTILITIES,
    SHOPPING,
    SALARY,
    OTHER
};

class Transaction {
private:
    TransactionType type;
    Category category;
    double amount;
    std::string description;
    time_t timestamp;

public:
    Transaction(TransactionType t, Category c, double amt, const std::string& desc);
    
    TransactionType getType() const { return type; }
    Category getCategory() const { return category; }
    double getAmount() const { return amount; }
    std::string getDescription() const { return description; }
    time_t getTimestamp() const { return timestamp; }
    
    std::string getCategoryString() const;
    std::string getTypeString() const;
    std::string getDateString() const;
};

#endif