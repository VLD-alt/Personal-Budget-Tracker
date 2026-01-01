# Personal Budget Tracker (C++)

Console application for managing personal finances. Users can add income/expenses, edit/delete transactions, filter them, and generate a report. Data is saved to a text file and loaded automatically on program start.

  Features
Add income and expense transactions
Edit or delete existing transactions
View all transactions in a list
Filter transactions by category or date
Calculate the total balance
Show separate totals for income and expenses
Generate a financial report (totals + category summary)
Save/load data from a file

  OOP concepts
Abstraction: Transaction (abstract base class)
Inheritance: Income, Expense
Polymorphism: overridden methods
Encapsulation: class fields + methods

  Build (Visual Studio)
1. Open `.sln`
2. Set C++ standard to C++17
3. Build and Run


Data storage
The app uses `budget_data.txt` to store data (this file is ignored by Git).
Example format is provided in `budget_data_example.txt`.
