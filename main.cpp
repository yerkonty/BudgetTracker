#include <SFML/Graphics.hpp>
#include <iostream>
#include "Budget.h"
#include <sstream>
#include <iomanip>

const sf::Color COLOR_BACKGROUND(245, 240, 235);  // Бежевый Фон
const sf::Color COLOR_PRIMARY(70, 130, 180);      // blue main
const sf::Color COLOR_PRIMARY_DARK(50, 100, 150); // dark blue
const sf::Color COLOR_CARD(255, 255, 255);        // white for cards
const sf::Color COLOR_TEXT_DARK(40, 40, 40);      // black for texts
const sf::Color COLOR_TEXT_WHITE(255, 255, 255);  // white for cards
const sf::Color COLOR_SUCCESS(76, 175, 80);       // Green
const sf::Color COLOR_DANGER(244, 67, 54);        //  RED

// Screen Numbers
enum class Screen
{
    DASHBOARD,       // Main screen
    ADD_TRANSACTION, // Adding Transactions
    TRANSACTIONS,    // List of Transactions
    ANALYTICS        // Graphics
};

void drawText(sf::RenderWindow &window, const std::string &str, float x, float y,
              int size, sf::Color color, sf::Font &font)
{
    sf::Text text(str, font, size);
    text.setFillColor(color);
    text.setPosition(x, y);
    window.draw(text);
}

void drawCard(sf::RenderWindow &window, float x, float y, float width, float height, sf::Color color)
{
    sf::RectangleShape card(sf::Vector2f(width, height));
    card.setPosition(x, y);
    card.setFillColor(color);

    // shadow
    sf::RectangleShape shadow(sf::Vector2f(width, height));
    shadow.setPosition(x + 4, y + 4);
    shadow.setFillColor(sf::Color(0, 0, 0, 30)); // прозрачно черный

    window.draw(shadow);
    window.draw(card);
}

void drawBar(sf::RenderWindow& window, float x, float y, float width, float height, sf::Color color, 
            const std::string& label,const std::string& value,sf::Font& font) {
    sf::RectangleShape bar(sf::Vector2f(width,height));
    bar.setPosition(x,y - height);
    bar.setFillColor(color);
    window.draw(bar);
    
    sf::Text labelText(label,font,16);
    labelText.setFillColor(COLOR_TEXT_DARK);
    labelText.setPosition(x, y + 10);
    window.draw(labelText);

    sf::Text valueText(value,font,16);
    valueText.setFillColor(COLOR_TEXT_DARK);
    valueText.setPosition(x, y - height - 25);
    window.draw(valueText);

    }

