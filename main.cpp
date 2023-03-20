#include <iostream>
#include <fstream>
#include <sstream>
#include <math.h>
#include <stdlib.h>
#include <regex>

using namespace std;

class dnode
{
private:
    string website;
    string username;
    string password;
    dnode *prev, *next;
    dnode(string website1, string username1, string password1)
    {
        website = website1;
        username = username1;
        password = password1;
        next = NULL;
        prev = NULL;
    }
    friend class dlist;
};

class dlist
{
    dnode *head, *temp, *ptr;
    dnode *ptr1, *ptr2, *dup;

public:
    void addPassword();
    void display();
    void deletePassword(string n);
    void deleteEncryptedNode(string n);
    void addDecryptedNode(string website, string name, string password);
    void displayEncryptedPass(const char *website1, const char *username1, const char *password1);
    void searchByWebsite(string web);
    void update(string ch);
    long long encrypt(int p_letter);
    int decrypt(long long p_letter);
    int checkFile(string attempt, const char *p_fileName);
    void saveFile(string p_line, string p_fileName, const int &id);
    void createFolder();
    int getLastID();
    void addUser();
    dlist()
    {
        head = NULL;
        temp = NULL;
        ptr = NULL;
        ptr1 = NULL;
        ptr2 = NULL;
        dup = NULL;
    }
};

void dlist::addDecryptedNode(string website, string name, string password)
{
    temp = new dnode(website, name, password);
    if (head == NULL)
    {
        head = temp;
    }
    else
    {
        ptr = head;
        while (ptr->next != NULL)
        {
            ptr = ptr->next;
        }
        ptr->next = temp;
        temp->prev = ptr;
    }
}

void dlist::createFolder()
{
}

void dlist::addUser()
{
    ptr = head;

    while (ptr != NULL)
    {
        int id = 1 + getLastID();
        saveFile(ptr->website, "logweb.dat", id);
        saveFile(ptr->username, "logusers.dat", id);
        saveFile(ptr->password, "logpswds.dat", id);
        deleteEncryptedNode(ptr->website);
        ptr = ptr->next;
    }
}

void dlist::displayEncryptedPass(const char *website1, const char *username1, const char *password1)
{

    string line;
    string s1;
    vector<string> web1;
    vector<string> user1;
    vector<string> pass1;
    string website;
    string name;
    string password;
    string ans;

    ifstream file;
    file.open(website1, ios::in);
    if (file.is_open())
    {
        while (getline(file, line))
        {
            if (line[0] != '#')
            {

                long long temp = std::stoll(line);
                s1.push_back(static_cast<char>(decrypt(temp)));
            }
            else
            {
                web1.push_back(s1);
                s1.clear();
            }
        }
        file.close();
    }
    else
        cout << "Unable to open file";

    ifstream file1;
    file1.open(username1, ios::in);
    if (file1.is_open())
    {
        while (getline(file1, line))
        {
            if (line[0] != '#')
            {

                long long temp = std::stoll(line);
                s1.push_back(static_cast<char>(decrypt(temp)));
            }
            else
            {
                user1.push_back(s1);
                s1.clear();
            }
        }
        file1.close();
    }
    else
        cout << "Unable to open file";

    ifstream file2;
    file2.open(password1, ios::in);
    if (file2.is_open())
    {
        while (getline(file2, line))
        {
            if (line[0] != '#')
            {

                long long temp = std::stoll(line);
                s1.push_back(static_cast<char>(decrypt(temp)));
            }
            else
            {
                pass1.push_back(s1);
                s1.clear();
            }
        }
        file2.close();
    }
    else
        cout << "Unable to open file";

    for (unsigned int i = 0; i < web1.size(); i++)
    {
        addDecryptedNode(web1[i], user1[i], pass1[i]);
    }
    display();
    ofstream ofs;
    ofs.open(website1, std::ofstream::out | std::ofstream::trunc);
    ofs.close();
    ofs.open(username1, std::ofstream::out | std::ofstream::trunc);
    ofs.close();
    ofs.open(password1, std::ofstream::out | std::ofstream::trunc);
    ofs.close();
};

