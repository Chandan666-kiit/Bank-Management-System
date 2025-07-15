# 💰 Bank Management System in C

This is a simple console-based Bank Management System written in C. It allows users to manage bank accounts including creating accounts, depositing and withdrawing money, transferring funds, checking balance, and viewing transaction history.

## 📋 Features

- Create a new bank account
- Deposit money into an account
- Withdraw money from an account
- Check account balance
- View full transaction history
- Delete an existing account
- Calculate interest on savings
- Transfer money between accounts
- Admin view of all accounts
- View detailed account information
- Mini statement (last 5 transactions)
- Data stored using binary files (`account.dat`, `transactions.dat`)

## 📁 File Structure

- `main.c` – Contains the complete implementation of the Bank Management System
- `account.dat` – Stores account data (created at runtime)
- `transactions.dat` – Stores transaction history (created at runtime)

## ⚙️ Functions Overview

| Function                | Description                                              |
|------------------------|----------------------------------------------------------|
| `create_account()`      | Creates a new bank account                               |
| `deposit_money()`       | Deposits money into an account                          |
| `withdraw_money()`      | Withdraws money from an account                         |
| `check_balance()`       | Displays the current balance                            |
| `view_transaction_history()` | Shows all transactions for a given account     |
| `save_transaction()`    | Internal: Records a transaction                         |
| `delete_account()`      | Deletes a bank account                                  |
| `calculate_interest()`  | Calculates simple interest on savings                   |
| `transfer_money()`      | Transfers money between two accounts                    |
| `view_all_accounts()`   | Admin: Lists all accounts                               |
| `view_account_details()`| Displays account holder details                         |
| `mini_statement()`      | Displays the last 5 transactions of an account          |

## 🖥️ How to Compile & Run

### Linux / macOS
```bash
gcc main.c -o bank
./bank
