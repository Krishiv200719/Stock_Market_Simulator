#include <iostream>
#include <string>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <fstream>
using namespace std;

struct Stock {
    string name;
    double price;
};

struct Holding {
    string name;
    int quantity;
    double avgBuyPrice;
};

const int MAX_STOCKS = 5;
const int MAX_HOLDINGS = 5;

Stock stocks[MAX_STOCKS] = {
    {"Reliance", 2450.00},
    {"TCS", 3520.75},
    {"Infosys", 1480.50},
    {"HDFC Bank", 1650.25},
    {"ITC", 430.00}
};

Holding portfolio[MAX_HOLDINGS];
int holdingCount = 0;
double balance = 50000.00;

void updatePrices() {
    ofstream logFile("transaction_logs.txt", ios::app);
    if (logFile) {
        logFile << "\n--- MARKET PRICE UPDATE ---" << endl;
    }

    for (int i = 0; i < MAX_STOCKS; i++) {
        // Random change between -5.0% to +5.0%
        double changePercent = ((rand() % 1001) / 1000.0 * 10.0) - 5.0;
        double newPrice = stocks[i].price * (1.0 + (changePercent / 100.0));
        if (newPrice < 1.0) newPrice = 1.0;
        
        if (logFile) {
            logFile << "UPDATE: " << left << setw(12) << stocks[i].name 
                    << " | Old: Rs." << right << setw(8) << fixed << setprecision(2) << stocks[i].price
                    << " | New: Rs." << setw(8) << fixed << setprecision(2) << newPrice 
                    << " (" << (changePercent >= 0 ? "+" : "") << fixed << setprecision(2) << changePercent << "%)" << endl;
        }

        stocks[i].price = newPrice;
    }

    if (logFile) {
        logFile.close();
    }
}

void displayStocks() {
    cout << "\n====================================================" << endl;
    cout << "              AVAILABLE STOCKS" << endl;
    cout << "====================================================" << endl;
    cout << left << setw(8) << "No."
         << setw(20) << "Stock Name"
         << right << setw(15) << "Price (Rs.)" << endl;
    cout << "----------------------------------------------------" << endl;
    for (int i = 0; i < MAX_STOCKS; i++) {
        cout << left << setw(8) << i + 1
             << setw(20) << stocks[i].name
             << right << setw(15) << fixed << setprecision(2) << stocks[i].price << endl;
    }
    cout << "====================================================" << endl;
    cout << "Available Balance: Rs. " << fixed << setprecision(2) << balance << endl;
    cout << "====================================================" << endl;
}

int findHolding(string name) {
    for (int i = 0; i < holdingCount; i++) {
        if (portfolio[i].name == name)
            return i;
    }
    return -1;
}

void logTransaction(const string& type, const string& stockName, int qty, double price, double total, double profitLoss = 0) {
    ofstream logFile("transaction_logs.txt", ios::app);
    if (!logFile) return;
    
    logFile << type << ": " << stockName << " | Qty: " << qty 
            << " | Price: Rs." << fixed << setprecision(2) << price 
            << " | Total: Rs." << total;
            
    if (type == "SOLD") {
        logFile << " | P/L: Rs." << (profitLoss >= 0 ? "+" : "") << profitLoss;
    }
    logFile << endl;
    logFile.close();
}

