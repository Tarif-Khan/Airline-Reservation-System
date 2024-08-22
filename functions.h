#ifndef FUNCTIONS_H
#define FUNCTIONS_H
using namespace std;

struct Passenger {
	string name;
	string seat;
};

bool authenticate(const string& username, const string& password);
void display_employee_menu();
void display_passenger_menu();
void display_seating_chart(const vector<vector<string>>& seats);
void print_passenger_manifest(vector<Passenger>& manifest);
void automated_seat_assignment(vector<vector<string>>& seats, vector<Passenger>& manifest);
void manual_seat_assignment(vector<vector<string>>& seats, vector<Passenger>& manifest);
void load_state(vector<vector<string>>& seats, vector<Passenger>& manifest);
void change_cancel_seat(vector<vector<string>>& seats, vector<Passenger>& manifest);
void print_pass(vector<vector<string>>& seats, vector<Passenger>& manifest);
void save_state(const vector<vector<string>>& seats, const vector<Passenger>& manifest);


#endif
