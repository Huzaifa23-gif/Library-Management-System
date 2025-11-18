#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <cctype>
#include <vector>
#include <iomanip>
#include <limits>

using namespace std;

const int title_length = 100;
const int author_length = 50;
const int genre_length = 30;
const int max_books = 1000;
const int initial_capacity = 50;

struct Book {
    int id;
    char title[title_length];
    char author[author_length];
    char genre[genre_length];
    int year;
    float price;
    int is_available; 
};

Book* books = nullptr;
int book_count = 0;
int capacity = 0;

void save_system_header() {
    ofstream file("library_header.txt");
    if (!file) {
        cout << "Error: Could not create system header file!" << endl ;
        return;
    }
    
    file << "Library Management System" << endl ;
    file << "Created by: Muhammad Huzaifa" << endl ;
    file << "Roll Number: BCSF24M552" << endl;
    file << "Total Books: " << book_count << endl ;
    file << "Data File: books.dat" << endl ;
    file << "Max Capacity: " << max_books << endl ;
    
    file.close();
    cout << "System header saved to 'library_header.txt'\n";
}

void load_system_header() {
    ifstream file("library_header.txt");
    if (!file) {
        cout << "No system header found. Creating new one.\n";
        save_system_header();
        return;
    }
    
    cout << endl << "=== SYSTEM INFORMATION ===" << endl;
    string line;
    while (getline(file, line)) {
        cout << line << endl ;
    }
    cout << endl ;
    file.close();
}

void save_books_to_binary() {
    ofstream file("books.dat", ios::binary);
    if (!file) {
        cout << "Error: Could not create binary file!\n";
        return;
    }
    
    file.write(reinterpret_cast<char*>(&book_count), sizeof(book_count));
    
    for (int i = 0; i < book_count; i++) {
        file.write(reinterpret_cast<char*>(&books[i]), sizeof(Book));
    }
    
    file.close();
    cout << "Data saved to 'books.dat' (" << book_count << " books)" << endl;
}

void load_books_from_binary() {
    ifstream file("books.dat", ios::binary);
    if (!file) {
        cout << "No data" << endl ;
        return;
    }

    int file_count;
    file.read(reinterpret_cast<char*>(&file_count), sizeof(file_count));
    
    if (file_count > capacity) {
        Book* temp = new Book[file_count + 10];
        for (int i = 0; i < book_count; i++) {
            temp[i] = books[i];
        }
        delete[] books;
        books = temp;
        capacity = file_count + 10;
    }
    
    for (int i = 0; i < file_count; i++) {
        file.read(reinterpret_cast<char*>(&books[i]), sizeof(Book));
    }
    
    book_count = file_count;
    file.close();
    cout << "Loaded " << book_count << " books from file.\n";
}

void initialize_system() {
    capacity = initial_capacity;
    books = new Book[capacity];
    book_count = 0;
    cout << "System initialized with capacity for " << capacity << " books\n";
}

void free_memory() {
    if (books != nullptr) {
        delete[] books;
        books = nullptr;
    }
    cout << "Memory freed successfully\n";
}

void resize_books_array() {
    if (book_count >= capacity) {
        int new_capacity = capacity * 2;
        Book* temp = new Book[new_capacity];
        
        for (int i = 0; i < book_count; i++) {
            temp[i] = books[i];
        }
        
        delete[] books;
        books = temp;
        capacity = new_capacity;
        cout << "Array resized to capacity: " << capacity << "\n";
    }
}

bool validate_title(const char* title) {
    if (strlen(title) == 0 || strlen(title) >= title_length) {
        return false;
    }
    return true;
}

bool validate_author(const char* author) {
    if (strlen(author) == 0 || strlen(author) >= author_length) {
        return false;
    }
    for (int i = 0; author[i]; i++) {
        if (!isalpha(author[i]) && author[i] != ' ' && author[i] != '.') {
            return false;
        }
    }
    return true;
}

bool validate_year(int year) {
    return (year >= 1000 && year <= 2024);
}

bool validate_price(float price) {
    return (price >= 0.0f);
}

