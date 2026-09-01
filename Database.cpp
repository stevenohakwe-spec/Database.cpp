#include <iostream>
#include <string>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <thread>
#include <chrono>
#include <limits>
#include <cctype>

using namespace std;

const int Max_People = 100;
const int columns = 6;
const string DATABASE_FILE = "database.txt";

enum MenuOption
{
    EXIT,
    ADD,
    REMOVE,
    DISPLAY,
    EXPORT,
    IMPORT,
    HIGHEST_EARNER,
    AVERAGE_SALARY,
    BUDGET_CHECK,
    SORT
};

enum DisplayChoice
{
    NO,
    YES
};

void clearScreen()
{
#ifdef _WIN32
    system("cls");
#elif __APPLE__ || __linux__
    system("clear");
#else
    cout << string(50, '\n');
#endif
}

void displayMenu()
{
    cout << "\n======== Employee Database ========\n";
    cout << "1. Add people \n";
    cout << "2. Remove \n";
    cout << "3. Display \n";
    cout << "4. Export \n";
    cout << "5. Import \n";
    cout << "6. Find Highest Earner\n";
    cout << "7. Find Average Salary \n";
    cout << "8. Check Budget Optimization \n";
    cout << "9. Sort Database \n";
    cout << "0. Exit Database \n";
    cout << "Please enter a number to choose an option: ";
}

void fileImport(string People[Max_People][columns], bool occupied[Max_People][columns])
{
    ifstream file(DATABASE_FILE);

    if (!file)
    {
        cout << "Error. Could not open file." << endl;
        return;
    }
    else
        cout << "\nImporting records from " << DATABASE_FILE << "...\n";
    this_thread::sleep_for(chrono::seconds(2));

    string line;

    getline(file, line);
    getline(file, line);

    int i = 0;

    while (i < Max_People && file >> People[i][0]
                               >> People[i][1]
                               >> People[i][2]
                               >> People[i][3]
                               >> People[i][4]
                               >> People[i][5])
    {
        for (int j = 0; j < columns; ++j)
        {
            occupied[i][j] = true;
        }

        i++;
    }

    file.close();
    cout << i << " records imported. Returning to main menu..." << endl;
    this_thread::sleep_for(chrono::seconds(2));
    clearScreen();
}

void fileExport(string People[Max_People][columns], bool occupied[Max_People][columns])
{
    int recordsExported = 0;

    ofstream file(DATABASE_FILE);
    if (!file)
    {
        cout << "Error. File could not open." << endl;
        return;
    }
    else
        cout << "\nExporting records to " << DATABASE_FILE << "...\n";
    this_thread::sleep_for(chrono::seconds(2));

    file << left
         << setw(12)
         << "Position"
         << setw(15)
         << "ID Number"
         << setw(25)
         << "Name"
         << setw(15)
         << "Division ID"
         << setw(15)
         << "Manager ID"
         << setw(12)
         << "Salary" << endl;
    file << string(90, '-') << endl;

    for (int i = 0; i < Max_People; ++i)
    {
        if (occupied[i][0])
        {
            file << left
                 << setw(12)
                 << People[i][0]
                 << setw(15)
                 << People[i][1]
                 << setw(25)
                 << People[i][2]
                 << setw(15)
                 << People[i][3]
                 << setw(15)
                 << People[i][4]
                 << setw(12)
                 << People[i][5] << endl;
            recordsExported++;
        }
        else
            break;
    }

    file.close();
    cout << recordsExported << " records exported. File closed. Returning to main menu..." << endl;
    this_thread::sleep_for(chrono::seconds(2));
    clearScreen();
}