int dlist::getLastID()
{
    fstream filer;
    filer.open("logusers.dat", ios::in | ios::out);
    filer.seekg(0, ios::end);
    if (filer.tellg() == 0 || filer.tellg() == -1)
        return 0;

    string s;

    for (int i = -1; s.find("#") == string::npos; i--)
    {
        filer.seekg(i, ios::end);
        filer >> s;
    }

    filer.close();
    s.erase(0, 4);

    int id;
    istringstream(s) >> id;

    return id;
}

int dlist::checkFile(string attempt, const char *p_fileName)
{
    string line;
    fstream file;

    string currentChar;
    long long eChar;

    file.open(p_fileName, ios::in);

    while (1)
    {
        file >> currentChar;
        if (currentChar.find("#ID:") != string::npos)
        {
            if (attempt == line)
            {
                file.close();
                currentChar.erase(0, 4);
                int id;
                istringstream(currentChar) >> id;
                return id;
            }
            else
            {
                line.erase(line.begin(), line.end());
            }
        }
        else
        {
            istringstream(currentChar) >> eChar;
            line += (char)decrypt(eChar);
            currentChar = "";
        }

        if (file.peek() == EOF)
        {
            file.close();
            return 0;
        }
    }
}

void dlist::saveFile(string p_line, string p_fileName, const int &id)
{
    fstream file;
    file.open(p_fileName, ios::app);
    file.seekg(0, ios::end);

    if (file.tellg() != 0)
        file << "\n";

    file.seekg(0, ios::beg);

    for (unsigned int i = 0; i < p_line.length(); i++)
    {
        file << encrypt(p_line[i]);
        file << "\n";
    }

    file << "#ID:" << id;

    file.close();
}

long long dlist::encrypt(int p_letter)
{
    return powf(p_letter, 5) * 4 - 14;
}
int dlist::decrypt(long long p_letter)
{
    return powf((p_letter + 14) / 4, 1 / 5.f);
}

void dlist::addPassword()
{
    string website;
    string name;
    string password;
    string ans;
    do
    {
        cout << "Website: ";
        cin >> website;
        cout << "UserName: ";
        cin >> name;
        cout << "Password: ";
        cin >> password;

        temp = new dnode(website, name, password);
        if (head == NULL)
        {
            head = temp;
        }
        else
        {
            ptr = head;
            while (ptr->next != NULL)
            {
                ptr = ptr->next;
            }
            ptr->next = temp;
            temp->prev = ptr;
        }

        //TODO: add yes/no validation
        cout << "Do you want to continue (y/n):- ";
        cin >> ans;
    } while (ans == "y");
}

void dlist::deletePassword(string s)
{
    int c = 0;
    ptr = head;
    while (ptr != NULL)
    {
        if (s.compare(ptr->website) == 0)
        {
            c = 1;
            break;
        }
        else
        {
            c = 2;
        }
        ptr = ptr->next;
    }
    if (c == 1 && ptr != head && ptr->next != NULL)
    {
        ptr->prev->next = ptr->next;
        ptr->next->prev = ptr->prev;
        delete (ptr);
        cout << "Your Password is successfully deleted\n\n";
    }
    else if (ptr == head)
    {
        head = ptr->next;
        delete (ptr);
        cout << "Your Password is successfully deleted\n\n";
    }
    else if (ptr->next == NULL)
    {
        ptr->prev->next = NULL;
        ptr->prev = NULL;
        delete (ptr);
        cout << "Your Password is successfully deleted\n\n";
    }
    else if (c == 2)
    {
        cout << "Website not found...";
    }
}

