#include <cmath>
#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <ostream>
#include <sstream>

#include "Customer.h"
#include "QueueList.h"
#include "Register.h"
#include "RegisterList.h"

using namespace std;

// Function Declarations:

// Set mode of the simulation
string getMode();

// Register
void parseRegisterAction(stringstream &lineStream, string mode);
void openRegister(
    stringstream &lineStream,
    string mode);  // register opens (it is upto customers to join)
void closeRegister(stringstream &lineStream,
                   string mode);  // register closes 

// Customer
void addCustomer(stringstream &lineStream,
                 string mode);  // customer wants to join


// Helper functions
bool getInt(stringstream &lineStream, int &iValue);
bool getDouble(stringstream &lineStream, double &dValue);
bool foundMoreArgs(stringstream &lineStream);
void Queues(string mode, Customer* customer);
void Update(string mode,int ID, Customer* customer, QueueList* customerList, Register* registerUpdate);
void Queue(int ID, string mode);
void Depart(int ID, double departTime);
double standard_Deviation(QueueList* doneList, double average, int doneCounter);
void Statistics(double expTimeElapsed, double max, double total, int done);

// Global variables
RegisterList *registerList; // holding the list of registers
QueueList *doneList; // holding the list of customers served
QueueList *singleQueue; // holding customers in a single virtual queue
RegisterList *doneRegister;
double expTimeElapsed; // time elapsed since the beginning of the simulation
double maxWaitTime = 0.0;
double totalWaitTime = 0.0;
int doneCounter = 0;

// List of commands:
// To open a register
// register open <ID> <secPerItem> <setupTime> <timeElapsed>
// To close register
// register close <ID> <timeElapsed>
// To add a customer
// customer <items> <timeElapsed>

int main() {
  registerList = new RegisterList();
  doneList = new QueueList();
  singleQueue = new QueueList();
  doneRegister = new RegisterList();
  expTimeElapsed = 0;

  // Set mode by the user
  string mode = getMode();

  string line;
  string command;

  cout << "> ";  // Prompt for input
  getline(cin, line);

  while (!cin.eof()) {
    stringstream lineStream(line);
    lineStream >> command;
    if (command == "register") {
      parseRegisterAction(lineStream, mode);
    } else if (command == "customer") {
      addCustomer(lineStream, mode);
    } else {
      cout << "Invalid operation" << endl;
    }
    cout << "> ";  // Prompt for input
    getline(cin, line);
  }

  Statistics(expTimeElapsed, maxWaitTime, totalWaitTime, doneCounter);

  return 0;
}

string getMode() {
  string mode;
  cout << "Welcome to ECE 244 Grocery Store Queue Simulation!" << endl;
  cout << "Enter \"single\" if you want to simulate a single queue or "
          "\"multiple\" to "
          "simulate multiple queues: \n> ";
  getline(cin, mode);

  if (mode == "single") {
    cout << "Simulating a single queue ..." << endl;
  } else if (mode == "multiple") {
    cout << "Simulating multiple queues ..." << endl;
  }

  return mode;
}