std::string formatMoney(double amount) {
    
    //округляем до 2 знаков после запятой
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2) << amount;
    std::string result = oss.str();

    //добавляем запятые для разделения тысячных
    size_t decimalPos = result.find('.');
    if (decimalPos == std::string::npos) decimalPos = result.length();

    int insertPosition = decimalPos - 3;
    while (insertPosition > 0) {
        result.insert(insertPosition, ",");
        insertPosition -= 3;
    }
    return "$" + result;
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(1000, 700), "SMART BUDGET TRACKER");

    sf::Font font;
    if (!font.loadFromFile("fonts/IMPACT.TTF"))
    {
        std::cout << "ERROR: Cannot load font" << std::endl;
        return -1;
    }

    std::cout << "Font loaded succesfully" << std::endl;

    // объект Budget
    Budget budget("data/test_transactions.txt");
    std::cout << "Budget loaded. Balance: $" << budget.getBalance() << std::endl;

    // переменные для ввода транзакции
    std::string amountInput = "";
    bool inputActive = false;
    int selectedCategory = 0;
    int selectedType = 0; // Expense = 0, Income = 1

    // Current Screen
    Screen currentScreen = Screen::DASHBOARD;

    //удаление транзакции
    bool deleteMode = false;
    std::string deleteInput = "";

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed)
            {
                // переключить режим ввода суммы Таб
                if (event.key.code == sf::Keyboard::Tab)
                {
                    inputActive = !inputActive;
                }

                // переключить тип income or expenses
                if (event.key.code == sf::Keyboard::T)
                {
                    selectedType = 1 - selectedType;
                }

                // предыдущая категория = вверх
                if (event.key.code == sf::Keyboard::Up)
                {
                    selectedCategory = (selectedCategory - 1 + 7) % 7;
                }

                // следующая категория = вниз
                if (event.key.code == sf::Keyboard::Down)
                {
                    selectedCategory = (selectedCategory + 1) % 7;
                }

                // enter добавить транзакцию
                if (event.key.code == sf::Keyboard::Return && !amountInput.empty())
                {
                    try
                    {
                        double amount = std::stod(amountInput);
                        Category cat = static_cast<Category>(selectedCategory);
                        
                        TransactionType type;
                        if (cat == Category::SALARY) {
                             type = TransactionType::INCOME;
                        } else {
                            type = (selectedType == 0) ? TransactionType::EXPENSE : TransactionType::INCOME;
                        }

                        Transaction t(type,cat,amount, "Manual Entry");
                        budget.addTransaction(t);

                        amountInput = "";

                        std::cout <<"Transaction added: " << (type == TransactionType::INCOME ? "Income" : "Expense")
                         << " $" << amount << std::endl;
                    }
                    catch (...) {}
                }

                // клавиши для переключения экранов
                if (event.key.code == sf::Keyboard::Num1 && !deleteMode && !inputActive)
                {
                    currentScreen = Screen::DASHBOARD;
                    inputActive = false;
                    deleteMode = false;
                    deleteInput = "";
                    std::cout << "Switched to DASHBOARD" << std::endl;
                }

                if (event.key.code == sf::Keyboard::Num2 && !deleteMode && !inputActive)
                {
                    currentScreen = Screen::ADD_TRANSACTION;
                    inputActive = false;
                    deleteMode = false;
                    deleteInput = "";
                    std::cout << "Switched to ADD_TRANSACTION" << std::endl;
                }

                if (event.key.code == sf::Keyboard::Num3 && !deleteMode && !inputActive)
                {
                    currentScreen = Screen::TRANSACTIONS;
                    inputActive = false;
                    std::cout << "Switched to TRANSACTIONS" << std::endl;
                }

                if (event.key.code == sf::Keyboard::Num4 && !deleteMode)
                {
                    currentScreen = Screen::ANALYTICS;
                    inputActive = false;
                    deleteMode = false;
                    deleteInput = "";
                    std::cout << "Switched to ANALYTICS" << std::endl;
                }

                if (event.key.code == sf::Keyboard::D && currentScreen == Screen::TRANSACTIONS) {
                deleteMode = !deleteMode;
                deleteInput = "";
                std::cout << "=== DELETE MODE TOGGLED ===" << std::endl;
                std::cout << "Delete mode is now: " << (deleteMode ? "ON" : "OFF") << std::endl;
                std::cout << "Current Screen: " << (currentScreen == Screen::TRANSACTIONS ? "TRANSACTIONS" : "OTHER") << std::endl;
            }               

                if (event.key.code == sf::Keyboard::Return && deleteMode && !deleteInput.empty()) {
                try {
                    int index = std::stoi(deleteInput);
                    int actualIndex = budget.getTransactions().size() - 1 - index;
                    budget.deleteTransaction(actualIndex);
                    deleteInput = "";
                    deleteMode = false;
                    std::cout << "Transactions deleted!" <<std::endl;
                } catch(...) {
                    std::cout << "Invalid number" << std::endl;
                }
            }
            }

            // ввод текста (цифры для суммы)
            if (event.type == sf::Event::TextEntered && inputActive && currentScreen == Screen::ADD_TRANSACTION)
            {
                std::cout << "=== TEXT EVENT ===" << std::endl;
                std::cout << "Unicode: " << event.text.unicode << std::endl;
                std::cout << "Before: [" << amountInput << "]" << std::endl;

                if (event.text.unicode == 8 && !amountInput.empty()){
                    amountInput.pop_back();
                } else if (event.text.unicode >= 48 && event.text.unicode <= 57) {
                    amountInput += static_cast<char>(event.text.unicode);
                    
                } else if (event.text.unicode == 46){
                    amountInput = '.';
                }

                std::cout << "After: [" << amountInput << "]" << std::endl;
            }

            //ввод для номера удаления
            if (event.type == sf::Event::TextEntered && deleteMode && currentScreen == Screen::TRANSACTIONS) {
                if (event.text.unicode == 8 && !deleteInput.empty()) {
                    deleteInput.pop_back();
                } else if (event.text.unicode >= 48 && event.text.unicode <=57) {
                    deleteInput += static_cast <char>(event.text.unicode);
                }
            }
        }

        window.clear(COLOR_BACKGROUND); // Бежевый фон

        switch (currentScreen)
        {
        case Screen::DASHBOARD:
        {
            // заголовок
            drawText(window, "SMART BUDGET TRACKER", 40, 50, 36, COLOR_PRIMARY, font);
            drawText(window, "Dashboard", 50, 100, 24, COLOR_TEXT_DARK, font);

            // cards with financial info
            // card 1: Income
            drawCard(window, 50, 130, 280, 120, sf::Color(230, 255, 230)); // light green
            drawText(window, "Total Income: ", 70, 150, 20, COLOR_TEXT_DARK, font);
            drawText(window, formatMoney(budget.getTotalIncome()), 70, 190, 32, COLOR_SUCCESS, font);

            // card 2: Expenses
            drawCard(window, 360, 130, 280, 120, sf::Color(255, 235, 235)); // light red
            drawText(window, "Total Expenses: ", 380, 150, 20, COLOR_TEXT_DARK, font);
            drawText(window, formatMoney(budget.getTotalExpenses()), 380, 190, 32, COLOR_DANGER, font);

            // Карточка 3: Balance (синяя)
            drawCard(window, 670, 130, 280, 120, sf::Color(220, 240, 255)); // Светло-синий
            drawText(window, "Current Balance", 690, 150, 20, COLOR_TEXT_DARK, font);
            drawText(window, formatMoney(budget.getBalance()), 690, 190, 32, COLOR_PRIMARY, font);

            // Кнопки навигации (карточки-кнопки)
            drawText(window, "Quick Actions:", 50, 290, 24, COLOR_TEXT_DARK, font);

            // Кнопка Add Transaction
            drawCard(window, 50, 330, 280, 80, COLOR_PRIMARY);
            drawText(window, "Add Transaction", 80, 355, 22, COLOR_TEXT_WHITE, font);
            drawText(window, "Press 2", 180, 385, 16, COLOR_TEXT_WHITE, font);

            // Кнопка View Transactions
            drawCard(window, 360, 330, 280, 80, COLOR_PRIMARY);
            drawText(window, "View Transactions", 380, 355, 22, COLOR_TEXT_WHITE, font);
            drawText(window, "Press 3", 480, 385, 16, COLOR_TEXT_WHITE, font);

            // Кнопка Analytics
            drawCard(window, 670, 330, 280, 80, COLOR_PRIMARY);
            drawText(window, "Analytics & Charts", 690, 355, 22, COLOR_TEXT_WHITE, font);
            drawText(window, "Press 4", 790, 385, 16, COLOR_TEXT_WHITE, font);

            break;
        }

        case Screen::ADD_TRANSACTION:
        {
            // Zagolovok
            drawText(window, "ADD NEW TRANSACTION", 50, 30, 36, COLOR_PRIMARY, font);
            drawText(window, "Fill in the details below", 50, 75, 24, COLOR_TEXT_DARK,font);

            // Card 1, choosing type
            drawCard(window, 50, 130, 450, 100, COLOR_CARD);
            drawText(window, "Transaction Type (Press T)",70,150,20, COLOR_TEXT_DARK, font);
            std::string typeStr = (selectedType == 0) ? "EXPENSE" : "INCOME";
            sf::Color typeColor = (selectedType == 0) ? COLOR_DANGER : COLOR_SUCCESS;
            drawText(window,typeStr, 70,185,32,typeColor, font);

            // Card 2, choosing category
            drawCard(window, 530,130,420,100,COLOR_CARD);
            drawText(window, "Category(Up/Down arrows)", 550, 150, 20, COLOR_TEXT_DARK,font);
            std::string categories[] = {"Food", "Transport", "Entertainment", 
                                        "Utilities", "Shopping", "Salary", "Other"};
            drawText(window, categories[selectedCategory],550,185,32,COLOR_PRIMARY,font);

            //Card 3, entering values
            drawCard(window,50,260,900,100,COLOR_CARD);
            drawText(window, "Amount (Press Tab to start typing)",70,280,20, COLOR_TEXT_DARK,font);
            std::string amountDisplay = "$" + amountInput;
            if (inputActive) amountDisplay += "_";
            sf::Color amountColor = inputActive ? COLOR_PRIMARY : COLOR_TEXT_DARK;
            drawText(window,amountDisplay, 70,315,36, amountColor,font);

            //Preview of transaction
            if (!amountInput.empty()) {
                drawCard(window,50,390,900,140,sf::Color(240,248,255));
                drawText(window, "Preview:", 70,410,20,COLOR_TEXT_DARK,font);

                //Details
                drawText(window, "Type:",70,450,18, COLOR_TEXT_DARK,font);
                drawText(window,typeStr,180,450,20,typeColor,font);

                drawText(window, "Category:",70,480,18,COLOR_TEXT_DARK,font);
                drawText(window,categories[selectedCategory],180,480,18,COLOR_PRIMARY,font);

                drawText(window,"Amount:",70,510,18,COLOR_TEXT_DARK,font);
                drawText(window, formatMoney(std::stod(amountInput)), 180,510,18, typeColor, font);
            }

            //Adding button
            if(!amountInput.empty()) {
                drawCard(window,50,560,900,70,COLOR_PRIMARY);
                drawText(window, "Press ENTER to Add Transaction",320,585,24,COLOR_TEXT_WHITE, font);
            } else {
                drawCard(window,50,560,900,70,sf::Color(180,180,180));
                drawText(window, "Enter amount to continue", 340,585,24,COLOR_TEXT_WHITE,font);
            }

            //Hint
            drawText(window, "Press 1 to return to Dashboard",50,650,16,COLOR_TEXT_DARK,font);

            //Current Balance
            drawText(window, "Current Balance: " + formatMoney(budget.getBalance()),
                     650,650,16,COLOR_PRIMARY,font);
            
            break;
        }

        case Screen::TRANSACTIONS:
        {
            drawText(window, "TRANSACTION HISTORY", 50, 30, 36, COLOR_PRIMARY, font);
            drawText(window, "All your Transactions", 50, 75, 24, COLOR_TEXT_DARK, font);

            const auto& transactions = budget.getTransactions();

            if (transactions.empty()) {
                drawText(window,"No transactions yet. Add some on screen 2!", 300,300,20,COLOR_TEXT_DARK,font);
            } else {
                drawText(window,"#", 60,130,18,COLOR_TEXT_DARK,font);
                drawText(window, "Date", 130, 130, 18, COLOR_TEXT_DARK, font);
                drawText(window, "Type", 250, 130, 18, COLOR_TEXT_DARK, font);
                drawText(window, "Category", 380, 130, 18, COLOR_TEXT_DARK, font);
                drawText(window, "Amount", 580, 130, 18, COLOR_TEXT_DARK, font);
                drawText(window, "Description", 720, 130, 18, COLOR_TEXT_DARK, font);

                sf::RectangleShape headerLine(sf::Vector2f(880,2));
                headerLine.setPosition(50,155);
                headerLine.setFillColor(COLOR_PRIMARY);
                window.draw(headerLine);

                int count = 0;
                int maxDisplay = 12;
                float yPos = 170;

                for (auto it = transactions.rbegin(); it != transactions.rend() &&  count < maxDisplay; ++it, ++count) {

                    if (count % 2 == 0) {
                        sf::RectangleShape rowBg(sf::Vector2f(880,35));
                        rowBg.setPosition(50,yPos);
                        rowBg.setFillColor(sf::Color(255,255,255,100));
                        window.draw(rowBg);
                    }

                    drawText(window,std::to_string(count), 60, yPos + 8, 14, COLOR_PRIMARY, font);
                    drawText(window, it->getDateString(), 120, yPos + 8, 14, COLOR_TEXT_DARK, font);

                    sf::Color typeColor = (it->getType() == TransactionType::INCOME) ? COLOR_SUCCESS : COLOR_DANGER;
                    drawText(window, it->getTypeString(), 300, yPos + 8, 14, typeColor,font);

                    drawText(window, it->getCategoryString(), 430, yPos+8, 14, COLOR_TEXT_DARK, font);

                    std::string amountStr = formatMoney(it->getAmount());
                    drawText(window,amountStr, 630, yPos+8,14, typeColor, font);

                    std::string desc = it->getDescription();
                    if (desc.length()>20) desc = desc.substr(0,17)+"...";
                    drawText(window,desc, 720, yPos+8,14, COLOR_TEXT_DARK, font);

                    yPos += 40;
                }

                std::string totalInfo = "Showing " + std::to_string(std::min((int)transactions.size(), maxDisplay)) + "of " + std::to_string(transactions.size()) + " transactions:";
                drawText(window,totalInfo,50,630,16, COLOR_TEXT_DARK, font);
            }

            if (deleteMode) {
                drawCard(window,50,560,900,60,COLOR_DANGER);
                std::string deletePrompt = "Delete mode: Enter Transaction # and press ENTER: " + deleteInput + "_";
                drawText(window,deletePrompt,70,580,20,COLOR_TEXT_WHITE,font);
            } else {
                drawText(window, "Press D to delete transaction",400,630,16,COLOR_TEXT_DARK,font);
            }

            drawText(window, "Press 1 to return to Dashboard", 700,630,16,COLOR_TEXT_DARK,font);           
            
            break;
        }

        case Screen::ANALYTICS:
        {
            drawText(window, "MONTHLY EXPENSE REPORT", 50, 30, 36, COLOR_PRIMARY, font);
            drawText(window, "November 2025 - Category Breakdown", 50, 75, 24, COLOR_TEXT_DARK, font);

            auto categoryExpenses = budget.getCategoryExpenses();

            double maxExpense = 0;
            for (const auto& pair : categoryExpenses) {
                if (pair.second > maxExpense) maxExpense = pair.second;
            }

            if (maxExpense > 0) {
                std::string categoryNames[] = {"Food", "Transport", "Entertainment", 
                "Utilities", "Shopping", "Salary", "Other"};
                sf::Color barColors[] = {
                sf::Color(255, 99, 71),   // Red
                sf::Color(54, 162, 235),  // Blue
                sf::Color(255, 206, 86),  // Yellow
                sf::Color(75, 192, 192),  // Teal
                sf::Color(153, 102, 255), // Purple
                sf::Color(76, 175, 80),  // Orange
                sf::Color(201, 203, 207)  // Gray
            };    
            
            float baseY = 550;
            float startX = 80;
            float barWidth = 100;
            float spacing = 30;
            float maxBarHeight = 300;

            for (int i=0;i<7;i++) {
                Category cat = static_cast<Category>(i);
                double expense = budget.getExpenseByCategory(cat);

                if (expense>0) {
                    float barHeight = (expense/maxExpense)*maxBarHeight;

                    drawBar(window,
                            startX + i*(barWidth+spacing),
                            baseY,
                            barWidth,
                            barHeight,
                            barColors[i],
                            categoryNames[i],
                            formatMoney(expense),
                            font);
                }
            }

            sf::RectangleShape baseline(sf::Vector2f(900,2));
            baseline.setPosition(50, baseY);
            baseline.setFillColor(COLOR_TEXT_DARK);
            window.draw(baseline);

            drawCard(window,50,580,420,100,COLOR_CARD);
            drawText(window, "Total Income:", 70, 600, 18, COLOR_TEXT_DARK,font);
            drawText(window, formatMoney(budget.getTotalIncome()), 250, 600, 20, COLOR_SUCCESS,font);

            drawText(window, "Total Expenses:",70,635,18,COLOR_TEXT_WHITE,font);
            drawText(window, formatMoney(budget.getTotalExpenses()), 250, 635, 20, COLOR_DANGER, font);

            drawCard(window, 500,580,450,100, COLOR_PRIMARY);
            drawText(window, "Net Balance:", 530, 615, 24, COLOR_TEXT_WHITE, font);
            drawText(window, formatMoney(budget.getBalance()), 730, 615, 32, COLOR_TEXT_WHITE,font);

            } else {    
                drawText(window, "No expenses yet. Add some transactions!", 300,300,20, COLOR_TEXT_DARK,font);
            }
            drawText(window, "Press 1 to return to Dashboard",50,630,16,COLOR_TEXT_DARK, font);

            break;
        }
}

        window.display();
    }

    return 0;
}
