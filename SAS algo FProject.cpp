/*
 * ============================================================
 * SMART RFID ATTENDANCE SYSTEM USING HASHING - ENHANCED
 * Design & Analysis of Algorithms -- Project
 *
 * Algorithm : Hash Table (Separate Chaining for collisions)
 * Language : C++ (Compatible with Dev-C++ 5.11 / old GCC)
 *
 * Attendance Rules:
 * ARRIVAL:
 * <= 5 min late -> Present (on time)
 * 6-25 min late -> Half Present
 * > 25 min late -> Absent
 *
 * EXIT (based on time spent in class):
 * >= (duration - 5 min) -> Present (92%+ stayed)
 * >= half but < (duration - 5 min) -> Half Present
 * < half lecture duration -> Absent
 *
 * FINAL STATUS = worse of arrival + exit statuses
 *
 * Complexity :
 * Search -> O(1) average
 * Insert -> O(1) average
 * Full class attendance -> O(n)
 * Space : O(n)
 * ============================================================
 */

#include <iostream>
#include <string>
#include <iomanip>
#include <fstream> // Added for File I/O
#include <time.h> // Added for time measurement
using namespace std;

// -------------------------------------------------------
// ANSI COLOR CODES
// -------------------------------------------------------
#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define CYAN "\033[36m"
#define BOLD "\033[1m"

// -------------------------------------------------------
// CONSTANTS - UPDATED RULES - 5/25 RULE
// -------------------------------------------------------
const int TABLE_SIZE = 100;
const int PRESENT_LIMIT = 5; // <= 5 min late -> Present
const int HALF_PR_LIMIT = 25; // 6-25 min late -> Half Present | >25 -> Absent
const int EXIT_GRACE = 5; // 5 min grace for Present on exit

// -------------------------------------------------------
// HELPER: integer to string (replaces C++11 to_string)
// -------------------------------------------------------
string intToStr(int n) {
    if (n == 0) return "0";
    string result = "";
    while (n > 0) {
        result = (char)('0' + n % 10) + result;
        n /= 10;
    }
    return result;
}

// -------------------------------------------------------
// DATA STRUCTURES
// -------------------------------------------------------

struct TimeStamp {
    int hours;
    int minutes;

    TimeStamp() : hours(0), minutes(0) {}
    TimeStamp(int h, int m) : hours(h), minutes(m) {}

    int toMinutes() const {
        return hours * 60 + minutes;
    }

    string toString() const {
        string h = (hours < 10)? "0" + intToStr(hours) : intToStr(hours);
        string m = (minutes < 10)? "0" + intToStr(minutes) : intToStr(minutes);
        return h + ":" + m;
    }
};

struct Student {
    int rfidID;
    string name;
    string rollNo;
    string attendanceStatus;
    TimeStamp entryTime;
    TimeStamp exitTime;
    bool entryScanned;
    bool exitScanned;
    Student* next; // For separate chaining (collision handling)

    Student(int id, string n, string r)
        : rfidID(id), name(n), rollNo(r),
          attendanceStatus("Not Marked"),
          entryScanned(false), exitScanned(false), next(NULL) {
        entryTime = TimeStamp(0, 0);
        exitTime = TimeStamp(0, 0);
    }
};

struct AttendanceLog {
    string rollNo;
    string name;
    int rfidID;
    TimeStamp entryTime;
    TimeStamp exitTime;
    int minsStayed;
    string arrivalStatus; // status based on arrival delay
    string exitStatus; // status based on time stayed
    string finalStatus; // final combined status

    AttendanceLog() : rfidID(0), minsStayed(0) {}
};

// -------------------------------------------------------
// STATUS HELPERS - 5/25 RULE
// -------------------------------------------------------

// Rank: lower number = worse status (used to pick the worse of two)
int statusRank(const string& s) {
    if (s == "Present") return 2;
    if (s == "Half Present") return 1;
    return 0; // Absent
}

string worseStatus(const string& a, const string& b) {
    return (statusRank(a) <= statusRank(b))? a : b;
}

// Arrival status based on how late student arrived - 5/25 RULE
string arrivalStatus(int delayMins) {
    if (delayMins <= PRESENT_LIMIT) return "Present";
    if (delayMins <= HALF_PR_LIMIT) return "Half Present";
    return "Absent"; // > 25 min late
}

