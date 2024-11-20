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