void clear_input_buffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void press_enter_to_continue() {
    cout << endl << "Press Enter to continue...";
    clear_input_buffer();
    cin.get();
}

void to_lowercase(char* str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower(str[i]);
    }
}

int find_book_by_id(int id) {
    for (int i = 0; i < book_count; i++) {
        if (books[i].id == id) {
            return i;
        }
    }
    return -1;
}

int generate_book_id() {
    static int next_id = 1001;
    return next_id++;
}

void display_book(const Book* book) {
    cout << "ID: " << book->id 
         << "  Title: " << left << setw(30) << book->title
         << "  Author: " << left << setw(20) << book->author
         << "  Genre: " << left << setw(15) << book->genre
         << "  Year: " << book->year
         << "  Price: Rs." << fixed << setprecision(2) << book->price
         << "  Status: " << (book->is_available ? "Available" : "Borrowed") << endl ;
}

void add_book() {
    resize_books_array();
    
    Book new_book;
    new_book.id = generate_book_id();
    
    cout << endl << "=== ADD NEW BOOK ===" << endl ;
    
    do {
        cout << "Enter book title: ";
        cin.getline(new_book.title, title_length);
    } while (!validate_title(new_book.title));
    
    do {
        cout << "Enter author name: ";
        cin.getline(new_book.author, author_length);
    } while (!validate_author(new_book.author));
    
    cout << "Enter genre: ";
    cin.getline(new_book.genre, genre_length);
    
    do {
        cout << "Enter publication year: ";
        cin >> new_book.year;
        clear_input_buffer();
    } while (!validate_year(new_book.year));
    
    do {
        cout << "Enter price: Rs.";
        cin >> new_book.price;
        clear_input_buffer();
    } while (!validate_price(new_book.price));
    
    new_book.is_available = 1;
    books[book_count] = new_book;
    book_count++;
    
    cout << "Book added successfully! ID: " << new_book.id << "\n";
}

void display_all_books() {
    if (book_count == 0) {
        cout << "No books in the library!\n";
        return;
    }
    
    cout << "\n=== ALL BOOKS (" << book_count << ") ===\n";
    for (int i = 0; i < book_count; i++) {
        cout << setw(3) << right << i + 1 << ". ";
        display_book(&books[i]);
    }
}

void search_books_by_title() {
    if (book_count == 0) {
        cout << "No books to search!" << endl ;
        return;
    }
    
    char search_title[title_length];
    cout << "Enter title to search: ";
    cin.getline(search_title, title_length);
    
    to_lowercase(search_title);
    
    bool found = false;
    cout << endl << "Search Results:" << endl;
    
    for (int i = 0; i < book_count; i++) {
        char temp_title[title_length];
        strcpy(temp_title, books[i].title);
        to_lowercase(temp_title);
        
        if (strstr(temp_title, search_title) != nullptr) {
            display_book(&books[i]);
            found = true;
        }
    }
    
    if (!found) {
        cout << "No books found with title containing: " << search_title << "\n";
    }
}

void search_books_by_author() {
    if (book_count == 0) {
        cout << "No book to search!" << endl ;
        return;
    }
    
    char search_author[author_length];
    cout << "Enter author to search: ";
    cin.getline(search_author, author_length);
    
    to_lowercase(search_author);
    
    bool found = false;
    cout << endl << "Books by author '" << search_author << endl ;
    
    for (int i = 0; i < book_count; i++) {
        char temp_author[author_length];
        strcpy(temp_author, books[i].author);
        to_lowercase(temp_author);
        
        if (strstr(temp_author, search_author) != nullptr) {
            display_book(&books[i]);
            found = true;
        }
    }
    
    if (!found) {
        cout << "No book found by author: " << search_author << endl;
    }
}

void search_books_by_genre() {
    if (book_count == 0) {
        cout << "No book to search!" << endl ;
        return;
    }
    
    char search_genre[genre_length];
    cout << "Enter genre to search: ";
    cin.getline(search_genre, genre_length);
    
    to_lowercase(search_genre);
    
    bool found = false;
    cout << endl << "Books in genre '" << search_genre << endl ;
    
    for (int i = 0; i < book_count; i++) {
        char temp_genre[genre_length];
        strcpy(temp_genre, books[i].genre);
        to_lowercase(temp_genre);
        
        if (strcmp(temp_genre, search_genre) == 0) {
            display_book(&books[i]);
            found = true;
        }
    }
    
    if (!found) {
        cout << "No book found in genre: " << search_genre << endl ;
    }
}