void addPeople(string People[Max_People][columns], bool occupied[Max_People][columns])
{
    bool running = true;
    while (running)
    {
        int n;
        int startIndex = 0;
        string firstName;
        string lastName;

        cout << endl
             << "How many people do you want to add? ";
        cin >> n;
        cout << endl;

        for (int i = 0; i < Max_People; ++i)
        {
            if (occupied[i][0])
                startIndex++;
            else
                break;
        }

        for (int i = startIndex; i < n + startIndex; ++i)
        {
            occupied[i][0] = true;
            People[i][0] = to_string(i + 1);

            cout << "Enter the person's ID number: ";
            cin >> People[i][1];
            cout << endl;
            occupied[i][1] = true;

            cout << "Enter the person's first name: ";
            cin >> firstName;
            cout << endl
                 << "Enter the person's last name: ";
            cin >> lastName;
            cout << endl;
            People[i][2] = firstName + "," + lastName;
            occupied[i][2] = true;

            cout << "Enter the person's Division ID: ";
            cin >> People[i][3];
            cout << endl;
            occupied[i][3] = true;

            cout << "Enter the person's Manager ID: ";
            cin >> People[i][4];
            cout << endl;
            occupied[i][4] = true;

            cout << "Enter the person's salary: ";
            cin >> People[i][5];
            cout << endl;
            occupied[i][5] = true;

            cout << "\nPerson added" << endl;
        }

        cout << "\nYou have succesfully added " << n << " people.\n" << endl;
        this_thread::sleep_for(chrono::seconds(2));
        
        int choice;
        do{
        clearScreen();
        cout << endl;    
        choice = 0;
        cout << "Would you like to add more people? 1 - (yes) 2 - (no)" << endl;
        cin >> choice;
        if (choice == 1)
            running = true;
        else if (choice == 2)
            running = false;
        else
        {
            cout << "\nInput invalid. Please enter only 1 or 2.\n" << endl;
            this_thread::sleep_for(chrono::seconds(2));
            continue;
        }
        } while ((choice != 1) && (choice != 2));
    }

    cout << "Returning to main menu..." << endl;
    this_thread::sleep_for(chrono::seconds(2));
    clearScreen();
}

void Display(string People[Max_People][columns], bool occupied[Max_People][columns])
{
    
    cout << endl
         << endl
         << left
         << setw(12)
         << "Position"
         << setw(15)
         << "ID Number"
         << setw(25)
         << "Name"
         << setw(15)
         << "Division ID"
         << setw(15)
         << "Manager ID"
         << setw(12)
         << "Salary" << endl;

    for (int i = 0; i < Max_People; ++i)
    {
        if (occupied[i][0])
            cout << left
                 << setw(12)
                 << People[i][0]
                 << setw(15)
                 << People[i][1]
                 << setw(25)
                 << People[i][2]
                 << setw(15)
                 << People[i][3]
                 << setw(15)
                 << People[i][4]
                 << setw(12)
                 << People[i][5] << endl;
    }
    
}

void removePerson(string People[Max_People][columns], bool occupied[Max_People][columns])
{
    bool running = true;

    while (running)
    {
        bool personFound = false;
        int listEndPosition = 0;
        string identificationNumber;

        cout << endl
             << "Enter the ID number of the person you want to remove? ";
        cin >> identificationNumber;
        cout << endl;

        for (int i = 0; i < Max_People; ++i)
        {
            if (identificationNumber == People[i][1])
            {
                for (int j = 1; j < columns; ++j)
                {
                    People[i][j] = "";
                }

                for (int n = i; n < Max_People - 1; ++n)
                {
                    for (int j = 1; j < columns; ++j)
                    {
                        People[n][j] = People[n + 1][j];
                    }
                }

                for (int n = 0; n < Max_People; ++n)
                {
                    if (occupied[n][0])
                        listEndPosition++;
                }

                personFound = true;
                break;
            }
        }

        if (!personFound)
            cout << "ID number not found." << endl;
        else
            cout << "Person has been removed.\n";

        if (listEndPosition > 0)
        {
            for (int j = 0; j < columns; ++j)
            {
                People[listEndPosition - 1][j] = "";
                occupied[listEndPosition - 1][j] = false;
            }
        }

        int choice;
        cout << "Would you like to remove another person? 1-(yes); 2-(no)\n";

        cin >> choice;
        if (choice == 1)
            running = true;
        else if (choice == 2)
            running = false;
        else
            cout << "\nInput invalid. Please type only 1 or 2." << endl;
    }

    cout << "Returning to main menu..." << endl;
    this_thread::sleep_for(chrono::seconds(2));
    clearScreen();
}

