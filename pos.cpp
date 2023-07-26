#include <iostream>
#include <list>
#include <string>
#include <iomanip>
#include <vector>
#include <fstream> // Added this header for file handling

using namespace std;

// Struct to store product information
struct Product {
    string name;
    double price;
    int quantity;

    Product() : name(""), price(0.0), quantity(0) {}
    Product(string n, double p, int q) : name(n), price(p), quantity(q) {}
};

// Supermarket class
class Supermarket {
private:
    list<Product> inventory;
    bool loggedIn;
    int loginAttempts;

public:
    Supermarket() : loggedIn(false), loginAttempts(0) {}

    // Add product to inventory
    void addProduct(string name, double price, int quantity) {
        Product product(name, price, quantity);
        inventory.push_back(product);
    }

    // Remove product from inventory
    void removeProduct(string name) {
        for (auto it = inventory.begin(); it != inventory.end(); ++it) {
            if (it->name == name) {
                inventory.erase(it);
                break;
            }
        }
    }

    // Update product quantity in inventory
    void updateQuantity(string name, int quantity) {
        for (auto& product : inventory) {
            if (product.name == name) {
                product.quantity = quantity;
                break;
            }
        }
    }

    // Make a sale
    void makeSale() {
        if (!loggedIn) {
            cout << "Please login first." << endl;
            return;
        }

        list<pair<string, int>> cart;  // Product name and quantity pairs
        string productName;
        int productQuantity;

        cout << "Enter customer name: ";
        string customerName;
        cin.ignore();
        getline(cin, customerName);

        cout << "Enter customer phone number: ";
        string phoneNumber;
        cin >> phoneNumber;

        cout << "Enter 'done' when finished adding products to the cart." << endl;
        while (true) {
            cout << "Enter product name: ";
            cin.ignore();
            getline(cin, productName);
            if (productName == "done")
                break;

            cout << "Enter product quantity: ";
            cin >> productQuantity;

            // Check if product is available in inventory and quantity is sufficient
            bool productFound = false;
            for (auto it = inventory.begin(); it != inventory.end(); ++it) {
                if (it->name == productName) {
                    productFound = true;
                    if (productQuantity <= it->quantity) {
                        cart.push_back(make_pair(productName, productQuantity));
                        it->quantity -= productQuantity;
                        cout << "Product added to cart." << endl;
                    } else {
                        cout << "Insufficient quantity available for that product." << endl;
                    }
                    break;
                }
            }

            if (!productFound) {
                cout << "Product not found in inventory." << endl;
            }
        }

        // Print receipt
        double totalPrice = 0.0;
        cout << "\n------------------------" << endl;
        cout << "      CUSTOMER RECEIPT" << endl;
        cout << "------------------------" << endl;
        cout << "Customer Name: " << customerName << endl;
        cout << "Phone Number: " << phoneNumber << endl;
        cout << "------------------------" << endl;
        cout << "Product      Quantity    Price" << endl;
        cout << "------------------------" << endl;
        for (const auto& item : cart) {
            const string& name = item.first;
            int quantity = item.second;
            double price = 0.0;

            // Find the price of the product in the inventory
            for (const auto& product : inventory) {
                if (product.name == name) {
                    price = product.price;
                    break;
                }
            }

            double itemPrice = price * quantity;
            cout << setw(12) << left << name << setw(10) << quantity << setw(10) << itemPrice << endl;
            totalPrice += itemPrice;
        }
        cout << "------------------------" << endl;
        cout << "Total Price: " << totalPrice << endl;
        cout << "------------------------" << endl;
    }

    // Login functionality
    void login() {
        string username, password;
        cout << "Enter username: ";
        cin >> username;
        cout << "Enter password: ";
        cin >> password;

        // Simple check for login credentials
        if (username == "kelvin" && password == "khalid") {
            loggedIn = true;
            cout << "Login successful!" << endl;
        } else {
            cout << "Invalid credentials. Login failed." << endl;
            loginAttempts++;
            if (loginAttempts >= 3) {
                cout << "Maximum login attempts reached. Please create a new account." << endl;
                createAccount();
            } else {
                cout << "Please try again." << endl;
                login();
            }
        }
    }

