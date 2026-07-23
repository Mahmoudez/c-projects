#include <iostream>
#include <map>
#include <vector>
#include <fstream>
#include <string>
#include <filesystem>
#include <ostream>
#include <sstream>
#include <bits/stdc++.h>
using namespace std;
vector <string>ReadFileLines(string path) {

    vector<string> lines;
    fstream file_handler(path.c_str());
    if (file_handler.fail()) {
        cout << "\n\nError: Can't opent this file\n\n";
        return lines;
    }
    string line;

    while (getline(file_handler, line)) {
        if (line.size() == 0)
            continue;
        lines.push_back(line);
    }
    file_handler.close();
    return lines;
}
void WriteFileLines(string path, vector<string>lines, bool append = true) {
    auto status = append ? (ios::in | ios::out | ios::app) : (ios::in | ios::out | ios::trunc);
    fstream file_handluer(path.c_str());
    if (file_handluer.fail()) {
        cout << "\n\n Error: Can't open this file\n\n";
        return;
    }
    for (auto& line : lines) {
        file_handluer << line << "\n";
    }
    file_handluer.close();
}
vector<string> SplitString(string str, string delimiter = ",") {
    vector<string> strs;
    string sbstr = "";
    int pos = 0;
    while ((pos = (int)str.find(delimiter)) != -1) {
        sbstr = str.substr(0, pos);
        str.erase(0, pos + delimiter.length());
        strs.push_back(sbstr);
    }
    strs.push_back(str);
    return strs;
}
int ToInt(string str) {
    istringstream oss(str);
    int num;
    oss >> num;
    return num;
}
int ReadInt(int low, int high) {
    cout << "\nEnter number in range " << low << "-" << high << ":";
    int value;
    cin >> value;
    if (low <= value && value <= high) {
        return value;
    }
    cout << "Error : invalid number .. try again\n";
    return(ReadInt(low, high));
}
int ShowReadMenu(vector<string> choices) {
    cout << "\nMenu:\n";
    for (int ch = 0; ch < (int)choices.size(); ++ch) {
        cout << "\t" << ch + 1 << choices[ch] << endl;
    }

    return ReadInt(1, (int)choices.size());
}



class User {
private:
    string name;
    int id;
    string  password;
    string email;
    bool isactive = 1;

public:
    friend class Admin;
    User() : name(""), email(""), id(-1), password("") {
    }
    User(string userData) {
        vector<string> sub_str = SplitString(userData, ",");
        if (sub_str.size() >= 5) {
            id = ToInt(sub_str[0]);
            name = sub_str[1];
          password= sub_str[2];
            email = sub_str[3];
            isactive = ToInt(sub_str[4]);
        }

    }
    void print() const {
        cout << "User name " << name << " ID " << id << '\n';
    }
    string ToString() {
        ostringstream oss;
        oss << id << "," << name << "," << password << "," << email << "," << isactive;
        return oss.str();
    }

    //seters & Geters
    void Set_user_name(string name_) {
        this->name = name_;
    }
    const string& Get_user_name() {
        return name;
    }
    void Set_user_id(int   id_) {
        this->id = id_;
    }
    const int& Get_user_id() {
        return id;
    }
    void Set_user_email(string email) {
        this->email = email;
    }
    const string& Get_user_email() {
        return email;
    }
    void Set_user_password(string password_) {
        this->password = password_;
    }
    const string& Get_user_password() {
        return password;
    }
    void Set_is_Acitve(bool activation) {
        this->isactive = activation;
    }
    const bool& Get_is_Acitve() {
        return isactive;
    } 
};
class UserManager {
private:
    map<int, vector<string>> list_books_ids;
    map<string, User> username_to_userobject;
    User current_user;
    public:
    
