// Name: Joshua Christensen
// Project: makeChange
// File: main.cpp
// Purposes: Run a shop and cash register to make change for people
// Date: 1/28/20

#include <iostream> //for using cout / cin
#include <iomanip> //for setting ios flags
#include <cmath> //for using round and log functions
#include <map> //for creating maps to hold things
#include <ctime> //for getting and manipulating time

using namespace std; //used to make code cleaner

//Used to justify text later
void justify(char direction, string str, int width, bool newLine = false, char fill = ' ', double num = 0, int precision = 2);

//a function that stores the majority of the execution and gets called multiple times by main until it returns true.
bool reRun();

//An Item class that represents a single product sold in the store
class Item {
protected: //cant be accessed outside of class.
    //properties
    float price;
    string name;
public: //can be externally viewed
    //properties
    int quantity;

    //Constructors
    Item () { //Default Constructor (needed by map)
        price = 0.0;
        name = "";
        quantity = 0;
    }

    Item (float price, string name, int quantity = 0) { //Constructor that allows for setting price and names
        this->price = price;
        this->quantity = quantity;
        this->name = name;
    };

    //Methods
    float getPrice() {
        return price;
    }

    double totalPrice() { //returns the totalPrice for the Item depending on quantity
        return price * quantity;
    };

    string getName() { //get method for the private name property
        return name;
    };

};

//A shop class that will hold all of the Items and handle checkout and shopping
class Shop {
private: //only available in the class
    //properties
    float tax = .0865;
    double sales = 0;
    int state = 0;
    map<string, Item> catalog; //creates a map based on the template with <string, Item>
public:
    //Constructors
    Shop() {
        Item * Sandwich = new Item(5.99, "Sandwich");
        Item * Chips = new Item(1.99,"Chips");
        Item * Brownie = new Item(1.99,"brownie");
        Item * LDrink = new Item(2.99,"Large Drink");
        Item * SDrink = new Item(1.99,"Small Drink");


        catalog["sandwich"] = *Sandwich;
        catalog["chips"] = *Chips;
        catalog["brownie"] = *Brownie;
        catalog["LDrink"] = *LDrink;
        catalog["SDrink"] = *SDrink;
    }

    //Methods
    int getState() { //get method for finished
        return state;
    };

    double getPrice() { //gets the total price of the cart so far.
        double temp = 0; //temp counter
        map<string, Item>::iterator iterator = catalog.begin(); //define an iterator that counts through each item in the map

        while (iterator != catalog.end()) { //while not at end of map
            string name = iterator->first; //pull out name
            Item product = iterator->second; //pull out Item

            temp += product.totalPrice(); //add the totalPrice of the item to the temp total

            iterator++; //increment the Iterator to the next entry
        }

        return temp;
    };

    void printCatalog() {
        cout << endl;
        justify('l',"",100, true,'-'); //make horizontal line

        //make header
        justify('c',"Product ID",20);
        justify('l',"|",2);
        justify('c',"Name",29);
        justify('l',"|",2);
        justify('c',"Price",12);
        justify('l',"|",2);
        justify('c',"# Ordered",12);
        justify('l',"|",2);
        justify('c',"Total",19, true);

        justify('l',"",100, true,'-'); //make horizontal line

        map<string, Item>::iterator iterator = catalog.begin(); //define an iterator that counts through each item in the map

        while (iterator != catalog.end()) { //while not at end of map
            string name = iterator->first; //pull out name
            Item product = iterator->second; //pull out Item

            //output each item
            justify('l',name,20);
            justify('l',"| ",2);
            justify('l',product.getName(),28);
            justify('l'," | $",4);
            justify('r',"",10, false,' ',product.getPrice());
            justify('l'," | ",3);
            justify('r',"",11, false,' ',product.quantity,0);
            justify('l'," | $",4);
            justify('r',"",18, true,' ',product.totalPrice());

            iterator++; //increment the Iterator to the next entry
        }

        justify('l',"",100, true,'-'); //make horizontal line
        justify('r',"Subtotal | $",82);
        justify('r', "",18, true,' ',getPrice());
        justify('l',"",100, true,'-'); //make horizontal line
    }

