
// Created by Salma Emara on 2023-06-02.
#include "Register.h"

// Constructor
Register::Register(int id, double timePerItem, double overhead,
                   double entryTime) {
  ID = id;
  secPerItem = timePerItem;
  overheadPerCustomer = overhead;
  availableTime = entryTime;
  next = nullptr;           
  queue = new QueueList();  
}

// Destructor
Register::~Register() {
  delete queue;  // Release memory allocated for the queue
}

// Getter for the queue
QueueList* Register::get_queue_list() {
  return queue;
}

// Getter for the next register
Register* Register::get_next() {
  return next;
}

// Getter for Register ID
int Register::get_ID() {
  return ID;
}

// Getter for time per item
double Register::get_secPerItem() {
  return secPerItem;
}

// Getter for overhead per customer
double Register::get_overheadPerCustomer() {
  return overheadPerCustomer;
}

// Getter for available time
double Register::get_availableTime() {
  return availableTime;
}

// Setter for the next register
void Register::set_next(Register* nextRegister) {
  next = nextRegister;
}

// Setter for available time
void Register::set_availableTime(double availableSince) {
  availableTime = availableSince;
}

// Calculate departure time for the first customer in the queue
double Register::calculateDepartTime() {
  double departTime = 0.0;
  if (queue ->get_head() == nullptr){
    return -1.0;
  }
  else if (queue->get_head()->get_arrivalTime() > availableTime){
    departTime = queue->get_head()->get_arrivalTime() + (queue->get_head()->get_numOfItems() * secPerItem) + overheadPerCustomer;
  }
  else{
    departTime = availableTime + (queue->get_head()->get_numOfItems() * secPerItem) + overheadPerCustomer;
  }
  return departTime;
}

// Depart the first customer from the queue
void Register::departCustomer(QueueList* doneList) {
  if (queue->get_head() != NULL) {
    Customer* departedCustomer = queue->dequeue();
    departedCustomer->set_departureTime(calculateDepartTime());
    doneList->enqueue(departedCustomer);
    availableTime = departedCustomer->get_departureTime();
  }
}

// Print register information
void Register::print() {
  std::cout << "Register ID: " << ID << std::endl;
  std::cout << "Time per item: " << secPerItem << std::endl;
  std::cout << "Overhead per customer: " << overheadPerCustomer << std::endl;
  if (queue->get_head() != nullptr) {
    std::cout << "Queue has customers: \n";
    queue->print();
  }
}