/*
* FILE : Databases1.cpp
* PROJECT : PROG2111 - [ASSIGNMENT 1] – [E-COMMERCE INVENTORY MANAGEMENT SYSTEM] 
* PROGRAMMER : Brad Kajganich - ID 9036321
* FIRST VERSION : 2025-10-01
* DESCRIPTION : This Program is used to access/add/adjust/remove items from
                 two text files in an e-commerce inventory database
*/
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>

#define TEMPFILE "temp.txt"
#define INVENTORY "Inventory.txt"
#define ITEMS "items.txt"

#pragma warning(disable: 4996)

void getString(int size, char string[]);
int getInt();
float getFloat();
bool searchFile(int itemID, char fileName[], char record[]);
void logData(int itemID, char action[]);
bool getBool();
char menuDisplay();
void trimNewLine(char line[]);
bool updateCount(int itemID);
bool updateCat(int itemID);
void updateItem(int itemID);
void readData(int itemID);
void writeItem(int itemID);
void deleteItem(int itemID);
bool deleteFromFile(char fileName[], int itemID);
void clearInput();

/*
* FUNCTION      : main()
* DESCRIPTION   : Presents menu to the user and performs function calls based on user input. Continues running until user exits with 
*                   E<x>it input.
* PARAMETERS    : None
* RETURNS       : None
*/
int main()
{
    //char inventoryFile[] = "Inventory.txt";
    //char itemsFile[] = "items.txt";
    int itemID;
    //char record[100];
    bool exit = false;
    char menuChoice;
    while (!exit) {

        menuChoice = menuDisplay();
        if (menuChoice == 'X' || menuChoice == 'x') {
            exit = true;
            continue;
        }
        printf("Please enter itemID: ");
        itemID = getInt();
        if (menuChoice == 'R' || menuChoice == 'r') {
            readData(itemID);
        }
        else if (menuChoice == 'D' || menuChoice == 'd') {
            deleteItem(itemID);
        }
        else if (menuChoice == 'U' || menuChoice == 'u') {
            updateItem(itemID);
        }
        else if (menuChoice == 'W' || menuChoice == 'w') {
            writeItem(itemID);
        }
    }
    return 1;
}

/*
* FUNCTION      : menuDisplay()
* DESCRIPTION   : Presents menu to the user and gets input. After validating the input as a single valid character, 
*                   returns that character.
* PARAMETERS    : noen
* RETURNS       : User Menu Selected character
*/
char menuDisplay() {
    char inputString[20];
    char choice;
    bool valid = false;
    printf("\nWelcome to the main menu.\n");
    printf("<R>ead\n"); 
    printf("<W>rite\n");
    printf("<U>pdate\n"); 
    printf("<D>elete\n");
    printf("E<X>it\nPlease make a selection:\n");
    while (!valid){
        if (fgets(inputString, sizeof(inputString), stdin) == NULL) {
            printf("Input cannot be empty");
            continue;
        }
        inputString[strcspn(inputString, "\n")] = '\0';
        if (strlen(inputString) == 1) {
            choice = (unsigned char)inputString[0];
            if (choice != 'R' && choice != 'r' && choice != 'W' && choice != 'w'
                && choice != 'U' && choice != 'u' && choice != 'D' && choice != 'd'
                && choice != 'X' && choice != 'x') {
                printf("Please enter an option from above. \n<R>ead, <W>rite, <U>pdate, <D>elete or E<x>it.\n Try again\n");
            }
            else {
                valid = true;
            }
        }
        else {
            printf("Please enter an option from above. \n<R>ead, <W>rite, <U>pdate, <D>elete or E<x>it.\n");
            printf("Try again: ");
            clearInput();
        }
    }
    return choice;
}

