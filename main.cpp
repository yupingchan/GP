// ==============================================
// SEHH2042 Frequent Flyer Program System
// CLEAN GROUP PROJECT FRAMEWORK
// This is only a skeleton.
// Each member fills their own R function.
// Public functions are READY to use.
// ==============================================

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <cctype>
#include <cstdlib>
#include <ctime>

using namespace std;

// ==============================================
// ================================
// PUBLIC COMMON AREA (SHARABLE)
// All people can use these.
// No need to rewrite.
// ================================
// ==============================================

// 1. Fixed values (shared by everyone)
const int MAX_MEMBERS = 100;
const int MAX_FLIGHTS = 200;
const string DEFAULT_DATE = "30-06-2025";

// 2. Classes (OOP Data Structure)
class Member {
public:
    string memberNumber;
    string tier;
    string passportNumber;
    int mrz;
    string name;
    int mileageBalance;
    bool isActive;

    Member() {
        mileageBalance = 0;
        isActive = true;
    }
};

class Flight {
public:
    string memberNumber;
    string origin;
    string destination;
    string flightNumber;
    string cabinClass;
    string departureDate;
    string creationDate;
    bool updated;

    Flight() {
        updated = false;
    }
};

// 3. Shared data
Member members[MAX_MEMBERS];
Flight flights[MAX_FLIGHTS];
int memberCount = 0;
int flightCount = 0;
string systemDate;
bool dataLoaded = false;

// 4. Public helper functions
bool isDateValid(string date) {
    if (date.length() != 10 || date[2] != '-' || date[5] != '-')
        return false;
    string year = date.substr(6, 4);
    return (year == "2025");
}

int calculateMRZ(string passport) {
    int weights[] = {7, 3, 1, 7, 3, 1, 7, 3, 1};
    int sum = 0;
    for (int i = 0; i < 9; i++) {
        char c = passport[i];
        int value;
        if (isalpha(c))
            value = toupper(c) - 'A' + 10;
        else
            value = c - '0';
        sum += value * weights[i];
    }
    return sum % 10;
}

string toUpper(string s) {
    for (int i = 0; i < s.size(); i++) {
        s[i] = toupper(s[i]);
    }
    return s;
}

bool yesNoPrompt(string message) {
    char input;
    while (true) {
        cout << message;
        cin >> input;
        input = toupper(input);
        if (input == 'Y') return true;
        if (input == 'N') return false;
        cout << "Please enter Y or N only." << endl;
    }
}

int findMemberByNumber(string num) {
    for (int i = 0; i < memberCount; i++) {
        if (members[i].memberNumber == num && members[i].isActive)
            return i;
    }
    return -1;
}

bool isPassportValid(string passport) {
    if (passport.length() != 9) return false;
    if (!isalpha(passport[0])) return false;
    for (int i = 1; i < 9; i++) {
        if (!isdigit(passport[i]))
            return false;
    }
    return true;
}

string getFlightOrigin(string fn) {
    if (fn == "CC81" || fn == "CC31" || fn == "CC32") return "Hong Kong";
    if (fn == "CC82" || fn == "CC61") return "London";
    if (fn == "CC62") return "Dubai";
    return "";
}

string getFlightDest(string fn) {
    if (fn == "CC81") return "London";
    if (fn == "CC82") return "Hong Kong";
    if (fn == "CC31") return "Dubai";
    if (fn == "CC32") return "Hong Kong";
    if (fn == "CC61") return "Dubai";
    if (fn == "CC62") return "London";
    return "";
}

int getBaseMileage(string o, string d, string cabin) {
    if ((o == "Hong Kong" && d == "London") || (o == "London" && d == "Hong Kong")) {
        if (cabin == "Economy") return 4000;
        if (cabin == "Business") return 8000;
        if (cabin == "First") return 16000;
    }
    if ((o == "Hong Kong" && d == "Dubai") || (o == "Dubai" && d == "Hong Kong")) {
        if (cabin == "Economy") return 2000;
        if (cabin == "Business") return 4000;
        if (cabin == "First") return 8000;
    }
    if ((o == "London" && d == "Dubai") || (o == "Dubai" && d == "London")) {
        if (cabin == "Economy") return 2000;
        if (cabin == "Business") return 4000;
        if (cabin == "First") return 8000;
    }
    return 0;
}

