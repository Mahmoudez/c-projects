#include <bits/stdc++.h>
using namespace std;
vector <string>ReadFileLines(string path) {

    vector<string> lines;
    fstream file_handler(path.c_str());
    if (file_handler.fail()) {
        cout << "\n\nError: Can't opent this file\n\n";
        return lines;
    }
    string line;

    while (getline(file_handler, line)) {
        if (line.size() == 0)
            continue;
        lines.push_back(line);
    }
    file_handler.close();
    return lines;
}
void WriteFileLines(string path, vector<string>lines, bool append = true) {
    auto status = append ? (ios::in | ios::out | ios::app) : (ios::in | ios::out | ios::trunc);
    fstream file_handluer(path.c_str());
    if (file_handluer.fail()) {
        cout << "\n\n Error: Can't open this file\n\n";
        return;
    }
    for (auto& line : lines) {
        file_handluer << line << "\n";
    }
    file_handluer.close();
}
vector<string> SplitString(string str, string delimiter = ",") {
    vector<string> strs;
    string sbstr = "";
    int pos = 0;
    while ((pos = (int)str.find(delimiter)) != -1) {
        sbstr = str.substr(0, pos);
        str.erase(0, pos + delimiter.length());
        strs.push_back(sbstr);
    }
    strs.push_back(str);
    return strs;
}
int ToInt(string str) {
    istringstream oss(str);
    int num;
    oss >> num;
    return num;
}
int ReadInt(int low, int high) {
    cout << "\nEnter number in range " << low << "-" << high << ":";
    int value;
    cin >> value;
    if (low <= value && value <= high) {
        return value;
    }
    cout << "Error : invalid number .. try again\n";
    return(ReadInt(low, high));
}
int ShowReadMenu(vector<string> choices) {
    cout << "\nMenu:\n";
    for (int ch = 0; ch < (int)choices.size(); ++ch) {
        cout << "\t" << ch + 1 << choices[ch] << endl;
    }

    return ReadInt(1, (int)choices.size());
}
class Vehicle {
private:
	string  vehicle_id;
	string brand;
	string model;
	double dailyrate;
	string  year;
	bool is_Avilabile;
public:
	Vehicle(string vehicle_id_,string brand_,string model_,double dailyrate_,string year_,bool active_):
		vehicle_id(vehicle_id_),
		brand(brand_),model(model_),dailyrate(dailyrate_)
		,year(year_), is_Avilabile(active_){ }
string GetVehicleInfo() const { 
        ostringstream oss;
        oss << "ID: " << vehicle_id << " | " << brand << " " << model 
            << " (" << year << ") | Rate: $" << dailyrate 
            << " | Status: " << (is_Avilabile ? "Available" : "Rented");
        return oss.str(); 
    }
    string ToCSV() const {
        ostringstream oss;
        oss << vehicle_id << "," << brand << "," << model << "," 
            << dailyrate << "," << year << "," << is_Avilabile;
        return oss.str();
    }
      void rent(){
        is_Avilabile = false;
      }
      void returnVehicle(){is_Avilabile= true;}
      double calculetRentalCoast(int days) const{
        double cost = days * dailyrate;
        return cost;
      }
      bool IsAvilable()const {
        return is_Avilabile;
      }
      string GetVehicleID()const{
        return vehicle_id;
      }
};
class Custmore{
    private:
    string customer_id;
    string name ,email,phone;
    string driverslincenseNumbe;
    public:
    Custmore(string customreid, string name, stirng email, string phone, strnig driverslincenesN):
      customer_id(customreid), name(name),email(email),
      phone(phone),driverslincenseNumbe(driverslincenesN) {}

string GetCustomerInfo() const {
        ostringstream oss;
        oss << "ID: " << customer_id << " | Name: " << name 
            << " | Email: " << email << " | Phone: " << phone 
            << " | License: " << driverslincenseNumbe;
        return oss.str();
    }

    string ToCSV() const {
        ostringstream oss;
        oss << customer_id << "," << name << "," << email << "," 
            << phone << "," << driverslincenseNumbe;
        return oss.str();
    }

string GetCustomerID()const{
    return customer_id;
}

 };
