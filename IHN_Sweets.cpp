#include <iostream> 
#include <vector> 
#include <string> 
#include <iomanip> 
#include <limits> 

// 1. Info of an Item in the Cart
struct cartItem 
{
    int id;
    std::string name;
    double price;
    int stock; 
};

// structure of the prog.
void displayMenu(const std::vector<cartItem>& inventory);
void processOrder(std::vector<cartItem>& inventory);
void printReceipt(const std::vector<std::pair<cartItem, int>>& orderedItems, double total);
void initializeInventory(std::vector<cartItem>& inventory);


// anti crash (to avoid incorrect input)
void cleanInput()
{
    std::cin.clear(); 
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}


// Main or Start of the Prog.
int main() 
{
    std::vector<cartItem> inventory;
    initializeInventory(inventory); 

    std::cout << "========================================" << std::endl;
    std::cout << "        WELCOME TO THE SHN SWEETS!!     " << std::endl;
    std::cout << "========================================" << std::endl;

    char choice;
    do 
    {
        // Show Menu
        displayMenu(inventory);

        // Place an order
        processOrder(inventory);
        
        // Ask if they want to make another order
        std::cout << "\nDo you want to start a new order? (Yes/No): ";
        std::cin >> choice;

        cleanInput(); 

    } while (choice == 'y' || choice == 'Y'); 

    std::cout << "\nThank you for visitung SHN SWEETS. Goodbye and have a sweet day!" << std::endl;
    return 0; 
}


// Inventory 
void initializeInventory(std::vector<cartItem>& inventory) 
{
    inventory.push_back({1, "Chocolate Chip Cookies", 30.00, 50});
    inventory.push_back({2, "Tiramisu Cake (1 slice)", 50.00, 40});
    inventory.push_back({3, "Nutella Donut", 30.00, 25});
    inventory.push_back({4, "Banana Muffin", 20.00, 30});
    inventory.push_back({5, "Chocolate Mousse", 40.00, 20});
}
 
// Display Menu
void displayMenu(const std::vector<cartItem>& inventory) 
{
    std::cout << "\n--- SHN SWEETS MENU (Current Stock) ---" << std::endl;
    std::cout << std::setw(4) << "ID"
              << std::setw(25) << std::left << "Item Name"
              << std::setw(10) << "Price"
              << std::setw(10) << "Stock" << std::endl;
    std::cout << std::string(49, '-') << std::endl; 

    
    for (const auto& item : inventory) 
    {
        std::cout << std::setw(4) << item.id
                  << std::setw(25) << std::left << item.name
                  << std::setw(1) << "$"
                  << std::setw(9) << std::right << std::fixed << std::setprecision(2) << item.price
                  << std::setw(10) << item.stock << std::endl;
    }
    std::cout << std::string(49, '-') << std::endl;
}


// Order Process
void processOrder(std::vector<cartItem>& inventory) 
{
    std::vector<std::pair<cartItem, int>> orderDetails; 
    double subtotal = 0.0;
    int itemID, quantity;

    std::cout << "\n--- STARTING NEW ORDER ---" << std::endl;
    
    while (true) 
    {
        std::cout << "Enter Item ID (Enter 0 to finish your order): ";
        
        while (!(std::cin >> itemID) || itemID < 0) 
        {
            std::cout << "Invalid ID. Please enter a positive number or 0 to finish: ";
            cleanInput();
        }

        if (itemID == 0) 
        {
            break; 
        }

        auto it = inventory.begin();
        bool found = false;
        
        
        while (it != inventory.end()) 
        {
            if (it->id == itemID) 
            {
                found = true;
                break; 
            }
            ++it; 
        }

        if (!found) 
        {
            std::cout << "Error: Item ID " << itemID << " not found. Please check the menu ID and try again." << std::endl;
            continue; 
        }

        //ask how many
        std::cout << "Enter quantity for " << it->name << ": ";
        while (!(std::cin >> quantity) || quantity <= 0) 
        {
            std::cout << "Invalid quantity. Please enter a quantity greater than 0: ";
            cleanInput();
        }

        //Check Stock
        if (quantity > it->stock) 
        {
            std::cout << "Error: Only " << it->stock << " " << it->name << "(s) in stock. Please reduce the quantity." << std::endl;
            continue; 
        }

        // inventory upd. subtotal calc.
        it->stock -= quantity; 
        double itemTotal = it->price * quantity;
        subtotal += itemTotal;

        orderDetails.push_back({*it, quantity}); 

        std::cout << "** Added " << quantity << "x " << it->name 
                  << " to order. Current Subtotal: $" << std::fixed << std::setprecision(2) << subtotal << " **\n" << std::endl;
    } // End of the main order loop

    // finalization and receipt
    if (orderDetails.empty()) 
    {
        std::cout << "No items were ordered. Order finished." << std::endl;
    } else 
    {
        std::cout << "\n--- Order Complete!! ---" << std::endl;
        printReceipt(orderDetails, subtotal);
    }
} // End of the processOrder function


// Receipt process
void printReceipt(const std::vector<std::pair<cartItem, int>>& orderedItems, double subtotal) 
{
    const double TAX_RATE = 0.07; 
    double taxAmount = subtotal * TAX_RATE;
    double finalTotal = subtotal + taxAmount;

    std::cout << "========================================" << std::endl;
    std::cout << "          SHN SWEETS RECEIPT            " << std::endl;
    std::cout << "========================================" << std::endl;
    
    for (const auto& detail : orderedItems) 
    {
        const cartItem& item = detail.first; 
        int quantity = detail.second;       
        double itemTotal = item.price * quantity;

        std::cout << std::setw(3) << quantity << "x "
                  << std::setw(25) << std::left << item.name
                  << std::setw(1) << "$"
                  << std::setw(8) << std::right << std::fixed << std::setprecision(2) << itemTotal << std::endl;
    }

    std::cout << std::string(40, '-') << std::endl;
    
    // final totals
    std::cout << std::setw(28) << std::right << "Subtotal:"
              << std::setw(1) << "$"
              << std::setw(10) << std::fixed << std::setprecision(2) << subtotal << std::endl;

    std::cout << std::setw(28) << std::right << "Tax (7%):"
              << std::setw(1) << "$"
              << std::setw(10) << std::fixed << std::setprecision(2) << taxAmount << std::endl;

    std::cout << std::string(40, '=') << std::endl;

    std::cout << std::setw(28) << std::right << "TOTAL:"
              << std::setw(1) << "$"
              << std::setw(10) << std::fixed << std::setprecision(2) << finalTotal << std::endl;

    std::cout << "========================================" << std::endl;
    std::cout << "      Thank you for stopping by!!       " << std::endl;
    std::cout << "========================================" << std::endl;
}