// Exit status based on how long student stayed
string exitStatus(int minsStayed, int lectureDuration) {
    int halfDuration = lectureDuration / 2;
    int presentThreshold = lectureDuration - EXIT_GRACE; // 60-5 = 55 min

    if (minsStayed < halfDuration) return "Absent";
    if (minsStayed < presentThreshold) return "Half Present";
    return "Present";
}

// -------------------------------------------------------
// HASH TABLE CLASS
// -------------------------------------------------------
class HashTable {
private:
    Student* table[TABLE_SIZE];
    int count;

public:
    HashTable() {
        for (int i = 0; i < TABLE_SIZE; i++)
            table[i] = NULL;
        count = 0;
    }

    // Hash Function: RFID % TABLE_SIZE -> O(1)
    int hashFunction(int rfidID) {
        return rfidID % TABLE_SIZE;
    }

    // Insert student -> O(1)
    void insertStudent(int rfidID, string name, string rollNo) {
        int index = hashFunction(rfidID);
        Student* newStu = new Student(rfidID, name, rollNo);

        newStu->next = table[index];
        table[index] = newStu;
        count++;

        cout << GREEN << " [+] Registered: " << name
             << " | RFID: " << rfidID
             << " | Hash Index: " << index << RESET << "\n";
    }

    // Search student by RFID -> O(1) average
    Student* searchStudent(int rfidID) {
        int index = hashFunction(rfidID);
        Student* curr = table[index];

        while (curr!= NULL) {
            if (curr->rfidID == rfidID)
                return curr;
            curr = curr->next;
        }
        return NULL;
    }

    // Search with time measurement for O(1) proof
    Student* searchStudentTimed(int rfidID) {
        clock_t start = clock();
        Student* result = searchStudent(rfidID);
        clock_t end = clock();
        double time_taken = ((double)(end - start))/CLOCKS_PER_SEC;
        cout << CYAN << " [TIME] Search for RFID " << rfidID << " took: "
             << fixed << setprecision(6) << time_taken << " seconds" << RESET << "\n";
        return result;
    }

    // Display all students in hash table
    void displayAll() {
        cout << CYAN << "\n";
        for(int i=0;i<62;i++) cout<<"="; cout<<"\n";
        cout << BOLD << " REGISTERED STUDENTS IN HASH TABLE" << RESET << CYAN << "\n";
        for(int i=0;i<62;i++) cout<<"="; cout<<"\n";
        cout << left
             << setw(8) << "Index"
             << setw(10) << "RFID"
             << setw(22) << "Name"
             << setw(12) << "Roll No"
             << "Status" << RESET << "\n";
        for(int i=0;i<62;i++) cout<<"-"; cout<<"\n";

        for (int i = 0; i < TABLE_SIZE; i++) {
            Student* curr = table[i];
            while (curr!= NULL) {
                cout << left
                     << setw(8) << i
                     << setw(10) << curr->rfidID
                     << setw(22) << curr->name
                     << setw(12) << curr->rollNo;

                if(curr->attendanceStatus=="Present") cout << GREEN;
                else if(curr->attendanceStatus=="Half Present") cout << YELLOW;
                else cout << RED;
                cout << curr->attendanceStatus << RESET << "\n";
                curr = curr->next;
            }
        }
        cout << CYAN; for(int i=0;i<62;i++) cout<<"="; cout<<"\n" << RESET;
    }

    // Hash Table Statistics
    void printHashStats() {
        int filled=0, collisions=0, maxChain=0;
        for(int i=0;i<TABLE_SIZE;i++) {
            if(table[i]!=NULL) {
                filled++;
                int chainLen=0; Student* curr=table[i];
                while(curr!=NULL) { chainLen++; curr=curr->next; }
                if(chainLen>1) collisions++;
                if(chainLen>maxChain) maxChain=chainLen;
            }
        }
        cout << CYAN << "\n"; for(int i=0;i<62;i++) cout<<"-"; cout<<"\n";
        cout << BOLD << " HASH TABLE STATISTICS" << RESET << CYAN << "\n";
        for(int i=0;i<62;i++) cout<<"-"; cout<<"\n";
        cout << " Total Students: " << count << "\n";
        cout << " Table Size: " << TABLE_SIZE << "\n";
        cout << " Load Factor: " << (float)count/TABLE_SIZE << "\n";
        cout << " Filled Buckets: " << filled << "/100\n";
        cout << " Collision Buckets: " << collisions << "\n";
        cout << " Max Chain Length: " << maxChain << "\n";
        for(int i=0;i<62;i++) cout<<"-"; cout<<"\n" << RESET;
    }

