#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <cmath> // for EMI calculation
using namespace std;

// (a) User Class (Base Class)
class User {
protected:
    int userID;
    string name;
    string password;

public:
    User(int id, string n, string pass) : userID(id), name(n), password(pass) {}
    virtual void login(string enteredPassword) {
        if (password == enteredPassword) {
            cout << "Login Successful.\n";
        } else {
            cout << "Invalid Password.\n";
        }
    }
    virtual void viewDetails() {
        cout << "User ID: " << userID << ", Name: " << name << endl;
    }
};

// (b) Account Class (Base for Savings and Current)
class Account {
protected:
    int accountNumber;
    double balance;
    string accountType;

public:
    Account(int accNo, double bal, string type) : accountNumber(accNo), balance(bal), accountType(type) {}

    virtual void deposit(double amount) {
        balance += amount;
        cout << "Deposited: " << amount << ". New Balance: " << balance << endl;
    }

    virtual void withdraw(double amount) {
        if (balance >= amount) {
            balance -= amount;
            cout << "Withdrawn: " << amount << ". Remaining Balance: " << balance << endl;
        } else {
            cout << "Insufficient balance.\n";
        }
    }

    virtual double calculateInterest() = 0; // Pure Virtual Function
};

// SavingsAccount Derived
class SavingsAccount : public Account {
public:
    SavingsAccount(int accNo, double bal) : Account(accNo, bal, "Savings") {}

    double calculateInterest() override {
        double rate = 0.04; // 4% interest
        return balance * rate;
    }
};

// CurrentAccount Derived
class CurrentAccount : public Account {
public:
    CurrentAccount(int accNo, double bal) : Account(accNo, bal, "Current") {}

    double calculateInterest() override {
        double rate = 0.02; // 2% interest
        return balance * rate;
    }
};

// (d) Transaction Class
class Transaction {
private:
    int transactionID;
    double amount;
    string transactionType;
    string date;

public:
    Transaction(int id, double amt, string type) : transactionID(id), amount(amt), transactionType(type) {
        time_t now = time(0);
        date = ctime(&now);
    }

    void recordTransaction() {
        cout << "Transaction Recorded: " << transactionType << " of " << amount << " on " << date;
    }

    void showTransactionHistory() {
        cout << "Transaction ID: " << transactionID << ", Type: " << transactionType
             << ", Amount: " << amount << ", Date: " << date << endl;
    }
};

// (e) Loan Class
class Loan {
private:
    int loanID;
    double loanAmount;
    double interestRate;
    int duration; // in months

public:
    Loan(int id, double amt, double rate, int dur) : loanID(id), loanAmount(amt), interestRate(rate), duration(dur) {}

    void applyLoan() {
        cout << "Loan Applied: ID = " << loanID << ", Amount = " << loanAmount << ", Rate = " << interestRate << "%\n";
    }

    double calculateEMI() {
        double monthlyRate = interestRate / (12 * 100);
        double emi = (loanAmount * monthlyRate * pow(1 + monthlyRate, duration)) / (pow(1 + monthlyRate, duration) - 1);
        return emi;
    }

    void payEMI() {
        double emi = calculateEMI();
        cout << "Monthly EMI to pay: " << emi << endl;
    }
};

// (f) Interest Class
class Interest {
protected:
    double interestRate;
    string accountType;

public:
    Interest(double rate, string type) : interestRate(rate), accountType(type) {}

    virtual double calculateInterest(double balance) {
        return balance * interestRate;
    }
};

// (g) Bank Class (Manager)
class Bank {
private:
    vector<User> users;
    vector<Account*> accounts;
    vector<Transaction> transactions;
    vector<Loan> loans;

public:
    void addUser(User user) {
        users.push_back(user);
    }

    void addAccount(Account* account) {
        accounts.push_back(account);
    }

    void addTransaction(Transaction transaction) {
        transactions.push_back(transaction);
    }

    void addLoan(Loan loan) {
        loans.push_back(loan);
    }

    void userLogin(int userID, string password) {
        for (auto& user : users) {
            user.login(password);
        }
    }

    void processInterest() {
        for (auto acc : accounts) {
            double interest = acc->calculateInterest();
            cout << "Interest for Account " << interest << endl;
        }
    }

    void showAllTransactions() {
        for (auto t : transactions) {
            t.showTransactionHistory();
        }
    }
};

int main() {
    Bank bank;

    // Example Users
    User user1(101, "John Doe", "pass123");
    bank.addUser(user1);

    // Example Accounts
    SavingsAccount* sa = new SavingsAccount(1001, 5000.0);
    bank.addAccount(sa);

    CurrentAccount* ca = new CurrentAccount(1002, 8000.0);
    bank.addAccount(ca);

    // Example Transaction
    Transaction t1(1, 1000, "Deposit");
    bank.addTransaction(t1);

    // Example Loan
    Loan loan1(501, 50000, 7.5, 24); // 2 years
    bank.addLoan(loan1);

    // Simulating actions
    user1.viewDetails();
    sa->deposit(2000);
    sa->withdraw(1000);

    loan1.applyLoan();
    loan1.payEMI();

    bank.processInterest();
    bank.showAllTransactions();

    return 0;
}