void addCustomer(stringstream &lineStream, string mode) {
  int items;
  double timeElapsed;
  if (!getInt(lineStream, items) || !getDouble(lineStream, timeElapsed)) {
      cout << "Error: too few arguments." << endl;
      return;
  }
  if (foundMoreArgs(lineStream)) {
      cout << "Error: too many arguments." << endl;
      return;
  }

  expTimeElapsed = expTimeElapsed + timeElapsed; 
  Register *registerUpdate = registerList->calculateMinDepartTimeRegister(expTimeElapsed);
  while (registerUpdate != nullptr && registerUpdate ->calculateDepartTime()<=expTimeElapsed && registerUpdate->calculateDepartTime() != -1) 
  {
      QueueList *customerList = registerUpdate->get_queue_list();
      Customer* leader = singleQueue->get_head();
      int Ident = registerUpdate ->get_ID();
      double departTime = registerUpdate->calculateDepartTime();  
      registerUpdate->set_availableTime(departTime); 
      Depart(registerUpdate->get_ID(),departTime);
      customerList->get_head()->set_departureTime(departTime);
      double wait = departTime - customerList ->get_head()->get_arrivalTime();
      if (wait>maxWaitTime){
        maxWaitTime = wait;
      }
      totalWaitTime += wait;
      doneList->enqueue(customerList->get_head());     
      customerList->dequeue();
      doneCounter++;
      Update(mode, Ident, leader, customerList, registerUpdate);
      registerUpdate = registerList->calculateMinDepartTimeRegister(expTimeElapsed);
      ////////////////////////
      if ((registerUpdate -> calculateDepartTime() == -1 || registerUpdate -> calculateDepartTime() > expTimeElapsed)&& registerUpdate -> get_next()!= nullptr){
        registerUpdate = registerUpdate -> get_next();
      if (registerUpdate -> get_next() != nullptr){
         if (registerUpdate -> calculateDepartTime() > registerUpdate -> get_next() -> calculateDepartTime()){
          registerUpdate = registerUpdate -> get_next();
         }  
      }
      }
      ///////////////////////////////
      //
  }

  Customer *newCustomer = new Customer(expTimeElapsed, items);
  newCustomer->set_arrivalTime(expTimeElapsed);
  
  cout << "A customer entered" << endl;
  if (mode == "single"){
    singleQueue->enqueue(newCustomer);
  }
  Queues(mode,newCustomer);
}

void parseRegisterAction(stringstream &lineStream, string mode) 
{
string operation;
lineStream >> operation;
if (operation == "open") 
{
  openRegister(lineStream, mode);
} 
else if (operation == "close") 
{ 
  closeRegister(lineStream, mode);
} 
else 
{
  cout << "Invalid operation" << endl;
}
}

void openRegister(stringstream &lineStream, string mode) {
  int ID;
  double secPerItem, setupTime, timeElapsed;
  // convert strings to int and double
  if (!getInt(lineStream, ID) || !getDouble(lineStream, secPerItem) ||
      !getDouble(lineStream, setupTime) ||
      !getDouble(lineStream, timeElapsed)) {
    cout << "Error: too few arguments." << endl;
    return;
  }
  if (foundMoreArgs(lineStream)) {
    cout << "Error: too many arguments" << endl;
    return;
  }

  if (registerList->foundRegister(ID)) {
      cout << "Error: register " << ID << " is already open" << endl;
      return;
  }

  expTimeElapsed = expTimeElapsed + timeElapsed;  

  Register *registerUpdate = registerList->calculateMinDepartTimeRegister(expTimeElapsed);
  while (registerUpdate != nullptr && registerUpdate ->calculateDepartTime()<=expTimeElapsed && registerUpdate->calculateDepartTime() != -1) 
  {
      QueueList *customerList = registerUpdate->get_queue_list();
      Customer* leader = singleQueue->get_head();
      int Ident = registerUpdate->get_ID();
      double departTime = registerUpdate->calculateDepartTime();  
      registerUpdate->set_availableTime(departTime); 
      Depart(registerUpdate->get_ID(),departTime);
      customerList->get_head()->set_departureTime(departTime);
      double wait = departTime - customerList ->get_head()->get_arrivalTime();
      if (wait>maxWaitTime){
        maxWaitTime = wait;
      }
      totalWaitTime += wait;
      doneList->enqueue(customerList->get_head());     
      customerList->dequeue();
      doneCounter++;
      Update(mode, Ident, leader, customerList, registerUpdate);
      registerUpdate = registerList->calculateMinDepartTimeRegister(expTimeElapsed);
      ////////////////////////
      if ((registerUpdate -> calculateDepartTime() == -1 || registerUpdate -> calculateDepartTime() > expTimeElapsed)&& registerUpdate -> get_next()!= nullptr){
        registerUpdate = registerUpdate -> get_next();
      if (registerUpdate -> get_next() != nullptr){
         if (registerUpdate -> calculateDepartTime() > registerUpdate -> get_next() -> calculateDepartTime()){
          registerUpdate = registerUpdate -> get_next();
         }  
      }
      }
      ///////////////////////////////
      //
      
  }

  Register* newRegister = new Register(ID, secPerItem, setupTime, expTimeElapsed);
  registerList->enqueue(newRegister);
  cout << "Opened register " << ID << endl;
  newRegister->set_availableTime(expTimeElapsed);

  // If we were simulating a single queue, and there were customers in line,
  // then assign a customer to the new register
  if (mode == "single") 
  {
      Register* freeRegister = registerList->get_free_register();
      while (singleQueue->get_head() != NULL && freeRegister != NULL) 
      {
          Customer* dequeuedCustomer = singleQueue->dequeue();
          freeRegister->get_queue_list()->enqueue(dequeuedCustomer);
          Queue(freeRegister->get_ID(), mode);
          freeRegister = registerList->get_free_register();
      }
  }
}