    // Record student ENTRY scan
    string markEntry(int rfidID, TimeStamp entryTime,
                     TimeStamp classStart,
                     AttendanceLog log[], int& logCount) {

        Student* student = searchStudent(rfidID);

        if (student == NULL)
            return "ERROR: RFID not found in system!";

        if (student->entryScanned)
            return "WARNING: Entry already recorded for " + student->name;

        int delay = entryTime.toMinutes() - classStart.toMinutes();
        if (delay < 0) delay = 0; // early arrivals count as on time

        student->entryTime = entryTime;
        student->entryScanned = true;

        // Tentative status from arrival alone (exit may update it)
        student->attendanceStatus = arrivalStatus(delay);

        // Add to log (exit time and final status filled later)
        log[logCount].rollNo = student->rollNo;
        log[logCount].name = student->name;
        log[logCount].rfidID = student->rfidID;
        log[logCount].entryTime = entryTime;
        log[logCount].exitTime = TimeStamp(0, 0);
        log[logCount].minsStayed = 0;
        log[logCount].arrivalStatus = student->attendanceStatus;
        log[logCount].exitStatus = "Pending";
        log[logCount].finalStatus = student->attendanceStatus;
        logCount++;

        return "SUCCESS";
    }

    // Record student EXIT scan and finalise status
    string markExit(int rfidID, TimeStamp exitTime,
                    int lectureDuration,
                    AttendanceLog log[], int logCount) {

        Student* student = searchStudent(rfidID);

        if (student == NULL)
            return "ERROR: RFID not found in system!";

        if (!student->entryScanned)
            return "ERROR: No entry recorded for " + student->name;

        if (student->exitScanned)
            return "WARNING: Exit already recorded for " + student->name;

        int minsStayed = exitTime.toMinutes() - student->entryTime.toMinutes();
        if (minsStayed < 0) minsStayed = 0;

        string eStatus = exitStatus(minsStayed, lectureDuration);
        string aStatus = student->attendanceStatus; // arrival status set at entry

        // Final status is the worse of arrival and exit
        string fStatus = worseStatus(aStatus, eStatus);

        student->exitTime = exitTime;
        student->exitScanned = true;
        student->attendanceStatus = fStatus;

        // Update the log entry for this student
        for (int i = 0; i < logCount; i++) {
            if (log[i].rfidID == rfidID) {
                log[i].exitTime = exitTime;
                log[i].minsStayed = minsStayed;
                log[i].exitStatus = eStatus;
                log[i].finalStatus = fStatus;
                break;
            }
        }

        return "SUCCESS";
    }

    // Add absent (no-show) students to the log
    void addAbsentStudents(AttendanceLog log[], int& logCount) {
        for (int i = 0; i < TABLE_SIZE; i++) {
            Student* curr = table[i];
            while (curr!= NULL) {
                if (!curr->entryScanned) {
                    curr->attendanceStatus = "Absent";

                    log[logCount].rollNo = curr->rollNo;
                    log[logCount].name = curr->name;
                    log[logCount].rfidID = curr->rfidID;
                    log[logCount].entryTime = TimeStamp(0, 0);
                    log[logCount].exitTime = TimeStamp(0, 0);
                    log[logCount].minsStayed = 0;
                    log[logCount].arrivalStatus = "Absent";
                    log[logCount].exitStatus = "N/A";
                    log[logCount].finalStatus = "Absent";
                    logCount++;
                }
                curr = curr->next;
            }
        }
    }

    ~HashTable() {
        for (int i = 0; i < TABLE_SIZE; i++) {
            Student* curr = table[i];
            while (curr!= NULL) {
                Student* temp = curr;
                curr = curr->next;
                delete temp;
            }
        }
    }
};

// -------------------------------------------------------
// HELPER PRINT FUNCTIONS
// -------------------------------------------------------