void highestEarner(string People[Max_People][columns], bool occupied[Max_People][columns])
{
    string divisionHighest[Max_People][columns] = {};
    string totalHighest[columns] = {};
    string uniqueDivisions[Max_People] = {};

    int numberOfDivisions = 1;

    divisionHighest[0][5] = "0";
    totalHighest[5] = "0";

    if (!occupied[0][0])
    {
        cout << "Database is empty.\n";
        return;
    }

    uniqueDivisions[0] = People[0][3];

    for (int i = 0; i < Max_People; ++i)
    {
        if (!occupied[i][0])
            break;

        bool divisionAlreadyExists = false;
        bool uniqueDivisionMatch = false;

        for (int n = 0; n < numberOfDivisions; ++n)
        {
            if (stoi(People[i][3]) == stoi(uniqueDivisions[n]))
            {
                divisionAlreadyExists = true;
                break;
            }
        }

        if (!divisionAlreadyExists)
        {
            uniqueDivisions[numberOfDivisions] = People[i][3];
            divisionHighest[numberOfDivisions][5] = "0";
            numberOfDivisions++;
        }

        if (stoi(People[i][5]) > stoi(totalHighest[5]))
        {
            for (int j = 0; j < columns; ++j)
                totalHighest[j] = People[i][j];
        }

        for (int n = 0; n < numberOfDivisions; ++n)
        {
            if (stoi(People[i][3]) == stoi(uniqueDivisions[n]))
            {
                uniqueDivisionMatch = true;
                if (uniqueDivisionMatch)
                {
                    if (stoi(People[i][5]) > stoi(divisionHighest[n][5]))
                    {
                        for (int j = 0; j < columns; ++j)
                        {
                            divisionHighest[n][j] = People[i][j];
                        }
                    }
                }
            }
        }
    }

    cout << "Would you like to see the highest earners in each division and in total? (Y/N) ";

    string choiceDisplay;
    cin >> choiceDisplay;
    cout << "\n";

    if ((choiceDisplay == "y") || (choiceDisplay == "Y"))
        choiceDisplay = "1";
    else if ((choiceDisplay == "n") || (choiceDisplay == "N"))
        choiceDisplay = "0";
    else
        choiceDisplay = "5";

    switch (stoi(choiceDisplay))
    {
        case YES:
        {
            cout << "\nRetrieving results...\n"
                 << endl;
            this_thread::sleep_for(chrono::seconds(2));
            cout << "The highest total earner is " << totalHighest[2]
                 << " with a salary of $" << totalHighest[5] << ".\n"
                 << endl;

            int i = 0;
            string dummy;
            while ((i < Max_People) && (divisionHighest[i][5] != ""))
            {
                cout << "The highest earner in division " << divisionHighest[i][3]
                     << " is " << divisionHighest[i][2]
                     << " with a salary of $" << divisionHighest[i][5] << ".\n"
                     << endl;
                i++;
            }

            cout << "\nPress ENTER whenever you want to leave this screen and continue: ";
            cin.ignore();
            getline(cin, dummy);
            cout << " transitioning..." << endl;
            this_thread::sleep_for(chrono::seconds(2));
            clearScreen();
            break;
        }
        case NO:
            cout << "\nReturning to main menu..." << endl;
            this_thread::sleep_for(chrono::seconds(2));
            clearScreen();
            break;
        default:
            cout << "Input invalid. Please type in only y or n.\n"
                 << endl;
            break;
    }
}

