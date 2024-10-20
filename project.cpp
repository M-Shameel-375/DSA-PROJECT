#include <iostream>
#include <string>
#include <iomanip>
#include <algorithm>
#include <Windows.h>
using namespace std;

// Class for contact details
class Contact {
public:
    string name;
    string number;
    string email;
    string type;

    Contact(string name = "", string number = "", string email = "", string type = "") 
        : name(name), number(number), email(email), type(type) {}
};

// Node class for Binary Search Tree (BST)
class Node {
public:
    Contact contact;
    Node* left;
    Node* right;

    Node(Contact contact) : contact(contact), left(nullptr), right(nullptr) {}
};

// Binary Search Tree (BST) class to store contacts
class BST {
private:
    Node* root;

    // Helper function to insert contact into the BST
    Node* insert(Node* node, Contact contact) {
        if (node == nullptr) {
            return new Node(contact);
        }

        // Insert based on name comparison, but allow same names with different numbers or emails
        if (contact.name < node->contact.name) {
            node->left = insert(node->left, contact);
        } else if (contact.name > node->contact.name) {
            node->right = insert(node->right, contact);
        } else {
            // Allow same name, but check if number or email are different
            if (contact.number != node->contact.number ) {
            node->left = insert(node->left, contact);
            
            }
            else
            {
            cout << "\nContact with the same number already exists.\n";

            }
             if (contact.email != node->contact.email) {
            node->left = insert(node->left, contact);
            cout << "\nContact with the same email already exists.\n";
            } 
            else{
            cout << "\nContact with the same email already exists.\n";


            }

        }
        return node;
    }

    // Helper function to search a contact by name
    Node* search(Node* node, string name) {
        if (node == nullptr || node->contact.name == name) {
            return node;
        }

        if (name < node->contact.name) {
            return search(node->left, name);
        }
        return search(node->right, name);
    }

    // Helper function to display contacts in-order (sorted)
    void inOrder(Node* node) {
        if (node == nullptr) return;
        inOrder(node->left);
        displayContact(node->contact);
        inOrder(node->right);
    }

    // Helper function to delete a contact from the BST
    Node* deleteNode(Node* node, string name) {
        if (node == nullptr) return node;

        if (name < node->contact.name) {
            node->left = deleteNode(node->left, name);
        } else if (name > node->contact.name) {
            node->right = deleteNode(node->right, name);
        } else {
            if (node->left == nullptr) {
                Node* temp = node->right;
                delete node;
                return temp;
            } else if (node->right == nullptr) {
                Node* temp = node->left;
                delete node;
                return temp;
            }

            Node* temp = minValueNode(node->right);
            node->contact = temp->contact;
            node->right = deleteNode(node->right, temp->contact.name);
        }
        return node;
    }

    // Helper function to find the node with minimum value (for deletion)
    Node* minValueNode(Node* node) {
        Node* current = node;
        while (current && current->left != nullptr) {
            current = current->left;
        }
        return current;
    }

public:
    BST() : root(nullptr) {}

    // Make the validation functions public to be accessible by other classes
    bool isValidPhoneNumber(const string& number) {
        // Check if the number is valid PTCL or Local number
        if (number.size() == 10 || number.size() == 11) {
            return all_of(number.begin(), number.end(), ::isdigit);
        }
        return false;
    }

    bool isValidEmail(const string& email) {
        return email.size() > 10 && email.substr(email.size() - 10) == "@gmail.com";
    }

    void insert(Contact contact) {
        root = insert(root, contact);
    }

    void searchContact(string name) {
        Node* result = search(root, name);
        if (result) {
            cout << "\nContact found:\n";
            displayContact(result->contact);
        } else {
            cout << "\nNo contact found with the name " << name << endl;
        }
    }

    void deleteContact(string name) {
        root = deleteNode(root, name);
        cout << "\nContact deleted (if it existed).\n";
    }

    void displayAllContacts() {
        cout << "\nDisplaying all contacts:\n";
        inOrder(root);
    }

    void displayContact(const Contact& contact) {
        cout << "\nName: " << contact.name
             << "\nNumber: " << contact.number
             << "\nEmail: " << contact.email
             << "\nType: " << contact.type << endl;
    }
};

class PhoneBookApp {
private:
    BST bst; // BST to store contacts
    string password = "123"; // default password

public:
    void startApp() {
        string choice;
        do {
            displayMenu();
            getline(cin, choice);
            if (choice == "1") {
                addContact();
            } else if (choice == "2") {
                editContact();
            } else if (choice == "3") {
                searchContact();
            } else if (choice == "4") {
                deleteContact();
            } else if (choice == "5") {
                displayAllContacts();
            } else if (choice == "6") {
                changePassword();
            }
        } while (choice != "0");
    }

    void addContact() {
        string name, number, email, type;

        // Input name
        cout << "\nEnter name: ";
        getline(cin, name);

        // Input and validate number
        while (true) {
            cout << "Enter number (PTCL or Local): ";
            getline(cin, number);
            if (bst.isValidPhoneNumber(number)) {
                break;
            } else {
                cout << "\nInvalid number. Please enter a valid PTCL or Local number (10-11 digits).\n";
            }
        }

        // Input and validate email
        while (true) {
            cout << "Enter email (must end with '@gmail.com'): ";
            getline(cin, email);
            if (bst.isValidEmail(email)) {
                break;
            } else {
                cout << "\nInvalid email. Please ensure the email ends with '@gmail.com'.\n";
            }
        }

        // Input type
        cout << "Enter type (PTCL, Local, Emergency): ";
        getline(cin, type);

        Contact newContact(name, number, email, type);
        bst.insert(newContact);
        cout << "\nContact added successfully!\n";
    }

    void searchContact() {
        string name;
        cout << "\nEnter the name of the contact to search: ";
        getline(cin, name);
        bst.searchContact(name);
    }

    void deleteContact() {
        string name;
        cout << "\nEnter the name of the contact to delete: ";
        getline(cin, name);
        bst.deleteContact(name);
    }

    void editContact() {
        string name;
        cout << "\nEnter the name of the contact to edit: ";
        getline(cin, name);
        bst.deleteContact(name);  // Delete existing contact
        addContact();             // Add the new contact with updated details
    }

    void displayAllContacts() {
        bst.displayAllContacts();
    }

    void displayMenu() {
        cout << "\n\n\t__________ Phone Book Menu __________\n";
        cout << "\t1. Add Contact\n";
        cout << "\t2. Edit Contact\n";
        cout << "\t3. Search Contact\n";
        cout << "\t4. Delete Contact\n";
        cout << "\t5. Display All Contacts\n";
        cout << "\t6. Change Password\n";
        cout << "\t0. Exit\n";
        cout << "\tEnter your choice: ";
    }

    void changePassword() {
        string oldPassword, newPassword;
        cout << "\nEnter old password: ";
        getline(cin, oldPassword);
        if (oldPassword == password) {
            cout << "\nEnter new password: ";
            getline(cin, newPassword);
            password = newPassword;
            cout << "\nPassword changed successfully!\n";
        } else {
            cout << "\nIncorrect old password.\n";
        }
    }
};

int main() {
    PhoneBookApp app;
    app.startApp();
    return 0;
}
