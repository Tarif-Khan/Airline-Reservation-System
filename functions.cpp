#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include "functions.h"
#include <iomanip>
#include <unordered_map>
using namespace std;

// This function uses the deliminator of a comma and : to look for username and password feilds.
// Then it checks and allows for authentication.
bool authenticate(const string& username, const string& password){
	ifstream users("users.txt");
	unordered_map<string, string> creds;
	string line;

	while(getline(users, line)){
		size_t delimiterPos = line.find(": ");
		if (delimiterPos != string::npos) {
			string key = line.substr(0, delimiterPos);
			string value = line.substr(delimiterPos + 2);
			creds[key] = value;
		}
	}
	users.close();
	return (creds["username"] == username && creds["password"] == password);
}

// This method just outputs the employee menu.
void display_employee_menu(){
	cout << "Welcome to the Employee Menu. Please choose one of the options." << endl;
	cout << "1. Display seating arrangement as a chart." << endl;
	cout << "2. Print out the passenger manifest." << endl;
	cout << "3. Load the previous seating arrangement and passenger manifest." << endl;
	cout << "4. Save the current seating arrangement and passenger manifest." << endl;
	cout << "5. Cancel a seating arrangement or assign again." << endl;
	cout << "6. Quit the program." << endl;
	cout << "\n";
}

// This method just outputs the passenger menu.
void display_passenger_menu(){
	cout << "Welcome to the Passenger Menu. Please choose one of the following options." << endl;
	cout << "1. Display seating arrangement as a chart." << endl;
	cout << "2. Choose a seat using our automated system." << endl;
	cout << "3. Choose a seat using our manual system." << endl;
	cout << "4. Change or cancel seat arrangement." << endl;
	cout << "5. Print your boarding pass." << endl;
	cout << "6. Quit the program." << endl;
	cout << "\n";
}

// This method displays a seating chart.
void display_seating_chart(const vector<vector<string>>& seats){
	cout << "Here is the seating chart" << endl;
	for(int row = 0; row < 12; row++) {
		for(int col = 0; col < 8; col++) {
			if(seats[row][col].empty() && col != 2 && col != 6){
				cout << "O ";
			} else if(!seats[row][col].empty() && col !=2 && col != 6){
                                cout << "X ";
			} else {
				cout << "Aisle ";
			}

		}
		cout << "\n";
	}
}

// This method saves the state to a txt file.
void save_state(const vector<vector<string>>& seats, const vector<Passenger>& manifest){
	ofstream output_file("state.txt");

	for(int row = 0; row < seats.size(); row++){
		for(int col = 0; col < seats[row].size(); col++){
			if (col == 2 || col == 6){
			output_file << "Aisle ";
			} else if (seats[row][col].empty()) {
				output_file << "O ";
			} else {
				output_file << "X ";
			}
		}
		output_file << "\n";
	}

	for (const auto& passenger : manifest) {
		output_file << passenger.name << " " << passenger.seat << "\n";
	}
	output_file.close();
}

// This method loads the state from a txt file.
void load_state(vector<vector<string>>& seats, vector<Passenger>& manifest) {
	ifstream input_file("state.txt");
	
	for(int row = 0; row < seats.size(); row++){
		for(int col = 0; col < seats[row].size(); col++){
			string seat;
			input_file >> seat;
			if (seat == "Aisle ") {
			continue;
			} else if (seat == "O ") {
			seats[row][col].clear();
			} else if (seat == "X "){
			seats[row][col] = "X ";
			}
		}
	}

	manifest.clear();
	string name;
	string seat;

	while(input_file >> name >> seat) {
	manifest.push_back({name,seat});
	}

	input_file.close();
}

// This method auomatically helps seat people based on their inputs.
void automated_seat_assignment(vector<vector<string>>& seats, vector<Passenger>& manifest){
	string name;
	cout << "Please enter the name of the passenger you want to assign: ";
	getline(cin, name);

	char class_of_seat;
	cout << "Please enter which class you would like on the plane. F - First Class, B - Business Class, E - Economy Class ";
	cin >> class_of_seat;
	cin.ignore();

	int start = 0;
	int end = 0;
	switch(class_of_seat) {
		case 'F':
			start = 0;
			end = 0;
			break;
		case 'B':
			start = 1;
			end = 3;
			break;
		case 'E':
			start = 4;
			end = 11;
			break;
		default:
			cout << "The class you have chosen is not available for this flight." << endl;
			return;
	}

	bool seat_assigned = false;
	for (int seat_attempt = 0; seat_attempt < 100; seat_attempt++){
		int row = rand() % (end - start + 1) + start;
		int col = rand() % 8;
		if(col == 2 || col == 6) {
			col += 1;
		}
		if(seats[row][col].empty()) {
			seats[row][col] = "X ";
			string seat_code;
			if(col == 0){
				seat_code += 'A' + to_string(row);
			}else if(col == 1){
				seat_code += 'B' + to_string(row);
			}else if(col == 3) {
				seat_code += 'D' + to_string(row);
			}else if(col == 4) {
				seat_code += 'E' + to_string(row);
			}else if(col == 5) {
				seat_code += 'F' + to_string(row);
			}else if(col == 7) {
				seat_code += 'H' + to_string(row);
			}
		manifest.push_back({name, seat_code});
		cout << "The seat you have been assigned is " << seat_code << "The seat code is read as column, row." << endl;
		seat_assigned = true;
		save_state(seats, manifest);
		break;
		}
	}
	if(!seat_assigned) {
		cout << "We are unable to assign you a seat automatically at this time." << endl;
	}
}