double getBonusPercent(string tier) {
    if (tier == "Green")   return 0.00;
    if (tier == "Silver")  return 0.02;
    if (tier == "Gold")    return 0.04;
    if (tier == "Diamond") return 0.06;
    return 0.00;
}



// ==============================================
// R0 ~ R6 FUNCTIONS (EMPTY FRAMEWORK)
// Each member writes code inside their own function.
// ==============================================

// R0: Show main menu
void showMainMenu() {
    cout << "\n*** FFP Main Menu ***" << endl;
    cout << "[1] Load Starting Data" << endl;
    cout << "[2] Show All Member Accounts" << endl;
    cout << "[3] Open or Close Member Account" << endl;
    cout << "[4] Member Account Operations" << endl;
    cout << "[5] Generate Daily Statement" << endl;
    cout << "[6] Credits and Exit" << endl;
    cout << "Option (1 - 6): ";
}

// R1: Load starting data
void loadStartingData() {

    // ---------------------------
    // STEP 1: Load MEMBER data
    // ---------------------------
    memberCount = 4;

    // Member 1
    members[0].memberNumber = "M001";
    members[0].name = "Chan Tai Man";
    members[0].tier = "Gold";
    members[0].passportNumber = "A12345678";
    members[0].mrz = calculateMRZ("A12345678");
    members[0].mileageBalance = 5000;
    members[0].isActive = true;

    // Member 2
    members[1].memberNumber = "M002";
    members[1].name = "Li Siu Hong";
    members[1].tier = "Silver";
    members[1].passportNumber = "B87654321";
    members[1].mrz = calculateMRZ("B87654321");
    members[1].mileageBalance = 2000;
    members[1].isActive = true;

    // Member 3
    members[2].memberNumber = "M003";
    members[2].name = "Wong Wing Yan";
    members[2].tier = "Green";
    members[2].passportNumber = "C11223344";
    members[2].mrz = calculateMRZ("C11223344");
    members[2].mileageBalance = 500;
    members[2].isActive = true;

    // Member 4
    members[3].memberNumber = "M004";
    members[3].name = "Cheung Hoi Ting";
    members[3].tier = "Diamond";
    members[3].passportNumber = "D99887766";
    members[3].mrz = calculateMRZ("D99887766");
    members[3].mileageBalance = 10000;
    members[3].isActive = true;

    // ---------------------------
    // STEP 2: Load FLIGHT data
    // ---------------------------
    flightCount = 2;

    // Flight 1
    flights[0].memberNumber = "M001";
    flights[0].flightNumber = "CC81";
    flights[0].origin = "Hong Kong";
    flights[0].destination = "London";
    flights[0].cabinClass = "Business";
    flights[0].departureDate = "01-07-2025";
    flights[0].creationDate = systemDate;
    flights[0].updated = false;

    // Flight 2
    flights[1].memberNumber = "M002";
    flights[1].flightNumber = "CC31";
    flights[1].origin = "Hong Kong";
    flights[1].destination = "Dubai";
    flights[1].cabinClass = "Economy";
    flights[1].departureDate = "15-07-2025";
    flights[1].creationDate = systemDate;
    flights[1].updated = false;

    // ---------------------------
    // STEP 3: Set system date
    // ---------------------------
    systemDate = "01-06-2025";

    // ---------------------------
    // STEP 4: MARK DATA AS LOADED
    // ---------------------------
    dataLoaded = true;

    cout << "\nData loaded successfully!\n";
    cout << "Members loaded: " << memberCount << endl;
    cout << "Flights loaded: " << flightCount << endl;
    cout << "System date: " << systemDate << "\n";
}


