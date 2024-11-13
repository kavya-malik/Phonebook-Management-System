#include <iostream>
#include <string>
#include <stack>
#include <fstream>
using namespace std;

class Contact {
private:
    string name;
    string number;
    Contact* next;

public:

    Contact(string nameinput, string numberinput) {
        name = nameinput;
        number = numberinput;
        next = NULL;
    }

    void set_next(Contact* next_contact) {
        next = next_contact;
    }

    Contact* get_next() {
        return next;
    }

    string get_name() {
        return name;
    }

    string get_number() {
        return number;
    }

    void set_name(const string& nameinput) {
        name = nameinput;
    }

    void set_number(const string& numberinput) {
        number = numberinput;
    }
};

class Phonebook {
    
private:

    Contact* head;
    stack<Contact*> history;

    void save_history() {
        Contact* temp = head;
        Contact* prev = NULL;
        Contact* new_head = NULL;
        while (temp != NULL) {
            Contact* new_contact = new Contact(temp->get_name(), temp->get_number());
            if (prev == NULL) {
                new_head = new_contact;
            } else {
                prev->set_next(new_contact);
            }
            prev = new_contact;
            temp = temp->get_next();
        }
        history.push(new_head);
    }

    Contact* merge(Contact* left, Contact* right, bool by_name) {
        if (!left) return right;
        if (!right) return left;

        if (by_name) {
            if (left->get_name() < right->get_name()) {
                left->set_next(merge(left->get_next(), right, by_name));
                return left;
            } else {
                right->set_next(merge(left, right->get_next(), by_name));
                return right;
            }
        } else {
            if (left->get_number() < right->get_number()) {
                left->set_next(merge(left->get_next(), right, by_name));
                return left;
            } else {
                right->set_next(merge(left, right->get_next(), by_name));
                return right;
            }
        }
    }

    Contact* merge_sort(Contact* node, bool by_name) {
        if (!node || !node->get_next()) return node;

        Contact* middle = get_middle(node);
        Contact* left = node;
        Contact* right = middle->get_next();
        middle->set_next(NULL);

        left = merge_sort(left, by_name);
        right = merge_sort(right, by_name);

        return merge(left, right, by_name);
    }

    Contact* get_middle(Contact* node) {
        if (!node) return node;

        Contact* slow = node;
        Contact* fast = node;

        while (fast->get_next() && fast->get_next()->get_next()) {
            slow = slow->get_next();
            fast = fast->get_next()->get_next();
        }

        return slow;
    }

    void load_contacts() {
        ifstream infile("contacts.txt");
        string name, number;
        while (getline(infile, name) && getline(infile, number)) {
            Contact* new_contact = new Contact(name, number);
            if (head == NULL) {
                head = new_contact;
            } else {
                Contact* temp = head;
                while (temp->get_next() != NULL) {
                    temp = temp->get_next();
                }
                temp->set_next(new_contact);
            }
        }
        infile.close();
    }

    void save_contacts() {
        ofstream outfile("contacts.txt");
        Contact* temp = head;
        while (temp != NULL) {
            outfile << temp->get_name() << endl;
            outfile << temp->get_number() << endl;
            temp = temp->get_next();
        }
        outfile.close();
    }

    Contact* binary_search(Contact* start, string target, bool by_name) {
        Contact* left = start;
        Contact* right = NULL;

        while (left != right) {
            Contact* middle = get_middle(left);
            if (!middle) return NULL;

            if (by_name) {
                if (middle->get_name() == target) {
                    return middle;
                } else if (middle->get_name() < target) {
                    left = middle->get_next();
                } else {
                    right = middle;
                }
            } else {
                if (middle->get_number() == target) {
                    return middle;
                } else if (middle->get_number() < target) {
                    left = middle->get_next();
                } else {
                    right = middle;
                }
            }
        }

        return NULL;
    }

public:
    Phonebook() {
        head = NULL;
        load_contacts();
    }

    bool is_digit(char c) {
        return (c >= '0' && c <= '9');
    }

    bool is_valid_number(const string& number) {
        if (number.length() != 10) return false;
        for (int i = 0; i < number.length(); ++i) {
            if (!is_digit(number[i])) return false;
        }
        return true;
    }

    void add_contact() {
        string name, number;
        cout << "Enter name: ";
        cin >> name;

        do {
            cout << "Enter phone number (10 digits): ";
            cin >> number;
            if (!is_valid_number(number)) {
                cout << "Invalid number. Please enter a valid 10-digit number." << endl;
            }
        } while (!is_valid_number(number));

        save_history();

        Contact* new_contact = new Contact(name, number);
        if (head == NULL) {
            head = new_contact;
        } else {
            Contact* temp = head;
            while (temp->get_next() != NULL) {
                temp = temp->get_next();
            }
            temp->set_next(new_contact);
        }

        save_contacts();
        cout << "Contact added successfully." << endl;
    }

    void display_contacts() {
        if (head == NULL) {
            cout << "Phonebook is empty." << endl;
            return;
        }
        Contact* temp = head;
        cout << "Phonebook Contacts:" << endl;
        while (temp != NULL) {
            cout << "Name: " << temp->get_name() << ", Phone Number: " << temp->get_number() << endl;
            temp = temp->get_next();
        }
    }

