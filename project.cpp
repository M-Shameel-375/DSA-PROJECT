#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <fstream>
#include <Windows.h>
#include <vector>
#include <conio.h>

using namespace std;

bool menuChoice(string choice)
{
    for (int i = 0; i < choice.size(); ++i)
    {
        if (!isdigit(choice[i]))
        {
            return false;
        }
    }
    if (choice.empty())
    {
        return false;
    }
    for (int i = 0; i < choice.size(); ++i)
    {
        if (choice[i] == ' ')
        {
            return false;
        }
    }
    return true;
}

bool nameValidation(const string &str)
{
    if (str.empty())
    {
        return false;
    }

    for (char c : str)
    {
        if (!isalpha(c) && c != ' ')
        {
            return false;
        }
    }
    return true;
}

bool phoneValidation(const string &phone)
{
    // Check if the phone number is not empty
    if (phone.empty())
    {
        return false;
    }

    // Check if the phone number length is less than or equal to 25 characters
    if (phone.length() > 25)
    {
        return false;
    }

    // Check if all characters in the phone number are digits
    for (char c : phone)
    {
        if (!isdigit(c))
        {
            return false;
        }
    }

    return true;
}

bool emailValidation(const string &email)
{
    const string gmailSuffix = "@gmail.com";

    if (email.empty())
    {
        return false;
    }

    for (char ch : email)
    {
        if (isspace(ch))
        {
            return false;
        }
    }

    if (email.size() <= gmailSuffix.size() + 4)
    {
        return false;
    }

    string prefix = email.substr(0, email.size() - gmailSuffix.size());
    string suffix = email.substr(email.size() - gmailSuffix.size());

    if (suffix != gmailSuffix)
    {
        return false;
    }

    for (char ch : prefix)
    {
        if (!islower(ch) && !isdigit(ch) && ch != '.' && ch != '_' && ch != '-')
        {
            return false;
        }
    }
    return true;
}

// String validation to check if not empty and not only spaces
bool stringValidation(const string &str)
{
    if (str.empty())
    {
        return false;
    }

    for (char ch : str)
    {
        if (!isspace(ch))
        {
            return true;
        }
    }

    return false;
}

// Function for password logic with masking and reveal option
void passLogic(string &password, const string &promptText)
{
    char pass[32] = {0};
    char ch;
    bool enter = false;
    int i = 0;
    bool show = false;

    cout << promptText;

    while (!enter)
    {
        ch = _getch();

        if (isalnum(ch))
        {
            pass[i] = ch;
            if (show)
            {
                cout << ch;
            }
            else
            {
                cout << "*";
            }
            i++;
        }

        if (ch == '\b' && i >= 1) // Handle backspace
        {
            cout << "\b \b";
            i--;
        }

        if (ch == '\r') // Enter key
        {
            enter = true;
        }

        if (ch == '\t') // Toggle show/hide password
        {
            show = !show;
            cout << "\r" << promptText;
            for (int j = 0; j < i; j++)
            {
                cout << (show ? pass[j] : '*');
            }
        }
    }

    pass[i] = '\0';
    password = pass;
}

class Contact
{
private:
    string name;
    string phone;
    string email;
    bool isFavorite;
    bool isBlocked;

public:
    // Default Constructor
    Contact()
        : name(""), phone(""), email(""), isFavorite(false), isBlocked(false) {}

    // Parameterized Constructor
    Contact(string n, string p = "", string e = "")
        : name(n), phone(p), email(e), isFavorite(false), isBlocked(false) {}

    // Getters
    string getName() const { return name; }
    string getPhone() const { return phone; }
    string getEmail() const { return email; }
    bool getBlockedStatus() const { return isBlocked; }
    bool getFavoriteStatus() const { return isFavorite; }

    // Setters
    void setPhone(const string &p) { phone = p; }
    void setEmail(const string &e) { email = e; }
    void setName(const string &n) { name = n; }
    void setBlockedStatus(bool status) { isBlocked = status; }
    void setFavoriteStatus(bool status) { isFavorite = status; }
};

class FileManager; // Forward declaration

class BST
{
private:
    struct Node
    {
        Contact contact;
        Node *left;
        Node *right;

