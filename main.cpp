// ==============================================
// SEHH2042 Frequent Flyer Program System
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
// PUBLIC CONSTANTS
// ==============================================
const int MAX_MEMBERS = 100;
const int MAX_FLIGHTS = 200;
const int MAX_TRANSACTIONS = 500;
const string DEFAULT_DATE = "30-06-2025";
const string DATE_MIN = "01-01-2025";
const string DATE_MAX = "31-12-2025";

// ==============================================
// CLASSES (OOP DESIGN)
// ==============================================
struct Transaction {
    string memberNumber;
    string type;
    int mileage;
    string description;
};

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

// ==============================================
// GLOBAL DATA
// ==============================================
Member members[MAX_MEMBERS];
Flight flights[MAX_FLIGHTS];
Transaction transactions[MAX_TRANSACTIONS];
int memberCount = 0;
int flightCount = 0;
int transCount = 0;
string systemDate;
bool dataLoaded = false;

// ==============================================
// HELPER FUNCTIONS
// ==============================================
void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void clearInput() {
    cin.clear();
    cin.ignore(10000, '\n');
}

string toUpper(string s) {
    for (int i = 0; i < s.size(); i++) s[i] = toupper(s[i]);
    return s;
}

string formatName(string fullName) {
    size_t space = fullName.find(' ');
    if (space == string::npos) return fullName;
    string surname = toUpper(fullName.substr(0, space));
    string given = fullName.substr(space);
    return surname + given;
}

bool yesNoPrompt(string msg) {
    char c;
    while (true) {
        cout << msg;
        cin >> c;
        clearInput();
        c = toupper(c);
        if (c == 'Y') return true;
        if (c == 'N') return false;
        cout << "Invalid input! Please enter Y/N only." << endl;
    }
}

int findMemberByNumber(string num) {
    for (int i = 0; i < memberCount; i++)
        if (members[i].memberNumber == num && members[i].isActive)
            return i;
    return -1;
}

bool isPassportDuplicate(string p) {
    for (int i = 0; i < memberCount; i++)
        if (members[i].passportNumber == p && members[i].isActive)
            return true;
    return false;
}

bool isPassportValid(string p) {
    if (p.size() != 9) return false;
    if (!isalpha(p[0])) return false;
    for (int i = 1; i < 9; i++) if (!isdigit(p[i])) return false;
    return true;
}

int calculateMRZ(string passport) {
    int weights[] = {7, 3, 1, 7, 3, 1, 7, 3, 1};
    int sum = 0;
    for (int i = 0; i < 9; i++) {
        char c = passport[i];
        int val = isalpha(c) ? (toupper(c) - 'A' + 10) : (c - '0');
        sum += val * weights[i];
    }
    return sum % 10;
}

bool isValidTier(string t) {
    return (t == "Green" || t == "Silver" || t == "Gold" || t == "Diamond");
}

bool isValidFlightNumber(string fn) {
    return (fn == "CC81" || fn == "CC82" || fn == "CC31" || fn == "CC32" || fn == "CC61" || fn == "CC62");
}

bool isValidCabin(string c) {
    return (c == "Economy" || c == "Business" || c == "First");
}

string getFlightOrigin(string fn) {
    if (fn == "CC81") return "Hong Kong";
    if (fn == "CC82") return "London";
    if (fn == "CC31") return "Hong Kong";
    if (fn == "CC32") return "Dubai";
    if (fn == "CC61") return "London";
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
    if (tier == "Green") return 0.00;
    if (tier == "Silver") return 0.02;
    if (tier == "Gold") return 0.04;
    if (tier == "Diamond") return 0.06;
    return 0.00;
}

bool isDateValidFormat(string date) {
    if (date.size() != 10) return false;
    if (date[2] != '-' || date[5] != '-') return false;
    for (int i = 0; i < 10; i++) {
        if (i == 2 || i == 5) continue;
        if (!isdigit(date[i])) return false;
    }
    int day = stoi(date.substr(0, 2));
    int month = stoi(date.substr(3, 2));
    int year = stoi(date.substr(6, 4));
    if (year != 2025) return false;
    if (month < 1 || month > 12) return false;
    int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    return day >= 1 && day <= daysInMonth[month - 1];
}

