#ifndef BUDGET_H
#define BUDGET_H

#include "Transaction.h"
#include <vector>
#include <map>
#include <string>

class Budget {
private:
    std::vector<Transaction> transactions;
    std::map<Category, double> budgetLimits;
    std::string dataFile;

public:
    Budget(const std::string& file = "data/transactions.txt");
    
    void addTransaction(const Transaction& t);
    void deleteTransaction(int index);
    const std::vector<Transaction>& getTransactions() const { return transactions; }
    
    void setBudgetLimit(Category cat, double limit);
    double getBudgetLimit(Category cat) const;
    
    double getTotalIncome() const;
    double getTotalExpenses() const;
    double getBalance() const;
    double getExpenseByCategory(Category cat) const;
    std::map<Category, double> getCategoryExpenses() const;
    
    bool isBudgetExceeded(Category cat) const;
    std::vector<Category> getExceededCategories() const;
    
    void saveToFile();
    void loadFromFile();
};

#endif