void closeRegister(stringstream &lineStream, string mode) {
  int ID;
  double timeElapsed;
  // convert string to int
  if (!getInt(lineStream, ID) || !getDouble(lineStream, timeElapsed)) {
    cout << "Error: too few arguments." << endl;
    return;
  }
  if (foundMoreArgs(lineStream)) {
    cout << "Error: too many arguments" << endl;
    return;
  }
  // Check if the register is open
  // If it is open dequeue it and free it's memory
  // Otherwise, print an error message 
  // Check if the register is open
  expTimeElapsed = expTimeElapsed + timeElapsed;
  
  Register *registerUpdate = registerList->calculateMinDepartTimeRegister(expTimeElapsed);
  while (registerUpdate != nullptr && registerUpdate ->calculateDepartTime()<=expTimeElapsed && registerUpdate->calculateDepartTime() != -1) 
  {
      QueueList *customerList = registerUpdate->get_queue_list();
      Customer* leader = singleQueue->get_head();
      int Ident = registerUpdate->get_ID();
      double departTime = registerUpdate->calculateDepartTime();  
      registerUpdate->set_availableTime(departTime); 
      Depart(registerUpdate->get_ID(),departTime);
      customerList->get_head()->set_departureTime(departTime);
      double wait = departTime - customerList ->get_head()->get_arrivalTime();
      if (wait>maxWaitTime){
        maxWaitTime = wait;
      }
      totalWaitTime += wait;     
      doneList->enqueue(customerList->get_head()); 
      customerList->dequeue();
      doneCounter++;
      Update( mode, Ident,  leader, customerList, registerUpdate);
      registerUpdate = registerList->calculateMinDepartTimeRegister(expTimeElapsed);
      ////////////////////////
      if ((registerUpdate -> calculateDepartTime() == -1 || registerUpdate -> calculateDepartTime() > expTimeElapsed)&& registerUpdate -> get_next()!= nullptr){
        registerUpdate = registerUpdate -> get_next();
      if (registerUpdate -> get_next() != nullptr){
         if (registerUpdate -> calculateDepartTime() > registerUpdate -> get_next() -> calculateDepartTime()){
          registerUpdate = registerUpdate -> get_next();
         }  
      }
      }
      ///////////////////////////////
      //
  }
  
  if (registerList -> foundRegister(ID)){
    Register* closedRegister = registerList->dequeue(ID);

while (closedRegister->get_queue_list()->get_head() != nullptr) {
    QueueList* customerList = closedRegister->get_queue_list();
    double departTime = closedRegister->calculateDepartTime();
    customerList->get_head()->set_departureTime(departTime);
    double wait = departTime - customerList->get_head()->get_arrivalTime();
    totalWaitTime += wait;
    if (wait > maxWaitTime) {
        maxWaitTime = wait;
    }

    doneList->enqueue(customerList->get_head());   
    customerList ->dequeue();
    Customer* dequeuedCustomer = customerList->dequeue();
    Depart(closedRegister->get_ID(),departTime);
    doneCounter++;
    closedRegister->set_availableTime(departTime);

    if (singleQueue->get_head() != NULL) {
        Customer* incomingCustomer = singleQueue->get_head();
        singleQueue->dequeue();
        customerList->enqueue(incomingCustomer);
        Queue( closedRegister->get_ID(), "single");
    }
    // Free the memory for the dequeued customer
    delete dequeuedCustomer;
}

// Free the memory for the closedRegister
delete closedRegister;
    cout << "Closed register "<< ID << endl;
  } else {
    cout << "Error: register " << ID << " is not open" << endl;
  }
}
bool getInt(stringstream &lineStream, int &iValue) {
  // Reads an int from the command line
  string command;
  lineStream >> command;
  if (lineStream.fail()) {
    return false;
  }
  iValue = stoi(command);
  return true;
}