bool isDateBeforeOrEqual(string d1, string d2) {
    int y1 = stoi(d1.substr(6, 4)), m1 = stoi(d1.substr(3, 2)), d1Num = stoi(d1.substr(0, 2));
    int y2 = stoi(d2.substr(6, 4)), m2 = stoi(d2.substr(3, 2)), d2Num = stoi(d2.substr(0, 2));
    if (y1 < y2) return true;
    if (y1 > y2) return false;
    if (m1 < m2) return true;
    if (m1 > m2) return false;
    return d1Num <= d2Num;
}

bool isDateAfter(string d1, string d2) {
    return !isDateBeforeOrEqual(d1, d2);
}

bool isDateInRange(string date) {
    return isDateBeforeOrEqual(DATE_MIN, date) && isDateBeforeOrEqual(date, DATE_MAX);
}

string getSystemYear() {
    return systemDate.substr(6, 4);
}

bool isInteger(string s) {
    for (char c : s) if (!isdigit(c)) return false;
    return true;
}

// ==============================================
// R0: WELCOME & MAIN MENU
// ==============================================
void showWelcome() {
    cout << "==============================================" << endl;
    cout << "     WELCOME TO FREQUENT FLYER PROGRAM SYSTEM     " << endl;
    cout << "==============================================" << endl;
}

void showMainMenu() {
    cout << "\n=== FFP Main Menu ===" << endl;
    cout << "[1] Load Starting Data" << endl;
    cout << "[2] Show All Member Accounts" << endl;
    cout << "[3] Open or Close Member Account" << endl;
    cout << "[4] Member Account Operations" << endl;
    cout << "[5] Generate Daily Statement" << endl;
    cout << "[6] Credits and Exit" << endl;
    cout << "================================" << endl;
    cout << "Option (1 - 6): ";
}

// ==============================================
// R1: LOAD STARTING DATA + R1.2 SET SYSTEM DATE
// ==============================================
void loadStartingData() {
    clearScreen();
    memberCount = 4;
    flightCount = 6;
    transCount = 0;

    members[0] = Member();
    members[0].memberNumber = "202456734";
    members[0].tier = "Gold";
    members[0].passportNumber = "A56677890";
    members[0].mrz = calculateMRZ("A56677890");
    members[0].name = "WONG Claire";
    members[0].mileageBalance = 45000;
    members[0].isActive = true;

    members[1] = Member();
    members[1].memberNumber = "202333890";
    members[1].tier = "Green";
    members[1].passportNumber = "C78678908";
    members[1].mrz = calculateMRZ("C78678908");
    members[1].name = "MA Kathy";
    members[1].mileageBalance = 10000;
    members[1].isActive = true;

    members[2] = Member();
    members[2].memberNumber = "202067856";
    members[2].tier = "Silver";
    members[2].passportNumber = "E38876890";
    members[2].mrz = calculateMRZ("E38876890");
    members[2].name = "CHAN Peter";
    members[2].mileageBalance = 53200;
    members[2].isActive = true;

    members[3] = Member();
    members[3].memberNumber = "202211843";
    members[3].tier = "Gold";
    members[3].passportNumber = "E38900078";
    members[3].mrz = calculateMRZ("E38900078");
    members[3].name = "CHEUNG Alice";
    members[3].mileageBalance = 30000;
    members[3].isActive = true;

    flights[0] = Flight(); flights[0].memberNumber="202211843"; flights[0].origin="Hong Kong"; flights[0].destination="London"; flights[0].flightNumber="CC81"; flights[0].cabinClass="First"; flights[0].departureDate="28-05-2025"; flights[0].creationDate="01-05-2025"; flights[0].updated=false;
    flights[1] = Flight(); flights[1].memberNumber="202211843"; flights[1].origin="London"; flights[1].destination="Hong Kong"; flights[1].flightNumber="CC82"; flights[1].cabinClass="First"; flights[1].departureDate="10-06-2025"; flights[1].creationDate="01-05-2025"; flights[1].updated=false;
    flights[2] = Flight(); flights[2].memberNumber="202333890"; flights[2].origin="London"; flights[2].destination="Dubai"; flights[2].flightNumber="CC61"; flights[2].cabinClass="Economy"; flights[2].departureDate="12-06-2025"; flights[2].creationDate="10-06-2025"; flights[2].updated=false;
    flights[3] = Flight(); flights[3].memberNumber="202067856"; flights[3].origin="Hong Kong"; flights[3].destination="Dubai"; flights[3].flightNumber="CC31"; flights[3].cabinClass="Business"; flights[3].departureDate="05-07-2025"; flights[3].creationDate="20-06-2025"; flights[3].updated=false;
    flights[4] = Flight(); flights[4].memberNumber="202067856"; flights[4].origin="Dubai"; flights[4].destination="London"; flights[4].flightNumber="CC62"; flights[4].cabinClass="Business"; flights[4].departureDate="08-07-2025"; flights[4].creationDate="20-06-2025"; flights[4].updated=false;
    flights[5] = Flight(); flights[5].memberNumber="202456734"; flights[5].origin="Dubai"; flights[5].destination="Hong Kong"; flights[5].flightNumber="CC32"; flights[5].cabinClass="Business"; flights[5].departureDate="05-08-2025"; flights[5].creationDate="02-08-2025"; flights[5].updated=false;

    cout << "Starting data loaded successfully!" << endl;
    cout << "\n=== Set System Date for updating Mileage Points ===" << endl;
    int err = 0;
    string inputDate;
    bool ok = false;

    while (err < 3) {
        cout << "Please enter system date (DD-MM-YYYY): ";
        cin >> inputDate;
        clearInput();
        if (isDateValidFormat(inputDate) && isDateInRange(inputDate)) {
            systemDate = inputDate;
            ok = true;
            break;
        }
        err++;
        cout << "Invalid format/range! Attempt " << err << "/3" << endl;
    }
    if (!ok) {
        systemDate = DEFAULT_DATE;
        cout << "3 invalid inputs! Using default date: " << systemDate << endl;
    }
    dataLoaded = true;
    cout << "System date set to: " << systemDate << endl;
}