        Node(Contact c) : contact(c), left(nullptr), right(nullptr) {}
    };

    Node *root;

    Node *insert(Node *node, Contact contact)
    {
        if (node == nullptr)
        {
            return new Node(contact);
        }
        if (contact.getName() < node->contact.getName())
        {
            node->left = insert(node->left, contact);
        }
        else if (contact.getName() > node->contact.getName())
        {
            node->right = insert(node->right, contact);
        }
        else
        {
            // Contact already exists
            return node;
        }
        return node;
    }

    Node *search(Node *node, const string &name) const
    {
        if (node == nullptr || node->contact.getName() == name)
        {
            return node;
        }
        if (name < node->contact.getName())
        {
            return search(node->left, name);
        }
        return search(node->right, name);
    }

    void deleteTree(Node *node)
    {
        if (node == nullptr)
        {
            return;
        }
        deleteTree(node->left);
        deleteTree(node->right);
        delete node;
    }

    void saveContactsToSecondary(Node *node, const FileManager &fileManager);

    void writeContactsToFile(ofstream &file, const Node *node) const
    {
        if (node != nullptr)
        {
            file << node->contact.getName() << ","
                 << node->contact.getPhone() << ","
                 << node->contact.getEmail() << ","
                 << (node->contact.getFavoriteStatus() ? "Yes" : "No") << ","
                 << (node->contact.getBlockedStatus() ? "Yes" : "No") << "\n";
            writeContactsToFile(file, node->left);
            writeContactsToFile(file, node->right);
        }
    }

    // Helper function to display blocked contacts
    void displayBlockContacts(Node *node, bool &found) const
    {
        if (node != nullptr)
        {
            displayBlockContacts(node->left, found);
            if (node->contact.getBlockedStatus())
            {
                found = true;
                cout << "---------------------------\n";
                cout << "Name: " << node->contact.getName() << "\n";
                cout << "Number: " << node->contact.getPhone() << "\n";
                cout << "Email: " << node->contact.getEmail() << "\n";
                cout << "Blocked: Yes\n";
                cout << "---------------------------\n";
            }
            displayBlockContacts(node->right, found);
        }
    }

public:
    BST() : root(nullptr) {}

    // Public method to get the root node
    Node *getRoot() const
    {
        return root;
    }

    void addContact(Contact contact)
    {
        root = insert(root, contact);
    }

    void deleteAllContacts()
    {
        deleteTree(root);
        root = nullptr;
        cout << "\nAll contacts have been deleted.\n";
    }

    void saveAllContactsToSecondary(const FileManager &fileManager)
    {
        saveContactsToSecondary(root, fileManager);
    }

    bool contactExists(const string &name) const
    {
        return search(root, name) != nullptr;
    }

    Contact *getContact(const string &name) const
    {
        Node *node = search(root, name);
        if (node)
        {
            return &(node->contact);
        }
        return nullptr;
    }

    void displayAllContacts(Node *node) const
    {
        if (node != nullptr)
        {
            displayAllContacts(node->left);
            cout << "---------------------------\n";
            cout << "Name: " << node->contact.getName() << "\n";
            cout << "Number: " << node->contact.getPhone() << "\n";
            cout << "Email: " << node->contact.getEmail() << "\n";
            cout << "Favorite: " << (node->contact.getFavoriteStatus() ? "Yes" : "No") << "\n";
            cout << "Blocked: " << (node->contact.getBlockedStatus() ? "Yes" : "No") << "\n";
            cout << "---------------------------\n";
            displayAllContacts(node->right);
        }
        else
        {
            cout << "\n\t\tNo Contact Found\n";
        }
    }

    void displayFavoriteContacts(Node *node) const
    {
        if (node != nullptr)
        {
            displayFavoriteContacts(node->left);
            if (node->contact.getFavoriteStatus())
            {
                cout << "---------------------------\n";
                cout << "Name: " << node->contact.getName() << "\n";
                cout << "Number: " << node->contact.getPhone() << "\n";
                cout << "Email: " << node->contact.getEmail() << "\n";
                cout << "Favorite: Yes\n";
                cout << "---------------------------\n";
            }
            displayFavoriteContacts(node->right);
        }
        else
        {
            cout << "\n\t\tNo Contact Found\n";
        }
    }