void printBanner() {
    cout << BOLD << CYAN << "\n";
    for(int i=0;i<62;i++) cout<<"*"; cout<<"\n";
    cout << "* SMART RFID ATTENDANCE SYSTEM USING HASHING *" << "\n";
    cout << "* Design & Analysis of Algorithms -- C++ Project *" << "\n";
    cout << "* Hash Function : index = RFID % 100 *" << "\n";
    cout << "* Collision Fix : Separate Chaining (Linked List) *" << "\n";
    for(int i=0;i<62;i++) cout<<"*"; cout<<"\n\n" << RESET;
}

void printRules(int lectureDuration) {
    int half = lectureDuration / 2;
    int presentMin = lectureDuration - EXIT_GRACE;
    cout << GREEN << "\n";
    for(int i=0;i<62;i++) cout<<"-"; cout<<"\n";
    cout << BOLD << " ATTENDANCE RULES (Lecture Duration: "
         << lectureDuration << " mins)" << RESET << GREEN << "\n";
    for(int i=0;i<62;i++) cout<<"-"; cout<<"\n";
    cout << " ARRIVAL:\n";
    cout << " <= 5 min late -> Present\n";
    cout << " 6 to 25 min late -> Half Present\n";
    cout << " > 25 min late -> Absent\n";
    cout << " Did not come at all -> Absent\n";
    cout << " EXIT (time spent in class):\n";
    cout << " < " << half << " mins stayed -> Absent\n";
    cout << " " << half << " to <" << presentMin
         << " mins stayed -> Half Present\n";
    cout << " >= " << presentMin << " mins stayed -> Present\n";
    cout << " FINAL STATUS = worse of Arrival & Exit status\n";
    for(int i=0;i<62;i++) cout<<"-"; cout<<"\n" << RESET;
}

// Color coded report - NO LATE
void printAttendanceReport(AttendanceLog log[], int logCount,
                           TimeStamp classStart, int lectureDuration) {
    cout << BOLD << CYAN << "\n";
    for(int i=0;i<78;i++) cout<<"="; cout<<"\n";
    cout << " ATTENDANCE REPORT | Class Start: " << classStart.toString()
         << " | Duration: " << lectureDuration << " mins\n";
    for(int i=0;i<78;i++) cout<<"="; cout<<"\n" << RESET;
    cout << left
         << setw(10) << "Roll No"
         << setw(18) << "Name"
         << setw(8) << "RFID"
         << setw(8) << "Entry"
         << setw(8) << "Exit"
         << setw(8) << "Stayed"
         << setw(14) << "ArrivalStatus"
         << setw(14) << "ExitStatus"
         << "Final\n";
    for(int i=0;i<78;i++) cout<<"-"; cout<<"\n";

    int present = 0, halfPresent = 0, absent = 0;

    for (int i = 0; i < logCount; i++) {
        string stayed = (log[i].minsStayed > 0)
                  ? intToStr(log[i].minsStayed) + "m"
                        : "N/A";
        string entryStr = (log[i].entryTime.toMinutes() > 0)
                   ? log[i].entryTime.toString()
                          : "N/A";
        string exitStr = (log[i].exitTime.toMinutes() > 0)
                  ? log[i].exitTime.toString()
                         : "N/A";

        cout << left
             << setw(10) << log[i].rollNo
             << setw(18) << log[i].name
             << setw(8) << log[i].rfidID
             << setw(8) << entryStr
             << setw(8) << exitStr
             << setw(8) << stayed
             << setw(14) << log[i].arrivalStatus
             << setw(14) << log[i].exitStatus;

        // Color coding - NO LATE
        if(log[i].finalStatus=="Present") cout << GREEN; // Green
        else if(log[i].finalStatus=="Half Present") cout << YELLOW; // Yellow
        else cout << RED; // Red for Absent

        cout << log[i].finalStatus << RESET << "\n";

        if (log[i].finalStatus == "Present") present++;
        else if (log[i].finalStatus == "Half Present") halfPresent++;
        else absent++;
    }

    for(int i=0;i<78;i++) cout<<"-"; cout<<"\n";
    cout << GREEN << " Present: " << present << RESET
         << YELLOW << " Half Present: " << halfPresent << RESET
         << RED << " Absent: " << absent << RESET
         << " Total: " << logCount << "\n";
    cout << BOLD << CYAN; for(int i=0;i<78;i++) cout<<"="; cout<<"\n" << RESET;
}

