
//Aparaitites bibliothikes
#include <iostream> // parexei leitoyrgies ejodou ( cout cin klp )
#include <fstream> // epitrepei tin anagnwsi kai eggrafi arxeiwn 
#include <vector> // dynamiki domi dedomenon ( xrhsh vector )
#include <ctime>  // parexei leitourgies sxetikes me ton xrono kai tin imerominia
#include <iomanip> // Bibliothiki gia to setw()
using namespace std;


// Klasi 'Product' gia tin diaxirisi twn proiontwn
class Product {
public:
    int barcode;
    string name;
    int quantity;
    double price;

    Product(int bc, string n, int q, double p) {
        barcode = bc;
        name = n;
        quantity = q;
        price = p;
    }
};

// Dimioyrgoume Vector gia ta proionta tis apothikis
vector<Product> warehouse;


// Megisti xwritikotita
const int MAX_PRODUCTS = 1000;
// Metavliti gia to trexon sinoliko apothema
int currentStock = 0;

// Sinartisi pou ipologizei to sinoliko apothema 
void calculateStock() {
    currentStock = 0;
    for (int i = 0; i < warehouse.size(); i++) {
        currentStock += warehouse[i].quantity;
    }
}

// Sinolika esoda
double totalRevenue = 0;

// Ora enarxis
time_t startTime;

// Arxika 10 proionta an to arxeio den iparxei
void addInitialProducts() {
    if (!warehouse.empty()) return;  // An iparxoun idi ta proionta den ta prosthetoume xana

    warehouse.push_back(Product(520001, "Milk", 50, 1.50));
    warehouse.push_back(Product(520002, "Bread", 30, 0.80));
    warehouse.push_back(Product(520003, "Cheese", 20, 2.30));
    warehouse.push_back(Product(520004, "Butter", 40, 3.00));
    warehouse.push_back(Product(520005, "Eggs", 60, 2.50));
    warehouse.push_back(Product(520006, "Sugar", 25, 1.20));
    warehouse.push_back(Product(520007, "Salt", 15, 0.60));
    warehouse.push_back(Product(520008, "Coffee", 35, 4.50));
    warehouse.push_back(Product(520009, "Tea", 45, 3.80));
    warehouse.push_back(Product(520010, "Juice", 55, 2.90));

    cout << "10 initial products added to warehouse.\n";
}

// Fortosi proiontwn apo to warehouse.txt
void loadProducts() {
    ifstream file("warehouse.txt");
    if (!file) {
        addInitialProducts(); // An to arxeio den iparxei prosthetoume ta arxika proionta
        return;
    }

    int bc, q;
    double p;
    string n;
    while (file >> bc >> n >> q >> p) {
        warehouse.push_back(Product(bc, n, q, p));
    }
    file.close();

    if (warehouse.empty()) {
        addInitialProducts(); // An to arxeio einai adeio prosthetoume ta proionta
    }
}

// Apothikeysi proiontwn sto arxeio
void saveProducts() {
    ofstream file("warehouse.txt");
    for (int i = 0; i < warehouse.size(); i++) {
        file << warehouse[i].barcode << " " << warehouse[i].name << " "
             << warehouse[i].quantity << " " << warehouse[i].price << endl;
    }
    file.close();
}

