# Stock Market Simulator

A console-based stock market simulator written in C++ that allows users to experience basic trading mechanics without real financial risk.

## Features
- **Real-time Price Simulation**: Stock prices randomly fluctuate between -5.0% and +5.0% on each turn, mimicking market volatility.
- **Buy / Sell Execution**: Users can buy stocks given they have sufficient balance and sell their existing holdings.
- **Portfolio Management**: The system tracks your stock holdings, average buy prices, current value, total investment, cash balance, and overall Net Profit/Loss.
- **Pre-loaded Stocks**: Includes 5 major Indian stocks out of the box (Reliance, TCS, Infosys, HDFC Bank, ITC).
- **Graceful Error Handling**: Capable of protecting against infinite loops caused by invalid non-numeric inputs.

## Getting Started

### Prerequisites
- A C++ compiler (like GCC, Clang, or MSVC)

### Compilation & Execution
1. Open your terminal.
2. Navigate to the directory containing `stock.cpp`.
3. Compile the file:
   ```bash
   clang++ stock.cpp -o stock
   ```
4. Run the executable:
   ```bash
   ./stock
   ```

## How to Play
When you start the program, you are granted a starting balance of **Rs. 50,000.00**. The Main Menu offers 5 distinct actions:
1. **View Stocks**: See the current prices of all available stocks for the current turn.
2. **Buy Stock**: Enter the stock number and the quantity you want to purchase. Ensure you have the required balance. Multiple buys of the same stock will automatically average out the buying price.
3. **Sell Stock**: Enter the holding number and quantity to sell from your portfolio. Selling updates your cash balance and logs any capital profit/loss.
4. **View Portfolio**: Displays a comprehensive summary of your investments, including average buy price, current price, total value, and P/L metrics.
5. **Exit**: Terminates the simulation and displays the final portfolio balance.

## Technical Details
- Written strictly using standard C++ libraries (`<iostream>`, `<string>`, `<iomanip>`, `<cstdlib>`, `<ctime>`).
- Utilizes basic structured programming paradigms using standard C++ `struct` to group closely related properties (`Stock` and `Holding`).
- Validates user input effectively to prevent negative quantities, out-of-bounds menu selections, and `cin` state failures.
- Uses standard PRNG (`srand` and `rand()`) keyed with the current time (`time(0)`) to ensure pseudo-random stock volatility on each launch.