    int last_id = -1;
    UserManager() : last_id(0) {

    }
    void LoadDatabase() {
        last_id = 0;
        list_books_ids.clear();
        username_to_userobject.clear();
        vector<string>lines = ReadFileLines("users_book.txt");
        for (auto& line : lines) {
            User user(line);
            username_to_userobject[user.Get_user_name()] = user;
            last_id = max(last_id, user.Get_user_id());
        }
        
    }
    void LoadUserbooks(){
        vector<string> user_books = ReadFileLines("user_to_books.txt");
        for(auto & line : user_books){
             vector<string> data = SplitString(line);
            if(current_user.Get_user_id() == ToInt(data[0])){
                list_books_ids[current_user.Get_user_id()].push_back(data[1]);
            }
        }
    }
    bool  AccessSystem() {
        LoadDatabase();
        while (true) {
            int choise;
            cout << "Enter 1) for login or 2) for sign up: ";
            cin >> choise;
            if (choise == 1) {
               return DoLogin();
            }
            if (choise == 2) {
                return  DoSignin();
            }
            cout << "Invalid choice plz try again\n";
        }
    }
    bool DoLogin() {
        
        while (true) {
            cout << "Enter you name and password \n";

            string name;  cin >> name;
            string password; cin >> password;
            current_user.Set_user_name(name);
            current_user.Set_user_password(password);
            if (!username_to_userobject.count(current_user.Get_user_name())) {
                cout << "Invaleid user name \n";
                continue;
            }
            User user_exist = username_to_userobject[current_user.Get_user_name()];
            if (user_exist.Get_user_password() != current_user.Get_user_password()) {
                cout << "Invaleid user password\n";
                continue;
            }
            current_user = user_exist;
                LoadUserbooks();
            if(password[0] == 'a'){
                return true;
            }
            break;
        }
        return false;
    }
bool userhasBook(string bookidx){
     int useridx = current_user.Get_user_id();
     auto it  = list_books_ids.find(useridx);
     if(it == list_books_ids.end()){
        return false;
     }
     for(auto id_b : it->second){
        if(id_b == bookidx) return  true;
     }
     return false;
}
 vector<string> GetUserBooks(){
    int useridx = current_user.Get_user_id();
    if(!list_books_ids.count(useridx)){
        return {};
    }
    return list_books_ids[useridx];
 }
 void AddBookToCurrentUser(const string& book_id) {
    if (userhasBook(book_id))
        return;

    int uid = current_user.Get_user_id();
    list_books_ids[uid].push_back(book_id);

    ofstream out("user_to_books.txt", ios::app);
    out << uid << "," << book_id << "\n";
}



    bool DoSignin() {
        while (true)
        {
            cout << "Enter Your name and  email and password (note : please dont user spacese ):\n";
            string name, email_;
            string  pass;
            cin >> name;
            cin >> email_;
            cin >> pass;
            if (name != "") {
                last_id += 1;
                int id_ = last_id;
                current_user.Set_user_id(id_);
                current_user.Set_user_name(name);
                current_user.Set_user_email(email_);
                current_user.Set_user_password(pass);
                UpdataDatabase(current_user);
                break;
                    if(pass[0] == 'a'){
            return true;
        }else{
            return false;
        }
            }

        }
      return false;
    }
    void UpdataDatabase(User& current_user_) {
        string line = current_user_.ToString();
        vector<string>lines = ReadFileLines("users_book.txt");
        lines.push_back(line);
        WriteFileLines("users_book.txt", lines);
    }
    const User Get_Current_user() {
        return this->current_user;
    }

};