/*
* FUNCTION      : searchFile()
* DESCRIPTION   : Takes itemID, searches the given filename for the item and if found, appends the information passed the first token (ID) to the 
*                  3rd parameter in the event of information display being required.
* PARAMETERS    : int itemID, char fileName[], char record[]
* RETURNS       : Boolean, t/f if itemID found in given file
*/
bool searchFile(int itemID, char fileName[], char record[]) {
    bool itemFound = false;
    int fileItemID;
    char *fileItemStr, *remainingLine;
    char currentLine[500];
    FILE* pFile = fopen(fileName, "r"); // test from here

    if (pFile == NULL) {
        perror("Error opening file");    
        return itemFound;
    }

    while (itemFound == false && fgets(currentLine, sizeof(currentLine), pFile) != NULL) {
        fileItemStr = strtok_s(currentLine, "|", &remainingLine);
        remainingLine = strtok(remainingLine, "\n");
        fileItemID = atoi(currentLine);
        if (itemID == fileItemID) {
            strcat(record, remainingLine);
            itemFound = true;
            fclose(pFile);
            return itemFound;
        }
    }
    fclose(pFile);
    return itemFound;
}

/*
* FUNCTION      : writeItem()
* DESCRIPTION   : Takes an itemID and checks the database files for preexistence. If found, returns user to main menu. If not, allows the user to 
*                   input new item data and records item in all database files. If recorded, logs the activity.
* PARAMETERS    : int itemID
* RETURNS       : None
*/
void writeItem(int itemID) {
    FILE* pItems;
    FILE* pInventory;
    char action[] = "Write";
    char inventoryFile[] = "Inventory.txt";
    char itemFile[] = "items.txt";
    char itemInfo[120] = "";
    bool written = false;
    bool itemFound1 = false;
    bool itemFound2 = false;
    char itemName[21];
    char itemCat[21];
    char itemDesc[41];
    int itemCount;
    float itemPrice;

    itemFound1 = searchFile(itemID, inventoryFile, itemInfo);
    itemFound2 = searchFile(itemID, itemFile, itemInfo);
    
    if (itemFound1 || itemFound2) {
        printf("This item already exists, please either Delete and Rewrite the item\n");
        printf("or Update. Both can be done in the following menu\n");
        return;
    }
    else{
        pInventory = fopen(inventoryFile, "a");
        pItems = fopen(itemFile, "a");

        if (pInventory == NULL || pItems == NULL) {
            perror("Error opening file\n");
            if (pInventory) {
                fclose(pInventory);
            }
            if (pItems) {
                fclose(pItems);
            }
            return;
        }
        logData(itemID, action);
        printf("Please enter an Item Name (max 20 characters): ");
        getString(20, itemName);
        printf("Please enter an Item Category (max 20 characters): ");
        getString(20, itemCat);
        printf("Please enter an Item Description (max 40 characters): ");
        getString(40, itemDesc);
        itemPrice = getFloat();
        printf("Please enter the number of units on hand: ");
        itemCount = getInt();
        fprintf(pInventory, "\n%d | %d", itemID, itemCount);
        fprintf(pItems, "\n%d | %s | %s | %s | %.2f", itemID, itemName, itemCat, itemDesc, itemPrice);
        fclose(pInventory);
        fclose(pItems);
    }
    return;
}

/*
* FUNCTION      : clearInput()
* DESCRIPTION   : Clears the stdin stream of existing data, allowing for future healthy input
* PARAMETERS    : None
* RETURNS       : None
*/
void clearInput() {
    char clear;
    while ((clear = getchar()) != '\n' && clear != EOF);
}

/*
* FUNCTION      : trimNewLine()
* DESCRIPTION   : Removes newLine character (if existing) from given character array
* PARAMETERS    : char line[]
* RETURNS       : None
*/
void trimNewLine(char line[]) {
    line[strcspn(line, "\n")] = '\0';
}