// ==============================================
// R2: SHOW ALL MEMBERS & FLIGHTS (ADD Creation Date)
// ==============================================
void showAllMemberAccounts() {
    clearScreen();
    cout << "\n[Member Account Records] (Sorted by Name ASC)" << endl;
    vector<Member> mList;
    for (int i = 0; i < memberCount; i++) if (members[i].isActive) mList.push_back(members[i]);
    sort(mList.begin(), mList.end(), [](Member a, Member b) { return a.name < b.name; });

    cout << left << setw(12) << "MemberNo" << setw(8) << "Tier" << setw(15) << "Passport"
         << setw(5) << "MRZ" << setw(20) << "Name" << setw(10) << "Mileage" << endl;
    cout << string(70, '-') << endl;
    for (Member m : mList) {
        cout << left << setw(12) << m.memberNumber << setw(8) << m.tier << setw(15) << m.passportNumber
             << setw(5) << m.mrz << setw(20) << m.name << setw(10) << m.mileageBalance << endl;
    }

    cout << "\n[Unupdated Flight Records] (Sorted by Departure DESC)" << endl;
    vector<Flight> fList;
    for (int i = 0; i < flightCount; i++) if (!flights[i].updated) fList.push_back(flights[i]);
    sort(fList.begin(), fList.end(), [](Flight a, Flight b) { return isDateAfter(a.departureDate, b.departureDate); });

    cout << left << setw(12) << "MemberNo" << setw(12) << "Origin" << setw(12) << "Dest"
         << setw(10) << "Flight" << setw(10) << "Cabin" << setw(12) << "Departure"
         << setw(12) << "CreationDate" << setw(8) << "Updated" << endl;
    cout << string(92, '-') << endl;
    for (Flight f : fList) {
        cout << left << setw(12) << f.memberNumber << setw(12) << f.origin << setw(12) << f.destination
             << setw(10) << f.flightNumber << setw(10) << f.cabinClass << setw(12) << f.departureDate
             << setw(12) << f.creationDate << setw(8) << "0" << endl;
    }
    cout << "\nPress Enter to continue...";
    cin.get();
}

