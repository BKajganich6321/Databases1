// Application1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>

bool SrchFile(char itemID[], FILE* file, char record[], bool deleteoread);
void LogData(int itemID, char action);
bool GetBool();
int MenuChoice();
void ReadData(int itemID);
void Write(int itemID);
void Update(int itemID);
void Delete(int itemID);

//ReadData(itemID) -
//char toScreen[] = itemID + " | ”
//LogAction(itemID, "Read") DEBATABLE LOCATION
//Bool invFound = SearchFile(itemID, Inv, toScreen, T)
//Bool stockFound = SearchFile(itemID, stock, toScreen, T)
//If Inv and stock T : printf(toScreen[])
//Else If both F - print item didn't exist
//If GetBool(add ? ) = T
//Write(item ID)
//Else If Inv only - print item not recorded
//If GetBool(add ? ) = T
//DeleteFromFile(item ID, stock, "\n", F)
//Write(ItemID)
//Else If stock only - print item not inventoried
//If GetBool(Update ? )
//Update(itemID)
//Return
//Write(itemID)
//Update(itemID) -
//newCat
//newCount
//SearchFile(itemID)
//----------------------
//----------------------
//Delete(itemID)
//Log1 = DeleteFromFile(itemID, inv)
//Log2 = DeleteFromFile(itemID, stock)
//If(Log 1 or log 2 = T)
//LogData(itemID, " | Delete \n")
//Printf("item has been removed from all sources”)
//    Else print("item never existed")
//    Return
//    ----------------------
//
//    ----------------------
//    GetInt()
//    GetString(bufferSize)
//    GetBool()
//    GetFloat()
//    Int MenuChoice()
//    ----------------------
//    struct product
//    int itemID
//    char[] name
//    char[] category
//    char[] description
//    float unitPrice
//    int inventory
int main()
{
    std::cout << "Hello World!\n";
}

int MenuChoice() {
    int choice;
    printf("Welcome to the main menu.\n");
    printf("<R>ead\n"); 
    printf("<W>rite\n");
    printf("<U>pdate\n"); 
    printf("<D>elete\n");
    printf("E<X>it\n Please make a selection:\n");
    scanf("%c", &choice);
    return choice;
}

bool SrchFile(int itemID, FILE* file, char record[], bool deleteOrRead) {
    bool itemFound = false;
    int fileItemID;
    while (itemFound == false) {
        if (itemID == fileItemID) {
            if (deleteOrRead == true) {
                //Append line to record
                itemFound = true;
            }
            else {
                //remove line from file("\n" to "\n")
                itemFound = true;
            }
            return itemFound;
        }
		//get next fileItemID
    }
} 

void ReadData(int itemID) {
	FILE* pInventory = fopen("Inventory.txt", "rw");
    FILE* pStock = fopen("items.txt", "rw");
    char toScreen[] = "%s | ", itemID;
    bool invFound = SrchFile(itemID, pInventory, toScreen, true);
    bool stockFound = SrchFile(itemID, pStock, toScreen, true);
    if (invFound && stockFound) {
        printf(toScreen);
    }
    else if (invFound == false && stockFound == false) {
        printf("item doesn't exist, please write into database.\n");
    }
    /*If GetBool(add ? ) = T
    Write(item ID)*/
    else if (invFound && !stockFound) {
        printf("Item not recorded in stock file.\n");
    }
    else if(!invFound && stockFound) {
        printf("Item not recorded in inventory file.\n");
		printf("Would you like to add it to the inventory file? <Y>es / <N>o)\n");
        bool update = GetBool();
        if(update) {
            Update(itemID);
		}
	}
    //if GetBool(add ? 
    //    DeleteFromFile(item ID, stock, "\n", F)
    //    Write(ItemID)
    //    Else If stock only - print item not inventoried
    //    If GetBool(Update ? )
    //    Update(itemID)
    return;
}



void LogData(int itemID, char action[]) {
    using 
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