/*
* FUNCTION      : getBool()
* DESCRIPTION   : Gets user input that satisfies a 'Y', 'N', 'y', 'n' condition and returns true/false once input
*                   conditions are met.
* PARAMETERS    : None
* RETURNS       : bool 
*/
bool getBool() {
    char response[] = "";
    bool complete = false;
    do {
        int received = scanf("%s", &response);
        if (response == "Y\n" || response == "y\n") {
            complete = true;
            return true;
        }
        else if (response == "N\n" || response == "n\n") {
            complete = true;
            return false;
        }
        else
        {
            printf("\nPlease enter Y for Yes or N for No: ");
        }
    } while (!complete);
}

/*
* FUNCTION      : getInt()
* DESCRIPTION   : Takes user input as a string. If successfully converted to an integer after newLine removal, returns conversion.
*                   If not, requests again 
* PARAMETERS    : None
* RETURNS       : int
*/
int getInt() {
    char response[100];
    char *lastChar;
    int value;
    bool complete = false;
    do {
        if (fgets(response, sizeof(response), stdin) == NULL) {
            printf("Error reading your input, try again.\n");
            clearInput();
            continue;
        }
        //trim newline and convert to int
        else{
            trimNewLine(response);
            if (strlen(response) != 0) {
                value = (int)strtol(response, &lastChar, 10);
                if (*lastChar == '\0') {
                    complete = true;
                    return value;
                }
            }
            clearInput();
        }
        printf("\nPlease enter an integer:");
    } while (!complete);
    return value;
}

/*
* FUNCTION      : getFloat()
* DESCRIPTION   : Takes user input as a string. If successfully converted to an float after newLine removal, returns conversion.
*                   If not, requests again
* PARAMETERS    : None
* RETURNS       : float
*/ 
float getFloat() {
    float cost;
    char response[100];
    char* lastChar;
    bool complete = false;
    printf("Please enter the unit price (rounding will be automated): ");
    do {
        if (fgets(response, sizeof(response), stdin) == NULL) {
            printf("Error reading your input, try again.\n");
            clearInput();
            continue;
        }
        //trim newline and convert to float
        else {
            trimNewLine(response);
            if (strlen(response) != 0) {
                cost = strtof(response, &lastChar);
                if (*lastChar == '\0') {
                    complete = true;
                    return cost;
                }
            }
        }
        printf("\nPlease enter a valid price: ");
        clearInput();
    } while (!complete);
    return cost;
}

/*
* FUNCTION      : getString(int buffer, char response[])
* DESCRIPTION   : Takes user input, and adjusts to ensure its no longer that length given by 'buffer'. Adjusts 'response' to reflect 
*                   users input or requests again if parameters are not satisfied.
* PARAMETERS    : int buffer - string length maximum, char response[] - destination char array
* RETURNS       : None
*/
void getString(int buffer, char response[]) {
    bool complete = false;
    do {
        //read input
        if (fgets(response, buffer, stdin) != NULL) {
            size_t len = strlen(response);
            //clears stdin if input is too long
            if (len > 0 && response[len - 1] != '\n') {
                clearInput();
                printf("Please limit your input to %d characters. Try again: ", buffer - 1);
                continue;
            }
            //clears newLine if short
            if (len > 0 && response[len - 1] == '\n') {
                response[len - 1] = '\0';
                complete = true;
            }
            if (strlen(response) == 0) {
                printf("Input cannot be blank. Please try again.\n");
                complete = false;
                continue;
            }
        }
    } while (!complete);
}