// R2: Show all members
void showAllMemberAccounts() {
    cout << "\n=== Member Account Records ===\n";

    // 1. Member sorting (in ascending order of surname)
    for (int i = 0; i < memberCount - 1; i++) {
        for (int j = i + 1; j < memberCount; j++) {
            if (members[i].name > members[j].name) {
                Member temp = members[i];
                members[i] = members[j];
                members[j] = temp;
            }
        }
    }

    // 2. Display member table
    cout << left << setw(12) << "MemberNo"
         << setw(8)  << "Tier"
         << setw(15) << "Passport"
         << setw(5)  << "MRZ"
         << setw(20) << "Name"
         << setw(10) << "Mileage" << endl;
    cout << string(70, '-') << endl;

    for (int i = 0; i < memberCount; i++) {
        if (members[i].isActive) {
            cout << left << setw(12) << members[i].memberNumber
                 << setw(8)  << members[i].tier
                 << setw(15) << members[i].passportNumber
                 << setw(5)  << members[i].mrz
                 << setw(20) << members[i].name
                 << setw(10) << members[i].mileageBalance
                 << endl;
        }
    }

    // 3. Filter flight records (only show updated == false)
    Flight unupdated[MAX_FLIGHTS];
    int unupdatedCount = 0;
    for (int i = 0; i < flightCount; i++) {
        if (!flights[i].updated) {
            unupdated[unupdatedCount++] = flights[i];
        }
    }

    // 4. Flights sorted (in descending order of departure date)
    for (int i = 0; i < unupdatedCount - 1; i++) {
        for (int j = i + 1; j < unupdatedCount; j++) {
            if (unupdated[i].departureDate < unupdated[j].departureDate) {
                Flight temp = unupdated[i];
                unupdated[i] = unupdated[j];
                unupdated[j] = temp;
            }
        }
    }

    // 5. Display flight table
    cout << "\n=== Flight Records (Unupdated) ===\n";
    cout << left << setw(12) << "MemberNo"
         << setw(12) << "Origin"
         << setw(12) << "Destination"
         << setw(10) << "FlightNo"
         << setw(10) << "Cabin"
         << setw(12) << "Departure"
         << setw(12) << "Creation"
         << setw(8)  << "Updated" << endl;
    cout << string(90, '-') << endl;

    for (int i = 0; i < unupdatedCount; i++) {
        cout << left << setw(12) << unupdated[i].memberNumber
             << setw(12) << unupdated[i].origin
             << setw(12) << unupdated[i].destination
             << setw(10) << unupdated[i].flightNumber
             << setw(10) << unupdated[i].cabinClass
             << setw(12) << unupdated[i].departureDate
             << setw(12) << unupdated[i].creationDate
             << setw(8)  << (unupdated[i].updated ? "1" : "0")
             << endl;
    }

    cout << "\nReturning to Main Menu...\n";
}

// R3: Open / Close account
void openOrCloseAccount() {
    //enter number
    string memNum;
    cout << "\nEnter Member Number: ";
    cin >> memNum;
    cin.clear();
    cin.ignore(10000, '\n');
    //open acount R3.1
    if (findMemberByNumber(memNum) == -1) {
        //check full or not 
        if (memberCount >= MAX_MEMBERS) {
            cout << "System full! Cannot add more members." << endl;
            return;
        }
        //open acount
        cout << "\n--- Open New Member Account ---" << endl;
        int attemptCount = 0;
        bool nameSuccess = false, passportSuccess = false, tierSuccess = false;
        string newName, newPassport, newTier;
        //main logic
        while (attemptCount < 3) {
            bool stepFailed = false;
            //name 
            if (!nameSuccess) {
                cout << "Please input your name: ";
                getline(cin, newName);
                //check lenght
                if (newName.length() > 30 || newName.length() == 0) {
                    cout << "\nYour name have to be between 1-30" << endl;
                    stepFailed = true;
                }
                else {
                    //locate surname
                    int space = newName.length();
                    for (int i = 0; i < newName.length(); i++) {
                        if (newName[i] == ' ') {
                            space = i;
                            break;
                        }
                    }
                    //trun to capital letter with surname
                    string surname = newName.substr(0, space);
                    string restOfTheName = newName.substr(space);
                    surname = toUpper(surname);
                    newName = surname + restOfTheName;
                    nameSuccess = true;
                }
            }
            //passport number
            if (!stepFailed && !passportSuccess) {
                cout << "please input your passport number(e.g H12345678): ";
                cin >> newPassport;
                cin.clear();
                cin.ignore(10000, '\n');
                //check format
                if (!isPassportValid(newPassport)) {
                    cout << "\nYour passport number wrong" << endl;
                    stepFailed = true;
                }
                else {
                    passportSuccess = true; 
                }
            }
            //member tier
            if (!stepFailed && !tierSuccess) {
                cout << "Please input Member Tier (Green, Silver, Gold, Diamond): ";
                cin >> newTier;
                cin.clear();
                cin.ignore(10000, '\n');
                //enter tier
                if (newTier == "Green" || newTier == "Silver" || newTier == "Gold" || newTier == "Diamond") {
                    tierSuccess = true;
                }
                else {
                    cout << "Invalid tier selected." << endl;
                    stepFailed = true;
                }

            }
            if (!stepFailed) {
                break;
            }
            else {
                cout << "please try again you remain " << 2 - attemptCount << " to try" << endl;
                attemptCount++;
            }
        }
        //if too many error input retrun main meun
        if (!nameSuccess || !passportSuccess || !tierSuccess) {
            cout << "Too many invalid attempts. Returning to Main Menu." << endl;
            return;
        }
        //generate mrz,id
        cout << "Information verified successfully!" << endl;
        int mrzCode = calculateMRZ(newPassport);
        string newMemberID = "2026" + to_string(rand() % 90000 + 10000);
        //save data
        members[memberCount].memberNumber = newMemberID;
        members[memberCount].name = newName;
        members[memberCount].passportNumber = newPassport;
        members[memberCount].tier = newTier;
        members[memberCount].mrz = mrzCode;
        members[memberCount].mileageBalance = 0;   
        members[memberCount].isActive = true;
        memberCount++;
        cout << "Account created successfully! Your Member Number is: " << newMemberID << endl;
    }
    //delete acount R3.2
    else {
        //print data
        int idx = findMemberByNumber(memNum);
        cout << "\n--- Member Information ---" << endl;
        cout << "Member Number : " << members[idx].memberNumber << endl;
        cout << "Name          : " << members[idx].name << endl;
        cout << "Passport      : " << members[idx].passportNumber << endl;
        cout << "Tier          : " << members[idx].tier << endl;
        cout << "MRZ           : " << members[idx].mrz << endl;
        cout << "Mileage       : " << members[idx].mileageBalance << endl;
        if (yesNoPrompt("Please enter (Y/y) to confirm delete your account or enter (N/n) to cancel: ")) {
            //detel flight
            for (int i = 0; i < flightCount; ) {
                if (flights[i].memberNumber == memNum) {
                    for (int j = i; j < flightCount - 1; j++) {
                        flights[j] = flights[j + 1];
                    }
                    flightCount--; 
                }
                else {
                    i++;
                }
            }
            //move mumber id
            for (int i = idx; i < memberCount - 1; i++) {
                members[i] = members[i + 1]; 
            }
            memberCount--;
            cout << "Account successfully deleted.Returning to menu." << endl;
            return;
        }
        else {
            //how to retrun
            cout << "Account deletion cancelled. Returning to menu." << endl;
            return;
        }
    }
}


