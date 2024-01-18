#include "QueueList.h"

#include "Customer.h"

QueueList::QueueList() { head = NULL;}

QueueList::QueueList(Customer* customer) { head = customer; }

QueueList::~QueueList() {  //DESTRUCTOR
/*while (head != nullptr) {
        Customer* current = head;
        head = head->get_next();  // Move the head to the next node
        delete current;           // Delete the current node
    }*/
    //Wasted so much time on something I don't even have to code :(
}

Customer* QueueList::get_head() { return head; }

void QueueList::enqueue(Customer* customer) {
  // a customer is placed at the end of the queue
  // if the queue is empty, the customer becomes the head
  if (head == nullptr) {
      head = customer;
  } else {
      Customer* temp = head;
      while (temp->get_next() != nullptr) {
          temp = temp->get_next();
      }
      temp->set_next(customer);
  }
}

Customer* QueueList::dequeue() {
  // remove a customer from the head of the queue 
  // and return a pointer to it
  if (head == nullptr) {
      return nullptr;  // Queue is empty
  }
  Customer* dequeuedCustomer = head;
  head = head->get_next();
  dequeuedCustomer->set_next(nullptr);
  return dequeuedCustomer;
}

int QueueList::get_items() {
  // count total number of items each customer in the queue has
  int totalItems = 0;
    Customer* temp = head;
    while (temp != nullptr) {
        totalItems += temp->get_numOfItems();
        temp = temp->get_next();
    }
    return totalItems;
}

bool QueueList::empty_queue() {//don't use it
    return false;
    //
}

void QueueList::print() {
  // print customers in a queue
  Customer* temp = head;
  while (temp != nullptr) {
    temp->print();
    temp = temp->get_next();
  }
}