// Here the user is able to manually choose their seating.
void manual_seat_assignment(vector<vector<string>>& seats, vector<Passenger>& manifest){
	string name;
	cout << "Please enter the name for the passenger you want to assign manually. ";
	getline(cin, name);
	display_seating_chart(seats);

	int row;
	char col_code;

	cout << "You will be entering a seat code, designed as (Column, Row) -- (Alpha), (Numeric). " << endl;
	cout << "Enter your column as A B D E F H ";
	cin >> col_code;
	cout << "Enter the row you would like to be in as a number 1 - 12." << endl;
	cin >> row;
	cin.ignore();

	int col;
	switch(col_code){
		case 'A': col = 0; break;
		case 'B': col = 1; break;
		case 'D': col = 3; break;
		case 'E': col = 4; break;
		case 'F': col = 5; break;
	        case 'H': col = 7; break;
		default:
			  cout << "Invalid column chosen";
			  return;
	}

	if(row < 1 || row > 12 || !seats[row - 1][col].empty()){
		cout << "This seat code is either invalid or unavailable.";
	} else {
		seats[row-1][col] = "X ";
		string seat_code;
		seat_code += col_code + to_string(row); 
		manifest.push_back({name, seat_code});
		cout << "This passenger has been assigned " << seat_code << "which is row " << row << "and column " << col_code << endl;
		save_state(seats, manifest);
	}
}

// A user should be able to change or cancel their seat here.
void change_cancel_seat(vector<vector<string>>& seats, vector<Passenger>& manifest) {
    string name;
    cout << "Please enter the name for the passenger you want to cancel or change for: ";
    getline(cin, name);

    bool found_in_manifest = false;
    auto it = manifest.begin();
    while (it != manifest.end()) {
        if (it->name == name) {
            cout << "Passenger found: " << it->name << ", Seat: " << it->seat << endl;

            if (it->seat.length() < 2) {
                cout << "Invalid seat format.\n";
                return;
            }

            int row;
            try {
                row = stoi(it->seat.substr(1));

		if (row == 0) {
			row += 2;
		}
                if (row < 1 || row > 12) {
                    cout << "Seat row out of bounds.\n";
                    return;
              	}

                row -= 1; // Adjust for 0-based index
            } catch (exception& e) {
                cout << "Invalid seat row number: " << e.what() << endl;
                return;
            }

            char col_code = it->seat[0];
            int col;
            switch (col_code) {
                case 'A': col = 0; break;
                case 'B': col = 1; break;
                case 'D': col = 3; break;
                case 'E': col = 4; break;
                case 'F': col = 5; break;
                case 'H': col = 7; break;
                default: 
                    cout << "Invalid seat code.\n";
                    return;
            }

            
            seats[row][col] = "O"; 
            it = manifest.erase(it);
            found_in_manifest = true;
            break;
        } else {
            ++it;
        }
    }

    if (!found_in_manifest) {
        cout << "No passenger found with the name " << name << endl;
        return;
    }

    char cancel_or_change;
    cout << "Would you like to cancel or change your seat? Choose Z to cancel or R to change: ";
    cin >> cancel_or_change;
    cin.ignore();

    if (cancel_or_change == 'R') {
        manual_seat_assignment(seats, manifest);
	save_state(seats, manifest);
    } else if (cancel_or_change == 'Z') {
        cout << "This seat assignment has been cancelled." << endl;
	save_state(seats, manifest);
    } else {
        cout << "Please choose a valid option." << endl;
    }
}

// This prints the passenger manifest.
void print_passenger_manifest(vector<Passenger>& manifest){
	cout << "This is the passenger manifest \n";
	cout << left << setw(20) << "Name" << setw(10) << "Seat Code" << "\n";
	cout << "\n";
	for (const auto& passenger : manifest){
		cout << left << setw(20) << passenger.name << setw(10) << passenger.seat << endl;
	}
}


// This prints the pass for a user.
void print_pass(vector<vector<string>>& seats, vector<Passenger>& manifest) {
    string name;
    cout << "Please enter the name of the passenger you would like a pass for: ";
    getline(cin, name);

    bool found_in_manifest = false;
    for (const auto& passenger : manifest) {
        if (passenger.name == name) {
            char seat = passenger.seat[0];
            int row = stoi(passenger.seat.substr(1));
            string class_of_seat;
            if (row == 0) {
                class_of_seat = "First Class";
            } else if (row >= 1 && row <= 3) {
                class_of_seat = "Business Class";
            } else if (row >= 4 && row <= 11) {
                class_of_seat = "Economy Class";
            } else {
                cout << "Check Class For Pass again";
                return;
            }
            cout << passenger.name << " is assigned to seat " << passenger.seat << " in " << class_of_seat << endl;
            found_in_manifest = true;
            break;
        }
    }

    if (!found_in_manifest) {
        cout << "No pass available, sorry. Please try again." << endl;
    }
}





