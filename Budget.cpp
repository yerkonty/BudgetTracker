#include "Budget.h"
#include <fstream>
#include <sstream>


Budget::Budget(const std::string& file) : dataFile(file) {
    loadFromFile();
}


void Budget::addTransaction(const Transaction& t) {
    transactions.push_back(t);
    saveToFile();
}


void Budget::setBudgetLimit(Category cat, double limit) {
    budgetLimits[cat] = limit;
}


double Budget::getBudgetLimit(Category cat) const {
    auto it = budgetLimits.find(cat);
    return (it != budgetLimits.end()) ? it->second : 0.0;
}


double Budget::getTotalIncome() const {
    double total = 0.0;
    for (const auto& t : transactions) {
        if (t.getType() == TransactionType::INCOME) {
            total += t.getAmount();
        }
    }
    return total;
}

double Budget::getTotalExpenses() const {
    double total = 0.0;
    for (const auto& t : transactions) {
        if (t.getType() == TransactionType::EXPENSE) {
            total += t.getAmount();
        }
    }
    return total;
}


double Budget::getBalance() const {
    return getTotalIncome() - getTotalExpenses();
}


double Budget::getExpenseByCategory(Category cat) const {
    double total = 0.0;
    for (const auto& t : transactions) {
        if (t.getType() == TransactionType::EXPENSE && t.getCategory() == cat) {
            total += t.getAmount();
        }
    }
    return total;
}


std::map<Category, double> Budget::getCategoryExpenses() const {
    std::map<Category, double> expenses;
    for (const auto& t : transactions) {
        if (t.getType() == TransactionType::EXPENSE) {
            expenses[t.getCategory()] += t.getAmount();
        }
    }
    return expenses;
} 


void Budget::saveToFile() {
    std::ofstream file(dataFile);
    if (!file.is_open()) return;
    for (const auto&t : transactions) {
        file << static_cast<int>(t.getType()) << ","
             << static_cast<int>(t.getCategory()) << ","
             << t.getAmount() << ","
             << t.getDescription() << ","
             << t.getTimestamp() << "\n";
    }
    file.close();
}


void Budget::loadFromFile() {
    std::ifstream file(dataFile);
    if (!file.is_open()) return;

    transactions.clear();
    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string item;
        std::vector<std::string> tokens;

        while (std::getline(ss, item, ',')) {
            tokens.push_back(item);
        }

        if (tokens.size() >=5) {
            TransactionType type = static_cast<TransactionType>(std::stoi(tokens[0]));
            Category cat = static_cast<Category>(std::stoi(tokens[1]));
            double amount = std::stod(tokens[2]);
            std::string desc = tokens[3];

            Transaction t(type, cat, amount, desc);
            transactions.push_back(t);

    }
    }
    file.close();
}

bool Budget::isBudgetExceeded(Category cat) const {
    double limit = getBudgetLimit(cat);
    if (limit<=0) return false;
    return getExpenseByCategory(cat) > limit;
}

std::vector<Category> Budget::getExceededCategories() const {
    std::vector<Category> exceeded;
    for (const auto& pair : budgetLimits) {
        if (isBudgetExceeded(pair.first)) {
            exceeded.push_back(pair.first);
        }
    }
    return exceeded;
}

void Budget::deleteTransaction(int index) {
    if (index >= 0 && index < transactions.size()) {
        transactions.erase(transactions.begin() + index);
        saveToFile();
    }
}