// Prosthiki i enimerwsi proiontos
void addOrUpdateProduct() {
    int bc;
    cout << "Enter barcode: ";
    cin >> bc;

    // Elegxos an iparxei idi to proion
    for (int i = 0; i < warehouse.size(); i++) {
        if (warehouse[i].barcode == bc) {
            cout << "Product found: " << warehouse[i].name << endl;
            cout << "How many more items do you want to add? ";
            int addQty;
            cin >> addQty;

            // Elegxos an jepernaei to orio twn 100 proiontwn
            if (currentStock + addQty > MAX_PRODUCTS) {
                cout << " Error: Warehouse has only space for " << (MAX_PRODUCTS - currentStock) << " more products.\n";
                return;
            }

            warehouse[i].quantity += addQty;
            calculateStock(); // Enimerwsi apothematos

            cout << "Do you want to change the price? (1 = Yes, 0 = No): ";
            int changePrice;
            cin >> changePrice;
            if (changePrice == 1) {
                cout << "Enter new price: ";
                cin >> warehouse[i].price;
            }

            cout << "Do you want to change the name? (1 = Yes, 0 = No): ";
            int changeName;
            cin >> changeName;
            if (changeName == 1) {
                cout << "Enter new name: ";
                cin >> warehouse[i].name;
            }

            saveProducts();
            cout << "Product updated successfully!\n";
            return;
        }
    }

    // An i apothiki einai gemati den mporoume na prosthesoume nea proionta
    if (currentStock >= MAX_PRODUCTS) {
        cout << " Error: Warehouse is full! Cannot add more products.\n";
        return;
    }

    string name;
    unsigned long quantity;

    double price;

    cout << "Enter product name: ";
    cin >> name;
    cout << "Enter quantity: ";
    cin >> quantity;

	// Elegxos an i prosthiki jeperna to orio
    if (currentStock + quantity > MAX_PRODUCTS) {
        cout << "? Error: Not enough space in the warehouse! Only " << (MAX_PRODUCTS - currentStock) << " more products can be added.\n";
        return;
    }

    cout << "Enter price: ";
    cin >> price;

    warehouse.push_back(Product(bc, name, quantity, price));
    calculateStock(); // Enimerwsi apothematos
    saveProducts();
    cout << "Product added successfully!\n";
}

// Provoli proiontwn se pinaka
void showProducts() {
    if (warehouse.empty()) {
        cout << "No products available in warehouse.\n";
        return;
    }

    // emfanisi proionton
    cout << " ----------------------------------------------------------\n";
    cout << "|  " << setw(8) << "BARCODE" << "  |  " 
         << setw(15) << "NAME" << "  |  " 
         << setw(8) << "QUANTITY" << "  |  " 
         << setw(8) << "PRICE $" << "  |\n";
    cout << " ----------------------------------------------------------\n";

    // Emfanisi proiontwn
    for (int i = 0; i < warehouse.size(); i++) {
        cout << "|  " << setw(8) << warehouse[i].barcode << "  |  " 
             << setw(15) << warehouse[i].name << "  |  " 
             << setw(8) << warehouse[i].quantity << "  |  " 
             << setw(8) << fixed << setprecision(2) << warehouse[i].price << "  |\n";
    }

    cout << " ----------------------------------------------------------\n";
}

// Apostoli proiontos
void shipProduct() {
    showProducts(); 

    int bc, qty;
    cout << "Enter barcode to ship: ";
    cin >> bc;

    for (int i = 0; i < warehouse.size(); i++) {
        if (warehouse[i].barcode == bc) {
            cout << "Product found: " << warehouse[i].name << endl;
            cout << "Enter quantity to ship: ";
            cin >> qty;

            if (qty > warehouse[i].quantity) {
                cout << "Only " << warehouse[i].quantity << " available. Ship all? (1 = Yes, 0 = No): ";
                int choice;
                cin >> choice;
                if (choice == 1) {
                    qty = warehouse[i].quantity;
                } else {
                    return;
                }
            }

            double cost = qty * warehouse[i].price;
            totalRevenue += cost;
            warehouse[i].quantity -= qty;

            cout << "Shipped " << qty << " items. Total cost: $" << cost << endl;

            saveProducts();
            return;
        }
    }

    cout << "Product not found or out of stock.\n";
}

// Exodos kai emfanisi sinolikou xronou kai esodwn
void exitProgram() {
    time_t endTime = time(0);
    int duration = (int)difftime(endTime, startTime);
    cout << "Total revenue: " << totalRevenue << " $" << endl;
    cout << "Warehouse was active for " << duration << " seconds.\n";
}

// Main program
int main() {
    startTime = time(0); 
    loadProducts();
    calculateStock();  // Ypologizoume to arxiko sinoliko apothema

    int choice;
    do {
        cout << "\nWAREHOUSE MENU\n1. Add/Update Product\n2. Show Products\n3. Ship Product\n4. Exit\nEnter \2choice: ";
        cin >> choice;
        switch (choice) {
            case 1: addOrUpdateProduct(); 
				break;
            case 2: showProducts(); 
				break;
            case 3: shipProduct(); 
				break;
            case 4: exitProgram(); 
				break;
            default: cout << "Invalid choice! Try again.\n";
        }
    } while (choice != 4);

    return 0;
}