// ==============================================
// R3: OPEN / CLOSE ACCOUNT (FULL FIX)
// ==============================================
void openOrCloseAccount() {
    clearScreen();
    string memNum;
    cout << "Enter Member Number: ";
    cin >> memNum;
    clearInput();
    int idx = findMemberByNumber(memNum);

    if (idx == -1) {
        cout << "\n--- OPEN NEW MEMBER ACCOUNT ---" << endl;
        if (memberCount >= MAX_MEMBERS) { cout << "Member limit reached!" << endl; return; }
        string name, passport, tier;
        bool pass = false;
        int attempt = 0;

        while (attempt < 3) {
            pass = true;
            cout << "Enter Member Name (max 30 chars): ";
            getline(cin, name);
            if (name.empty() || name.size() > 30) { cout << "Name invalid/too long!" << endl; pass = false; }

            if (pass) {
                cout << "Enter Passport Number (1 uppercase letter + 8 digits, e.g., A12345678): ";
                cin >> passport;
                if (!isPassportValid(passport) || isPassportDuplicate(passport)) {
                    cout << "Invalid/duplicate passport!" << endl; pass = false;
                }
            }
            if (pass) {
                cout << "Enter Tier (Green/Silver/Gold/Diamond): ";
                cin >> tier;
                if (!isValidTier(tier)) pass = false;
            }
            if (pass) break;
            attempt++;
            cout << "Error! Attempt " << attempt << "/3" << endl;
            clearInput();
        }
        if (!pass) { cout << "Too many errors! Account not created." << endl; return; }

        string year = getSystemYear();
        int randNum = rand() % 90000 + 10000;
        string newMemNum = year + to_string(randNum);
        int mrz = calculateMRZ(passport);
        string fmtName = formatName(name);

        members[memberCount].memberNumber = newMemNum;
        members[memberCount].name = fmtName;
        members[memberCount].passportNumber = passport;
        members[memberCount].tier = tier;
        members[memberCount].mrz = mrz;
        members[memberCount].mileageBalance = 0;
        members[memberCount].isActive = true;
        memberCount++;

        cout << "\nAccount created successfully!" << endl;
        cout << "Your Member Number: " << newMemNum << endl;
    } else {
        cout << "\n--- CLOSE MEMBER ACCOUNT ---" << endl;
        cout << "Member Full Details:" << endl;
        cout << "MemberNo: " << members[idx].memberNumber << " | Tier: " << members[idx].tier
             << " | Passport: " << members[idx].passportNumber << " | MRZ: " << members[idx].mrz
             << " | Name: " << members[idx].name << " | Mileage: " << members[idx].mileageBalance << endl;
        
        int deleteFlightCount = 0;
        for (int i = 0; i < flightCount; i++) {
            if (flights[i].memberNumber == memNum) deleteFlightCount++;
        }

        if (yesNoPrompt("Confirm close account? (Y/N): ")) {
            for (int i = idx; i < memberCount - 1; i++) members[i] = members[i + 1];
            memberCount--;

            int newFlight = 0;
            Flight temp[MAX_FLIGHTS];
            for (int i = 0; i < flightCount; i++) {
                if (flights[i].memberNumber != memNum) temp[newFlight++] = flights[i];
            }
            for (int i = 0; i < newFlight; i++) flights[i] = temp[i];
            flightCount = newFlight;

            cout << "Account closed successfully! " << deleteFlightCount << " flight records deleted." << endl;
        } else {
            cout << "Close operation cancelled." << endl;
        }
    }
}