    void displayBlocked() const
    {
        bool found = false;
        displayBlockContacts(root, found);
        if (!found)
        {
            cout << "\n\t\tNo Contact Found\n";
        }
    }

    void display() const
    {
        displayAllContacts(root);
    }
    void displayFavorites() const
    {
        displayFavoriteContacts(root);
    }

    friend class FileManager; // Grant FileManager access to BST's private members

    // Helper function to find the node with the minimum value
    Node *minValueNode(Node *node)
    {
        Node *current = node;
        while (current && current->left != nullptr)
        {
            current = current->left;
        }
        return current;
    }

    // Helper function to delete a contact
    Node *deleteNode(Node *root, const string &name)
    {
        if (root == nullptr)
        {
            return root;
        }

        if (name < root->contact.getName())
        {
            root->left = deleteNode(root->left, name);
        }
        else if (name > root->contact.getName())
        {
            root->right = deleteNode(root->right, name);
        }
        else
        {
            // Node to be deleted found
            if (root->left == nullptr)
            {
                Node *temp = root->right;
                delete root;
                return temp;
            }
            else if (root->right == nullptr)
            {
                Node *temp = root->left;
                delete root;
                return temp;
            }

            // Node with two children: Get the inorder successor
            Node *temp = minValueNode(root->right);
            root->contact = temp->contact;
            root->right = deleteNode(root->right, temp->contact.getName());
        }
        return root;
    }
    
    // Public method to delete a contact by name
    void deleteContactByName(const string &name)
    {
        root = deleteNode(root, name);
    }

    // Public method to search for a contact
    Node *searchContact(const string &name) const
    {
        return search(root, name);
    }

    // Public method to update a contact
    void updateContact(const string &oldName, const string &newName, const string &newPhone, const string &newEmail)
    {
        Node *node = search(root, oldName);
        if (node)
        {
            if (!newName.empty() && newName != oldName)
            {
                // Remove the old node and insert a new one with updated name
                Contact updatedContact = node->contact;
                updatedContact.setName(newName);
                if (!newPhone.empty())
                {
                    updatedContact.setPhone(newPhone);
                }
                if (!newEmail.empty())
                {
                    updatedContact.setEmail(newEmail);
                }
                deleteContactByName(oldName); // Remove old contact
                addContact(updatedContact);   // Insert updated contact
            }
            else
            {
                // Update phone and email
                if (!newPhone.empty())
                {
                    node->contact.setPhone(newPhone);
                }
                if (!newEmail.empty())
                {
                    node->contact.setEmail(newEmail);
                }
            }
            cout << "Contact details updated.\n";
        }
        else
        {
            cout << "Contact not found.\n";
        }
    }

    // Function to mark a contact as favorite
    void markAsFavorite(const string &name)
    {
        Node *node = search(root, name);
        if (node)
        {
            node->contact.setFavoriteStatus(true);
            cout << "Contact " << name << " marked as favorite.\n";
        }
        else
        {
            cout << "Contact not found.\n";
        }
    }
    // Function to unmark a contact as favorite
    void unmarkAsFavorite(const string &name)
    {
        Node *node = search(root, name);
        if (node)
        {
            node->contact.setFavoriteStatus(false);
            cout << "Contact " << name << " unmarked as favorite.\n";
        }
        else
        {
            cout << "Contact not found.\n";
        }
    }

    // Function to mark a contact as favorite
    void markAsBlock(const string &name)
    {
        Node *node = search(root, name);
        if (node)
        {
            node->contact.setBlockedStatus(true);
            cout << "Contact " << name << " marked as favorite.\n";
        }
        else
        {
            cout << "Contact not found.\n";
        }
    }
    // Function to unmark a contact as favorite
    void unmarkAsUnBlock(const string &name)
    {
        Node *node = search(root, name);
        if (node)
        {
            node->contact.setBlockedStatus(false);
            cout << "Contact " << name << " unmarked as favorite.\n";
        }
        else
        {
            cout << "Contact not found.\n";
        }
    }
 Node *searchContact(const string &name) // Declares function with parameter
    {
        Node *result = search(root, name); // Calls recursive search function correctly
        if (result)
        {
            cout << "\nContact found:\n"
                 << result->contact.getName();
        }
        else
        {
            cout << "\nNo contact found with the name " << name << endl;
        }
        return result;
    }