void averageSalary(string People[Max_People][columns], bool occupied[Max_People][columns])
{
    string uniqueDivisionSalaries[Max_People][3] = {};
    uniqueDivisionSalaries[0][0] = People[0][3];
    uniqueDivisionSalaries[0][1] = "0";
    uniqueDivisionSalaries[0][2] = "0";

    int numberOfDivisions = 1;
    int totalSalary = 0;
    int numberOfPeople = 0;

    if (!occupied[0][0])
    {
        cout << "Database is empty.\n";
        return;
    }

    for (int i = 0; i < Max_People; ++i)
    {
        if (!occupied[i][0])
            break;

        numberOfPeople++;

        bool divisionAlreadyExists = false;
        for (int n = 0; n < numberOfDivisions; ++n)
        {
            if (stoi(uniqueDivisionSalaries[n][0]) == stoi(People[i][3]))
            {
                uniqueDivisionSalaries[n][2] = to_string(stoi(uniqueDivisionSalaries[n][2]) + 1);
            }
            if (stoi(uniqueDivisionSalaries[n][0]) == stoi(People[i][3]))
            {
                divisionAlreadyExists = true;
                break;
            }
        }

        if (!divisionAlreadyExists)
        {
            uniqueDivisionSalaries[numberOfDivisions][0] = People[i][3];
            uniqueDivisionSalaries[numberOfDivisions][1] = "0";
            uniqueDivisionSalaries[numberOfDivisions][2] = "1";
            numberOfDivisions++;
        }

        for (int j = 0; j < numberOfDivisions; ++j)
        {
            if (stoi(uniqueDivisionSalaries[j][0]) == stoi(People[i][3]))
            {
                uniqueDivisionSalaries[j][1] = to_string(stoi(uniqueDivisionSalaries[j][1]) + stoi(People[i][5]));
            }
        }

        totalSalary = totalSalary + stoi(People[i][5]);
    }

    cout << "The average salary across all division is $"
         << (totalSalary * 1.00) / (numberOfPeople * 1.00) << ".\n";

    for (int i = 0; i < numberOfDivisions; ++i)
    {
        cout << "\nIn Division " << uniqueDivisionSalaries[i][0]
             << " the average is "
             << (stoi(uniqueDivisionSalaries[i][1]) * 1.00) /
                    (stoi(uniqueDivisionSalaries[i][2]) * 1.00)
             << ".\n";
    }

    string dummy;
    cout << "Press ENTER when you want to return to the main menu. ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, dummy);
    cout << "Returning to main menu..." << endl;
    this_thread::sleep_for(chrono::seconds(2));
    clearScreen();
}