/*
* FUNCTION      : updateCat(int itemID)
* DESCRIPTION   : Takes an itemID and given filename. Gets new category from user and rewrites given file info into Temp File,
                    adjusting the category for the given itemID. Replaces given file with newly edited temp file (and renames)
* PARAMETERS    : int itemID
* RETURNS       : Bool - true if update is successful, false if not.
*/
bool updateCat(int itemID) {
    char items[] = "items.txt";
    bool updated = false;
    int lineItemID;
    char buffer[100]; 
    char line[100];
    char *lineID, *lineName, *lineCategory, *lineDescription, *linePrice;
    char newCat[100] = "";
    char tempFile[] = "temp.txt";
    FILE* pFile = fopen(items, "r");
    FILE* pTemp = fopen(tempFile, "w");

    //error if file doesn't open
    if (pFile == 0 || pTemp == 0) {
        perror("error opening file");
        return false;
    }

    while (fgets(buffer, sizeof(buffer), pFile)) {
        strcpy(line, buffer);
        lineID = strtok(buffer, " | ");
        lineName = strtok(NULL, " | ");
        lineCategory = strtok(NULL, " | ");
        lineDescription = strtok(NULL, " | ");
        linePrice = strtok(NULL, "\0");
        lineItemID = atoi(lineID);

        //if itemID found, prompts user for new inventory and prints adjusted information into temp(new) file
        if (lineItemID == itemID) {
            printf("Please enter the new category: ");
            getString(20, newCat);
            fprintf(pTemp, "%d | %s | %s | %s | %s\n", lineItemID, lineName, newCat, lineDescription, linePrice);
            updated = true;
        }
        else {
            fprintf(pTemp, line);
        }
    }
    fclose(pFile);
    fclose(pTemp);

    //replaces files
    remove(items);
    rename(tempFile, items);
    return updated;
}

/*
* FUNCTION      : updateCount(int itemID)
* DESCRIPTION   : Takes an itemID and given filename. Gets new inventory count from user and rewrites given file info into Temp File, 
                    adjusting the inventory count for the given itemID. Replaces given file with newly edited temp file (and renames)
* PARAMETERS    : int itemID
* RETURNS       : Bool - true if update is successful, false if not.
*/
bool updateCount(int itemID) {
    char itemFile[] = "Inventory.txt";
    char tempFile[] = "temp.txt";
    bool updated = false;
    char buffer[100];
    char line[100];
    char *lineID;
    int newCount;
    FILE* pFile = fopen(itemFile, "r");
    FILE* pTemp = fopen(tempFile, "w");
    
    //error if file doesn't open
    if (pFile == NULL || pTemp == NULL) {
        perror("error opening file");
    }
    while (fgets(buffer, sizeof(buffer), pFile)) {
        strcpy(line, buffer);
        lineID = strtok(buffer, " | ");
        int lineItemID = atoi(lineID);

        //if itemID found, prompts user for new inventory and prints adjusted information into temp(new) file
        if (lineItemID == itemID) {
            printf("Please enter the new inventory count: ");
            newCount = getInt();
            fprintf(pTemp, "%d | %d\n", lineItemID, newCount);
            updated = true;
        }
        else {
            fprintf(pTemp, line);
        }
    }
    fclose(pFile);
    fclose(pTemp);

    //replaces files.
    remove(itemFile);
    rename(tempFile, itemFile);
    return updated;
}

/*
* FUNCTION      : updateItem(int itemID)
* DESCRIPTION   : Takes an itemID and runs individual update functions pertaining to each database file. If each file update is successful,
*                   reports success to the user. If not, reports evidence of an error
* PARAMETERS    : int itemID
* RETURNS       : None
*/
void updateItem(int itemID) {
    char action[] = "Update";
    bool update1 = updateCat(itemID);
    bool update2 = updateCount(itemID);
    if (update1 && update2) {
        printf("Item has been successfully updated\n");
        logData(itemID, action);
    }
    else
    {
        printf("Item could not be updated in every file\n");
    }
    return;
}