class Book {
private:
    string book_name;
    vector<string> pages;
    string book_id;
    int pages_num;
    string book_athure;
public:
    Book() {

    }
    Book(string line) {
        vector<string> bookinfo = SplitString(line);
        book_name = bookinfo[0];
        book_id = bookinfo[1];
        book_athure = bookinfo[2];
        pages_num = ToInt(bookinfo[3]);
        for(int i = 1 ; i <= pages_num ;++i){
            pages.push_back(bookinfo[i + 3]);
        }
    }
    string Tostring() {
        ostringstream oss;
        oss << book_name << "," << book_id << "," << book_athure << "," << pages_num << ",";
        for (auto page : pages)
            oss << page << ",";
        return oss.str();
    }
    void print() {
        cout << "Book name : " << book_name << "Book athour " << book_athure << " Book  pages count " << pages_num  << '\n';
         for(auto page : pages){
            cout << page << " ";
         }
         cout << " \n";
    }
    // void ReadBook() {b
    //     cout << "Enter Book name \n";
    //     cin >> book_name;
    //     cout << "Enter Book id \n";
    //     cin >> book_id;  
    //     cout << "Enter number of pages \n";
    //     cin >> pages_num;
    //     for (int i = 0; i < pages_num; i++) {
    //         cin >> pages[i];
    //     }
    //     cout << "Enter Book athure name \n";
    //     cin >> book_athure;
    // }
    void Set_Book_name(string  bookname) {
        this->book_name = bookname;
    }
    const string& Get_Book_name() {
        return book_name;
    }
    void Set_Book_athure(string book_athure) {
        this->book_athure = book_athure;
    }
    const string& Get_Book_athure() {
        return book_athure;
    }
    void Set_Book_id(string  bookid) {
        this->book_id = bookid;
    }
    const  string& Get_Book_id() {
        return book_id;
    }
    void Set_Book_pages(vector<string> pages) {
        this->pages = pages;
    }
    const  vector<string>& Get_Book_pages() {
        return pages;
    }
    void Set_Book_pages_num(int pages_num) {
        this->pages_num = pages_num;
    }
    const int& Get_Book_pages_num() {
        return pages_num;
    }


};
class BookManager {
private:
    Book current_book;
    map<string, Book> bookname_to_bookobjects;
    int last_id;
public:
    BookManager() : last_id(-1) {
    }
    void LoadDatabase() {
        bookname_to_bookobjects.clear();
        vector<string>lines = ReadFileLines("books.txt");
        // for(auto pair : lines){
                 
        // } 
    }
    void UpdateDatabase(Book book)
    {
        string line = book.Tostring();
        vector<string> lines = ReadFileLines("books.txt");
        lines.push_back(line);
        WriteFileLines("books.txt", lines);

    }
    void AddBook(Book book) {
        bookname_to_bookobjects[book.Get_Book_id()] = book;
        UpdateDatabase(book);
    }
    void updataBook(Book book, string book_id) {
        if (bookname_to_bookobjects.count(book_id))
            bookname_to_bookobjects[book_id] = book;
          cout << "You updated the book\n";
    } 
    void DeleteBook() {

    }
    map<string, Book> Get_Books_name_booksObjects() {
        return bookname_to_bookobjects;
    }
};

class Userview {
private:
    UserManager & usermanger;
    BookManager bookmangae;
public:
    Userview( UserManager  & userm) : usermanger(userm){}

     void showbook_to_read(string idx){
        vector<string> lines = ReadFileLines("books.txt");

     for (auto line : lines) {
            Book book(line);
            string bokid = book.Get_Book_id() ;
            bool flag = 0;
            if(stoi(bokid) == stoi(idx)){
                cout << '\t' << book.Get_Book_name();
                int i = 1;
                flag= 1;
                vector<string> pages = book.Get_Book_pages();
                while(i <= book.Get_Book_pages_num()){
                    cout << "Page " << i  << '\n';
                    cout << pages[i -1] << "\n";
                    cout << "Enter 1 ot next page or 0 to prev or -1 to out :\n";
                    int action; cin >> action;
                    if(action == -1){
                        break;
                    }else if(action == 1){
                        i+=1;
                    }else{
                        i--;
                    }
                }
            }else cout << "Book not found\n";
            if(flag)break;
        }
    }
    void view_profile() {
        User c_user = usermanger.Get_Current_user();
         c_user.print();
    }
    void listmyBooks(){
        auto book= usermanger.GetUserBooks();
            for(auto id  : book ){
                cout << "Book id " << id << "\n";
            }
        cout << "Enter Book Id to read :\n";
        string idx; cin >> idx;
        showbook_to_read(idx);
    }