    void loadNmaes(Node *node, string name[], int &i) const
    {
        if (node == nullptr)
            return;
        name[i] = node->contact.getName();
        i++;
        loadNmaes(node->right, name, i);
        loadNmaes(node->left, name, i);
        return;
    }
    void loadNunbers(Node *node, string number[], int &j) const
    {
        if (node == nullptr)
            return;
        number[j] = node->contact.getName();
        j++;
        loadNunbers(node->right, number, j);
        loadNunbers(node->left, number, j);
        return;
    }

    void searchContacts(Node *node) const
    {
        const int size = 1000;

        string name[size];
        string numbers[size];

        int i = 0;
        loadNmaes(node, name, i);

        int j = 0;
        loadNunbers(node, numbers, j);

        string *matchedNames = new string[i];
        string *matchedNumbers = new string[j];

        string currentInput = "";

        while (true)
        {
            system("cls");
            cout << " ---------------------------------------\n";
            cout << "| Enter a key to search: " << currentInput << "\t\t|";
            cout << "\n ---------------------------------------\n";

            int matchCountnames = 0;
            int matchCountnumbers = 0;

            if (!currentInput.empty())
            {
                cout << "\n\n";
                string lowerInput = currentInput;
                transform(lowerInput.begin(), lowerInput.end(), lowerInput.begin(), ::tolower);

                for (int i = 0; i < size; i++)
                {
                    string lowerName = name[i];
                    transform(lowerName.begin(), lowerName.end(), lowerName.begin(), ::tolower);

                    if (lowerName.find(lowerInput) != string::npos)
                    {
                        matchedNames[matchCountnames++] = name[i]; // Add matched name to matchedNames
                    }
                }
                for (int i = 0; i < size; i++)
                {

                    if (numbers[i].find(lowerInput) != string::npos)
                    {
                        matchedNumbers[matchCountnumbers++] = numbers[i]; // Add matched name to matchedNames
                    }
                }
                if (matchCountnames > 0 || matchCountnumbers > 0)
                {
                    for (int i = 0; i < matchCountnames; i++)
                    {
                        cout << matchedNames[i] << "\n";
                    }
                    for (int i = 0; i < matchCountnumbers; i++)
                    {
                        cout << matchedNumbers[i] << "\n";
                    }
                }

                else
                {
                    cout << "No Contact found.\n";
                }
            }
            char ch = getch();

            if (ch == 13)
            {
                // if (matchCountnames <= 0 && matchCountnumbers <= 0)
                // {
                //     break;
                // }
                string lowerInput = currentInput;
                transform(lowerInput.begin(), lowerInput.end(), lowerInput.begin(), ::tolower);

                bool found = false;

                system("CLS");
                displayFullDetails(node, lowerInput, found);
                break;
            }
            else if (ch == 27)
            { // Escape key
                cout << "\nSearch discarded. Returning to main menu.\n";
                break;
            }

            // Handle backspace
            else if (ch == 8)
            {
                if (!currentInput.empty())
                {
                    currentInput.pop_back();
                }
            }
            else
            {
                currentInput += ch;
            }
        }
    }
    void displayFullDetails(Node *node, const string &currentInput, bool &found) const
    {
        if (!node)
            return;

        displayFullDetails(node->left, currentInput, found);
        string lowerName = node->contact.getName();
        for (size_t i = 0; i < lowerName.length(); ++i)
        {
            lowerName[i] = std::tolower(lowerName[i]);
        }

        if (lowerName.find(currentInput) != string::npos || node->contact.getName().find(currentInput) != string::npos)
        {
            cout << "\n---------------------------";
            cout << "\nName: " << node->contact.getName()
                 << "\nNumber: " << node->contact.getPhone()
                 << "\nEmail: " << node->contact.getEmail()
                 << "\nFavorite: " << (node->contact.getFavoriteStatus() ? "Yes" : "No")
                 << "\nFavorite: " << (node->contact.getBlockedStatus() ? "Yes" : "No");
            cout << "\n---------------------------";
            found = true;
        }

        displayFullDetails(node->right, currentInput, found);
    }
};