void buyStock() {
    displayStocks();
    int choice, qty;

    cout << "\nEnter stock number to buy (1-" << MAX_STOCKS << "): ";
    cin >> choice;

    if (choice < 1 || choice > MAX_STOCKS) {
        cout << "Invalid stock number!" << endl;
        return;
    }

    cout << "Enter quantity: ";
    cin >> qty;

    if (qty <= 0) {
        cout << "Invalid quantity!" << endl;
        return;
    }

    double totalCost = stocks[choice - 1].price * qty;

    if (totalCost > balance) {
        cout << "\nInsufficient balance!" << endl;
        cout << "Required: Rs. " << fixed << setprecision(2) << totalCost << endl;
        cout << "Available: Rs. " << fixed << setprecision(2) << balance << endl;
        return;
    }

    balance -= totalCost;

    int idx = findHolding(stocks[choice - 1].name);
    if (idx != -1) {
        double totalInvestment = portfolio[idx].avgBuyPrice * portfolio[idx].quantity + totalCost;
        portfolio[idx].quantity += qty;
        portfolio[idx].avgBuyPrice = totalInvestment / portfolio[idx].quantity;
    } else {
        if (holdingCount >= MAX_HOLDINGS) {
            cout << "Portfolio is full! Cannot add more stocks." << endl;
            balance += totalCost;
            return;
        }
        portfolio[holdingCount].name = stocks[choice - 1].name;
        portfolio[holdingCount].quantity = qty;
        portfolio[holdingCount].avgBuyPrice = stocks[choice - 1].price;
        holdingCount++;
    }

    cout << "\n====================================================" << endl;
    cout << "  Stock Purchased Successfully!" << endl;
    cout << "  " << stocks[choice - 1].name << " x " << qty
         << " @ Rs. " << fixed << setprecision(2) << stocks[choice - 1].price << endl;
    cout << "  Total Cost: Rs. " << fixed << setprecision(2) << totalCost << endl;
    cout << "  Remaining Balance: Rs. " << fixed << setprecision(2) << balance << endl;
    cout << "====================================================" << endl;

    logTransaction("BOUGHT", stocks[choice - 1].name, qty, stocks[choice - 1].price, totalCost);
}

void sellStock() {
    if (holdingCount == 0) {
        cout << "\nYour portfolio is empty! Nothing to sell." << endl;
        return;
    }

    cout << "\n====================================================" << endl;
    cout << "              YOUR HOLDINGS" << endl;
    cout << "====================================================" << endl;
    cout << left << setw(8) << "No."
         << setw(20) << "Stock Name"
         << setw(10) << "Qty"
         << right << setw(15) << "Avg Price" << endl;
    cout << "----------------------------------------------------" << endl;
    for (int i = 0; i < holdingCount; i++) {
        cout << left << setw(8) << i + 1
             << setw(20) << portfolio[i].name
             << setw(10) << portfolio[i].quantity
             << right << setw(15) << fixed << setprecision(2) << portfolio[i].avgBuyPrice << endl;
    }
    cout << "====================================================" << endl;

    int choice, qty;
    cout << "\nEnter holding number to sell (1-" << holdingCount << "): ";
    cin >> choice;

    if (choice < 1 || choice > holdingCount) {
        cout << "Invalid choice!" << endl;
        return;
    }

    cout << "Enter quantity to sell: ";
    cin >> qty;

    if (qty <= 0 || qty > portfolio[choice - 1].quantity) {
        cout << "Invalid quantity! You own " << portfolio[choice - 1].quantity << " shares." << endl;
        return;
    }

    int stockIdx = -1;
    for (int i = 0; i < MAX_STOCKS; i++) {
        if (stocks[i].name == portfolio[choice - 1].name) {
            stockIdx = i;
            break;
        }
    }

    double sellPrice = stocks[stockIdx].price;
    double totalRevenue = sellPrice * qty;
    double profitLoss = (sellPrice - portfolio[choice - 1].avgBuyPrice) * qty;

    balance += totalRevenue;
    portfolio[choice - 1].quantity -= qty;

    cout << "\n====================================================" << endl;
    cout << "  Stock Sold Successfully!" << endl;
    cout << "  " << portfolio[choice - 1].name << " x " << qty
         << " @ Rs. " << fixed << setprecision(2) << sellPrice << endl;
    cout << "  Total Revenue: Rs. " << fixed << setprecision(2) << totalRevenue << endl;
    if (profitLoss >= 0)
        cout << "  Profit: Rs. " << fixed << setprecision(2) << profitLoss << endl;
    else
        cout << "  Loss: Rs. " << fixed << setprecision(2) << -profitLoss << endl;
    cout << "  Remaining Balance: Rs. " << fixed << setprecision(2) << balance << endl;
    cout << "====================================================" << endl;

    logTransaction("SOLD", portfolio[choice - 1].name, qty, sellPrice, totalRevenue, profitLoss);

    if (portfolio[choice - 1].quantity == 0) {
        for (int i = choice - 1; i < holdingCount - 1; i++) {
            portfolio[i] = portfolio[i + 1];
        }
        holdingCount--;
    }
}