void budgetCheck(string People[Max_People][columns], bool occupied[Max_People][columns])
{
    string uniqueDivisionSalaries[Max_People][4] = {};
    uniqueDivisionSalaries[0][0] = People[0][3]; // division number
    uniqueDivisionSalaries[0][1] = "0";          // total salary
    uniqueDivisionSalaries[0][2] = "0";          // number of people
    uniqueDivisionSalaries[0][3] = "0";          // highest salary

    int numberOfDivisions = 1;

    if (!occupied[0][0])
    {
        cout << "Database is empty.\n";
        return;
    }

    for (int i = 0; i < Max_People; ++i)
    {
        if (!occupied[i][0])
            break;

        bool divisionAlreadyExists = false;
        for (int n = 0; n < numberOfDivisions; ++n)
        {
            if (stoi(uniqueDivisionSalaries[n][0]) == stoi(People[i][3]))
            {
                uniqueDivisionSalaries[n][2] = to_string(stoi(uniqueDivisionSalaries[n][2]) + 1);
            }
            if (stoi(uniqueDivisionSalaries[n][0]) == stoi(People[i][3]))
            {
                divisionAlreadyExists = true;
                break;
            }
        }

        if (!divisionAlreadyExists)
        {
            uniqueDivisionSalaries[numberOfDivisions][0] = People[i][3];
            uniqueDivisionSalaries[numberOfDivisions][1] = "0";
            uniqueDivisionSalaries[numberOfDivisions][2] = "1";
            uniqueDivisionSalaries[numberOfDivisions][3] = People[i][5];
            numberOfDivisions++;
        }

        for (int j = 0; j < numberOfDivisions; ++j)
        {
            if (stoi(uniqueDivisionSalaries[j][0]) == stoi(People[i][3]))
            {
                uniqueDivisionSalaries[j][1] = to_string(stoi(uniqueDivisionSalaries[j][1]) + stoi(People[i][5]));

                if (stoi(People[i][5]) > stoi(uniqueDivisionSalaries[j][3]))
                {
                    uniqueDivisionSalaries[j][3] = People[i][5];
                }
            }
        }
    }

    for (int i = 0; i < numberOfDivisions; ++i)
    {
        int highestSalary = stoi(uniqueDivisionSalaries[i][3]);
        int divisionTotal = stoi(uniqueDivisionSalaries[i][1]);

        if (2 * highestSalary < divisionTotal)
        {
            cout << "\nDivision # " << uniqueDivisionSalaries[i][0]
                 << " is budget optimized."
                 << "\nHighest salary: $" << highestSalary
                 << "\nDivision total salary: $" << divisionTotal << endl;
        }
        else
        {
            cout << "\nDivision # " << uniqueDivisionSalaries[i][0]
                 << " is NOT budget optimized."
                 << "\nHighest salary: $" << highestSalary
                 << "\nDivision total salary: $" << divisionTotal << endl;
        }
    }

    string dummy;
    cout << "Press ENTER when you want to return to the main menu. ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, dummy);
    cout << "Returning to main menu..." << endl;
    this_thread::sleep_for(chrono::seconds(2));
    clearScreen();
}


