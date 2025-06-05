
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <deque>


/** Trade Struct. Consider Example:
    Field	Value	Meaning
1. Trade ID	4926538690	Unique trade identifier
2. Price	105573.74000000	Price at which the trade happened
3. Quantity	0.00004000	Amount of asset traded
4. Quote quantity	4.22294960	Price × quantity (in quote asset, e.g., USDT)
5. Timestamp	1747699202961008	Time in microseconds or nanoseconds
6. Is Buyer Market Maker	False	True if the buyer is the market maker
7. Is Best Match	True	Whether this trade is the best match (used by Binance)
 */

struct Trade {
    long long tradeID;
    double price;
    double quantity;
    double quoteQuantity;
    long long timestamp;
    bool isBuyerMaker;
    bool isBestMatch;
};

// Fixed size investment and full exit.

int main() {

    // Reading the CSV File
    std::ifstream file("BTCUSDT-trades-2025-05-20.csv");
    std::string line, value;
    std::deque<Trade> ma50; // Moving Average 50 store.
    double currentAverage50 = 0; // Current Average.
    double runningSum = 0; // Current total.

    const double initalCapital = 100000;
    double currentCapital = initalCapital; // simulation capital (starting with $10,000)
    double fixedSizeInvestment = 1000;

    double buyQuantity = 0;
    double quantity = 0; // initial quantity is set to 0.
    // We are using a simple fixed size investment.


    double realized_pnl = 0; // finding the realised profit and loss
    double unrealized_pnl = 0; // finding the unrealised profit and loss


    // Returns the first 100 values from our csv file.
    for (int i = 0; i<1000;i++){
        std::getline(file, line);
        std::stringstream s (line);
        
        // Parsing the individual line to get all necessary information.
        Trade t;
        std::getline(s, value, ',');
        t.tradeID = std::stoll(value);
        std::getline(s, value, ',');
        t.price = std::stod(value);
        std::getline(s, value, ',');
        t.quantity = std::stod(value);
        std::getline(s, value, ',');
        t.quoteQuantity = std::stod(value);
        std::getline(s, value, ',');
        t.timestamp = std::stoll(value);
        std::getline(s, value, ',');
        if (value == "True"){t.isBuyerMaker = true;}
        else{t.isBuyerMaker = false;}
        std::getline(s, value, ',');
        if (value == "True"){t.isBestMatch = true;}
        else{t.isBestMatch = false;}

        
        ma50.push_back(t);
        runningSum = runningSum + (t.price);

        if(ma50.size()>50){
            Trade poppedValue = ma50.front();
            runningSum = runningSum - poppedValue.price;
            ma50.pop_front();
        }
        currentAverage50 = runningSum/ma50.size();
        std:: cout << currentAverage50 << std::endl;

        std::cout << "Trade ID: " << t.tradeID
        << ", Price: " << t.price
        << ", Quantity: " << t.quantity
        << ", Quote Quantity: " << t.quoteQuantity
        << ", Timestamp: " << t.timestamp
        << ", Buyer Maker: " << (t.isBuyerMaker ? "True" : "False")
        << ", Best Match: " << (t.isBestMatch ? "True" : "False")
        << std::endl;

        if (t.price>currentAverage50){
            std::cout << "Sell" << std::endl;
            currentCapital = currentCapital + quantity * t.price;
            quantity = 0;
        }
        else{
            std::cout << "Buy" << std::endl;
            currentCapital = currentCapital - fixedSizeInvestment;
            buyQuantity = fixedSizeInvestment/t.price;
            quantity = quantity + buyQuantity;
        }
        std::cout << "Current Capital = " <<  currentCapital << std::endl;
        

    }


    std::cout << "Size of ma50: "<<ma50.size() << std::endl;
    
    std::cout << "Current average price of last 50: " <<currentAverage50 << std:: endl;

    double currentTotalInPurchases = quantity * ma50.back().price;
    std::cout << "Current value in Stock: " << currentTotalInPurchases << std::endl;
    // std::cout << capital;

    std::cout << "Current total: " << currentCapital + currentTotalInPurchases << std:: endl;
    std::cout << "Total profit/loss: $" << (currentCapital + currentTotalInPurchases) - initalCapital << std::endl;
    return 0;
}
