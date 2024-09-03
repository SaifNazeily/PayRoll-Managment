#include <iostream>
#include <cstdlib>
#include <vector>
#include <string>
#include <map>
#include <fstream>

using namespace std;

// Base class for all employees
class Employee {
protected:
    string name;
    int id;
public:
    Employee(string name, int id) : name(name), id(id) {}

    // Pure virtual function to calculate pay, to be overridden by derived classes
    virtual double calculatePay() = 0;

    // Virtual function to display employee information
    virtual void displayInfo() {
        cout << "ID: " << id << ", Name: " << name;
    }

    // Getter function for employee ID
    int getID() const {
        return id;
    }
};

// Hourly employee class
class HourlyEmployee : public Employee {
    double hourlyRate;
    double hoursWorked;
public:
    // Constructor
    HourlyEmployee(string name, int id, double hourlyRate, double hoursWorked)
        : Employee(name, id), hourlyRate(hourlyRate), hoursWorked(hoursWorked) {}

    // Implementation of pure virtual function to calculate pay
    double calculatePay() override {
        return hourlyRate * hoursWorked;
    }

    // Function to calculate monthly pay
    double calculateMonthlyPay() {
        // Assuming 4 weeks in a month
        return calculatePay() * 4;
    }

    // Function to display employee information, overridden from base class
    void displayInfo() override {
        Employee::displayInfo();
        cout << ", Type: Hourly Employee, Monthly Pay: $" << calculateMonthlyPay() << endl;
    }

    // Function to calculate pay for given hours
    double calculatePayForHours(double hours) {
        return hourlyRate * hours;
    }
};

// Salaried employee class
class SalariedEmployee : public Employee {
    double salary;
public:
    // Constructor
    SalariedEmployee(string name, int id, double salary)
        : Employee(name, id), salary(salary) {}

    // Implementation of pure virtual function to calculate pay
    double calculatePay() override {
        return salary;
    }

    // Function to display employee information, overridden from base class
    void displayInfo() override {
        Employee::displayInfo();
        cout << ", Type: Salaried Employee, Monthly Pay: $" << calculatePay() << endl;
    }
};

// Function to verify login credentials
bool verifyLogin(const string& username, const string& password) {
    return (username == "admin" && password == "password");
}

// Forward declaration of the saveData function
void saveData(const map<int, Employee*>& employees, const string& filename);

// Function to add an hourly employee
void addHourlyEmployee(map<int, Employee*>& employees) {
    string name;
    int id;
    double hourlyRate;
    double hoursWorked;

    cout << "Enter name: ";
    cin >> name;
    cout << "Enter ID: ";
    cin >> id;
    cout << "Enter hourly rate: $";
    cin >> hourlyRate;
    cout << "Enter hours worked: ";
    cin >> hoursWorked;

    // Create a new HourlyEmployee object and add it to the map
    employees[id] = new HourlyEmployee(name, id, hourlyRate, hoursWorked);
    cout << "Hourly employee added successfully!\n";
}

// Function to add a salaried employee
void addSalariedEmployee(map<int, Employee*>& employees) {
    string name;
    int id;
    double salary;

    cout << "Enter name: ";
    cin >> name;
    cout << "Enter ID: ";
    cin >> id;
    cout << "Enter salary: $";
    cin >> salary;

    // Create a new SalariedEmployee object and add it to the map
    employees[id] = new SalariedEmployee(name, id, salary);
    cout << "Salaried employee added successfully!\n";
}

// Function to remove an employee
void removeEmployee(map<int, Employee*>& employees) {
    int id;
    cout << "Enter employee ID to remove: ";
    cin >> id;

    auto it = employees.find(id);
    if (it != employees.end()) {
        delete it->second; // Free memory allocated to the employee
        employees.erase(it); // Remove employee from the map
        cout << "Employee with ID " << id << " removed successfully!\n";
    }
    else {
        cout << "Employee not found.\n";
    }
}

// Function to display all employees and their monthly pay
void displayMonthlyReport(const map<int, Employee*>& employees) {
    if (employees.empty()) {
        cout << "No employees added yet.\n";
        return;
    }
    cout << "Monthly Report:\n";
    for (const auto& emp : employees) {
        emp.second->displayInfo();
    }
}

// Function to calculate pay for given hours for an hourly employee
void calculatePayForHours(const map<int, Employee*>& employees) {
    int id;
    double hours;
    cout << "Enter employee ID: ";
    cin >> id;
    auto it = employees.find(id);
    if (it != employees.end() && dynamic_cast<HourlyEmployee*>(it->second)) {
        HourlyEmployee* hourlyEmp = dynamic_cast<HourlyEmployee*>(it->second);
        cout << "Enter hours worked: ";
        cin >> hours;
        cout << "Pay for " << hours << " hours: $" << hourlyEmp->calculatePayForHours(hours) << endl;
    }
    else {
        cout << "Employee not found or is not hourly employee.\n";
    }
}

// Function to save employees data to a file
void saveData(const map<int, Employee*>& employees, const string& filename) {
    ofstream file(filename);
    if (file.is_open()) {
        for (const auto& emp : employees) {
            file << emp.second->getID() << "," << emp.second->calculatePay() << endl;
        }
        file.close();
        cout << "Data saved successfully.\n";
    }
    else {
        cout << "Unable to open file.\n";
    }
}

int main() {
    string username, password;
    cout << "Enter username: ";
    cin >> username;
    cout << "Enter password: ";
    cin >> password;

    // Verify login credentials
    if (!verifyLogin(username, password)) {
        cout << "Invalid username or password. Exiting...\n";
        return 1;
    }

    // Create a map to store employees
    map<int, Employee*> employees;
    char choice;
    string filename = "employee_data.txt"; // File to save/load employee data

    // Main menu loop
    do {
        system("cls"); // for Windows, use "cls"; for Unix-like systems (Linux, macOS), use "clear"
        cout << "\n=== Payroll Management System ===\n";
        cout << "1. Add Hourly Employee\n";
        cout << "2. Add Salaried Employee\n";
        cout << "3. Remove Employee\n";
        cout << "4. Display Monthly Report\n";
        cout << "5. Calculate Pay for Hours Worked\n";
        cout << "6. Save Data\n";
        cout << "7. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case '1':
            addHourlyEmployee(employees);
            break;
        case '2':
            addSalariedEmployee(employees);
            break;
        case '3':
            removeEmployee(employees);
            break;
        case '4':
            displayMonthlyReport(employees);
            break;
        case '5':
            calculatePayForHours(employees);
            break;
        case '6':
            saveData(employees, filename);
            break;
        case '7':
            cout << "Exiting...\n";
            break;
        default:
            cout << "Invalid choice. Please try again.\n";
        }

        cout << "Press Enter to continue...";
        cin.ignore(); // Clear input buffer
        cin.get();    // Wait for Enter key
    } while (choice != '7');

    // Free memory allocated for employees
    for (auto& emp : employees) {
        delete emp.second;
    }

    return 0;
}