// R4: Member operations
void memberAccountOperations() {

    // Ask user to enter a Member Number
    string memNum;
    cout << "\nEnter Member Number: ";
    cin >> memNum;

    // Check if this member exists and is active
    int idx = findMemberByNumber(memNum);
    if (idx == -1) {
        cout << "Member not found or inactive." << endl;
        return;
    }

    // Submenu loop for R4
    while (true) {
        // Show R4 submenu
        cout << "\n----- Member Account Operations -----" << endl;
        cout << "[1] Edit Member Information" << endl;
        cout << "[2] Update Mileage Points Balance" << endl;
        cout << "[3] Create Flight Records" << endl;
        cout << "[4] Redeem Mileage Points & Transfer" << endl;
        cout << "[5] Return to Main Menu" << endl;
        cout << "Option: ";

        int op;
        cin >> op;

        // ------------------------------------------------------
        // R4.1 Edit Member Information
        // ------------------------------------------------------
        if (op == 1) {
            cout << "\n--- Current Member Information ---" << endl;
                cout << "Member Number : " << members[idx].memberNumber << endl;
                cout << "Name          : " << members[idx].name << endl;
                cout << "Passport      : " << members[idx].passportNumber << endl;
                cout << "Tier          : " << members[idx].tier << endl;
                cout << "MRZ           : " << members[idx].mrz << endl;
                cout << "Mileage       : " << members[idx].mileageBalance << endl;
            cout << "\n--- Edit Member Info ---" << endl;

            string newName, newPassport, newTier;

            cout << "Enter new name: ";
            cin.ignore();
            getline(cin, newName);

            cout << "Enter new passport: ";
            cin >> newPassport;

            cout << "Enter new tier: ";
            cin >> newTier;

            // Ask for confirmation
            if (yesNoPrompt("Confirm update? (Y/N): ")) {
                members[idx].name = newName;
                members[idx].passportNumber = newPassport;
                members[idx].tier = newTier;
                members[idx].mrz = calculateMRZ(newPassport);

                cout << "Member information updated successfully." << endl;
                cout << "\n=== Updated Member Information ===" << endl;
                        cout << "Member Number : " << members[idx].memberNumber << endl;
                        cout << "Name          : " << members[idx].name << endl;
                        cout << "Passport      : " << members[idx].passportNumber << endl;
                        cout << "Tier          : " << members[idx].tier << endl;
                        cout << "MRZ           : " << members[idx].mrz << endl;
                        cout << "Mileage       : " << members[idx].mileageBalance << endl;
                        cout << "===================================" << endl;
            }
        }

        // ------------------------------------------------------
        // R4.2 Update Mileage Points
        // ------------------------------------------------------
        else if (op == 2) {
            cout << "\n--- Updating Mileage Points ---" << endl;

            int totalEarned = 0;

            // Check all flights for this member
            for (int i = 0; i < flightCount; i++) {
                if (flights[i].memberNumber == memNum &&
                    flights[i].updated == false &&
                    flights[i].departureDate <= systemDate) {

                    // Calculate base points and bonus
                    int base = getBaseMileage(flights[i].origin,
                                             flights[i].destination,
                                             flights[i].cabinClass);

                    double bonusRate = getBonusPercent(members[idx].tier);
                    int addPoints = base * (1 + bonusRate);

                    totalEarned += addPoints;
                    flights[i].updated = true;
                }
            }

            // Add points to member balance
            members[idx].mileageBalance += totalEarned;

            cout << "Total points earned: " << totalEarned << endl;
            cout << "New balance: " << members[idx].mileageBalance << endl;
        }

        // ------------------------------------------------------
        // R4.3 Create Flight Record
        // ------------------------------------------------------
        else if (op == 3) {
            cout << "\n--- Create New Flight ---" << endl;

            string flightNum, cabin, depDate;

            cout << "Enter Flight Number (e.g., CC81): ";
            cin >> flightNum;

            cout << "Enter Cabin Class (Economy/Business/First): ";
            cin >> cabin;

            cout << "Enter Departure Date (DD-MM-YYYY): ";
            cin >> depDate;

            // Get origin & destination automatically
            string ori = getFlightOrigin(flightNum);
            string dest = getFlightDest(flightNum);

            if (ori.empty() || dest.empty()) {
                cout << "Invalid flight number." << endl;
                continue;
            }

            // Confirm before adding
            if (yesNoPrompt("Add this flight? (Y/N): ")) {
                flights[flightCount].memberNumber = memNum;
                flights[flightCount].origin = ori;
                flights[flightCount].destination = dest;
                flights[flightCount].flightNumber = flightNum;
                flights[flightCount].cabinClass = cabin;
                flights[flightCount].departureDate = depDate;
                flights[flightCount].creationDate = systemDate;
                flights[flightCount].updated = false;

                flightCount++;
                cout << "Flight record created successfully." << endl;
            }
        }

        // ------------------------------------------------------
        // R4.4 Redeem Gift OR Transfer Points
        // ------------------------------------------------------
        else if (op == 4) {
            cout << "\n[1] Redeem Gift" << endl;
            cout << "[2] Transfer Points" << endl;
            cout << "Option: ";
            int subOp;
            cin >> subOp;

            // Redeem
            if (subOp == 1) {
                cout << "\n--- Redeem Mileage Points ---" << endl;
                cout << "1. Movie voucher      (3000 points)" << endl;
                cout << "2. $100 Super voucher (4000 points)" << endl;
                cout << "3. Lounge access      (6000 points)" << endl;
                cout << "Select gift: ";

                int gift;
                cin >> gift;
                int cost = 0;

                if (gift == 1) cost = 3000;
                else if (gift == 2) cost = 4000;
                else if (gift == 3) cost = 6000;

                if (members[idx].mileageBalance >= cost) {
                    members[idx].mileageBalance -= cost;
                    cout << "Redeem successful! New balance: "
                         << members[idx].mileageBalance << endl;
                } else {
                    cout << "Insufficient points." << endl;
                }
            }

            // Transfer
            else if (subOp == 2) {
                cout << "\n--- Transfer Points ---" << endl;

                string targetMemNum;
                int amount;

                cout << "Enter target member number: ";
                cin >> targetMemNum;
                cout << "Enter points to transfer: ";
                cin >> amount;

                int targetIdx = findMemberByNumber(targetMemNum);

                if (targetIdx == -1 || amount <= 0 ||
                    members[idx].mileageBalance < amount) {
                    cout << "Transfer failed. Check balance or member ID." << endl;
                    continue;
                }

                if (yesNoPrompt("Confirm transfer to "
                        + members[targetIdx].name + "? (Y/N): ")) {
                    members[idx].mileageBalance -= amount;
                    members[targetIdx].mileageBalance += amount;
                    cout << "Transfer successful!" << endl;
                }
            }
        }

        // ------------------------------------------------------
        // R4.5 Back to Main Menu
        // ------------------------------------------------------
        else if (op == 5) {
            cout << "Returning to Main Menu..." << endl;
            break;
        }
        else {
            cout << "Invalid option! Please try again." << endl;
        }
    }
}
// R5: Generate daily statement
    string memNum;
    cout << "\nEnter Member Number: ";
    cin >> memNum;

    int idx = findMemberByNumber(memNum);
    if (idx == -1) {
        cout << "Error: Member not found or inactive." << endl;
        return;
    }

    cout << "\n=== Daily Statement ===" << endl;
    cout << "Member Name   : " << members[idx].name << endl;
    cout << "Member Number : " << members[idx].memberNumber << endl;
    cout << "Statement Date: " << systemDate << endl;
    cout << "------------------------------------------------------------" << endl;

    // Transaction Summary (simple demo: show redemption if any)
    cout << "Transaction Summary:" << endl;
    cout << left << setw(12) << "Type"
         << setw(10) << "Mileage"
         << setw(20) << "Description" << endl;

    // 暫時冇交易紀錄，可以擴展到 R4 Redeem Gift
    cout << "No transactions recorded." << endl;
    cout << "------------------------------------------------------------" << endl;

    // Upcoming Itinerary
    cout << "Upcoming Itinerary:" << endl;
    cout << left << setw(12) << "Origin"
         << setw(12) << "Destination"
         << setw(12) << "Flight No"
         << setw(12) << "Cabin"
         << setw(12) << "Departure" << endl;

    bool hasUpcoming = false;
    for (int i = 0; i < flightCount; i++) {
        if (flights[i].memberNumber == memNum &&
            !flights[i].updated &&
            flights[i].departureDate > systemDate) {
            cout << left << setw(12) << flights[i].origin
                 << setw(12) << flights[i].destination
                 << setw(12) << flights[i].flightNumber
                 << setw(12) << flights[i].cabinClass
                 << setw(12) << flights[i].departureDate << endl;
            hasUpcoming = true;
        }
    }
    if (!hasUpcoming) {
        cout << "No upcoming flights." << endl;
    }
    cout << "------------------------------------------------------------" << endl;

    // Member Account Summary
    cout << "Member Account Summary:" << endl;
    cout << "Total Mileage Points Balance : " << members[idx].mileageBalance << endl;
    cout << "Member Tier                  : " << members[idx].tier << endl;
    cout << "Bonus Mileage Points         : " << (getBonusPercent(members[idx].tier) * 100) << "%" << endl;

    cout << "\nReturning to Main Menu...\n";
}