void dlist::deleteEncryptedNode(string s)
{
    int c = 0;
    ptr = head;
    while (ptr != NULL)
    {
        if (s.compare(ptr->website) == 0)
        {
            c = 1;
            break;
        }
        else
        {
            c = 2;
        }
        ptr = ptr->next;
    }
    if (c == 1 && ptr != head && ptr->next != NULL)
    {
        ptr->prev->next = ptr->next;
        ptr->next->prev = ptr->prev;
        delete (ptr);
    }
    else if (ptr == head && c == 1)
    {
        head = ptr->next;
        delete (ptr);
    }
    else if (ptr->next == NULL && c == 1)
    {
        ptr->prev->next = NULL;
        ptr->prev = NULL;
        delete (ptr);
    }
    else if (c == 2)
    {
        cout << "Website not found...";
    }
}

void dlist::display()
{
    ptr = head;

    while (ptr != NULL)
    {
        cout << "\n\nWebsite: " << ptr->website;
        cout << "\nUsername: " << ptr->username;
        cout << "\nPassword: " << ptr->password;
        ptr = ptr->next;
    }
}

void dlist::searchByWebsite(string na)
{
    int name_found = 0;
    ptr = head;
    while (ptr != NULL)
    {
        if (na.compare(ptr->website) == 0)
        {
            name_found = 1;
            cout << "\nWebsite Found!" << endl;
            cout << "Loading Details..." << endl;
            cout << "\nWebsite:\t" << ptr->website;
            cout << "\nUsername:\t" << ptr->username;
            cout << "\nPassword:\t" << ptr->password;
        }
        ptr = ptr->next;
    }
    if (name_found == 0)
    {
        cout << "Website not found..." << endl;
    }
}

void dlist::update(string n)
{
    char ans;
    int c;
    int is_found = 0;
    ptr = head;
    while (ptr != NULL)
    {
        if (n.compare(ptr->website) == 0)
        {
            is_found = 1;
            do
            {
                cout << "\nWhat do you want to Update...?\n1.Website:\n2.UserName:\n3.Password:\n";
                cin >> c;
                switch (c)
                {
                case 1:
                {

                    cout << "Enter New Website:-";
                    scanf("\n");
                    getline(cin, ptr->website);
                    break;
                }
                case 2:
                {

                    cout << "Enter New Username:-";
                    cin >> ptr->username;

                    break;
                }
                case 3:
                {

                    cout << "Enter New Password:-";
                    cin >> ptr->password;
                    break;
                }
                }
                cout << "Do you want to continue Updating(y/n):-";
                cin >> ans;
            } while (ans == 'y');
        }

        ptr = ptr->next;
    }
    if (is_found == 0)
    {
        cout << "Website not found..." << endl;
    }
}

class LoginManager
{
public:
    LoginManager()
    {
        accessGranted = 0;
    }
    void login()
    {
        cout << "A username is required. \nUsername:";
        cin >> userNameAttempt;

        int usrID = checkFile(userNameAttempt, "users.dat");
        if (usrID != 0)
        {
            cout << "Password:";
            cin >> passwordAttempt;

            int pwdID = checkFile(passwordAttempt, "pswds.dat");
            if (usrID == pwdID)
            {
#ifdef _WIN32
                system("cls");
#endif

#ifdef __linux__
                system("clear");
#endif

                cout << "You are successfully logged in!. \n"
                     << endl;
            }
            else
            {
                cout << "Incorrect Credentials." << endl;
                login();
            }
        }
        else
        {
            cout << "Try again" << endl;
            login();
        }
    }

    void addUser(const string username, const string password)
    {
        if (checkFile(username, "users.dat") != 0)
        {
            cout << "That username is not availble." << endl;
            return;
        }

        int id = 1 + getLastID();
        saveFile(username, "users.dat", id);
        saveFile(password, "pswds.dat", id);
#ifdef _WIN32
        system("cls");
#endif

#ifdef __linux__
        system("clear");
#endif

        cout << "You have signed up! \n"
             << endl;
    }

