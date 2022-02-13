#pragma once
#include <iostream>
#include <string>
#include "Credential.h"
#include <filesystem>
#define shift setfill(char(205))<< setw
namespace fs = std::filesystem;
using namespace std;
class car {
	string Name, Company, Colour, Type;
	float Price;
public:
	void SetData();
	void ViewData();
	friend ostream& operator<<(ostream&, car&);
	friend ofstream& operator <<(ofstream&, car&);
	friend ifstream& operator >>(ifstream&, car&);
};
void car::SetData() {
	cout << shift(30) << "Enter details for this vehicle:-\n";
	cout << shift(25) << "Name : ";
	cin >> Name;
	cout << shift(25) << "Company : ";
	cin >> Company;
	cout << shift(25) << "Colour : ";
	cin >> Colour;
	cout << shift(25) << "Type : ";
	cin >> Type;
	cout << shift(25) << "Price : ";
	cin >> Price;
	cout << "Data recorded...";
	system("pause");
	system("cls");
}
inline void car::ViewData() {
	//Written for purpose of debugging
	cout << shift(30) << "Details for this vehicle:-\n"
		<< shift(25) << "Name : " << Name << '\n'
		<< shift(25) << "Company : " << Company << '\n'
		<< shift(25) << "Colour : " << Colour << '\n'
		<< shift(25) << "Type : " << Type << '\n'
		<< shift(25) << "Price : " << Price << '\n'
		<< "Data recorded...";
	system("pause");
	system("cls");
}
ostream& operator <<(ostream& out, car& CarObj) {
	out << setiosflags(ios::fixed) << setprecision(2)
		<< setw(12) << CarObj.Name
		<< setw(12) << CarObj.Company
		<< setw(12) << CarObj.Colour
		<< setw(12) << CarObj.Type
		<< setw(12) << CarObj.Price
		<< '\n';
	return out;
}
ofstream& operator <<(ofstream& out, car& CarObj) {
	out << CarObj.Name << " ," << CarObj.Company << " ," << CarObj.Colour << " ," << CarObj.Type << " ," << CarObj.Price << " \n";
	return out;
}
ifstream& operator >>(ifstream& in, car& CarObj) {
	in >> CarObj.Name;
	in.ignore(5, ',');
	in >> CarObj.Company;
	in.ignore(5, ',');
	in >> CarObj.Colour;
	in.ignore(5, ',');
	in >> CarObj.Type;
	in.ignore(5, ',');
	in >> CarObj.Price;
	in.ignore();
	return in;
}
void DelCar() {
	if (fs::exists("CarSheet.csv")) {
		bool found = false;
		int Value;
		car CarObj;
		cout << "Enter Serial number : ";
		cin >> Value;
		ifstream CarData("CarSheet.csv");
		CarData.ignore(40, '\n');
		int Count = 0;
		while (CarData >> CarObj) {
			if (++Count == Value) {
				found = true;
				
				cout << setiosflags(ios::left) << setfill(' ')
					<<"Deleting the following entry!..\n\n"
					<< setw(6) << "Sr.No"
					<< setw(12) << "Name"
					<< setw(12) << "Company"
					<< setw(12) << "Colour"
					<< setw(12) << "Type"
					<< setw(12) << "Price" << '\n';
				cout << setw(6) << Count << CarObj << resetiosflags(ios::left);
				CarData.seekg(0, ios::beg);
				ofstream temp("TempCarSheet.csv", ios::out);
				char s[40];
				CarData.getline(s, 40, '\n');
				strcat_s(s, "\n");
				temp << s;
				int j = 1;
				while (CarData >> CarObj)
					if (j++ != Value)
						temp << CarObj;
				temp.close();
				CarData.close();
				fs::copy_file("TempCarSheet.csv", "CarSheet.csv", fs::copy_options::overwrite_existing);
				fs::remove("TempCarSheet.csv");
				break;
			}
		}
	}
	else
		cout << "No Data could be found!\n";
	system("pause");
	system("cls");
}