void update_book() {
    if (book_count == 0) {
        cout << "No book to update!" << endl ;
        return;
    }
    
    int book_id;
    cout << "Enter book ID to update: ";
    cin >> book_id;
    clear_input_buffer();
    
    int index = find_book_by_id(book_id);
    if (index == -1) {
        cout << "Book with ID " << book_id << " not found!\n";
        return;
    }
    
    cout << endl << "Current book details: " << endl ;
    display_book(&books[index]);
    
    cout << endl << "Enter new details (press enter to keep current value):" << endl ;
    
    char input[title_length];
    
    cout << "Title [" << books[index].title << "]: ";
    cin.getline(input, title_length);
    if (strlen(input) > 0 && validate_title(input)) {
        strcpy(books[index].title, input);
    }
    
    cout << "Author [" << books[index].author << "]: ";
    cin.getline(input, author_length);
    if (strlen(input) > 0 && validate_author(input)) {
        strcpy(books[index].author, input);
    }
    
    cout << "Genre [" << books[index].genre << "]: ";
    cin.getline(input, genre_length);
    if (strlen(input) > 0) {
        strcpy(books[index].genre, input);
    }
    
    cout << "Year [" << books[index].year << "]: ";
    cin.getline(input, title_length);
    if (strlen(input) > 0) {
        int new_year = atoi(input);
        if (validate_year(new_year)) {
            books[index].year = new_year;
        }
    }
    
    cout << "Price [Rs." << books[index].price << "]: ";
    cin.getline(input, title_length);
    if (strlen(input) > 0) {
        float new_price = atof(input);
        if (validate_price(new_price)) {
            books[index].price = new_price;
        }
    }
    
    cout << "Book updated successfully!" << endl;
}

void delete_book() {
    if (book_count == 0) {
        cout << "No book to delete!" << endl ;
        return;
    }
    
    int book_id;
    cout << "Enter book ID to delete: ";
    cin >> book_id;
    clear_input_buffer();
    
    int index = find_book_by_id(book_id);
    if (index == -1) {
        cout << "Book with ID " << book_id << " not found!" << endl ;
        return;
    }
    
    cout << "Book to delete:\n";
    display_book(&books[index]);
    
    char confirm;
    cout << "Are you sure? (y/n): ";
    cin >> confirm;
    clear_input_buffer();
    
    if (confirm == 'y' || confirm == 'Y') {
        for (int i = index; i < book_count - 1; i++) {
            books[i] = books[i + 1];
        }
        book_count--;
        cout << "Book deleted successfully!" << endl ;
    } else {
        cout << "Deletion cancelled!" << endl;
    }
}

void borrow_book() {
    if (book_count == 0) {
        cout << "No book available!" << endl ;
        return;
    }
    
    int book_id;
    cout << "Enter book ID to borrow: ";
    cin >> book_id;
    clear_input_buffer();
    
    int index = find_book_by_id(book_id);
    if (index == -1) {
        cout << "Book with ID " << book_id << " not found!\n";
        return;
    }
    
    if (!books[index].is_available) {
        cout << "Book is already borrowed!\n";
        return;
    }
    
    books[index].is_available = 0;
    cout << "Book '" << books[index].title << "' borrowed successfully!\n";
}

void return_book() {
    if (book_count == 0) {
        cout << "No books in the system!" << endl ;
        return;
    }
    
    int book_id;
    cout << "Enter book ID to return: ";
    cin >> book_id;
    clear_input_buffer();
    
    int index = find_book_by_id(book_id);
    if (index == -1) {
        cout << "Book with ID " << book_id << " not found!" << endl;
        return;
    }
    
    if (books[index].is_available) {
        cout << "Book is already available!" << endl;
        return;
    }
    
    books[index].is_available = 1;
    cout << "Book '" << books[index].title << "' returned successfully!" << endl ;
}