    void goShopping() { //displays all the possible choices for the user and ask if they want to keep shopping
        string purchaseID = "";
        int purchaseQuant = 0;

        system("clear"); //clear Screen
        printHeader(); //print out gamer header

        cout << endl << endl << "What would you like to order?" << endl;

        printCatalog(); // output catalog

        cout << endl << "Enter the product ID exactly as shown or type \"cancel\" or \"checkout\" to stop shopping or check out: ";
        cin >> purchaseID;

        if (purchaseID == "checkout") { //test if they typed checkout
            state = 1; //change flag
            return; //stop execution if done
        }

        if (purchaseID == "cancel") {
            state = 2;
            return;
        }

        cout << endl << "How many would you like to purchase? Enter an integer: ";
        cin >> purchaseQuant;

        catalog[purchaseID].quantity = purchaseQuant; //set the quanity of the selected item to the selected quanity
    };

    void makeChange(double tendered) {
        //create temp vars
        double rawBalance = tendered - ((1+tax) * getPrice());
        double rawCents = 100 * (rawBalance - floor(rawBalance));
        double balance = floor(rawBalance);
        double cents = round(rawCents);

        cout << endl << "Change: " << rawBalance <<endl; //output change

        //create and initalize a map of coins/bills
        map<float,int> change;
        change[100] = 0;
        change[50] = 0;
        change[20] = 0;
        change[10] = 0;
        change[5] = 0;
        change[1] = 0;
        change[.25] = 0;
        change[.1] = 0;
        change[.05] = 0;
        change[.01] = 0;

        //test for each bill/coin and subtract
        if (balance / 100 > 1) { //remove hundreds
            change[100] = (balance - ((int)balance % 100))/100;
            balance = (int)balance % 100;
        }

        if (balance / 50 > 1 ) {
            change[50] = (balance - ((int)balance % 50))/50;
            balance = (int)balance % 50;
        }

        if (balance / 20 > 1 ) {
            change[20] = (balance - ((int)balance % 20))/20;
            balance = (int)balance % 20;
        }

        if (balance / 10 > 1 ) {
            change[10] = (balance - ((int)balance % 10))/10;
            balance = (int)balance % 10;
        }

        if (balance / 5 > 1 ) {
            change[5] = (balance - ((int)balance % 5))/5;
            balance = (int)balance % 5;
        }

        change[1] = balance;

        if (cents / 25 > 1) {
            change[.25] = (cents - ((int)cents % 25))/25;
            cents = (int)cents % 25;
        }

        if (cents / 10 > 1) {
            change[.10] = (cents - ((int)cents % 10))/10;
            cents = (int)cents % 10;
        }

        if (cents / 5 > 1) {
            change[.05] = (cents - ((int)cents % 5))/5;
            cents = (int)cents % 5;
        }

        change[.01] = cents;

        //output the header
        justify('l'," ",100, true,'-');
        justify('l',"|",1);
        justify('c',"$0.01",9);
        justify('l',"|",1);
        justify('c',"$0.05",9);
        justify('l',"|",1);
        justify('c',"$0.10",9);
        justify('l',"|",1);
        justify('c',"$0.25",9);
        justify('l',"|",1);
        justify('c',"$1",9);
        justify('l',"|",1);
        justify('c',"$5",9);
        justify('l',"|",1);
        justify('c',"$10",9);
        justify('l',"|",1);
        justify('c',"$20",9);
        justify('l',"|",1);
        justify('c',"$50",9);
        justify('l',"|",1);
        justify('c',"$100",9);
        justify('l',"|",1, true);
        justify('l'," ",100, true,'-');

        //loop through the map of the coins/bills and output it
        map<float, int>::iterator iterator = change.begin(); //define an iterator that counts through each item in the map
        while (iterator != change.end()) { //while not at end of map
            float value = iterator->first; //pull out coin/bill
            int number = iterator->second; //pull out number of them

            //output the number of each
            justify('l',"|",1);
            justify('c',to_string(number),9);

            iterator++; //increment the Iterator to the next entry
        }

        justify('l',"|",1,true);
        justify('l'," ",100, true,'-');

    }

    void checkout() { //checks out the user with cash tendered and makes them change
        double tendered = 0; //test

        //clear everything and replace header and catalog
        system("clear");
        printHeader();
        printCatalog();

        //add on the tax and total
        justify('r',"Tax Rate | * ",83);
        justify('l', "",17, true,' ',tax,4);
        justify('r',"Total Tax | $",82);
        justify('r', "",18, true,' ',tax * getPrice());
        justify('r',"YOUR TOTAL IS | $",82);
        justify('r', "",18, true,' ',(1+tax) * getPrice());
        justify('l',"",100, true,'-'); //make horizontal line

        //handle money
        bool paid = false;
        do {
            cout << endl << "How much was tendered? $";
            cin >> tendered; //get amount tendered
            if (tendered < (1+tax) * getPrice()) { //test if the amount tendered is larger than the price
                cout << endl << "That's not enough money!"; //alert if not enough
                paid = true;
            } else {
                paid = false;
            }
        } while (paid);

        makeChange(tendered); //make change once we know that they paid enough
    };