void viewPortfolio() {
    cout << "\n=========================================================" << endl;
    cout << "                    PORTFOLIO SUMMARY" << endl;
    cout << "=========================================================" << endl;

    if (holdingCount == 0) {
        cout << "  No stocks in portfolio." << endl;
    } else {
        cout << left << setw(15) << "Stock"
             << setw(8) << "Qty"
             << setw(14) << "Avg Price"
             << setw(14) << "Cur Price"
             << setw(14) << "Value"
             << "P/L" << endl;
        cout << "---------------------------------------------------------" << endl;

        double totalInvestment = 0, totalValue = 0;

        for (int i = 0; i < holdingCount; i++) {
            int stockIdx = -1;
            for (int j = 0; j < MAX_STOCKS; j++) {
                if (stocks[j].name == portfolio[i].name) {
                    stockIdx = j;
                    break;
                }
            }

            double currentValue = stocks[stockIdx].price * portfolio[i].quantity;
            double invested = portfolio[i].avgBuyPrice * portfolio[i].quantity;
            double pl = currentValue - invested;

            totalInvestment += invested;
            totalValue += currentValue;

            cout << left << setw(15) << portfolio[i].name
                 << setw(8) << portfolio[i].quantity
                 << "Rs." << right << setw(10) << fixed << setprecision(2) << portfolio[i].avgBuyPrice
                 << "  Rs." << setw(10) << stocks[stockIdx].price
                 << "  Rs." << setw(10) << currentValue << "  ";
            if (pl >= 0)
                cout << "+" << fixed << setprecision(2) << pl;
            else
                cout << fixed << setprecision(2) << pl;
            cout << endl;
        }

        cout << "---------------------------------------------------------" << endl;
        cout << "  Total Investment : Rs. " << fixed << setprecision(2) << totalInvestment << endl;
        cout << "  Current Value    : Rs. " << fixed << setprecision(2) << totalValue << endl;
        double netPL = totalValue - totalInvestment;
        if (netPL >= 0)
            cout << "  Net Profit       : Rs. +" << fixed << setprecision(2) << netPL << endl;
        else
            cout << "  Net Loss         : Rs. " << fixed << setprecision(2) << netPL << endl;
    }

    cout << "=========================================================" << endl;
    cout << "  Cash Balance     : Rs. " << fixed << setprecision(2) << balance << endl;
    cout << "=========================================================" << endl;
}