/*
* FUNCTION      : readData(int itemID)
* DESCRIPTION   : Takes an itemID and runs search functions for existence in database files. If found in all files, displays all item information
*                   If not found at all, reports item non-existence. If found partially, offers further adjustment to conform database files.
* PARAMETERS    : int itemID
* RETURNS       : None
*/
void readData(int itemID) {
    char action[] = "Read";
    char toScreen[100];
    char inventory[] = "Inventory.txt";
    char items[] = "items.txt";
    bool invFound = false;
    bool stockFound = false;
    bool updateNow = false;

    sprintf(toScreen, "%d |", itemID);

    //search both files
    stockFound = searchFile(itemID, items, toScreen);
    strcat(toScreen, "|");
    invFound = searchFile(itemID, inventory, toScreen);
    
    if (!invFound && !stockFound) {
        printf("item doesn't exist, please write into database.\n");
        return;
    }

    //Found item somewhere, log the search
    logData(itemID, action);
    if (invFound && stockFound) {
        strcat(toScreen, "\n");
        printf(toScreen);
    }
    //Offer appropriate updating
    else if (invFound && !stockFound) {
        printf("Item not recorded in stock file.\n Would you like to delete and re-record the item? <Y>es / <N>o: ");
        updateNow = getBool();
        if (updateNow) {
            deleteItem(itemID);
            writeItem(itemID);
        }
    }
    else if (!invFound && stockFound) {
        printf("Item not recorded in inventory file.\n");
        printf("Would you like to Update the Item? <Y>es / <N>o: ");
        updateNow = getBool();
        if (updateNow) {
            updateItem(itemID);
        }
    }
    return;
}

/*
* FUNCTION      : deleteFromFile(char fileName[], int itemID)
* DESCRIPTION   : Takes an itemID and given filename. Rewrites file info into Temp File, 
                    abstaining from the given itemID. Replaces given file with newly edited temp file (and renames)
* RETURNS       : Bool - true if item found and deleted, false if item not found
*/
bool deleteFromFile(char fileName[], int itemID) {
    char tempFile[] = "TempFile.txt";
    char buffer[1000];
    char delimiter[] = " | ";
    int currentItem;
    char savedLine[200];
    bool found = false;

    FILE* pFile = fopen(fileName, "r");
    FILE* pTemp = fopen(tempFile, "w");

    if (pFile == NULL || pTemp == NULL) {
        perror("Error opening file");
        if (pFile != NULL) {
            fclose(pFile);
        }
        if (pTemp != NULL) {
            fclose(pTemp);
        }
        return found;
    }

    while (fgets(buffer, sizeof(buffer), pFile) != NULL) {
        strcpy(savedLine, buffer);
        currentItem = atoi(strtok(buffer, delimiter));
        if (currentItem == itemID) {
            found = true;
            continue;
        }
        else {
            fprintf(pTemp, savedLine);
        }
    }

    fclose(pTemp);
    fclose(pFile);

    remove(fileName);
    rename(tempFile, fileName);
    return found;
}

/*
* FUNCTION      : deleteItem(int itemID)
* DESCRIPTION   : Takes an itemID and attempts to delete its record from all database files. If itemID is found in a database
*                  logs its removal. If item doesn't exist, informs user. 
* PARAMETERS    : int itemID
* RETURNS       : None
*/
void deleteItem(int itemID) {
    bool log1 = false; 
    bool log2 = false;
    char inventory[] = "Inventory.txt";
    char items[] = "items.txt";
    char action[] = "Delete";

    //Try to delete item occurences.
    log1 = deleteFromFile(inventory, itemID);
    log2 = deleteFromFile(items, itemID);

    if (log1 or log2) {
        logData(itemID, action);
        printf("item has been removed from all sources");
    }
    else {
        printf("item never existed");
    }
    return;
}

/*
* FUNCTION      : logData(int itemID, char action[])
* DESCRIPTION   : Takes an ItemID and a given action (circumstantially, Read, Write, Delete or Update) and records both parameters 
*                   into a daily.log file 
* PARAMETERS    : int itemID, char action[]
* RETURNS       : None
*/
void logData(int itemID, char action[]) {
    FILE *logFile = fopen("Daily.log", "a");
    char logString[20];
    if (logFile == NULL) {
        perror("Error opening file");
        return;
    }
    else
    {
        sprintf(logString, "\n%d | ", itemID);
        strcat(logString, action);
        fprintf(logFile, logString);
        fclose(logFile);
        return;
    }
    fclose(logFile);
}