    void List_avilabel_book() {
        cout << "\nThe syetem books \n";
        vector<string> lines = ReadFileLines("books.txt");
        // map<string, Book> booksname_objects = bookmangae.Get_Books_name_booksObjects();
        for (auto line : lines) {
            Book book(line);
            if(usermanger.userhasBook(book.Get_Book_id())){
                continue;
            }else{
                cout << "\tBook Number " << book.Get_Book_id() << endl;
                cout << "\tBook Name : "<< book.Get_Book_name() << '\n';
                cout << "\tBook Auther: "<< book.Get_Book_athure() << '\n';
            }
        }
        cout <<"Enter number of Book to Added to your list or -1 to log out \n";
        string idx; cin >> idx;
        
        if( stoi(idx) < 0 ){
            return;
        }else{
            usermanger.AddBookToCurrentUser(idx);
        }
    }
        void Display() {
        vector<string> menu;
        menu.push_back("View Profile");
        menu.push_back("list your books");
        menu.push_back("List book system");
        menu.push_back("Logout");
        int choice;
        while (true)
        {
            choice = ShowReadMenu(menu);
            if (choice == 1) {
                view_profile();
                break;
            }
            else if (choice == 2) {
                listmyBooks();
                break;
            }
            else if(choice == 3){
                 List_avilabel_book();
            }
            else {
                break;
            }
        }

    }
};
class Admin {
private:
    int id;
    int password;
    string admin_name;
    string email;
public:

    void print() {
        cout << "Admin name : " << admin_name << " ID " << id << '\n';
    }
    string Tostring() {
        ostringstream oss;
        oss << admin_name << "," << id << "," << email << " \n";
        return oss.str();
    }
    // add book to system

      // geters & seters

    void Set_admin_name(string& admin_name_) {
        this->admin_name = admin_name_;
    }
    const string& Get_admin_name() {
        return admin_name;
    }

    void Set_admin_id(int& id_) {
        this->id = id_;
    }
    const int Get_admin_id() {
        return id;
    }
    void Set_admin_email(string& email_) {
        this->email = email_;
    }
    const string& Get_admin_email() {
        return email;
    }
    void Set_admin_password(int& password_) {
        this->password = password_;
    }
    const int Get_admin_password() {
        return password;
    }


};
class Reading_section{
private :
UserManager UserManager;
BookManager bookmanager;
public:

};
class AdminView {
private:
    UserManager usermanager;
    BookManager bookmanager;
    Admin current_admin;
public:
    vector<string> menu;
    void Display() { 
        menu.push_back("View Admin profile");
        menu.push_back("Add Book");
        menu.push_back("List book system ");
        menu.push_back("List user system ");
        menu.push_back("logout");
        while (true) {
            int choice = ShowReadMenu(menu);
            if (choice == 1) {
                view_profile();
                break;
            }
            else if (choice == 2) {
                AddBook();
                break;
            }else if(choice == 3){
                List_systembook();
            }else if(choice == 4){
                Listusersystem(); 
            }
            else break;
        }
    }
    void view_profile() {
        cout << current_admin.Tostring();

    }
    void Listusersystem(){
        vector<string> lines = ReadFileLines("users_book.txt");
        for(auto & line : lines){
            User user(line);
            cout << user.Get_user_name() << '\n';
            cout << user.Get_user_id() << '\n';

        }
    }
    void AddBook() {
        // adding new book to the system
        Book New_book;
        cout << "Enter book name\n";
        string name; getline(cin , name);
        New_book.Set_Book_name(name);
        cout << "Enter Author name\n";
        string author;  getline(cin , author);
        New_book.Set_Book_athure(author);
        cout << "Enter number of pages \n";
        int pages; cin >> pages;
        New_book.Set_Book_pages_num(pages);
        vector<string> pages_content;
        for (int i = 0; i < pages; ++i) {
            string page_;
            cout << "Enter content of page " << i + 1 << "\n";
            cin >> page_;
            pages_content.push_back(page_);
        }
        New_book.Set_Book_pages(pages_content);

        bookmanager.AddBook(New_book);
    }
    void List_systembook(){
        vector<string> lines = ReadFileLines("books.txt");
        for(auto & line : lines ){
            Book book(line);
            book.print();
        }
    }

};
class online_book {
private:
    UserManager user_manager_;
    BookManager book_manager;

public:
    void LoadDatabase() {
        user_manager_.LoadDatabase();
        book_manager.LoadDatabase();
    }
    void run_system() {
        LoadDatabase();
        while (true) {
          bool accsess = user_manager_.AccessSystem();
          if(!accsess){
            Userview view(user_manager_);
            view.Display();

          }else{
            AdminView admin;
            admin.Display();

          }
        }
    }

};


int main()
{

    online_book online_red;
    online_red.run_system();
    return 0;
}
