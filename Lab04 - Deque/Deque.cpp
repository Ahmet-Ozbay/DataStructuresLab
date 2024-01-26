#include "Deque.h"
#include <iostream>

///------------------------------------------------------
/// Default constructor
/// 
Deque::Deque() : head(nullptr), tail(nullptr), noOfItems(0) {
} //end-Deque

///------------------------------------------------------
/// Destructor
/// 
Deque::~Deque() {

    // Traverse and delete
    while (head != nullptr) {
        DequeNode* temp = head;
        head = head->next;
        delete temp;
    }

    // Empty tail and elements
    tail = nullptr;
    noOfItems = 0;
} //end-~Deque

///------------------------------------------------------
/// Adds a new item to the front of the Deque
/// 
void Deque::AddFront(int item) {

    // Create new node with the item
    DequeNode* newNode = new DequeNode(item);

    // Check if it is the only node
    if (head == nullptr) {
        head = tail = newNode;
    }
    else {
        // Add the node to the front
        newNode->next = head;
        head->prev = newNode;
        head = newNode;
    }

    // Increase elements
    noOfItems++;
} //end-AddFront

///------------------------------------------------------
/// Adds a new item to the end of the Deque
/// 
void Deque::AddRear(int item) {

    // Create new node with the item
    DequeNode* newNode = new DequeNode(item);

    // Check if it is the only node
    if (tail == nullptr) {
        head = tail = newNode;
    }
    else {
        // Add the node to the rear
        newNode->prev = tail;
        tail->next = newNode;
        tail = newNode;
    }

    noOfItems++;
} //end-AddRear

///------------------------------------------------------
/// Remove and return the item at the front of the Deque
/// If the Deque is empty, throw an exception
/// 
int Deque::RemoveFront() {
    
    // Check if que is empty 
    if (IsEmpty()) {
        throw std::out_of_range("Empty Deque. Cannot remove from the front.");
    }

    // Create a temporary head node
    DequeNode* temp = head;

    // Retrieve the item of the head
    int value = temp->item;

    // Rearrange the links between the nodes
    if (head->next != nullptr) {
        head = head->next;
        head->prev = nullptr;
    }
    else {
        // Deque has only one node
        head = tail = nullptr;
    }

    // Free up the memory
    delete temp;

    // Decrease item count
    noOfItems--;

    return value;
} //end-RemoveFront

///------------------------------------------------------
/// Remove and return the item at the rear of the Deque
/// If the Deque is empty, throw an exception
/// 
int Deque::RemoveRear() {

    // Check if que is empty
    if (IsEmpty()) {
        throw std::out_of_range("Empty Deque. Cannot remove from the rear.");
    }

    // Create temporary tail node
    DequeNode* temp = tail;

    // Retrieve the item of tail node
    int value = temp->item;

    // Rearrange links between nodes
    if (tail->prev != nullptr) {
        tail = tail->prev;
        tail->next = nullptr;
    }
    else {

        // Deque has only one node
        head = tail = nullptr;
    }

    // Free up the memory
    delete temp;

    // Decrease item count
    noOfItems--;

    return value;
} //end-RemoveRear

///------------------------------------------------------
/// Return the item at the front of the Deque (do not remove the item)
/// If the Deque is empty, throw an exception
/// 
int Deque::Front() {
    
    // Check if deque is empty 
    if (IsEmpty()) {
        throw std::out_of_range("Empty Deque. No item at the front.");
    }

    // Return if there is a head node
    return head->item;
} //end-Front

///------------------------------------------------------
/// Return the item at the rear of the Deque (do not remove the item)
/// If the Deque is empty, throw an exception
/// 
int Deque::Rear() {

    // Check if deque is empty
    if (IsEmpty()) {
        throw std::out_of_range("Empty Deque. No item at the rear.");
    }

    // Return if there is a tail node
    return tail->item;
} //end-Rear