class Rental{
private:
string rental_id;
int start_date, end_date;
bool is_Active;
Custmore *customre;
Vehicle *vehicle;
public:
Rental(string rental_id_, int start, int end, bool isactive, Custmore *cst, Vehicle *ve):
 rental_id(rental_id_),start_date(start),end_date(end), is_Active(isactive),customre(cst),vehicle(ve){}
 int GetRenatlDuration() const{
    return end_date - start_date;
 }
 double GetTotalCost(){
 double total_cost =    vehicle->calculetRentalCoast(GetRenatlDuration());
    return total_cost;
 }

 string GetRenatlInfo()
 {
    ostringstream oss;
    oss <<"Rental ID " << rental_id << "Start Date : " << start_date 
     << "End Date " << end_date << "Customer ID" << customre->GetCustomerID()
      << "Vehicle ID " << vehicle->GetVehicleID() ;
      return oss.str();  
 }
 void CompeletRental(){
    vehicle->returnVehicle();
    cout << "Your Total Coast is : " <<GetTotalCost() << '\n';
     is_Active= false; 
}
string GetRentalID()const{
    return rental_id;
}
bool GetActiveRental()const{
    return is_Active;
}
};
class RentalAgency{
    private:
    string agency_name;
    vector<Vehicle*>Vehicles;
    vector<Custmore*>Custmores;
    vector<Rental*>Rentals;

    map<string,Custmore*>custmore_id_to_custmore;
    map<string, Vehicle*> vehicle_id_to_vehicle;
    map<string,vector<Rental*>>customer_id_to_customer;
  public:
  RentalAgency(string agenName): agency_name(agenName){}
    void AddVehicle(string vehicleID, string Brand, string model ,string year, double dailyrate){
      // allocate Vehicle on heap and store pointer in Vehicles vector
      Vehicle* carx = new Vehicle(vehicleID,Brand,model,dailyrate,year,true);
      Vehicles.push_back(carx);
      vehicle_id_to_vehicle[vehicleID]=carx;

  }
  void registerCustomer(string customreid,string name,string email,string phone,string driverlensec){
      // allocate Custmore on heap and store pointer in Custmores vector
      Custmore* custmore = new Custmore(customreid,name,email,phone,driverlensec);
      Custmores.push_back(custmore);
      custmore_id_to_custmore[customreid]=custmore;
  }
  void GetAvilableVehicles(){
    for(auto &veh: Vehicles){
        if(veh->IsAvilable()){
         cout <<     veh->GetVehicleInfo();
        }

        cout << "\n======================================\n\n";
    }  
  }
  void  CreatRental(string rent_id,string customer_id, string vehicle_id, int start,int end){
  Customer* cust = customer_id_to_customer[customer_id];
  Vehicle* veh = vehicle_id_to_vehicle[vehicle_id];
  Rental* rent = new Rental(rent_id, start, end, true, cust, veh);
        Rentals.push_back(rent);
        customer_to_rentals[customer_id].push_back(rent);
    veh->rent();
  }
  double  compelateRental(string rentalID){
        for(auto &rent :Rentals){
            if(rent->GetRentalID() == rentalID && rent->GetActiveRental()){
                rent->CompeletRental();
                return rent->GetTotalCost();
                break;
            }
        }
        cout << "Rental ID not found or already completed.\n";
        return 0.0;
  }

  void GetActiveRentals(){
    cout << "\n--- Active Rentals ---\n";
    for(auto &rent: Rentals){
        if(rent->GetActiveRental()){
         cout << rent->GetRenatlInfo();
        cout << "\n======================================\n\n";
        }
    }
  }
  void GetCustmoreRentals(string cust_id){
    cout << "\n--- Rental History for Customer ID: " << cust_id << " ---\n";
        vector<Rental*> cuts_rentals = custmore_to_rentals[cust_id];
      cout << "\n----RentalsInfo---\n";
        for(auto & rental: cuts_rentals){
            cout << rental->GetRenatlInfo() <<'\n';
            cout << "\n==============================================\n";
        }
        cuts_rentals.clear();
    }
    void DisplayFleet(){
    cout << "\n--- Entire Fleet Status ---\n";
        for(auto &vehicl: Vehicles){
            cout << vehicl->GetVehicleInfo() << '\n';
            cout << "\n==============================================\n";
        }
    }
    ~RentalAgency() {
        for (auto v : Vehicle) delete v;
        for (auto c : Customers) delete c;
        for (auto r : R entals) delete r;
    }
};
int main() {




	return 0;
}