    // Create a new account
    void createAccount() {
        string newUsername, newPassword;
        cout << "Enter new username: ";
        cin >> newUsername;
        cout << "Enter new password: ";
        cin >> newPassword;

        cout << "Account created successfully! Please log in with your new credentials." << endl;
        loggedIn = false;
        loginAttempts = 0;
        login();
    }

    // Run the POS system
    void run() {
        if (!loggedIn) {
            cout << "Please login first." << endl;
            return;
        }

        cout << "Supermarket POS System" << endl;

        while (true) {
            int choice;
            cout << "\n1. Add product to inventory" << endl;
            cout << "2. Remove product from inventory" << endl;
            cout << "3. Update product quantity" << endl;
            cout << "4. Make a sale" << endl;
            cout << "5. Exit" << endl;
            cout << "Enter your choice: ";
            cin >> choice;

            string name;
            double price;
            int quantity;

            switch (choice) {
                case 1:
                    cout << "Enter product name: ";
                    cin.ignore();
                    getline(cin, name);
                    cout << "Enter product price: ";
                    cin >> price;
                    cout << "Enter product quantity: ";
                    cin >> quantity;
                    addProduct(name, price, quantity);
                    cout << "Product added to inventory." << endl;
                    break;
                case 2:
                    cout << "Enter product name: ";
                    cin.ignore();
                    getline(cin, name);
                    removeProduct(name);
                    cout << "Product removed from inventory." << endl;
                    break;
                case 3:
                    cout << "Enter product name: ";
                    cin.ignore();
                    getline(cin, name);
                    cout << "Enter new product quantity: ";
                    cin >> quantity;
                    updateQuantity(name, quantity);
                    cout << "Product quantity updated." << endl;
                    break;
                case 4:
                    makeSale();
                    break;
                case 5:
                    cout << "Exiting..." << endl;

                    // Save the inventory to product.txt before exiting
                    saveInventoryToFile("product.txt");

                    return;
                default:
                    cout << "Invalid choice. Try again." << endl;
                    break;
            }
        }
    }

    // Function to save the inventory to a file
    void saveInventoryToFile(const string& filename) {
        ofstream file(filename);
        if (file.is_open()) {
            for (const auto& product : inventory) {
                file << product.name << "," << product.price << "," << product.quantity << "\n";
            }
            file.close();
            cout << "Inventory saved to " << filename << " successfully." << endl;
        } else {
            cout << "Unable to open " << filename << " for saving inventory." << endl;
        }
    }

    // Function to load the inventory from a file
    void loadInventoryFromFile(const string& filename) {
        ifstream file(filename);
        if (file.is_open()) {
            inventory.clear(); // Clear the existing inventory before loading from the file
            string line;
            while (getline(file, line)) {
                size_t commaPos1 = line.find(",");
                size_t commaPos2 = line.rfind(",");
                if (commaPos1 != string::npos && commaPos2 != string::npos && commaPos1 != commaPos2) {
                    string name = line.substr(0, commaPos1);
                    double price = stod(line.substr(commaPos1 + 1, commaPos2 - commaPos1 - 1));
                    int quantity = stoi(line.substr(commaPos2 + 1));
                    inventory.push_back(Product(name, price, quantity));
                }
            }
            file.close();
            cout << "Inventory loaded from " << filename << " successfully." << endl;
        } else {
            cout << "Unable to open " << filename << " for loading inventory." << endl;
        }
    }
};

int main() {
    Supermarket supermarket;

    // Load the inventory from product.txt before starting the program
    supermarket.loadInventoryFromFile("product.txt");

    supermarket.login();
    supermarket.run();

    return 0;
}
