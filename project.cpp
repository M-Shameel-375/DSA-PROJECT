#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <fstream>
#include <Windows.h>
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
        if (c == ' ')
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
        if (node == nullptr)
        {
            return;
        }

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

    void displayFavoriteContacts(Node *node) const
    {
        if (node == nullptr)
        {
            return;
        }

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
        if (root == nullptr)
        {
            cout << "\n\t\tNo Contact Found\n";
            return;
        }
        displayAllContacts(root);
    }
    void displayFavorites() const
    {
        if (root == nullptr)
        {
            cout << "\n\t\tNo Contact Found\n";
            return;
        }
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
            if (node->contact.getFavoriteStatus())
            {
                cout << "Contact " << name << " is already marked as favorite.\n";
            }
            else
            {
                node->contact.setFavoriteStatus(true);
                cout << "Contact " << name << " marked as favorite.\n";
            }
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
            if (!node->contact.getFavoriteStatus())
            {
                cout << "Contact " << name << " is not marked as favorite.\n";
            }
            else
            {
                node->contact.setFavoriteStatus(false);
                cout << "Contact " << name << " unmarked as favorite.\n";
            }
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
            if (node->contact.getBlockedStatus())
            {
                cout << "Contact " << name << " is already blocked.\n";
            }
            else
            {
                node->contact.setBlockedStatus(true);
                cout << "Contact " << name << " marked as blocked.\n";
            }
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
            if (!node->contact.getBlockedStatus())
            {
                cout << "Contact " << name << " is not blocked.\n";
            }
            else
            {
                node->contact.setBlockedStatus(false);
                cout << "Contact " << name << " unmarked as blocked.\n";
            }
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
                if (matchCountnames <= 0 && matchCountnumbers <= 0)
                {
                    break;
                }
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

class FileManager
{
private:
    string primaryFile;
    string secondaryFile;

    void writeToFile(const string &filename, const BST &bst) const
    {
        ofstream file(filename);
        if (file.is_open())
        {
            bst.writeContactsToFile(file, bst.root);
            file.close();
        }
        else
        {
            cout << "Failed to open file for writing: " << filename << endl;
        }
    }

public:
    FileManager(const string &primary, const string &secondary)
        : primaryFile(primary), secondaryFile(secondary) {}

    void saveContacts(const BST &bst) const
    {
        writeToFile(primaryFile, bst);
    }

    void saveDeletedContact(const Contact &contact) const
    {
        ofstream file(secondaryFile, ios::app);
        if (file.is_open())
        {
            file << contact.getName() << ","
                 << contact.getPhone() << ","
                 << contact.getEmail() << ","
                 << (contact.getFavoriteStatus() ? "Yes" : "No") << ","
                 << (contact.getBlockedStatus() ? "Yes" : "No") << "\n";
            file.close();
        }
        else
        {
            cout << "Failed to open file for appending: " << secondaryFile << endl;
        }
    }

    void loadContacts(BST &bst) const
    {
        ifstream file(primaryFile);
        if (file.is_open())
        {
            string line;
            while (getline(file, line))
            {
                stringstream ss(line);
                string name, phone, email, favorite, blocked;
                getline(ss, name, ',');
                getline(ss, phone, ',');
                getline(ss, email, ',');
                getline(ss, favorite, ',');
                getline(ss, blocked, ',');

                Contact contact(name, phone, email);
                contact.setFavoriteStatus(favorite == "Yes");
                contact.setBlockedStatus(blocked == "Yes");
                bst.addContact(contact);
            }
            file.close();
        }
        else
        {
            cout << "Failed to open file for reading: " << primaryFile << endl;
        }
    }

    void restoreContacts(BST &bst) const
    {
        ifstream file(secondaryFile);
        if (file.is_open())
        {
            string line;
            while (getline(file, line))
            {
                stringstream ss(line);
                string name, phone, email, favorite, blocked;
                getline(ss, name, ',');
                getline(ss, phone, ',');
                getline(ss, email, ',');
                getline(ss, favorite, ',');
                getline(ss, blocked, ',');

                Contact contact(name, phone, email);
                contact.setFavoriteStatus(favorite == "Yes");
                contact.setBlockedStatus(blocked == "Yes");
                bst.addContact(contact);
            }
            file.close();
        }
        else
        {
            cout << "Failed to open file for reading: " << secondaryFile << endl;
        }
        saveContacts(bst);

        ofstream outFile(secondaryFile, ios::trunc);
        if (outFile.is_open())
        {
            outFile.close();
            cout << "All contacts have been restored and secondary file has been cleared.\n";
        }
        else
        {
            cout << "Failed to open file for truncating: " << secondaryFile << endl;
        }
    }
};

void BST::saveContactsToSecondary(Node *node, const FileManager &fileManager)
{
    if (node == nullptr)
    {
        return;
    }
    saveContactsToSecondary(node->left, fileManager);
    fileManager.saveDeletedContact(node->contact);
    saveContactsToSecondary(node->right, fileManager);
}

class Authentication
{
    string password;
    string securityAnswer;

public:
    Authentication() : password("123"), securityAnswer("blue") {}

    void changePassword()
    {
        while (true)
        {
            passLogic(password, "Enter new password: "); // Correct function usage
            if (stringValidation(password))
            {          // Validate the password
                break; // Exit loop if the password is valid
            }
            else
            {
                cout << "\n\tInvalid Password Pattern\n";
            }
        }
        cout << "\nPassword changed successfully!\n";
    }

    void restorePassword()
    {
        string answer;
        cout << "\nTo restore your password, answer the security question.";
        while (true)
        {
            cout << "\nWhat is your favorite color? ";
            getline(cin, answer);
            if (stringValidation(answer))
            {          // Validate the security answer
                break; // Exit loop if the answer is valid
            }
            else
            {
                cout << "\nInvalid Answer. Please try again\n";
            }
        }
        if (answer == securityAnswer)
        {
            cout << "\nYour password is: " << password << endl;
        }
        else
        {
            cout << "\nIncorrect answer. Cannot restore password.\n";
        }
    }

    bool authenticate()
    {
        int attempts = 0;
        string inputPassword;
        system("cls");
        string message = "Welcome to our Contact Book!\n";
        for (char c : message)
        {
            cout << c;
            Sleep(25);
        }
        while (attempts < 3)
        {
            passLogic(inputPassword, "Enter password: "); // Correct function usage
            if (inputPassword == password)
            {
                return true;
            }
            else
            {
                cout << "\nIncorrect password. Try again.\n";
                attempts++;
            }
        }
        cout << "\nToo many failed attempts. Exiting settings.\n";
        return false;
    }
};

class PhoneBookApp : protected Authentication
{
private:
    BST bst;
    FileManager fileManager;

public:
    PhoneBookApp(const string &primaryFile, const string &secondaryFile)
        : fileManager(primaryFile, secondaryFile) {}

    bool performLogin()
    {
        return authenticate();
    }
    // Public methods to change and restore password
    void changePassword()
    {
        Authentication::changePassword();
    }
    void restorePassword()
    {
        Authentication::restorePassword();
    }

    void loadContacts()
    {
        fileManager.loadContacts(bst);
    }

    void saveContacts() const
    {
        fileManager.saveContacts(bst);
    }

    void saveDeletedContact(const Contact &contact) const
    {
        fileManager.saveDeletedContact(contact);
    }

    void restoreContacts()
    {
        fileManager.restoreContacts(bst);
    }

    void addContact(Contact contact)
    {
        bst.addContact(contact);
    }

    void deleteContactByName(const string &name)
    {
        bst.deleteContactByName(name);
    }

    void deleteAllContacts()
    {
        bst.deleteAllContacts();
    }

    void saveAllContactsToSecondary()
    {
        bst.saveAllContactsToSecondary(fileManager);
    }

    bool contactExists(const string &name) const
    {
        return bst.contactExists(name);
    }

    Contact *getContact(const string &name) const
    {
        return bst.getContact(name);
    }

    void display() const
    {
        bst.display();
    }

    void displayFavorites() const
    {

        bst.displayFavorites();
    }
    void displayBlocked() const
    {
        bst.displayBlocked();
    }
    // New function to update a contact
    void updateContact(const string &oldName, const string &newName, const string &newPhone, const string &newEmail)
    {
        bst.updateContact(oldName, newName, newPhone, newEmail);
        saveContacts(); // Save changes to the primary file
    }
    // Function to mark a contact as favorite
    void markAsFavorite(const string &name)
    {
        bst.markAsFavorite(name);
        saveContacts();
        // Save changes to the primary file
    }
    // Function to unmark a contact as favorite
    void unmarkAsFavorite(const string &name)
    {
        bst.unmarkAsFavorite(name);
        saveContacts();
        // Save changes to the primary file
    }

    void markAsBlock(const string &name)
    {
        bst.markAsBlock(name);
        saveContacts();
        // Save changes to the primary file
    }
    // Function to unmark a contact as favorite
    void unmarkAsBlock(const string &name)
    {
        bst.unmarkAsUnBlock(name);
        saveContacts();
        // Save changes to the primary file
    }
    // Method to search contacts
    void searchContacts() const
    {
        bst.searchContacts(bst.getRoot());
    }
};
void appname()
{

    system("cls");
    cout << "\t\n|||||| ";
    cout << "\t\n||    ";
    cout << "\t\n||    ";
    cout << "\t\n||    ";
    cout << "\t\n||||||";
    Sleep(120);
    system("cls");
    cout << "\t\n|||||| |||||||";
    cout << "\t\n||     ||   ||";
    cout << "\t\n||     ||   ||";
    cout << "\t\n||     ||   ||";
    cout << "\t\n|||||| |||||||";

    Sleep(120);
    system("cls");
    cout << "\t\n|||||| ||||||| ||\\\\    || ";
    cout << "\t\n||     ||   || || \\\\   || ";
    cout << "\t\n||     ||   || ||  \\\\  || ";
    cout << "\t\n||     ||   || ||   \\\\ || ";
    cout << "\t\n|||||| ||||||| ||    \\\\|| ";

    Sleep(120);
    system("cls");
    cout << "\t\n|||||| ||||||| ||\\\\    || |||||||||| ";
    cout << "\t\n||     ||   || || \\\\   ||     ||   ";
    cout << "\t\n||     ||   || ||  \\\\  ||     || ";
    cout << "\t\n||     ||   || ||   \\\\ ||     ||";
    cout << "\t\n|||||| ||||||| ||    \\\\||     || ";

    Sleep(120);
    system("cls");
    cout << "\t\n|||||| ||||||| ||\\\\    || |||||||||| ||||||||| ";
    cout << "\t\n||     ||   || || \\\\   ||     ||     ||     ||";
    cout << "\t\n||     ||   || ||  \\\\  ||     ||     ||||||||| ";
    cout << "\t\n||     ||   || ||   \\\\ ||     ||     ||     || ";
    cout << "\t\n|||||| ||||||| ||    \\\\||     ||     ||     ||";

    Sleep(120);
    system("cls");
    cout << "\t\n|||||| ||||||| ||\\\\    || |||||||||| ||||||||| ||||||| ";
    cout << "\t\n||     ||   || || \\\\   ||     ||     ||     || ||    ";
    cout << "\t\n||     ||   || ||  \\\\  ||     ||     ||||||||| ||    ";
    cout << "\t\n||     ||   || ||   \\\\ ||     ||     ||     || ||     ";
    cout << "\t\n|||||| ||||||| ||    \\\\||     ||     ||     || ||||||| ";

    Sleep(120);
    system("cls");
    cout << "\t\n|||||| ||||||| ||\\\\    || |||||||||| ||||||||| ||||||| ||||||||";
    cout << "\t\n||     ||   || || \\\\   ||     ||     ||     || ||         ||   ";
    cout << "\t\n||     ||   || ||  \\\\  ||     ||     ||||||||| ||         ||   ";
    cout << "\t\n||     ||   || ||   \\\\ ||     ||     ||     || ||         ||    ";
    cout << "\t\n|||||| ||||||| ||    \\\\||     ||     ||     || |||||||    ||    ";

    Sleep(120);
    system("cls");
    cout << "\t\n|||||| ||||||| ||\\\\    || |||||||||| ||||||||| ||||||| ||||||||   ||||||| ";
    cout << "\t\n||     ||   || || \\\\   ||     ||     ||     || ||         ||      ||   // ";
    cout << "\t\n||     ||   || ||  \\\\  ||     ||     ||||||||| ||         ||      |||||| ";
    cout << "\t\n||     ||   || ||   \\\\ ||     ||     ||     || ||         ||      ||   \\\\  ";
    cout << "\t\n|||||| ||||||| ||    \\\\||     ||     ||     || |||||||    ||      |||||||  ";

    Sleep(120);
    system("cls");
    cout << "\t\n|||||| ||||||| ||\\\\    || |||||||||| ||||||||| ||||||| ||||||||   ||||||| |||||||| ";
    cout << "\t\n||     ||   || || \\\\   ||     ||     ||     || ||         ||      ||   // ||    ||";
    cout << "\t\n||     ||   || ||  \\\\  ||     ||     ||||||||| ||         ||      ||||||  ||    || ";
    cout << "\t\n||     ||   || ||   \\\\ ||     ||     ||     || ||         ||      ||   \\\\ ||    ||  ";
    cout << "\t\n|||||| ||||||| ||    \\\\||     ||     ||     || |||||||    ||      ||||||| |||||||| ";

    Sleep(120);
    system("cls");
    cout << "\t\n|||||| ||||||| ||\\\\    || |||||||||| ||||||||| ||||||| ||||||||   ||||||| |||||||| |||||||| ";
    cout << "\t\n||     ||   || || \\\\   ||     ||     ||     || ||         ||      ||   // ||    || ||    || ";
    cout << "\t\n||     ||   || ||  \\\\  ||     ||     ||||||||| ||         ||      ||||||  ||    || ||    || ";
    cout << "\t\n||     ||   || ||   \\\\ ||     ||     ||     || ||         ||      ||   \\\\ ||    || ||    || ";
    cout << "\t\n|||||| ||||||| ||    \\\\||     ||     ||     || |||||||    ||      ||||||| |||||||| |||||||| ";
    Sleep(120);
    system("cls");
    cout << "\t\n|||||| ||||||| ||\\\\    || |||||||||| ||||||||| ||||||| ||||||||   ||||||| |||||||| |||||||| ||  //";
    cout << "\t\n||     ||   || || \\\\   ||     ||     ||     || ||         ||      ||   // ||    || ||    || || //";
    cout << "\t\n||     ||   || ||  \\\\  ||     ||     ||||||||| ||         ||      ||||||  ||    || ||    || ||//\\   ";
    cout << "\t\n||     ||   || ||   \\\\ ||     ||     ||     || ||         ||      ||   \\\\ ||    || ||    || ||/ \\\\ ";
    cout << "\t\n|||||| ||||||| ||    \\\\||     ||     ||     || |||||||    ||      ||||||| |||||||| |||||||| ||   \\\\";
}
int main()
{
    PhoneBookApp app("contacts.txt", "deleted_contacts.txt");

    if (!app.performLogin())
    {
        cout << "Login failed! Exiting...\n";
        exit(0);
    }
    app.loadContacts();
    {
        string message = "\n\nAuthentication .... ";
        for (char c : message)
        {
            cout << c;
            Sleep(25);
        }
        Sleep(500);
        message = "\nFatching Data .... ";
        for (char c : message)
        {
            cout << c;
            Sleep(25);
        }
        Sleep(500);
        message = "\nLogging .... ";
        for (char c : message)
        {
            cout << c;
            Sleep(25);
        }
        Sleep(500);
        message = "\nWelcome To ";
        for (char c : message)
        {
            cout << c;
            Sleep(25);
        }
        Sleep(500);
        appname();

                cout<< "\n\n\t\t    --- Phone Book Management System ---\n\n";

        while (true)
        {
            string choice;
            cout << "\n\t\t ______________________________________________\n";
            cout << "\t\t|       |" << setw(40) << "|\n";
            cout << "\t\t| [1]   |     Add Contact" << setw(24) << "|\n";
            cout << "\t\t| [2]   |     Edit Contact" << setw(23) << "|\n";
            cout << "\t\t| [3]   |     Search Contact" << setw(21) << "|\n";
            cout << "\t\t| [4]   |     Manage Delete Contact" << setw(14) << "|\n";
            cout << "\t\t| [5]   |     Dsiplay All Contact" << setw(16) << "|\n";
            cout << "\t\t| [6]   |     Manage Favourite Contacts" << setw(10) << "|\n";
            cout << "\t\t| [7]   |     Settings" << setw(27) << "|\n";
            cout << "\t\t| [0]   |     Exit" << setw(31) << "|\n";
            cout << "\t\t|_______|______________________________________|\n";

            while (true)
            {
                cout << "\n\t\tSelect an option: ";
                getline(cin, choice);
                if (menuChoice(choice))
                {
                    break;
                }
                cout << "\n\t\tInvalid input.\n";
            }

            if (choice == "1")
            {
                system("CLS");
                // Manually add a contact
                string name, phone, email;
                cout << "\nEnter the contact details\n";
                while (true)
                {
                    while (true)
                    {
                        cout << "\nEnter the Name: ";
                        getline(cin, name);
                        if (nameValidation(name))
                        {
                            break;
                        }
                        else
                        {
                            cout << "\n\n\tInvalid Name. Please try again.\n";
                        }
                    }
                    if (app.contactExists(name))
                    {
                        cout << "Contact with this name already exists. Please enter a different name.\n";
                    }
                    else
                    {
                        break;
                    }
                }
                while (true)
                {
                    cout << "Enter number: ";
                    getline(cin, phone);
                    if (phoneValidation(phone))
                    {
                        break;
                    }
                    else
                    {
                        cout << "Invalid number. Please try again.\n";
                    }
                }
                while (true)
                {
                    cout << "Enter email (must end with '@gmail.com'): ";
                    getline(cin, email);
                    if (emailValidation(email))
                    {
                        break;
                    }
                    else
                    {
                        cout << "\nInvalid email. Please try again.\n";
                    }
                }
                Contact newContact(name, phone, email);
                app.addContact(newContact);
                // Save contacts to the primary file
                app.saveContacts();
            }
            else if (choice == "2")
            {
                system("CLS");
                // Manually edit a contact
                string oldName, newName, newPhone, newEmail;

                // Get the old contact name
                while (true)
                {
                    cout << "\nEnter the name of the contact to edit: ";
                    getline(cin, oldName);
                    if (nameValidation(oldName))
                    {
                        break;
                    }
                    else
                    {
                        cout << "\n\n\tInvalid Name. Please try again.\n";
                    }
                }

                // Get the new name
                while (true)
                {
                    cout << "\nEnter the new name (press Enter to keep unchanged): ";
                    getline(cin, newName);
                    if (newName == "" || nameValidation(newName))
                    {
                        if (newName == "n")
                            newName = ""; // Keep unchanged
                        break;
                    }
                    else
                    {
                        cout << "\n\n\tInvalid Name. Please try again.\n";
                    }
                }

                // Get the new phone number
                while (true)
                {
                    cout << "\nEnter the new phone number (press Enter to keep unchanged): ";
                    getline(cin, newPhone);
                    if (newPhone == "" || phoneValidation(newPhone))
                    {
                        if (newPhone == "")
                            newPhone = ""; // Keep unchanged
                        break;
                    }
                    else
                    {
                        cout << "Invalid number. Please try again.\n";
                    }
                }

                // Get the new email address
                while (true)
                {
                    cout << "\nEnter the new email address (press enter to keep unchanged): ";
                    getline(cin, newEmail);
                    if (newEmail == "" || emailValidation(newEmail))
                    {
                        if (newEmail == "n")
                            newEmail = ""; // Keep unchanged
                        break;
                    }
                    else
                    {
                        cout << "\nInvalid email. Please try again.\n";
                    }
                }

                // Update the contact
                app.updateContact(oldName, newName, newPhone, newEmail);
            }

            else if (choice == "3")
            {
                app.searchContacts();
            }
            else if (choice == "4")
            {
                system("CLS");
                while (true)
                {
                    string del;
                    cout << "\n\t\t ______________________________________________\n";
                    cout << "\t\t|       |" << setw(40) << "|\n";
                    cout << "\t\t| [1]   |     Delete Contact by Name" << setw(13) << "|\n";
                    cout << "\t\t| [2]   |     Delete all Contacts" << setw(16) << "|\n";
                    cout << "\t\t| [0]   |     Back" << setw(31) << "|\n";
                    cout << "\t\t|_______|______________________________________|\n";
                    while (true)
                    {
                        cout << "\n\t\tSelect an option: ";
                        getline(cin, del);
                        if (menuChoice(del))
                        {
                            break;
                        }
                        cout << "\n\t\tInvalid input.\n";
                    }

                    if (del == "1")
                    {
                        // Manually delete a contact
                        string deleteName;
                        cout << "\nEnter the name of the contact to delete: ";
                        getline(cin, deleteName);
                        Contact *deletedContact = app.getContact(deleteName);
                        if (deletedContact)
                        {
                            app.saveDeletedContact(*deletedContact); // Save to secondary file
                            app.deleteContactByName(deleteName);
                            system("CLS");
                            cout << "\nContact " << deleteName << " deleted.\n";
                        }
                        else
                        {
                            system("CLS");
                            cout << "\nContact not found.\n";
                        }
                        app.saveContacts();
                    }
                    else if (del == "2")
                    {
                        string deleteAll;
                        cout << "\nDo you want to delete all contacts? (y/n): ";
                        cin >> deleteAll;
                        cin.ignore(); // Clear the newline character from the buffer
                        if (deleteAll == "y" || deleteAll == "Y")
                        {                                     // Save all contacts to the secondary file before deleting
                            app.saveAllContactsToSecondary(); // Delete all contacts
                            app.deleteAllContacts();
                            app.saveContacts(); // Save the empty state
                            system("CLS");
                            cout << "\nAll contacts have been deleted and moved to the secondary file.\n";
                        }
                    }
                    else if (del == "0")
                    {
                        system("CLS");
                        cout << "\nBack to menu..." << endl;
                        break; // Exit the loop and return to the main menu
                    }
                    else
                    {
                        system("CLS");
                        cout << "\nInvalid choice! Please try again.\n";
                    }
                }
            }
            else if (choice == "5")
            {
                system("CLS");
                // Display all contacts after adding
                app.display();
            }
            else if (choice == "6")
            {
                system("CLS");
                while (true)
                {
                    string mark;
                    string subChoice;
                    cout << "\n\t\t ______________________________________________\n";
                    cout << "\t\t|       |" << setw(40) << "|\n";
                    cout << "\t\t| [1]   |     Add to Favourite" << setw(19) << "|\n";
                    cout << "\t\t| [2]   |     Remove From Favourite" << setw(14) << "|\n";
                    cout << "\t\t| [3]   |     Display Favourtie Contacts" << setw(9) << "|\n";
                    cout << "\t\t| [0]   |     Back" << setw(31) << "|\n";
                    cout << "\t\t|_______|______________________________________|\n";

                    while (true)
                    {
                        cout << "\n\t\tSelect an option: ";
                        getline(cin, subChoice);
                        if (menuChoice(subChoice))
                        {
                            break;
                        }
                        cout << "\n\t\tInvalid input.\n";
                    }

                    if (subChoice == "1")
                    {
                        system("CLS");
                        // Mark a contact as favorite

                        while (true)
                        {
                            cout << "\nEnter the name of the contact to mark as favorite: ";
                            getline(cin, mark);
                            if (nameValidation(mark))
                            {
                                break;
                            }
                            else
                            {
                                cout << "\n\n\tInvalid Name. Please try again.\n";
                            }
                        }
                        app.markAsFavorite(mark);
                    }
                    else if (subChoice == "2")
                    {
                        // Unmark a contact as favorite
                        system("CLS");
                        while (true)
                        {
                            cout << "\nEnter the name of the contact to unmark as favorite: ";
                            getline(cin, mark);
                            if (nameValidation(mark))
                            {
                                break;
                            }
                            else
                            {
                                system("CLS");

                                cout << "\n\n\tInvalid Name. Please try again.\n";
                            }
                        }
                        app.unmarkAsFavorite(mark);
                    }
                    else if (subChoice == "3")
                    {
                        system("CLS");
                        app.displayFavorites();
                    }
                    else if (subChoice == "0")
                    {
                        system("CLS");
                        cout << "\nExit Favorite Menu" << endl;
                        break; // Exit the loop
                    }
                    else
                    {
                        system("CLS");
                        cout << "\nInvalid option! Please try again.\n";
                    }
                }
            }
            else if (choice == "7")
            {
                system("CLS");
                // Perform login
                if (!app.performLogin())
                {
                    cout << "Login failed! Returning to main menu...\n";
                    continue;
                }
                system("CLS");
                while (true)
                {
                    string settingsChoice;
                    cout << "\n\t\t ______________________________________________\n";
                    cout << "\t\t|       |" << setw(40) << "|\n";
                    cout << "\t\t| [1]   |     Change Password" << setw(20) << "|\n";
                    cout << "\t\t| [2]   |     Restore Password" << setw(19) << "|\n";
                    cout << "\t\t| [3]   |     Restore Contacts" << setw(19) << "|\n";
                    cout << "\t\t| [4]   |     Manage Block Contact" << setw(15) << "|\n";
                    cout << "\t\t| [0]   |     Back" << setw(31) << "|\n";
                    cout << "\t\t|_______|______________________________________|\n";

                    while (true)
                    {
                        cout << "\n\t\tSelect an option: ";
                        getline(cin, settingsChoice);
                        if (menuChoice(settingsChoice))
                        {
                            break;
                        }
                        cout << "\n\t\tInvalid input.\n";
                    }

                    if (settingsChoice == "1")
                    {
                        system("CLS");
                        app.changePassword();
                    }
                    else if (settingsChoice == "2")
                    {
                        system("CLS");
                        app.restorePassword();
                    }
                    else if (settingsChoice == "3")
                    {
                        system("CLS");
                        // Restore contacts from the secondary file
                        app.restoreContacts();
                    }
                    else if (settingsChoice == "4")
                    {
                        system("CLS");
                        while (true)
                        {
                            string mark;
                            string subChoice;
                            cout << "\n\t\t ______________________________________________\n";
                            cout << "\t\t|       |" << setw(40) << "|\n";
                            cout << "\t\t| [1]   |     Add to Block" << setw(23) << "|\n";
                            cout << "\t\t| [2]   |     Remove From Block" << setw(18) << "|\n";
                            cout << "\t\t| [3]   |     Display Block Contacts" << setw(13) << "|\n";
                            cout << "\t\t| [0]   |     Back" << setw(31) << "|\n";
                            cout << "\t\t|_______|______________________________________|\n";

                            while (true)
                            {
                                cout << "\n\t\tSelect an option: ";
                                getline(cin, subChoice);
                                if (menuChoice(subChoice))
                                {
                                    break;
                                }
                                cout << "\n\t\tInvalid input.\n";
                            }

                            if (subChoice == "1")
                            {
                                // Mark a contact as favorite
                                system("CLS");
                                while (true)
                                {
                                    cout << "\nEnter the name of the contact to mark as Block: ";
                                    getline(cin, mark);
                                    if (nameValidation(mark))
                                    {
                                        break;
                                    }
                                    else
                                    {
                                        cout << "\n\n\tInvalid Name. Please try again.\n";
                                    }
                                }
                                app.markAsBlock(mark);
                            }
                            else if (subChoice == "2")
                            {
                                // Unmark a contact as favorite
                                while (true)
                                {
                                    cout << "\nEnter the name of the contact to unmark as Block: ";
                                    getline(cin, mark);
                                    if (nameValidation(mark))
                                    {
                                        break;
                                    }
                                    else
                                    {
                                        cout << "\n\n\tInvalid Name. Please try again.\n";
                                    }
                                }
                                app.unmarkAsBlock(mark);
                            }
                            else if (subChoice == "3")
                            {
                                system("CLS");
                                // Display favorite contacts
                                app.displayBlocked();
                            }
                            else if (subChoice == "0")
                            {
                                system("CLS");
                                cout << "\nExit Block Menu" << endl;
                                break; // Exit the loop
                            }
                            else
                            {
                                system("CLS");
                                cout << "\nInvalid option! Please try again.\n";
                            }
                        }
                    }
                    else if (settingsChoice == "0")
                    {
                        system("CLS");
                        cout << "\nExit Settings Menu" << endl;
                        break;
                    }
                    else
                    {
                        system("CLS");
                        cout << "\nInvalid choice!" << endl;
                    }
                }
            }
            else if (choice == "0")
            {
                cout << "\nExiting...\n";
                break;
            }
            else
            {
                cout << "\n\nInvalid option! Please try again!\n";
            }
        }

        return 0;
    }
}