void savePortfolio() {
    ofstream outFile("portfolio.txt");
    if (!outFile) return;
    
    outFile << "=========================================================\n";
    outFile << "                    FINAL PORTFOLIO\n";
    outFile << "=========================================================\n";

    if (holdingCount == 0) {
        outFile << "  No stocks in portfolio.\n";
    } else {
        outFile << left << setw(15) << "Stock"
             << setw(8) << "Qty"
             << setw(14) << "Avg Price"
             << setw(14) << "Cur Price"
             << setw(14) << "Value"
             << "P/L\n";
        outFile << "---------------------------------------------------------\n";

        double totalInvestment = 0, totalValue = 0;

        for (int i = 0; i < holdingCount; i++) {
            int stockIdx = -1;
            for (int j = 0; j < MAX_STOCKS; j++) {
                if (stocks[j].name == portfolio[i].name) {
                    stockIdx = j;
                    break;
                }
            }

            double currentValue = stocks[stockIdx].price * portfolio[i].quantity;
            double invested = portfolio[i].avgBuyPrice * portfolio[i].quantity;
            double pl = currentValue - invested;

            totalInvestment += invested;
            totalValue += currentValue;

            outFile << left << setw(15) << portfolio[i].name
                 << setw(8) << portfolio[i].quantity
                 << "Rs." << right << setw(10) << fixed << setprecision(2) << portfolio[i].avgBuyPrice
                 << "  Rs." << setw(10) << stocks[stockIdx].price
                 << "  Rs." << setw(10) << currentValue << "  ";
            if (pl >= 0)
                outFile << "+" << fixed << setprecision(2) << pl;
            else
                outFile << fixed << setprecision(2) << pl;
            outFile << "\n";
        }

        outFile << "---------------------------------------------------------\n";
        outFile << "  Total Investment : Rs. " << fixed << setprecision(2) << totalInvestment << "\n";
        outFile << "  Current Value    : Rs. " << fixed << setprecision(2) << totalValue << "\n";
        double netPL = totalValue - totalInvestment;
        if (netPL >= 0)
            outFile << "  Net Profit       : Rs. +" << fixed << setprecision(2) << netPL << "\n";
        else
            outFile << "  Net Loss         : Rs. " << fixed << setprecision(2) << netPL << "\n";
    }

    outFile << "=========================================================\n";
    outFile << "  Final Cash Balance : Rs. " << fixed << setprecision(2) << balance << "\n";
    outFile << "=========================================================\n";
    
    // Machine-readable data block for easy loading next time
    outFile << "#DATA#\n";
    outFile << balance << "\n";
    outFile << holdingCount << "\n";
    for (int i = 0; i < holdingCount; i++) {
        outFile << portfolio[i].name << "\n";
        outFile << portfolio[i].quantity << "\n";
        outFile << portfolio[i].avgBuyPrice << "\n";
    }

    outFile.close();
}

void loadPortfolio() {
    ifstream inFile("portfolio.txt");
    if (!inFile) return;
    
    string line;
    bool foundData = false;
    while (getline(inFile, line)) {
        if (line == "#DATA#") {
            foundData = true;
            break;
        }
    }
    
    if (foundData) {
        inFile >> balance;
        inFile >> holdingCount;
        inFile.ignore(); // consume newline
        for (int i = 0; i < holdingCount; i++) {
            getline(inFile, portfolio[i].name);
            inFile >> portfolio[i].quantity;
            inFile >> portfolio[i].avgBuyPrice;
            inFile.ignore(); // consume newline
        }
        cout << "Successfully loaded previous portfolio state from 'portfolio.txt'." << endl;
    }
    inFile.close();
}

int main() {
    srand(static_cast<unsigned int>(time(0)));
    int choice;
    
    loadPortfolio();

    cout << "====================================================" << endl;
    cout << "       STOCK MARKET SIMULATOR" << endl;
    cout << "       Starting Balance: Rs. 50,000.00" << endl;
    cout << "====================================================" << endl;

    do {
        cout << "\n----------------------------------------------------" << endl;
        cout << "                  MAIN MENU" << endl;
        cout << "----------------------------------------------------" << endl;
        cout << "  1. View Stocks" << endl;
        cout << "  2. Buy Stock" << endl;
        cout << "  3. Sell Stock" << endl;
        cout << "  4. View Portfolio" << endl;
        cout << "  5. Exit" << endl;
        cout << "----------------------------------------------------" << endl;
        cout << "  Enter your choice: ";
        cin >> choice;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid input! Please enter a number." << endl;
            continue;
        }

        // Simulate market movement on each turn
        if (choice >= 1 && choice <= 4) {
            updatePrices();
        }

        switch (choice) {
            case 1:
                displayStocks();
                break;
            case 2:
                buyStock();
                break;
            case 3:
                sellStock();
                break;
            case 4:
                viewPortfolio();
                break;
            case 5:
                cout << "\nThank you for using Stock Market Simulator!" << endl;
                cout << "Final Balance: Rs. " << fixed << setprecision(2) << balance << endl;
                savePortfolio();
                cout << "Portfolio details saved to 'portfolio.txt'." << endl;
                break;
            default:
                cout << "Invalid choice! Please try again." << endl;
        }
    } while (choice != 5);

    return 0;
}
