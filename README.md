# Smart Budget Tracker

A modern, graphical budget tracking application built with C++ and SFML. Track your income and expenses with an intuitive interface featuring multiple views for managing your personal finances.

## Features

- **Dashboard** - Overview of total income, expenses, and current balance
- **Add Transactions** - Easy-to-use interface for adding income and expense entries
- **Transaction History** - View all your transactions with detailed information
- **Analytics** - Visual bar charts showing expense breakdown by category
- **Delete Transactions** - Remove incorrect or unwanted entries
- **Persistent Storage** - All transactions are saved to a file

## Categories

The application supports 7 transaction categories:
- Food
- Transport
- Entertainment
- Utilities
- Shopping
- Salary (automatically set as Income)
- Other

## Requirements

- C++ compiler with C++11 support or higher
- SFML 2.x library (Simple and Fast Multimedia Library)
- Windows OS (currently configured for Windows)

## Project Structure

```
BudgetTracker/
├── main.cpp              # Main application with GUI
├── Budget.cpp/h          # Budget management class
├── Transaction.cpp/h     # Transaction handling class
├── data/                 # Transaction data storage
│   └── test_transactions.txt
├── fonts/                # Font files
│   └── IMPACT.TTF
└── libsfml-*.dll         # SFML library files
```

## Building the Project

1. Ensure SFML 2.x is installed and linked properly
2. Compile all .cpp files together:
```bash
g++ main.cpp Budget.cpp Transaction.cpp -o BudgetTracker.exe -lsfml-graphics -lsfml-window -lsfml-system
```

3. Make sure SFML DLL files are in the same directory as the executable

## Running the Application

Simply run the executable:
```bash
./BudgetTracker.exe
```

## How to Use

### Navigation
- **1** - Go to Dashboard
- **2** - Go to Add Transaction screen
- **3** - Go to Transaction History
- **4** - Go to Analytics

### Adding a Transaction
1. Press **2** to open Add Transaction screen
2. Press **T** to toggle between Expense/Income
3. Use **Up/Down arrows** to select category
4. Press **Tab** to activate amount input
5. Type the amount using number keys
6. Press **Enter** to add the transaction

### Deleting a Transaction
1. Press **3** to view Transaction History
2. Press **D** to enter delete mode
3. Type the transaction number (shown in # column)
4. Press **Enter** to confirm deletion

### Viewing Analytics
- Press **4** to see a bar chart of expenses by category
- View total income, expenses, and net balance

## Data Storage

Transactions are automatically saved to `data/test_transactions.txt` and persist between sessions.

## Screenshots

The application features:
- Clean, modern card-based UI design
- Color-coded income (green) and expenses (red)
- Interactive bar charts for expense analysis
- Real-time balance updates

## License

This project is open source and available for educational purposes.

## Contributing

Feel free to fork this project and submit pull requests for improvements.