// R6: Credits and exit
void creditsAndExit() {
    // Write your R6 code here
}

// ==============================================
// MAIN FUNCTION (PROGRAM STARTS HERE)
// ==============================================
int main() {
    srand((unsigned int)time(NULL));

    cout << "=========================================" << endl;
    cout << "  Frequent Flyer Program System (FFP)" << endl;
    cout << "=========================================" << endl;

    while (true) {
        showMainMenu();
        int option;
        cin >> option;

        switch (option) {
            case 1:
                loadStartingData();
                break;

            case 2:
                if (dataLoaded)
                    showAllMemberAccounts();
                else
                    cout << "Error: Please load data first (Option 1)!" << endl;
                break;

            case 3:
                if (dataLoaded)
                    openOrCloseAccount();
                else
                    cout << "Error: Please load data first (Option 1)!" << endl;
                break;

            case 4:
                if (dataLoaded)
                    memberAccountOperations();
                else
                    cout << "Error: Please load data first (Option 1)!" << endl;
                break;

            case 5:
                if (dataLoaded)
                    generateDailyStatement();
                else
                    cout << "Error: Please load data first (Option 1)!" << endl;
                break;

            case 6:
                creditsAndExit();
                return 0;
                break;

            default:
                cout << "Invalid option! Enter 1-6." << endl;
                break;
        }
    }
    return 0;
}