// ==============================================
// R4: MEMBER OPERATIONS (FULL FIX + INPUT PROTECTION)
// ==============================================
void memberAccountOperations() {
    clearScreen();
    string memNum;
    cout << "Enter Member Number: ";
    cin >> memNum;
    int idx = findMemberByNumber(memNum);
    if (idx == -1) { cout << "Member not found!" << endl; return; }

    while (true) {
        cout << "\n----- Member Account Operations Menu -----" << endl;
        cout << "[1] Edit Member Information" << endl;
        cout << "[2] Update Mileage Points Balance" << endl;
        cout << "[3] Create Flight Records" << endl;
        cout << "[4] Redeem & Transfer Points" << endl;
        cout << "[5] Return to Main Menu" << endl;
        cout << "Option (1-5): ";

        int op;
        string opStr;
        cin >> opStr;
        clearInput();
        if (!isInteger(opStr)) { cout << "Invalid input!" << endl; continue; }
        op = stoi(opStr);

        if (op == 1) {
            cout << "\n--- EDIT MEMBER INFO ---" << endl;
            cout << "Current Info: " << members[idx].name << " | " << members[idx].passportNumber << " | " << members[idx].tier << endl;
            string nName, nPass, nTier;
            bool pass = false;
            int attempt = 0;
            while (attempt < 3) {
                pass = true;
                cout << "New Name (max 30 chars): ";
                getline(cin, nName);
                if (nName.size() > 30) { pass = false; }
                if (pass) {
                    cout << "New Passport: ";
                    cin >> nPass;
                    if (!isPassportValid(nPass) || isPassportDuplicate(nPass)) pass = false;
                }
                if (pass) {
                    cout << "New Tier: ";
                    cin >> nTier;
                    if (!isValidTier(nTier)) pass = false;
                }
                if (pass) break;
                attempt++;
                cout << "Invalid input! Attempt " << attempt << "/3" << endl;
                clearInput();
            }
            if (!pass) { cout << "Too many errors! Update cancelled." << endl; continue; }
            if (yesNoPrompt("Confirm update? (Y/N): ")) {
                members[idx].name = formatName(nName);
                members[idx].passportNumber = nPass;
                members[idx].tier = nTier;
                members[idx].mrz = calculateMRZ(nPass);
                cout << "Information updated!" << endl;
            }
        }
        else if (op == 2) {
            cout << "\n--- UPDATE MILEAGE ---" << endl;
            cout << "Current Balance: " << members[idx].mileageBalance << endl;
            int total = 0;
            cout << left << setw(10) << "Flight" << setw(12) << "Departure" << setw(10) << "Points" << endl;
            cout << string(35,'-') << endl;
            for (int i = 0; i < flightCount; i++) {
                if (flights[i].memberNumber == memNum && !flights[i].updated && isDateBeforeOrEqual(flights[i].departureDate, systemDate)) {
                    int base = getBaseMileage(flights[i].origin, flights[i].destination, flights[i].cabinClass);
                    int add = base * (1 + getBonusPercent(members[idx].tier));
                    total += add;
                    flights[i].updated = true;
                    cout << left << setw(10) << flights[i].flightNumber << setw(12) << flights[i].departureDate << setw(10) << add << endl;
                }
            }
            members[idx].mileageBalance += total;
            if (total > 0) {
                transactions[transCount++] = {memNum, "Mileage Earned", total, "Flight points"};
            }
            cout << "\nTotal Earned: " << total << " | New Balance: " << members[idx].mileageBalance << endl;
        }
        else if (op == 3) {
            cout << "\n--- CREATE FLIGHT ---" << endl;
            string fn, cabin, dep;
            bool pass = false;
            int attempt = 0;
            while (attempt < 3) {
                pass = true;
                cout << "Flight No (CC81/CC82/CC31/CC32/CC61/CC62): "; cin >> fn;
                if (!isValidFlightNumber(fn)) pass = false;
                if (pass) {
                    cout << "Cabin Class: "; cin >> cabin;
                    if (!isValidCabin(cabin)) pass = false;
                }
                if (pass) {
                    cout << "Departure Date: "; cin >> dep;
                    if (!isDateValidFormat(dep) || isDateBeforeOrEqual(dep, systemDate)) pass = false;
                }
                if (pass) break;
                attempt++;
                cout << "Invalid input! Attempt " << attempt << "/3" << endl;
                clearInput();
            }
            if (!pass) { cout << "Too many errors! Flight not created." << endl; continue; }
            string ori = getFlightOrigin(fn);
            string dest = getFlightDest(fn);
            cout << "Auto Origin: " << ori << " | Auto Destination: " << dest << endl;
            if (yesNoPrompt("Confirm add flight? (Y/N): ")) {
                flights[flightCount].memberNumber = memNum;
                flights[flightCount].flightNumber = fn;
                flights[flightCount].origin = ori;
                flights[flightCount].destination = dest;
                flights[flightCount].cabinClass = cabin;
                flights[flightCount].departureDate = dep;
                flights[flightCount].creationDate = systemDate;
                flights[flightCount].updated = false;
                flightCount++;
                cout << "Flight added successfully!" << endl;
            }
        }
        else if (op == 4) {
            cout << "\n[1] Redeem Gift | [2] Transfer Points | Option: ";
            string sStr; cin >> sStr; clearInput();
            if (!isInteger(sStr)) { cout << "Invalid input!" << endl; continue; }
            int s = stoi(sStr);
            if (s == 1) {
                cout << "\n--- REDEEM GIFT ---" << endl;
                cout << "1.Movie(3000) 2.$100Voucher(4000) 3.Lounge(6000)" << endl;
                cout << "Your Balance: " << members[idx].mileageBalance << endl;
                cout << "Select Gift: ";
                string gStr; cin >> gStr; clearInput();
                if (!isInteger(gStr)) { cout << "Invalid input!" << endl; continue; }
                int g = stoi(gStr);
                int cost=0; string desc;
                if (g==1) { cost=3000; desc="Gift#1 Movie voucher"; }
                else if (g==2) { cost=4000; desc="Gift#2 $100 supermarket voucher"; }
                else if (g==3) { cost=6000; desc="Gift#3 Airport lounge access voucher"; }
                else { cout << "Invalid gift!" << endl; continue; }
                if (members[idx].mileageBalance < cost) { cout << "Insufficient points!" << endl; continue; }
                if (yesNoPrompt("Confirm redeem? (Y/N): ")) {
                    members[idx].mileageBalance -= cost;
                    transactions[transCount++] = {memNum, "Redemption", -cost, desc};
                    cout << "Redeem success! New Balance: " << members[idx].mileageBalance << endl;
                }
            }
            else if (s == 2) {
                cout << "\n--- TRANSFER POINTS ---" << endl;
                string target; string amtStr;
                cout << "Target Member No: "; cin >> target;
                cout << "Transfer Amount (positive integer): "; cin >> amtStr;
                clearInput();
                if (!isInteger(amtStr)) { cout << "Amount must be integer!" << endl; continue; }
                int amt = stoi(amtStr);
                int tIdx = findMemberByNumber(target);
                if (tIdx==-1 || amt<=0 || members[idx].mileageBalance<amt) {
                    cout << "Transfer failed! Invalid target/amount/balance." << endl; continue;
                }
                if (yesNoPrompt("Confirm transfer to "+members[tIdx].name+"? (Y/N): ")) {
                    members[idx].mileageBalance -= amt;
                    members[tIdx].mileageBalance += amt;
                    transactions[transCount++] = {memNum, "Transfer Out", -amt, "To "+target};
                    transactions[transCount++] = {target, "Transfer In", amt, "From "+memNum};
                    cout << "Transfer success!" << endl;
                    cout << "Your Balance: " << members[idx].mileageBalance << " | Target Balance: " << members[tIdx].mileageBalance << endl;
                }
            }
            else cout << "Invalid option!" << endl;
        }
        else if (op == 5) {
            cout << "\n=====================================" << endl;
            cout << "          MEMBER LATEST INFO          " << endl;
            cout << "=====================================" << endl;
            cout << "MemberNo: " << members[idx].memberNumber << endl;
            cout << "Name: " << members[idx].name << " | Tier: " << members[idx].tier << endl;
            cout << "Passport: " << members[idx].passportNumber << " | MRZ: " << members[idx].mrz << endl;
            cout << "Mileage Balance: " << members[idx].mileageBalance << endl;

            cout << "\n=====================================" << endl;
            cout << "        UNUPDATED FLIGHT RECORDS       " << endl;
            cout << "=====================================" << endl;
            cout << left << setw(10) << "Flight" << setw(12) << "Origin" << setw(12) << "Dest"
                 << setw(10) << "Cabin" << setw(12) << "Departure" << endl;
            cout << string(60, '-') << endl;
            bool found = false;
            for (int i = 0; i < flightCount; i++) {
                if (flights[i].memberNumber == memNum && !flights[i].updated) {
                    cout << left << setw(10) << flights[i].flightNumber << setw(12) << flights[i].origin
                         << setw(12) << flights[i].destination << setw(10) << flights[i].cabinClass
                         << setw(12) << flights[i].departureDate << endl;
                    found = true;
                }
            }
            if (!found) cout << "No unupdated flights." << endl;
            cout << "\nReturn to Main Menu." << endl;
            break;
        }
        else cout << "Invalid option!" << endl;
    }
}

