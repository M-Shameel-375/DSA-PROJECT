#include <iostream>
#include <string>
#include <iomanip>
#include <algorithm>
#include <fstream>
#include <Windows.h>
using namespace std;

// Class for contact details
class Contact {
public:
    string name;
    string number;
    string email;
    string type;
    bool isFavorite; 
    Contact(string name = "", string number = "", string email = "", string type = "") 
        : name(name), number(number), email(email), type(type), isFavorite(false) {}

    friend ostream& operator<<(ostream& os, const Contact& contact) {
        os << "\nName: " << contact.name
           << "\nNumber: " << contact.number
           << "\nEmail: " << contact.email
           << "\nType: " << contact.type
           << "\nFavorite: " << (contact.isFavorite ? "Yes" : "No") << endl;
        return os;
    }
};

// Node class for Binary Search Tree (BST)
class Node {
public:
    Contact contact;
    Node* left;
    Node* right;

    Node(Contact contact) : contact(contact), left(nullptr), right(nullptr) {}
    ~Node() {
        delete left;
        delete right;
    }
};

// Binary Search Tree (BST) class to store contacts
class BST {
private:
    Node* root;

    Node* insert(Node* node, Contact contact) {
        if (node == nullptr) {
            return new Node(contact);
        }

        if (contact.name < node->contact.name) {
            node->left = insert(node->left, contact);
        } else if (contact.name > node->contact.name) {
            node->right = insert(node->right, contact);
        } else {
            if (contact.number != node->contact.number || contact.email != node->contact.email) {
                node->right = insert(node->right, contact);  // Insert with the same name but different info
            } else {
                cout << "\nContact already exists with the same name, number, and email.\n";
            }
        }
        return node;
    }


    Node* search(Node* node, const string& name) const {
        if (node == nullptr || node->contact.name == name) {
            return node;
        }
        if (name < node->contact.name) {
            return search(node->left, name);
        }
        return search(node->right, name);
    }

    void inOrder(Node* node) const {
        if (node == nullptr) return;
        inOrder(node->left);
        cout << node->contact;
        inOrder(node->right);
    }


    Node* minValueNode(Node* node) const {
        Node* current = node;
        while (current && current->left != nullptr) {
            current = current->left;
        }
        return current;
    }
    

public:
    BST() : root(nullptr) {}
    ~BST() {
        delete root;
    }