bool getDouble(stringstream &lineStream, double &dvalue) {
  // Reads a double from the command line
  string command;
  lineStream >> command;
  if (lineStream.fail()) {
    return false;
  }
  dvalue = stod(command);
  return true;
}

bool foundMoreArgs(stringstream &lineStream) {
  string command;
  lineStream >> command;
  if (lineStream.fail()) {
    return false;
  } else {
    return true;
  }
}

void Depart (int ID, double departTime){
  cout << "Departed a customer at register ID " << ID << " at " << departTime << endl;
}
void Queue(int ID, string mode){
  if(mode == "single"){
  cout << "Queued a customer with free register " << ID << endl;
} else if (mode == "multiple"){
  cout << "Queued a customer with quickest register " << ID << endl;
}
}
void Update(string mode, int ID,  Customer* customer, QueueList* customerList, Register* registerUpdate){
if (mode == "single" && customer != NULL){
         customerList->enqueue(customer);
         singleQueue->dequeue();
         Queue(ID, mode);
      }
  }
void Queues (string mode, Customer* customer){
  string type = mode;
 if (type == "multiple")  {
      Register *minItemsRegister = registerList->get_min_items_register();
      if (minItemsRegister != nullptr) {
          minItemsRegister->get_queue_list()->enqueue(customer);
          Queue( minItemsRegister->get_ID(), "multiple");
      } 
  }
  if (type == "single") {
      Register *freeRegister = registerList->get_free_register();
      if (freeRegister != nullptr) {
          Customer* headCustomer = singleQueue->get_head(); 
          freeRegister->get_queue_list()->enqueue(headCustomer);
          singleQueue->dequeue();
          Queue(freeRegister->get_ID(), "single");
      } 
      else {
          cout << "No free registers" << endl;
      } 
  }  
}
double standard_Deviation(QueueList* doneList, double average, int doneCounter){
 double standarDev=0.0;
 Customer * currentCustomer = doneList ->get_head();
    while (currentCustomer != nullptr){
    double waiting=0.0;
    double temp=0.0;
    double departure=0.0;
    double arrival=0.0;
    departure = currentCustomer ->get_departureTime();
    arrival = currentCustomer ->get_arrivalTime();
    waiting = departure - arrival ;
    temp = waiting - average;
    temp = (temp * temp);
    temp = temp / doneCounter;
    standarDev = standarDev + temp;
    currentCustomer = currentCustomer ->get_next();
  }
 return standarDev = sqrt(standarDev);
}
void Statistics(double expTimeElapsed, double max, double total, int done){
  cout<<endl<<"Finished at time "<<expTimeElapsed<<endl<< "Statistics: "<<endl;
  cout<<"Maximum wait time: "<<max<<endl;
  cout<<"Average wait time: "<<total/done<<endl;
  cout<<"Standard Deviation of wait time: "<<standard_Deviation(doneList, total/done, doneCounter)<<endl;
}