    int getLastID()
    {
        fstream file;
        file.open("users.dat", ios::in);
        file.seekg(0, ios::end);

        if (file.tellg() == -1)
            return 0;

        string s;

        for (int i = -1; s.find("#") == string::npos; i--)
        {
            file.seekg(i, ios::end);
            file >> s;
        }

        file.close();
        s.erase(0, 4);

        int id;
        istringstream(s) >> id;

        return id;
    }

    int checkFile(string attempt, const char *p_fileName)
    {
        string line;
        fstream file;

        string currentChar;
        long long eChar;

        file.open(p_fileName, ios::in);

        while (1)
        {
            file >> currentChar;
            if (currentChar.find("#ID:") != string::npos)
            {
                if (attempt == line)
                {
                    file.close();
                    currentChar.erase(0, 4);
                    int id;
                    istringstream(currentChar) >> id;
                    return id;
                }
                else
                {
                    line.erase(line.begin(), line.end());
                }
            }
            else
            {
                istringstream(currentChar) >> eChar;
                line += (char)decrypt(eChar);
                currentChar = "";
            }

            if (file.peek() == EOF)
            {
                file.close();
                return 0;
            }
        }
    }

    void saveFile(string p_line, const char *p_fileName, const int &id)
    {
        fstream file;
        file.open(p_fileName, ios::app);
        file.seekg(0, ios::end);

        if (file.tellg() != 0)
            file << "\n";

        file.seekg(0, ios::beg);

        for (unsigned int i = 0; i < p_line.length(); i++)
        {
            file << encrypt(p_line[i]);
            file << "\n";
        }

        file << "#ID:" << id;
        file.close();
    }

    long long encrypt(int p_letter)
    {
        return powf(p_letter, 5) * 4 - 14;
    }
    int decrypt(long long p_letter)
    {
        return powf((p_letter + 14) / 4, 1 / 5.f);
    }

private:
    string userNameAttempt;
    string passwordAttempt;
    bool accessGranted;
};

int main()
{
    LoginManager app;
    string ans;
    int ch;
    dlist passlist;
    string website;

    string name1, pass1;
    int num = -1;

    int upt = -1;

    cout << "Welcome to Password Manager \n\n1.Login\n2.Sign Up\n\nEnter a Choice: ";

    cin >> num;

    if (num == 1)
    {
        app.login();
    }
    else if (num == 2)
    {
        cout << "Enter a new Username: ";
        cin >> name1;
        cout << "Enter a Password: ";
        cin >> pass1;

        app.addUser(name1, pass1);
    }

    do
    {
        cout << "\n\n\n1.Add Password\n2.Display Passwords\n3.Delete Password\n4.Search\n5.Update Details\n6.Encrypt Passwords\n7.Display Stored Passwords\n";
        cin >> ch;
        switch (ch)
        {
        case 1:
            passlist.addPassword();
            upt = 1;
            break;
        case 2:
            passlist.display();
            break;
        case 3:
            cout << "\nEnter Website Name: ";
            cin >> website;
            passlist.deletePassword(website);
            break;
        case 4:
            cout << "\nEnter Website Name: ";
            cin >> website;
            passlist.searchByWebsite(website);
            break;
        case 5:
            cout << "\nEnter Website Name: ";
            cin >> website;
            passlist.update(website);
            break;
        case 6:
            if (upt == 1)
            {
                cout << "\nYour passwords are being encrypted...";
                passlist.addUser();
                upt = 0;
            }
            else if (upt == 0)
            {
                cout << "\nYou have not added new passwords.";
            }
            break;
        case 7:
            passlist.displayEncryptedPass("logweb.dat", "logusers.dat", "logpswds.dat");
            upt = 1;
        }
        cout << "\n\nDo you want to continue operations(y/n):- ";
        cin >> ans;
#ifdef _WIN32
        system("cls");
#endif

#ifdef __linux__
        system("clear");
#endif
    } while (ans == "y");
}