    bool isValidEmail(const string& email) const {
        return email.size() > 10 && email.substr(email.size() - 10) == "@gmail.com";
    }
    Node* deleteNode(Node* node, const string& name) {
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

    void insert(Contact contact) {
        root = insert(root, contact);
    }

    Node* searchContact(const string& name) const {
        return search(root, name);
    }
    void deleteContact(const string& name) {
    root = deleteNode(root, name);
}

    void editContact(const string& name) {
        Node* contactNode = searchContact(name);
        if (contactNode) {
            cout << "\nEditing contact details for " << name << ":\n";
            cout << "Enter new number (or press enter to keep current): ";
            string newNumber;
            getline(cin, newNumber);
            if (!newNumber.empty()) contactNode->contact.number = newNumber;

            cout << "Enter new email (or press enter to keep current): ";
            string newEmail;
            getline(cin, newEmail);
            if (!newEmail.empty() && isValidEmail(newEmail)) contactNode->contact.email = newEmail;

            cout << "Enter new type (or press enter to keep current): ";
            string newType;
            getline(cin, newType);
            if (!newType.empty()) contactNode->contact.type = newType;

            cout << "\nContact updated successfully!\n";
        } else {
            cout << "\nContact not found.\n";
        }
    }
    void markAsFavorite(const string& name) {
        Node* contactNode = searchContact(name);
        if (contactNode) {
            contactNode->contact.isFavorite = true;
            cout << "\n" << name << " has been added to favorites.\n";
        } else {
            cout << "\nContact not found.\n";
        }
    }


    void displayAllContacts() const {
        if (root == nullptr) {
            cout << "\nNo contacts available.\n";
        } else {
            cout << "\nDisplaying all contacts:\n";
            inOrder(root);
        }
    }


// Application class that manages user interaction
class PhoneBookApp {
private:
    BST bst;
    string password;

    bool authenticate() {
        int attempts = 0;
        string inputPassword;
        while (attempts < 3) {
            cout << "\nEnter password: ";
            getline(cin, inputPassword);
            if (inputPassword == password) {
                return true;
            } else {
                cout << "\nIncorrect password. Try again.\n";
                attempts++;
            }
        }
        cout << "\nToo many failed attempts. Exiting program.\n";
        return false;
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

public:
    PhoneBookApp() : password("123") {}  // Default password

    void startApp() {
        if (authenticate()) {
            string choice;
            do {
                displayMenu();
                getline(cin, choice);
                if (choice == "1") {
                    system("cls");
                    addContact();
                } else if (choice == "2") {
                    system("cls");
                    editContact();
                } else if (choice == "3") {
                    system("cls");
                    searchContact();
                } else if (choice == "4") {
                    system("cls");
                    deleteContact();
                } else if (choice == "5") {
                    system("cls");
                    manageFavorites();
                } else if (choice == "6") {
                    system("cls");
                    displayAllContacts();
                } else if (choice == "7") {
                    system("cls");
                    changePassword();
                }
            } while (choice != "0");
        }
    }

    void manageFavorites() {
        string choice;
        do {
            favoriteMenu();
            getline(cin, choice);
            if (choice == "1") {
                system("cls");
                markAsFavorite();
            } else if (choice == "2") {
                system("cls");
                // removeFromFavorite();
            } else if (choice == "3") {
                system("cls");
                // displayFavoriteContacts();
            }
        } while (choice != "0");
    }

    void favoriteMenu() {
        cout << "\n--- Favorite Contacts Menu ---";
        cout << "\n1. Add to Favorite";
        cout << "\n2. Remove from Favorite";
        cout << "\n3. Display Favorite Contacts";
        cout << "\n0. Exit";
        cout << "\nSelect an option: ";
    }

    void addContact() {
        string name, number, email, type;

        cout << "\nEnter name: ";
        getline(cin, name);

        cout << "Enter number: ";
        getline(cin, number);

        while (true) {
            cout << "Enter email (must end with '@gmail.com'): ";
            getline(cin, email);
            if (bst.isValidEmail(email)) {
                break;
            } else {
                cout << "\nInvalid email. Please ensure the email ends with '@gmail.com'.\n";
            }
        }

        cout << "Enter type (PTCL, Local, Emergency): ";
        getline(cin, type);

        Contact newContact(name, number, email, type);
        bst.insert(newContact);
        cout << "\nContact added successfully!\n";
    }


    void deleteContact() {
        string name;
        cout << "\nEnter the name of the contact to delete: ";
        getline(cin, name);
        bst.deleteNode(bst.searchContact(name), name);
        cout << "\nContact deleted (if it existed).\n";
    }
        void markAsFavorite() {
        string name;
        cout << "\nEnter the name of the contact to mark as favorite: ";
        getline(cin, name);
        bst.markAsFavorite(name);
    }

    void editContact() {
        string name;
        cout << "\nEnter the name of the contact to edit: ";
        getline(cin, name);
        bst.editContact(name);
    }

    void searchContact() {
        string name;
        cout << "\nEnter the name of the contact to search: ";
        getline(cin, name);
        Node* result = bst.searchContact(name);
        if (result) {
            cout << "\nContact found:\n" << result->contact;
        } else {
            cout << "\nNo contact found with the name " << name << endl;
        }
    }

    void displayAllContacts() {
        bst.displayAllContacts();
    }

    void displayMenu() {
        cout << "\n--- Phone Book Management System ---";
        cout << "\n1. Add Contact";
        cout << "\n2. Edit Contact";
        cout << "\n3. Search Contact";
        cout << "\n4. Delete Contact";
        cout << "\n5. Manage Favorite Contacts";
        cout << "\n6. Display All Contacts";
        cout << "\n7. Change Password";
        cout << "\n0. Exit";
        cout << "\nSelect an option: ";
    }
};

int main() 
{
    PhoneBookApp app;
    app.startApp();
    return 0;
}