// Save report to file
void saveToFile(AttendanceLog log[], int logCount) {
    ofstream file("attendance_report.txt");
    file << "Roll No,Name,RFID,Entry,Exit,Stayed,ArrivalStatus,ExitStatus,Final Status\n";
    for(int i=0;i<logCount;i++) {
        string entryStr = (log[i].entryTime.toMinutes() > 0)? log[i].entryTime.toString() : "N/A";
        string exitStr = (log[i].exitTime.toMinutes() > 0)? log[i].exitTime.toString() : "N/A";
        string stayed = (log[i].minsStayed > 0)? intToStr(log[i].minsStayed) + "m" : "N/A";
        file << log[i].rollNo << "," << log[i].name << "," << log[i].rfidID << ","
             << entryStr << "," << exitStr << "," << stayed << ","
             << log[i].arrivalStatus << "," << log[i].exitStatus << "," << log[i].finalStatus << "\n";
    }
    file.close();
    cout << GREEN << BOLD << "\n[+] Report saved to attendance_report.txt" << RESET << "\n";
}

void printDryRun(int lectureDuration) {
    int half = lectureDuration / 2;
    int presentMin = lectureDuration - EXIT_GRACE;
    cout << YELLOW << "\n";
    for(int i=0;i<62;i++) cout<<"="; cout<<"\n";
    cout << BOLD << " DRY RUN (Matches Updated Rules)" << RESET << YELLOW << "\n";
    for(int i=0;i<62;i++) cout<<"="; cout<<"\n";
    cout << " Hash Function : 12345 % 100 = 45\n";
    cout << " Lecture Duration: " << lectureDuration
         << " mins | Half = " << half << " mins\n";
    cout << " ARRIVAL EXAMPLES:\n";
    cout << " Scan at +4 min -> Arrival: Present\n";
    cout << " Scan at +20 min -> Arrival: Half Present\n";
    cout << " Scan at +26 min -> Arrival: Absent\n";
    cout << " Did not come -> Arrival: Absent\n\n";
    cout << " EXIT EXAMPLES (assuming on-time entry):\n";
    cout << " Left after " << (half-5)
         << " min -> Exit: Absent -> Final: Absent\n";
    cout << " Left after " << half
         << " min -> Exit: Half Present -> Final: Half Present\n";
    cout << " Left after " << presentMin
         << " min -> Exit: Present -> Final: Present\n\n";
    cout << " COMBINED EXAMPLE:\n";
    cout << " Arrived 4 min late (Present)\n";
    cout << " Stayed 56 min (Present)\n";
    cout << " Final -> Present (worse status wins)\n";
    for(int i=0;i<62;i++) cout<<"="; cout<<"\n" << RESET;
}

void printLimitations() {
    cout << RED << "\n";
    for(int i=0;i<62;i++) cout<<"-"; cout<<"\n";
    cout << BOLD << " SYSTEM LIMITATIONS" << RESET << RED << "\n";
    for(int i=0;i<62;i++) cout<<"-"; cout<<"\n";
    cout << " 1. RFID Card Dependency - No card = no attendance\n";
    cout << " 2. Proxy Attendance - Friend's card can be misused\n";
    cout << " 3. Hardware Cost - RFID readers are expensive\n";
    cout << " 4. Hash Collision - Solved via Separate Chaining\n";
    cout << " 5. Power Failure Risk - System stops on power cut\n";
    cout << " 6. Exit Scan Dependency - Student must scan on exit too\n";
    cout << " 7. Network/DB Failure - Data may not save on crash\n";
    for(int i=0;i<62;i++) cout<<"-"; cout<<"\n" << RESET;
}

