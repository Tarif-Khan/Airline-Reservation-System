#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include "functions.h"
using namespace std;

// This is my main function.
int main(){
	vector<vector<string>> seats(12, vector<string>(8, ""));
	string username;
	string password;
	bool isEmployee = false;
	vector<Passenger> manifest;

	srand(static_cast<unsigned int>(time(0)));
	load_state(seats, manifest);


	cout << "This is the Airline Reservation System" << endl;
	cout << "Enter p for passenger or e for employee. ";
	char user;
	cin >> user;
	cin.ignore();

	if(user == 'e'){
		cout << "Enter your username ";
		getline(cin, username);
		cout << "Enter your password ";
		getline(cin, password);
		if(authenticate(username, password)){
			isEmployee = true;
		} else {
			cout << "Invalid Employee Credentials." << endl;
			return 1;		
		}
	}

	int menu_choice;
	do {
		if(isEmployee){
			display_employee_menu();
		} else {
			display_passenger_menu();
		}

		cin >> menu_choice;
		cin.ignore();

		switch(menu_choice){
			case 1:
				display_seating_chart(seats);
			break;
			
			case 2:
				if(isEmployee){
					print_passenger_manifest(manifest);
				} else {
					automated_seat_assignment(seats, manifest);
				}
			break;

			case 3:
				if(isEmployee){
					load_state(seats, manifest);
				} else {
					manual_seat_assignment(seats, manifest);
				}
			break;

			case 4:
				if (isEmployee){
					save_state(seats, manifest);
				} else {
					change_cancel_seat(seats, manifest);
				}
			break;

			case 5:
				if(isEmployee) {
					change_cancel_seat(seats, manifest);
				} else {
					print_pass(seats, manifest);
				}
			break;

			case 6:
				cout << "Quitting the program. Thank You!" << endl;
				return 0;
			break;
			default:
				cout << "Invalid response> Please follow the menu and try again." << endl;
				break;
		}
	} while (menu_choice != 6);
return 0;
}
