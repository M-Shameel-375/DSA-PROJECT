### ****Contact Management System - C++ Implementation********
This document provides an overview of the C++ code for a contact management system. The system allows users to efficiently store, manage,
and organize their contacts.

## **Key Features:**

**Contact Creation and Editing:** 
                              Users can add new contacts or modify existing ones with details like name, phone number, email, and type.
**Contact Search:**
                Quickly locate contacts by name, email, or phone number.
**Binary Search Tree (BST) Storage:** 
                                 Contacts are stored in a BST for efficient search and retrieval.
**Contact Deletion:**
                Users can easily remove unwanted contacts from the system.
**Email Validation:** 
                      Ensures contacts have valid email addresses ending with "@gmail.com".

## **Code Structure:**

**Contact Class:**
Represents a contact with attributes like name, number, email, and type.
**Node Class:**
Represents a node in the BST, containing a contact object and references to left and right child nodes.
**BST Class:**
Implements a Binary Search Tree for storing and managing contacts. Provides functions for insertion, searching, deletion, and in-order traversal.
**PhoneBookApp Class:**
Manages user interaction with the contact management system. Handles authentication, menu navigation, and calls relevant functions from the BST class.

## **Running the Application:**

Compile the C++ code using a suitable compiler like g++ or clang++.
Run the compiled executable.
The application will prompt for password authentication.
Once authenticated, the user interface displays a menu for adding, editing, searching, deleting, and displaying all contacts.

**Further Enhancements:**

**Password Hashing**: Implement secure password hashing for improved security.
**Platform Independence:** Consider using cross-platform libraries like ncurses for a more portable UI.
**Contact Grouping:** Allow users to organize contacts into groups for better categorization.
**Data Persistence:** Explore options like file I/O or databases to save contacts permanently.