void display_available_books() {
    if (book_count == 0) {
        cout << "No books in the library!" << endl;
        return;
    }
    
    bool found = false;
    cout << endl << "=== AVAILABLE BOOKS ===" << endl ;
    
    for (int i = 0; i < book_count; i++) {
        if (books[i].is_available) {
            display_book(&books[i]);
            found = true;
        }
    }
    
    if (!found) {
        cout << "No available book right now!" << endl ;
    }
}

void display_borrowed_books() {
    if (book_count == 0) {
        cout << "No books in the library!" << endl ;
        return;
    }
    
    bool found = false;
    cout << endl << "=== BORROWED BOOKS ===" << endl ;
    
    for (int i = 0; i < book_count; i++) {
        if (!books[i].is_available) {
            display_book(&books[i]);
            found = true;
        }
    }
    
    if (!found) {
        cout << "No borrowed books at the moment.\n";
    }
}

void display_statistics() {
    if (book_count == 0) {
        cout << "No books in the library!" << endl;
        return;
    }
    
    int available_count = 0;
    int borrowed_count = 0;
    float total_value = 0.0f;
    
    for (int i = 0; i < book_count; i++) {
        total_value += books[i].price;
        if (books[i].is_available) {
            available_count++;
        } else {
            borrowed_count++;
        }
    }
    
    cout << endl << "\t LIBRARY STATISTICS " << endl;
    cout << "Total Books: " << book_count << endl;
    cout << "Available Books: " << available_count << endl;
    cout << "Borrowed Books: " << borrowed_count << endl;
    cout << "Total Collection Value: $" << fixed << setprecision(2) << total_value << endl;
    cout << "Average Book Price: $" << fixed << setprecision(2) << (total_value / book_count) << endl;
}

void search_books_menu() {
    int choice;
    
    cout << "\t SEARCH BOOKS " << endl;
    cout << "1. Search by Title" << endl;
    cout << "2. Search by Author" << endl;
    cout << "3. Search by Genre" << endl;
    cout << "Enter choice: ";
    cin >> choice;
    clear_input_buffer();
    
    switch (choice) {
        case 1: search_books_by_title(); break;
        case 2: search_books_by_author(); break;
        case 3: search_books_by_genre(); break;
        default: cout << "Invalid choice!\n";
    }
}

void display_menu() {
    cout << "\t LIBRARY MANAGEMENT SYSTEM " << endl ;
    cout << "1. Add New Book\n";
    cout << "2. Display All Books\n";
    cout << "3. Search Books\n";
    cout << "4. Update Book Information\n";
    cout << "5. Delete Book\n";
    cout << "6. Borrow Book\n";
    cout << "7. Return Book\n";
    cout << "8. Display Available Books\n";
    cout << "9. Display Borrowed Books\n";
    cout << "10. Display Statistics\n";
    cout << "11. Save Data to File\n";
    cout << "12. Load Data from File\n";
    cout << "13. Show System Info\n";
    cout << "0. Exit\n";
    cout << "Enter your choice: ";
}

int main() {
    initialize_system();
    load_system_header();
    
    int choice;
    
    do {
        display_menu();
        cin >> choice;
        clear_input_buffer();
        
        switch (choice) {
            case 1: add_book(); break;
            case 2: display_all_books(); break;
            case 3: search_books_menu(); break;
            case 4: update_book(); break;
            case 5: delete_book(); break;
            case 6: borrow_book(); break;
            case 7: return_book(); break;
            case 8: display_available_books(); break;
            case 9: display_borrowed_books(); break;
            case 10: display_statistics(); break;
            case 11: save_books_to_binary(); break;
            case 12: load_books_from_binary(); break;
            case 13: load_system_header(); break;
            case 0: 
                save_books_to_binary();
                cout << "Thank you for using my project!\n";
                break;
            default: 
                cout << "Invalid choice! Try again.\n";
        }
        
        if (choice != 0) {
            press_enter_to_continue();
        }
    } while (choice != 0);
    
    free_memory();
    return 0;
}