    static void printHeader() {
        //declare time variables
        time_t now;
        time(&now); //initialize the var with right now
        time_t tenDays = 864000;
        struct tm * tenDaysFromNow = {nullptr};
        char tenDaysFromNowBuffer [10];
        char timeBuffer [39];
        char dateBuffer [39];

        //Calculate time char buffer
        tenDays = now + tenDays;
        tenDaysFromNow = localtime(&tenDays);
        strftime(tenDaysFromNowBuffer,10,"%h %d",tenDaysFromNow);
        strftime(timeBuffer,39," It is currently %I:%M %p on ",localtime(&now));
        strftime(dateBuffer,39," %a, %h %d, %Y ",localtime(&now));

        //output header
//        justify('c',"___",100,true);
//        justify('c',"|[_]|",100,true);
//        justify('c',"|+ ;|",100,true);
//        justify('c',"`---'",100,true);
        justify('l',"",100, true,'~');
        justify('c'," W E L C O M E   T O   S N A C K - S H O P ",100, true,'~');
        justify('c', string(timeBuffer), 100, true, '~');
        justify('c', string(dateBuffer),100, true,'~');
        justify('l',"",100, true, '~');
    }
};

//program execution begins here
int main() {
    bool stop = false; //define stop bool

    do {
        stop = reRun(); //rerun reRun until it tells us to stop
        system("clear");
    } while (stop);

    return 0;
}

//runs repeatedly until the user requests to stop
bool reRun(){
    char userResponse = 'n'; //define userResponse so we know when to stop

    Shop * myShop = new Shop(); //create a new shop

    do {
        myShop->goShopping(); //shop until they're ready to checkout
    } while (!myShop->getState());

    if (myShop->getState() == 1){
        myShop->checkout(); //run the checkout method
    }

    cout << "Would you like to run again? (Y/N): ";
    cin >> userResponse;

    return !(userResponse == 'n' || userResponse == 'N'); //end if they're done
}


//Justify Function Definition
void justify(char direction, string str, int width, bool newLine, char fill, double num, int precision) {
    cout << resetiosflags(ios::left | ios::right) << setiosflags(ios::fixed) << setprecision(precision); //reset all of the flags
    if (num != 0) { //skip all the string stuff if the number is set, if it actually is zero then the "0.00" string we used for str will get used
        if (direction == 'r') {
            cout << setiosflags(ios::right) << setfill(fill) << setw(width) << num; //justify Right
        } else if (direction == 'c') {
            int availableSpace = width - ((int)(log10(num)+3)); //find the amount of space that is available
            if( availableSpace < 0) { //if the space for justifying is less than 0
                cout << setiosflags(ios::left) << setfill(fill) << setw(width) << num; //just output as left justify
                return;
            }
            int stringStartLength = availableSpace/2; //the width before the num
            int stringEndLength = availableSpace-stringStartLength; //the width after the num, uses subtraction to avoid rounding errors
            cout << setiosflags(ios::left) << setfill(fill) << setw(stringStartLength) << "" << num << setw(stringEndLength) << ""; //couts the num sandwiched between to pads of setw()
        } else {
            cout << setiosflags(ios::left) << setfill(fill) << setw(width) << num; //justify Left
        }
    } else if (direction == 'r') {
        cout << setiosflags(ios::right) << setfill(fill) << setw(width) << str; //justify right
    } else if (direction == 'c') {
        int availableSpace = width - str.length(); //find the amount of space that is available
        if( availableSpace < 0) { //if the space for justifying is less than 0
            cout << setiosflags(ios::left) << setfill(fill) << setw(width) << str; //just output as left justify
            return;
        }
        int stringStartLength = availableSpace/2; //the width before the num
        int stringEndLength = availableSpace-stringStartLength; //the width after the num, uses subtraction to avoid rounding errors
        cout << setiosflags(ios::left) << setfill(fill) << setw(stringStartLength) << "" << str << setw(stringEndLength) << ""; //couts the str sandwiched between to pads of setw()
    } else {
        cout << setiosflags(ios::left) << setfill(fill) << setw(width) << str; // justify left
    }

    if (newLine) {
        cout << "\n"; //if there is supposed to be a new line after the phrase, then send it out
    }

    return;
}