// ==============================================
// R5: DAILY STATEMENT (STRICTLY FOLLOW ASSIGNMENT FORMAT)
// ==============================================
void generateDailyStatement() {
    clearScreen();
    string memNum;
    cout << "Enter Member Number: ";
    cin >> memNum;
    int idx = findMemberByNumber(memNum);
    if (idx == -1) { cout << "Member not found!" << endl; return; }

    cout << "\nMember Name: " << members[idx].name << endl;
    cout << "Member Number: " << members[idx].memberNumber << endl;
    cout << "Statement Date: " << systemDate << endl;

    cout << "\nTransaction Summary:" << endl;
    bool hasTrans = false;
    for (int i = 0; i < transCount; i++) {
        if (transactions[i].memberNumber == memNum) {
            cout << "Type " << transactions[i].type << " Mileage " << transactions[i].mileage
                 << " Description " << transactions[i].description << endl;
            hasTrans = true;
        }
    }
    if (!hasTrans) cout << "No transactions recorded." << endl;

    cout << "\nUpcoming Itinerary:" << endl;
    cout << "Origin Destination Flight Cabin Departure" << endl;
    bool hasTrip = false;
    for (int i = 0; i < flightCount; i++) {
        if (flights[i].memberNumber == memNum && !flights[i].updated && isDateAfter(flights[i].departureDate, systemDate)) {
            cout << flights[i].origin << " " << flights[i].destination << " " << flights[i].flightNumber
                 << " " << flights[i].cabinClass << " " << flights[i].departureDate << endl;
            hasTrip = true;
        }
    }
    if (!hasTrip) cout << "No upcoming flights." << endl;

    cout << "\nMember Account Summary:" << endl;
    cout << "Total Mileage Points Balance : " << members[idx].mileageBalance << endl;
    cout << "Member Tier : " << members[idx].tier << endl;
    cout << "Bonus Mileage Points : " << getBonusPercent(members[idx].tier)*100 << "%" << endl;

    cout << "\nPress Enter to continue...";
    cin.get();
}