// -------------------------------------------------------
// MAIN
// -------------------------------------------------------
int main() {
    printBanner();

    HashTable system;

    cout << YELLOW << "Loading student database into Hash Table...\n\n" << RESET;

    system.insertStudent(12345, "Ali Hassan", "F22-001");
    system.insertStudent(23456, "Sara Khan", "F22-002");
    system.insertStudent(34567, "Usman Raza", "F22-003");
    system.insertStudent(45678, "Fatima Noor", "F22-004");
    system.insertStudent(56789, "Ahmed Malik", "F22-005");
    system.insertStudent(67890, "Zara Qureshi", "F22-006");
    system.insertStudent(78901, "Bilal Anwar", "F22-007");
    system.insertStudent(89012, "Hina Sheikh", "F22-008");
    system.insertStudent(10045, "Hamza Siddiqui", "F22-009");
    system.insertStudent(20045, "Nadia Farooq", "F22-010");

    system.displayAll();
    system.printHashStats();

    // Class start time with validation
    cout << CYAN << "\nEnter class start time:\n" << RESET;
    int sh, sm;
    while(true) {
        cout << " Hours (e.g. 10) : "; cin >> sh;
        cout << " Minutes (e.g. 00) : "; cin >> sm;
        if(sh<0 || sh>23 || sm<0 || sm>59) {
            cout << BOLD << RED << " [ERROR] Invalid time! Hours: 0-23, Minutes: 0-59" << RESET << "\n";
        } else break;
    }
    TimeStamp classStart(sh, sm);

    // Lecture duration
    int lectureDuration;
    cout << "\nEnter total lecture duration in minutes (e.g. 60, 90, 120) : ";
    cin >> lectureDuration;

    printRules(lectureDuration);

    AttendanceLog log[200];
    int logCount = 0;

    // -- ATTENDANCE MARKING: ENTRY + EXIT FOR EACH STUDENT -----------------------------
    cout << BOLD << YELLOW << "\n";
    for(int i=0;i<62;i++) cout<<"="; cout<<"\n";
    cout << " ATTENDANCE MARKING\n";
    cout << " Enter RFID, Entry Time, then Exit Time for each student.\n";
    cout << " Enter 0 as RFID to finish and see report.\n";
    for(int i=0;i<62;i++) cout<<"="; cout<<"\n" << RESET;

    while (true) {
        int rfidInput;
        cout << "\n Enter RFID ID (0 to finish) : ";
        cin >> rfidInput;
        if (rfidInput == 0) break;

        // --- ENTRY TIME ---
        int th, tm;
        while(true) {
            cout << " Entry time Hours : "; cin >> th;
            cout << " Entry time Minutes : "; cin >> tm;
            if(th<0 || th>23 || tm<0 || tm>59) {
                cout << BOLD << RED << " [ERROR] Invalid time! Hours: 0-23, Minutes: 0-59" << RESET << "\n";
            } else break;
        }
        TimeStamp entryTime(th, tm);

        string result = system.markEntry(rfidInput, entryTime,
                                         classStart, log, logCount);

        if (result == "SUCCESS") {
            Student* s = system.searchStudentTimed(rfidInput);
            cout << GREEN << BOLD << " -> " << s->name << " | Entry Recorded" << RESET << "\n";
        } else {
            cout << BOLD << RED << " -> " << result << RESET << "\n";
            continue; // Skip to next student if entry failed
        }

        // --- EXIT TIME (ask immediately after entry) ---
        while(true) {
            cout << " Exit time Hours : "; cin >> th;
            cout << " Exit time Minutes : "; cin >> tm;
            if(th<0 || th>23 || tm<0 || tm>59) {
                cout << BOLD << RED << " [ERROR] Invalid time! Hours: 0-23, Minutes: 0-59" << RESET << "\n";
            } else break;
        }
        TimeStamp exitTime(th, tm);

        result = system.markExit(rfidInput, exitTime,
                                 lectureDuration, log, logCount);

        if (result == "SUCCESS") {
            Student* s = system.searchStudent(rfidInput);
            int minsStayed = exitTime.toMinutes() - s->entryTime.toMinutes();

            if(s->attendanceStatus=="Present") cout << GREEN << BOLD;
            else if(s->attendanceStatus=="Half Present") cout << YELLOW << BOLD;
            else cout << RED << BOLD;

            cout << " -> " << s->name
                 << " | Stayed: " << minsStayed << " min"
                 << " | Final Status: " << s->attendanceStatus << RESET << "\n";
        } else {
            cout << BOLD << RED << " -> " << result << RESET << "\n";
        }
    }

    // -- Add absent (no-show) students to log ---
    system.addAbsentStudents(log, logCount);

    printAttendanceReport(log, logCount, classStart, lectureDuration);
    saveToFile(log, logCount);
    printDryRun(lectureDuration);
    printLimitations();

    cout << BOLD << GREEN << "\nProgram ended. Press Enter to close..." << RESET << "\n";
    cin.ignore();
    cin.get();
    return 0;
}