void sortDatabase(string People[Max_People][columns], bool occupied[Max_People][columns])
{
    
    bool running = true;
    while (running)
    {
        cout << "Sorting database..." << endl;
        this_thread::sleep_for(chrono::seconds(2));
        clearScreen();
        
        if (!occupied[0][0])
        {
            cout << "Database is empty.\n";
            this_thread::sleep_for(chrono::seconds(2));
            clearScreen();
            return;
        }
        
        string duplicateDatabase[Max_People][columns] = {};
        bool personUsed[Max_People] = {false};
    
        int choice;
        int numberOfPeople = 0;
        char sortAgain = '0';
        
        clearScreen();
    
        cout << "\n";
        cout << "How do you want to sort the list?\n";
        cout << "1. Identification Order\n";
        cout << "2. Alphabetical Order\n";
        cout << "3. Division Order\n";
        cout << "4. Manager Identification Order\n";
        cout << "5. Salary Order\n";
        cout << "Choice: ";
    
        cin >> choice;
    
        if (cin.fail())
        {
            cout << "Input invalid. Please enter a number from 1 to 5.\n";
        
            cin.clear();
            cin.ignore(1000, '\n');
            continue;
        }
    
        
        while (numberOfPeople < Max_People && occupied[numberOfPeople][0])
        {
            numberOfPeople++;
        }
        if (choice == 1)
        {
            cout << "\nHere is the Employee ID sorted list...\n";
        
            int IDindex = 0;
        
            while (IDindex < numberOfPeople)
            {
                int lowestIndex = -1;
        
                for (int i = 0; i < numberOfPeople; ++i)
                {
                    if (personUsed[i])
                        continue;
                    
        
                    if (lowestIndex == -1 || stoi(People[i][1]) < stoi(People[lowestIndex][1]))
                        lowestIndex = i;
                    
                }
        
                duplicateDatabase[IDindex][0] = to_string(IDindex + 1);
        
                for (int j = 1; j < columns; ++j)
                    duplicateDatabase[IDindex][j] = People[lowestIndex][j];
                
        
                personUsed[lowestIndex] = true;
                IDindex++;
            }
        }
        else if (choice == 2)
        {
            cout << "\nHere is the alphabetically sorted list...\n";
            
            int ABCindex = 0;
            
            while (ABCindex < numberOfPeople)
            {
                int lowestIndex = -1;
                
                    for (int i = 0; i < numberOfPeople; ++i)
                    {
                            if (personUsed[i])
                                continue;
                                
                            size_t commaI = People[i][2].find(',');
                            string lastNameI = People[i][2].substr(commaI + 1);    
                                
                            if (lowestIndex == -1)
                                lowestIndex = i;
                                
                            else 
                            {
                                
                                size_t commaLowest = People[lowestIndex][2].find(',');
                                string lowestLastName = People[lowestIndex][2].substr(commaLowest + 1);
                
                                if (lastNameI < lowestLastName)
                                    lowestIndex = i;
                                
                            }
                                
                    }
                    
                    duplicateDatabase[ABCindex][0] = to_string(ABCindex + 1);
                    
                    for (int j = 1; j < columns; ++j)
                        duplicateDatabase[ABCindex][j] = People[lowestIndex][j];
                    
                    personUsed[lowestIndex] = true;
                    ABCindex++;
            }
        }
        else if (choice == 3)
        {
            cout << "\nHere is the Division ID sorted list...\n";
    
            int duplicateIndex = 0;
    
            while (duplicateIndex < numberOfPeople)
            {
                int lowestIndex = -1;
    
                for (int i = 0; i < numberOfPeople; ++i)
                {
                    if (personUsed[i])
                        continue;
    
                    if (lowestIndex == -1)
                        lowestIndex = i;
                    
                    else if (stoi(People[i][3]) < stoi(People[lowestIndex][3]))
                        lowestIndex = i;
                }
    
                duplicateDatabase[duplicateIndex][0] = to_string(duplicateIndex + 1);
    
                for (int j = 1; j < columns; ++j)
                {
                    duplicateDatabase[duplicateIndex][j] = People[lowestIndex][j];
                }
    
                personUsed[lowestIndex] = true;
                duplicateIndex++;
            }
    
        }
        else if (choice == 4)
        {
            cout << "\nHere is the Manager ID sorted list...\n";
            
            int managerIDIndex = 0;
            
            while (managerIDIndex < numberOfPeople)
            {
                int lowestIndex = -1;
    
                for (int i = 0; i < numberOfPeople; ++i)
                {
                    if (personUsed[i])
                        continue;
    
                    if (lowestIndex == -1)
                        lowestIndex = i;
                    
                    else if (stoi(People[i][4]) < stoi(People[lowestIndex][4]))
                        lowestIndex = i;
                }
    
                duplicateDatabase[managerIDIndex][0] = to_string(managerIDIndex + 1);
    
                for (int j = 1; j < columns; ++j)
                    duplicateDatabase[managerIDIndex][j] = People[lowestIndex][j];
    
                personUsed[lowestIndex] = true;
                managerIDIndex++;
            }
            
        }
        else if (choice == 5)
        {
            cout << "\nHere is the salary sorted list...\n";
            
            int salaryIndex = 0;
            
            while (salaryIndex < numberOfPeople)
            {
                int lowestIndex = -1;
    
                for (int i = 0; i < numberOfPeople; ++i)
                {
                    if (personUsed[i])
                        continue;
    
                    if (lowestIndex == -1)
                        lowestIndex = i;
                    
                    else if (stoi(People[i][5]) < stoi(People[lowestIndex][5]))
                        lowestIndex = i;
                }
    
                duplicateDatabase[salaryIndex][0] = to_string(salaryIndex + 1);
    
                for (int j = 1; j < columns; ++j)
                    duplicateDatabase[salaryIndex][j] = People[lowestIndex][j];
    
                personUsed[lowestIndex] = true;
                salaryIndex++;
            }
            
        }
        else
        {
            cout << "Input invalid. Please only enter the keys 1,2,3,4 or 5.\n";
            continue;
        }
        
        for (int i = 0; i < numberOfPeople; ++i)
        {
            for (int j = 0; j < columns; ++j)
            {
                People[i][j] = duplicateDatabase[i][j];
            }
        }
        
        this_thread::sleep_for(chrono::seconds(2));
        clearScreen();
        
        cout << "\nHere are your database results:\n" << endl;
        
        Display(People, occupied);
        
        do 
        {
        cout << "Would you like to sort the database again? Y/N "; cin >> sortAgain; cout << "\n";
        if (toupper(sortAgain) == 'Y')
            running = true;
        else if (toupper(sortAgain) == 'N')
            running = false;
        else
            cout << "Input invalid. Please select only Y or N.\n";
        } while ((toupper(sortAgain) != 'Y')&&(toupper(sortAgain) != 'N'));
        
    }
    
        cout << "Returning to main menu..." << endl;
        this_thread::sleep_for(chrono::seconds(2));
        clearScreen();
}