// ==============================================
// R6: CREDITS & EXIT (ADD GROUP INFO)
// ==============================================
void creditsAndExit() {
    if (!yesNoPrompt("Confirm exit system? (Y/N): ")) { cout << "Return to menu." << endl; return; }
    clearScreen();
    cout << "\n=====================================" << endl;
    cout << "           PROJECT CREDITS            " << endl;
    cout << "=====================================" << endl;
    cout << "SEHH2042 Computer Programming" << endl;
    cout << "Frequent Flyer Program System" << endl;
    cout << "Group Project 2025-2026 Semester 2" << endl;
    cout << "=====================================" << endl;
    cout << "Group Members:" << endl;
    cout << "1. Student Name: CHAN Yu Ping  | Student ID: 25092047A | Tutorial Group: B05A" << endl;
    cout << "2. Student Name: HUI Yan Yuet  | Student ID: 25165020A | Tutorial Group: B05B" << endl;
    cout << "3. Student Name: CHAN Ho Cheung| Student ID: 24077901A | Tutorial Group: B05B" << endl;
    cout << "4. Student Name: HUANG Meiqi   | Student ID: 25137820A | Tutorial Group: B05B" << endl;
    cout << "5. Student Name: Chen Xingxuan | Student ID: 24066658A | Tutorial Group: B05B" << endl;
    cout << "6. Student Name: Pang ka wai   | Student ID: 25144718A | Tutorial Group: B05B" << endl;
    cout << "=====================================\n" << endl;
    exit(0);
}

// ==============================================
// MAIN FUNCTION
// ==============================================
int main() {
    srand(time(0));
    showWelcome();
    while (true) {
        showMainMenu();
        string opStr;
        cin >> opStr;
        clearInput();
        if (!isInteger(opStr)) { cout << "Invalid input!" << endl; continue; }
        int op = stoi(opStr);

        if (!dataLoaded && op>=2 && op<=5) {
            cout << "Please load starting data first (Option 1)!" << endl;
            continue;
        }
        switch(op) {
            case 1: loadStartingData(); break;
            case 2: showAllMemberAccounts(); break;
            case 3: openOrCloseAccount(); break;
            case 4: memberAccountOperations(); break;
            case 5: generateDailyStatement(); break;
            case 6: creditsAndExit(); return 0;
            default: cout << "Invalid option!" << endl; break;
        }
    }
    return 0;
}