    void search_contact_by_name() {
        if (head == NULL) {
            cout << "Phonebook is empty." << endl;
            return;
        }

        string name;
        cout << "Enter name to search: ";
        cin >> name;

        head = merge_sort(head, true);

        Contact* result = binary_search(head, name, true);
        if (result) {
            cout << "Phone Number of " << name << " is " << result->get_number() << endl;
        } else {
            cout << "Contact not found." << endl;
        }
    }

    void search_contact_by_number() {
        if (head == NULL) {
            cout << "Phonebook is empty." << endl;
            return;
        }

        string number;
        cout << "Enter phone number to search: ";
        cin >> number;

        head = merge_sort(head, false);

        Contact* result = binary_search(head, number, false);
        if (result) {
            cout << "Contact found: " << result->get_name() << ", Phone Number: " << result->get_number() << endl;
        } else {
            cout << "Contact not found." << endl;
        }
    }

    void delete_contact() {
        string name;
        cout << "Enter name to delete: ";
        cin >> name;

        if (head == NULL) {
            cout << "Phonebook is empty." << endl;
            return;
        }

        save_history();

        if (head->get_name() == name) {
            Contact* temp = head;
            head = head->get_next();
            delete temp;
            save_contacts();
            cout << "Contact deleted successfully." << endl;
            return;
        }

        Contact* current = head;
        while (current->get_next() != NULL && current->get_next()->get_name() != name) {
            current = current->get_next();
        }

        if (current->get_next() != NULL) {
            Contact* temp = current->get_next();
            current->set_next(temp->get_next());
            delete temp;
            save_contacts();
            cout << "Contact deleted successfully." << endl;
        } else {
            cout << "Contact not found." << endl;
        }
    }

    void edit_contact() {
        string name;
        cout << "Enter the name of the contact you want to edit: ";
        cin >> name;

        Contact* temp = head;
        while (temp != NULL) {
            if (temp->get_name() == name) {
                string new_name, new_number;
                cout << "Enter new name: ";
                cin >> new_name;

                do {
                    cout << "Enter new phone number (10 digits): ";
                    cin >> new_number;
                } while (!is_valid_number(new_number));

                save_history();

                temp->set_name(new_name);
                temp->set_number(new_number);

                save_contacts();

                cout << "Contact updated successfully." << endl;
                return;
            }
            temp = temp->get_next();
        }

        cout << "Contact not found." << endl;
    }

    void sort_contacts() {
        if (head == NULL || head->get_next() == NULL) {
            cout << "No contacts to sort." << endl;
            return;
        }

        save_history();

        int choice;
        cout << "Sort By:\n";
        cout << "1. Name\n";
        cout << "2. Number\n";
        cout << "Enter your choice: ";
        cin >> choice;

        if (choice == 1) {
            head = merge_sort(head, true);
            cout << "Contacts sorted by Name." << endl;
        } else if (choice == 2) {
            head = merge_sort(head, false);
            cout << "Contacts sorted by Number." << endl;
        } else {
            cout << "Invalid choice." << endl;
            return;
        }

        save_contacts();
    }

    void empty_phonebook() {
        if (head == NULL) {
            cout << "Phonebook is already empty." << endl;
            return;
        }

        save_history();

        while (head != NULL) {
            Contact* temp = head;
            head = head->get_next();
            delete temp;
        }

        save_contacts();
        cout << "Phonebook emptied successfully." << endl;
    }

    void undo() {
        if (history.empty()) {
            cout << "No action to undo." << endl;
            return;
        }

        head = history.top();
        history.pop();
        save_contacts();
        cout << "Undo successful. Previous state restored." << endl;
    }

    ~Phonebook() {
        while (head != NULL) {
            Contact* temp = head;
            head = head->get_next();
            delete temp;
        }
    }
};

int main() {
    Phonebook phonebook;
    int choice;
    cout<< "***************************************************************************************************************************" << endl;
     cout << "-------------------------------------------------------------------------------------------------------------------------" << endl;
    cout << "                                               Phonebook Management" << endl;
     cout << "--------------------------------------------------------------------------------------------------------------------------" << endl;
    cout << "***************************************************************************************************************************" << endl;

    do {
        cout << "\nPhonebook Menu:\n";
        cout << "1. Add Contact\n";
        cout << "2. Display Contacts\n";
        cout << "3. Search Contact\n";
        cout << "4. Delete Contact\n";
        cout << "5. Sort Contacts\n";
        cout << "6. Edit Contact\n";
        cout << "7. Empty Phonebook\n";
        cout << "8. Undo\n";
        cout << "9. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                phonebook.add_contact();
                break;
            case 2:
                phonebook.display_contacts();
                break;
            case 3: {
                int search_choice;
                cout << "Search By:\n";
                cout << "1. Name\n";
                cout << "2. Number\n";
                cout << "Enter your choice: ";
                cin >> search_choice;

                if (search_choice == 1) {
                    phonebook.search_contact_by_name();
                } else if (search_choice == 2) {
                    phonebook.search_contact_by_number();
                } else {
                    cout << "Invalid choice." << endl;
                }
                break;
            }
            case 4:
                phonebook.delete_contact();
                break;
            case 5:
                phonebook.sort_contacts();
                break;
            case 6:
                phonebook.edit_contact();
                break;
            case 7:
                phonebook.empty_phonebook();
                break;
            case 8:
                phonebook.undo();
                break;
            case 9:
                cout << "Exiting phonebook." << endl;
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
        }
    } while (choice != 9);

    return 0;
}
