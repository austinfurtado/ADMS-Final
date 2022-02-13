#include <iostream>
#include <fstream>
#include <iomanip>
#include <conio.h>
#include <Windows.h>
#include "credential.h"
void ADMS(fstream&);	//Defined at bottom
int main() {
	SetConsoleTitle(L"ADMS");
	system("color f1");
	std::fstream CredFile;
	char ch;
	cout << "\n\n\t\t";
	fstream title("adms.txt", ios::in);
	while (title >> noskipws >> ch) {
		cout << ch;
		if (ch == '\n')
			cout << "\t\t";
	}
	title.close();
	cout << "\t\t";
	while (true) {
		try {
			CredFile.open("Credentials.dat", std::ios::binary | std::ios::in | std::ios::out);
			std::cout << setw(30)<<setfill(' ') << " ";
			if (CredFile.fail()) {
				std::cout << "Failed to open\n"
					<< setw(38) << setfill(' ') << ' ';
				system("pause");
				system("cls");
				throw 1;
			}
			else {
				char val;
				cout << "\n\n\n" << shift(30) << "Press ENTER key to login\n";
				cout << shift(30) << "Press ESC key to Exit\n";
				switch (val = _getch()) {
				case 13:	//Enter key code
					ADMS(CredFile);
					break;
				case 27:	//Escape key code
					cout << "Exiting....\n";
					system("pause");
					exit(0);
				default:
					cout << "Invalid input...";
					system("pause");
					system("\r\t\t\t\t\r");
				}
				system("cls");
			}
		}
		catch (int) {
			Manager WP;
			WP.SetData(CredType::admin);
			if (!CredFile.is_open())
				CredFile.open("Credentials.dat", std::ios::binary | std::ios::out);
			CredFile.write((char*)&WP + 8, sizeof(Manager) - 8);
			//8 bytes of vtable pointer-vfptr skipped because it cannot be transfered between instances.
		}
		CredFile.close();
	}
}
void ADMS(fstream &CredFile) {
	Profile* P;
	Credential crdi, crdf{};
	system("cls");
	cout << shift(30) << "Enter Login details\n\t";
	cin >> crdi;
	CredFile.seekg(ios::beg);
	bool Verified = false;
	do {
		CredFile.read((char*)&crdf, sizeof(Credential));
		CredFile.seekg(-((int)sizeof(Credential)), ios::cur);
		if (crdi == crdf) {
			cout << "Login successful!\n";
			Verified = true;
			switch (crdf.type) {
			case CredType::admin:
				P = new Manager;
				CredFile.read((char*)P + 8, sizeof(Manager) - 8);
				P->Menu(CredFile);
				break;
			case CredType::sales:
				P = new Sales;
				CredFile.read((char*)P + 8, sizeof(Sales) - 8);
				P->Menu(CredFile);
				break;
			case CredType::client:
				P = new Client;
				CredFile.read((char*)P + 8, sizeof(Client) - 8);
				P->Menu(CredFile);
				break;
			}
			break;
		}
		else {
			CredFile.seekg(sizeof(Profile) - 8, ios::cur);
		}
	} while (CredFile.peek() != EOF);
	if (!Verified) {
		std::cout << "Wrong username or password\n";
		system("pause");
		system("cls");
	}
}