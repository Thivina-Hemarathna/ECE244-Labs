#include "RegisterList.h"

RegisterList::RegisterList()
{
    head = NULL;
    size = 0;
}

RegisterList::~RegisterList() {
    // Delete all registers in the list
    Register* current = head;
    while (current != nullptr) {
        Register* nextRegister = current->get_next();
        delete current;
        current = nextRegister;
    }   
    // Ensure that head is set to nullptr to avoid dangling pointers
    head = nullptr;
}

Register* RegisterList::get_head() {
    return head;
}

int RegisterList::get_size() {
    return size;
}

Register* RegisterList::get_min_items_register() {
    // Loop through all registers to find the register with the least number of items
    Register* current = head;
    Register* next = head->get_next();
    if (head == nullptr) {
        return nullptr;  // No registers in the list
    }
    while (next != nullptr)  {
        if ( current->get_queue_list()->get_items() > next->get_queue_list()->get_items() ) {
            current = next;
        }
        next = next->get_next();
    }
    return current;
}

Register* RegisterList::get_free_register() {
    // Return the register with no customers
    // If all registers are occupied, return nullptr
    Register* current = head;
    while (current != nullptr)  {
        if (current->get_queue_list()->get_head() == nullptr) {
            return current;
        }
        current = current->get_next();
    }
    return nullptr;  // No free registers found
}

void RegisterList::enqueue(Register* newRegister) {
    // A register is placed at the end of the list
    // If the register's list is empty, the register becomes the head
    // Assume the next of the newRegister is set to null
    
    if (head == nullptr) {
        head = newRegister;
    } 
    else {
        Register* current = head;
        while (current->get_next() != nullptr) {
           current = current->get_next();
        }
        current->set_next(newRegister);
    }
    size++; 
}

bool RegisterList::foundRegister(int ID) {
    // Look for a register with the given ID
    // Return true if found, false otherwise
    Register* current = head;
    while (current != nullptr) {
        if (current->get_ID() == ID)  {
            return true;
        }
        current = current->get_next();
    }
    return false;  
}

Register* RegisterList::dequeue(int ID) {
    // Dequeue the register with the given ID
    // Return the dequeued register
    // Return nullptr if the register was not found
    
    if (head == nullptr) {
        return nullptr; 
    }
    if (head->get_ID() == ID) {
        Register* dequeuedRegister = head;
        head = head->get_next();
        dequeuedRegister->set_next(nullptr); 
        size--; 
        return dequeuedRegister;
    }
    Register* temp = head;
    while (temp->get_next() != nullptr) {
        if (temp->get_next()->get_ID() == ID) {
            Register* dequeuedRegister = temp->get_next();
            temp->set_next(temp->get_next()->get_next());
            dequeuedRegister->set_next(nullptr);  
            size--;  
            return dequeuedRegister;
        }
        temp = temp->get_next();
    }
    return nullptr;   
}

Register* RegisterList::calculateMinDepartTimeRegister(double expTimeElapsed) {
    // Return the register with the minimum time of departure of its customer
    // If all registers are free, return nullptr
    double Temp,Min;
    if (head == nullptr) {
        return nullptr;  // No registers in the list
    }
    Register* current = head;
    Register* temp = head->get_next();
    while (temp != nullptr) {
        Temp = temp->calculateDepartTime();
        Min = current->calculateDepartTime();
        if (Temp < Min) {
            current = temp;
        }
        temp = temp->get_next();
    }
    return current;
}

void RegisterList::print() {
    // Print registers in the list
    Register* temp = head;
    while (temp != nullptr) {
        temp->print();
        temp = temp->get_next();
    }
}