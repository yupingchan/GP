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
// END OF PUBLIC COMMON AREA
// ==============================================


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
    // Write your R1 code here
}

// R2: Show all members
void showAllMemberAccounts() {
    // Write your R2 code here
}

// R3: Open / Close account
void openOrCloseAccount() {
    // Write your R3 code here
}

// R4: Member operations
void memberAccountOperations() {
    // Write your R4 code here
}

// R5: Generate daily statement
void generateDailyStatement() {
    // Write your R5 code here
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
            case 1: loadStartingData();            break;
            case 2: showAllMemberAccounts();        break;
            case 3: openOrCloseAccount();           break;
            case 4: memberAccountOperations();      break;
            case 5: generateDailyStatement();       break;
            case 6: creditsAndExit(); return 0;     break;
            default:
                cout << "Invalid option! Enter 1-6." << endl;
                break;
        }
    }
    return 0;
}