int main()
{
    string People[Max_People][columns];
    bool occupied[Max_People][columns] = {false};
    int choiceMenu;

    bool running = true;
    string dummy;

    cout << "Welcome to the employee database. In this database, you will see the full list of employees along with their salary, ID number, Division ID, and Manager ID.\n "
         << endl;
    cout << "Press ENTER to coninue...";

    getline(cin, dummy);
    cout << "\nEntering main menu..." << endl;
    this_thread::sleep_for(chrono::seconds(2));

    clearScreen();

    while (running)
    {
        displayMenu();

        cin >> choiceMenu;
        cout << endl;

        if (cin.fail())
        {
            cout << "Input is invalid.\n";
            this_thread::sleep_for(chrono::seconds(2));
            cin.clear();
            cin.ignore(1000, '\n');
            clearScreen();
            continue;
        }

        switch (choiceMenu)
        {
        case ADD:
            clearScreen();
            addPeople(People, occupied);
            break;
        case REMOVE:
            clearScreen();
            removePerson(People, occupied);
            break;
        case DISPLAY:
            cout << "Loading up database for display...\n";
            this_thread::sleep_for(chrono::seconds(2));
            Display(People, occupied);
            cout << "\nPress ENTER whenever you want to return to the main menu.";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            getline(cin, dummy);
            cout << "\nReturning to main menu..." << endl;
            this_thread::sleep_for(chrono::seconds(2));
            clearScreen();
            break;
        case EXPORT:
            fileExport(People, occupied);
            break;
        case IMPORT:
            fileImport(People, occupied);
            break;
        case HIGHEST_EARNER:
            cout << "\nRetrieving data on the highest earners...\n";
            this_thread::sleep_for(chrono::seconds(2));
            highestEarner(People, occupied);
            break;
        case AVERAGE_SALARY:
            cout << "\nRetrieving data on the salary averages...\n";
            this_thread::sleep_for(chrono::seconds(2));
            averageSalary(People, occupied);
            break;
        case BUDGET_CHECK:
            cout << "\nRetireiving data on the division budgets...\n";
            this_thread::sleep_for(chrono::seconds(2));
            budgetCheck(People, occupied);
            break;
        case SORT:
            sortDatabase(People, occupied);
            break;
        case EXIT:
            running = false;
            cout << "Closing database..." << endl;
            this_thread::sleep_for(chrono::seconds(2));
            clearScreen();
            cout << "Database closed. Have a good day." << endl;
            break;
        default:
            cout << "Input is invalid.\n ";
            this_thread::sleep_for(chrono::seconds(2));
            clearScreen();
            break;
        }
    }

    return 0;
}