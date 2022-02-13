#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <string.h>
#include <conio.h>
#include <filesystem>
#include "Vehicle.h"
#define shift setfill(char(205))<< setw
namespace fs = std::filesystem;
using namespace std;
enum class CredType { admin, sales, client };
enum class Month { January = 1, February = 2, March = 3, April = 4, May = 5, June = 6, July = 7, August = 8, September = 9, October = 10, November = 11, December = 12, JAN = 1, FEB = 2, MAR = 3, APR = 4, MAY = 5, JUN = 6, JUL = 7, AUG = 8, SEP = 9, OCT = 10, NOV = 11, DEC = 12 };
enum class gender { Male = 'M', Female = 'F', Other = 'O' };
class Manager;
class Client;
class Sales;
class Profile;
//                                          Credential here
class Credential {
protected:
    friend Profile;
    char Username[20], Password[20];
public:
    CredType type;
    friend istream& operator>>(istream&, Credential&);
    inline bool operator ==(Credential);
    inline bool operator ==(string);
    //Created for purpose of debugging
    void viewCred() {
        cout << "UName " << Username << "  Pass " << Password << endl;
    }
};
bool Credential::operator==(Credential c) {
    if (string(Username) == string(c.Username) && string(Password) == string(c.Password))
        return true;
    else return false;
}
bool Credential::operator==(string pass) {
    if (pass == Username)
        return true;
    return false;
}
istream& operator>>(istream& in, Credential& crd) {
    cout << '\n' << shift(30) << "Enter username and password"
        << '\n' << shift(30) << "Username :";
    in >> crd.Username;
    cout << shift(30) << "Password :";
    in >> crd.Password;
    return in;
}
//                                              Date here
class Date {
    int Day, Year;
    Month month;
public:
    friend istream& operator>>(istream&, Date&);
    friend ostream& operator<<(ostream&, Date&);
};
//                                                          Profile here
class Profile : protected Credential {
protected:
    char FirstName[15], MiddleName[15], Surname[15];
    unsigned long long int ContactNo;
    gender Gender;
    Date dob;
    void ViewData();
    void ViewCars();
public:
    void SetData(CredType);
    virtual void Menu(fstream& out) = 0;    //pure virtual function
};
//                                              Profile SetData
void Profile::SetData(CredType type) {
    cout << shift(25) << "Details for ";
    string s =
        (type == CredType::admin) ? "Admin" : (type == CredType::sales) ? "Sales" : "Client";
    cout << s << "\n";
    Profile::type = type;
    cout << setw(30) << setfill('=') << right << "Enter new username and password\n\tUsername : ";
    cin >> Username;
    cout << "\tPassword : ";
    cin >> Password;
    system("cls");
    char g;
    cout << "\nName :-\n\tFirst Name : ";
    cin >> FirstName;
    cout << "\tMiddle Name : ";
    cin >> MiddleName;
    cout << "\tSurname : ";
    cin >> Surname;
    cout << "Gender(M/F/O) : ";
    cin >> g;
    (g == 'M' || g == 'm') ? Gender = gender::Male : (g == 'F' || g == 'f') ? Gender = gender::Female : Gender = gender::Other;
    cout << "DOB(dd/mm/yyy): ";
    cin >> dob;
    cout << "Contact Number: ";
    cin >> ContactNo;
}
//                                                Profile View data
void Profile::ViewData() {
    string g;
    cout << "\nName :-\n\tFirst Name : " << FirstName;
    cout << "\n\tMiddle Name : " << MiddleName;
    cout << "\n\tSurname : " << Surname;
    cout << "\nGender: ";
    (Gender == gender::Male) ? g = "Male" : (Gender == gender::Female) ? g = "Female" : g = "Other";
    cout << g;
    cout << "\nDOB(dd/mm/yyy): ";
    cout << dob;
    cout << "\nContact Number: " << ContactNo << endl;
}
void Profile::ViewCars() {
	car CarObj;
	if (fs::exists("CarSheet.csv")) {
		ifstream in("CarSheet.csv");
		in.ignore(35, '\n');
		//cout.setf(ios::adjustfield, ios::left);
        cout << setiosflags(ios::left)
            << setfill(' ') << '\n'
			<< setw(6) << "S.No."
			<< setw(12) << "Name"
			<< setw(12) << "Company"
			<< setw(12) << "Colour"
			<< setw(12) << "Type"
			<< setw(12) << "Price " << '\n';
		int Count = 0;
		while (in >> CarObj) {
			cout << setw(6) << ++Count << CarObj;
		}
        cout << resetiosflags(ios::left);
	}
	else cout << "No data could be found\n";
	cout << '\n';
	system("pause");
	system("cls");
}
istream& operator>>(istream& in, Date& dob) {
    string dd, mm, yyyy;
    while (!isdigit(in.peek()))
        in.ignore();
    in >> setw(2) >> dd;
    while (!isdigit(in.peek()))
        in.ignore();
    in >> setw(2) >> mm;
    while (!isdigit(in.peek()))
        in.ignore();
    in >> setw(4) >> yyyy;
    try {
        dob.Day = stoi(dd);
        dob.month = (Month)stoi(mm);
        dob.Year = stoi(yyyy);
    }
    catch (const std::invalid_argument& ia) {
        std::cerr << "Invalid argument: " << ia.what() << '\n';
        cout << "Please re-enter date\n";
        cin >> dob;
    }
    return in;
}
ostream& operator<<(ostream& out, Date& dob) {
    out << setw(2) << setfill('0') << dob.Day << '/' << setw(2) << setfill('0') << (int)dob.month << '/' << setw(4) << setfill('0') << dob.Year;
    return out;
}
class Client :public Profile {
public:
    virtual void Menu(fstream& out);
};
class Sales :public Profile {
public:
    virtual void Menu(fstream& out);
};
class Manager :public Profile{
public:
    virtual void Menu(fstream&);
    void WriteProfile(fstream&);
    void DeleteProfile(fstream&);
    void AddCar(car&);
};
void Client::Menu(fstream& out) {
    char ch;
    car CarObj;
    do {
        system("cls");
        cout << shift(25) << "Client Menu\n"
            << shift(30) << "1-View profile data\n"
            << shift(30) << "2-View available cars\n"
            << shift(30) << "Esc-EXIT\n";
        switch (ch = _getch()) {
        case '1':
            ViewData();
            system("pause");
            system("cls");
            break;
        case '2':
            system("pause");
            system("cls");
            ViewCars();
            break;
        case 27:
            system("pause");
            system("cls");
        }
    } while (ch != 27);
}
void Sales::Menu(fstream& out) {
    char ch;
    car CarObj;
    do {
        system("cls");
        cout << shift(25) << "Sales Menu\n"
            << shift(30) << "1-View profile data\n"
            << shift(30) << "2-View available cars\n"
            << shift(30) << "3-Sell car\n"
            << shift(30) << "Esc-EXIT\n";
        switch (ch = _getch()) {
        case '1':
            ViewData();
            break;
        case '2':
            ViewCars();
            break;
        case '3':
            DelCar();
            break;
        case 27:
            system("pause");
            system("cls");
        }
    } while (ch != 27);
}
void Manager::Menu(fstream& out) {
    char ch = 0;
    car CarObj;
    do {
        system("cls");
        cout << shift(25) << "Manager Menu\n"
            << shift(30) << "1-View profile data\n"
            << shift(30) << "2-Add new profile\n"
            << shift(30) << "3-Delete Profile\n"
            << shift(30) << "4-Add Car entry\n"
            << shift(30) << "5-View available cars\n"
            << shift(30) << "6-Delete entry\n"
            << shift(30) << "Esc-EXIT\n";
        ch = _getch();
        if(ch!=char(27))
            cout << '\n' << shift(25) << ch << " selected\n";
        switch (ch) {
        case '1':
            ViewData();
            break;
        case '2':
            WriteProfile(out);
            break;
        case '3':
            DeleteProfile(out);
            break;
        case '4':
            CarObj.SetData();
            AddCar(CarObj);
            break;
        case '5':
            ViewCars();
            break;
        case '6':
            DelCar();
            break;
        case 27:
            cout << "Exiting...\n";
            break;
        }
    } while (ch != 27);
}
void Manager::WriteProfile(fstream& out) {
    Profile* P;
    cout << shift(25) << "Choose profile type\n"
        << shift(30) << "1-Admin\n"
        << shift(30) << "2-Sales\n"
        << shift(30) << "3-Client\n"
        << shift(30) << "ESC-EXIT\n";
    bool success = true;
    char ch=_getch();
    cout << '\n' << shift(25) << ch << " selected\n";
    switch (ch) {
    case '1':
        out.seekp(0, ios::end);
        P = new Manager;
        P->SetData(CredType::admin);
        out.write((char*)P + 8, sizeof(Profile) - 8);
        break;
    case '2':
        out.seekp(0, ios::end);
        P = new Sales;
        P->SetData(CredType::sales);
        out.write((char*)P + 8, sizeof(Profile) - 8);
        break;
    case '3':
        out.seekp(0, ios::end);
        P = new Client;
        P->SetData(CredType::client);
        out.write((char*)P + 8, sizeof(Profile) - 8);
        break;
    default:
        cout << "Wrong input!\n";
    case 27:
        success = false;
        cout << "Exiting...";
        system("pause");
        system("cls");
        return;
    }
    if (success) {
        cout << "Data written!...";
        system("pause");
        system("cls");
    }
}
void Manager::DeleteProfile(fstream& out) {
    Profile* TempP;
    cout << "Enter username of profile to be deleted\n";
    string pass;
    Credential crdf;	//credentials from input and credentials form file
    cin >> pass;
    out.seekg(0, ios::beg);
    int DelBeg, DelEnd;
    bool Verified = false;
    do {
        out.read((char*)&crdf, sizeof(Credential));
        out.seekg(-((int)sizeof(Credential)), ios::cur);
        if (crdf == pass) {
            DelBeg = out.tellg();
            DelEnd = DelBeg + sizeof(Profile) - 8;
            cout << "Profile found...\nConfirm Deletion(Y/N)...\n";
            char yn = _getch();
            if (yn == 'Y' || yn == 'y')
                Verified = true;
            else {
                cout << "\nProfile deletion unsuccessful!\n";
                system("pause");
                return;
            }
            break;
        }
        out.seekg(sizeof(Profile) - 8, ios::cur);
    } while (out.peek() != EOF);
    if (Verified == true) {
        TempP = new Sales;
        out.seekg(0, ios::beg);
        fstream temp("TempCredentials.dat", ios::binary | ios::out | ios::trunc);
        while (out.tellp() != DelBeg){
            out.read((char*)TempP + 8, sizeof(Profile) - 8);
            temp.write((char*)TempP + 8, sizeof(Profile) - 8);
        }
        out.seekg(DelEnd, ios::beg);
        while (out.peek() != EOF) {
            out.read((char*)TempP + 8, sizeof(Profile) - 8);
            temp.write((char*)TempP + 8, sizeof(Profile) - 8);
        }
        out.close();
        temp.close();
        fs::copy_file("TempCredentials.dat", "Credentials.dat", fs::copy_options::overwrite_existing);
        fs::remove("TempCredentials.dat");
        out.open("Credentials.dat", ios::binary | ios::in | ios::out);
        cout << "Profile deleted successfully!\n";
    }
    else {
        cout << "Profile could not be found";
    }
    system("pause");
}
void Manager::AddCar(car& car) {
    if (!fs::exists("CarSheet.csv")) {
        ofstream out("CarSheet.csv");
        out << "Name ,Company ,Colour ,Type ,Price\n";
        out.close();
    }
    ofstream out("CarSheet.csv", ios::app);
    out << car;
    out.close();
}