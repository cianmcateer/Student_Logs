// Cian McAteer C++ CA1
// E-School
#include <iostream>
#include <string>
#include <fstream>
#include <iterator>
#include <vector>
#include <termios.h>
#include <unistd.h>
#include <cctype>
#include <algorithm>

#include "Student.h"
#include "Student_Store.h"
#include "util.h"

// Preprocessor macro variable when set to true
// will skip password validation for testing purposes
#define DEBUG false

using std::string;
using std::cout;
using std::cin;
using std::endl;

void init();
void menu();
void selection(Student_Store& st, int& choice);
string school_status(float& co_ef);


/**
* Entry function initialises Program
* @return 0
*/
int main(void) {
    DEBUG ? menu() : init();
    return 0;
}

/**
* initial start of application and password validator
*/
void init() {
    cout << "Welcome to e-school.com" << endl;

    cout << "Please enter you're name" << endl;
    string user_name;
    std::getline(cin, user_name);


    string user_password = "password";
    cout << "Please enter password" << endl;
    string password_attempt;
    cin.clear();

    // Allows us to hide user input for password
    termios old_terminal; //
    tcgetattr(STDIN_FILENO, &old_terminal);
    termios new_terminal = old_terminal;
    new_terminal.c_lflag &= ~ECHO; // Erases characters on screen '&=' Combines assignment '=' and AND'&' bitwise operators
    tcsetattr(STDIN_FILENO, TCSANOW, &new_terminal);
    std::getline(cin, password_attempt);

    int attempt = 4;
    while(!is_password(password_attempt)) {
        if(attempt < 1) {
            cout << "Access Denied, system has shut down!" << endl;
            return;
        }
        --attempt;
        cout << "Incorrect Password, try again" << endl;

        cout << "Enter password (attempts left = " << attempt << ")" << endl;
        cin.clear();
        std::getline(cin, password_attempt);
    }
    tcsetattr(STDIN_FILENO, TCSANOW, &old_terminal); // Resets masked user input

    cout << "Welcome " << user_name.substr(0,user_name.find(' ')) << "..." << endl;
    menu();

}

/**
* Local storage is initialised and allows user to choose from one of the options
*/
void menu() {

    int choice;
    Student_Store st;

    while(true) {
        read_file("menus/help.txt"); // Help menu
        while(!(cin >> choice)) { // Input validation only accepts integers
            cout << "Sorry please enter a number" << endl;
            cin.clear();
            cin.ignore(100,'\n'); // Stops message from printing more than once if user enters more than one character
        }
        selection(st, choice);
    }
}

void selection(Student_Store& st, int& choice) {
    switch(choice) {
        case 0: {
            cout << "Goodbye!" << endl;
            exit(0);
            break;
        }
        case 1: {
            // Prints out all data by overloading '<<'
            cout << st << endl;
            break;
        }
        case 2: {
            cout << "Display students" << endl;
            // Default sort using the '<' operator
            std::vector<Student> students = st.get_students();
            std::sort(students.begin(),students.end());

            st.print(students);
            break;
        }
        case 3: {
            cout << "Display by custom sort" << endl;

            std::vector<Student> students = st.get_students();

            cout << "Please enter choose from one of the following options." << endl;

            read_file("menus/sort_options.txt");

            int sort_type;

            while(!(cin >> sort_type)) {
                cout << "Please enter a number" << endl;
                cin.clear();
                cin.ignore(100,'\n'); // Stops message from printing more than once if user enters more than one character
            }

            if(sort_type == 1) {
                cout << "Sorted by Name." << endl;
                std::sort(students.begin(),students.end(),[](const Student& s1, const Student& s2) -> bool {

                    std::string upper_name1 = s1.get_name();
                    std::string upper_name2 = s2.get_name();

                    for(auto& c : upper_name1) c = toupper(c);
                    for(auto& c : upper_name2) c = toupper(c);

                    return upper_name1 < upper_name2;
                });
                st.print(students);
            } else if(sort_type == 2) {
                cout << "Sorted by Age." << endl;
                std::sort(students.begin(),students.end(),[](const Student& s1, const Student& s2) -> bool {
                    return s1.get_age() > s2.get_age();
                });
                st.print(students);
            } else if(sort_type == 3) {
                // Third argument allows us to custom sort our vector
                cout << "Sorted by GPA." << endl;
                std::sort(students.begin(),students.end(),[](const Student& s1, const Student& s2) -> bool {
                    return s1.get_gpa() > s2.get_gpa();
                });
                st.print(students);
            } else if (sort_type == 4) {
                cout << "Sorted by Attendance." << endl;
                std::sort(students.begin(),students.end(),[](const Student& s1, const Student& s2) -> bool {
                    return s1.get_attendance() > s2.get_attendance();
                });
                st.print(students);
            } else if (sort_type == 5) {
                cout << "Sorted by Comments." << endl;
                std::sort(students.begin(),students.end(),[](const Student& s1, const Student& s2) -> bool {
                    std::string upper_comment1 = s1.get_comment();
                    std::string upper_comment2 = s2.get_comment();

                    for(auto& c : upper_comment1) c = toupper(c);
                    for(auto& c : upper_comment2) c = toupper(c);

                    return upper_comment1 < upper_comment2;
                });
                st.print(students);
            }
            else {
                cout << "Invalid input" << endl;
            }

            break;
        }

        case 4: {
            cout << "Top students" << endl;
            st.top_ten();
            break;
        }

        case 5: {
            cout << "Display failing students" << endl;
            std::vector<Student> students = st.get_students();
            st.has_failed(students);
            break;
        }

        case 6: {
            cout << "Search by age" << endl;
            int age;

            do {
                cout << "Enter age" << endl;
                cin >> age;

                if(cin.fail()) {
                    cout << "Please enter a number (No characters)" << endl;
                    cin.clear();
                    cin.ignore(256, '\n');
                } else if(age > 18) {
                    cout << "Students can't be older than 18" << endl;
                } else if(age < 12) {
                    if(!is_pos(age)) {
                        cout << "Input must be a positive value" << endl;
                    } else {
                        cout << "Student is too young to attend this school" << endl;
                    }
                } else {
                    cout << "Students aged " << age << endl;
                    break;
                }
            } while(!cin.fail() || age > 18 || age < 12);

            st.search_age(age);
            break;
        }

        case 7: {

            cout << "Search by name" << endl;
            cout << "Enter search" << endl;
            string search;
            cin.ignore();
            std::getline(cin, search);

            // No special character, digits or spaces
            const string regex = "^[a-zA-Z]*$)";

            while(!is_correct(search, regex)) {
                cout << "Input mismatch please enter again" << endl;
                cin.clear();
                std::getline(cin, search);

                if(is_correct(search, regex)) {
                    break;
                }
            }
            cout << "Results for " << search << "." << endl;
            st.search_text(search, 0);
            break;
        }

        case 8: {
            cout << "Search by comment" << endl;
            string search;
            cout << "Enter search" << endl;
            cin.ignore();
            std::getline(cin, search);

            // No special character, digits
            const string regex = "^[a-zA-Z]*$";

            while(!is_correct(search, regex)) {
                cout << "Input mismatch Please enter again" << endl;
                cin.clear();
                std::getline(cin, search);

                if(is_correct(search, regex)) {
                    break;
                }
            }
            cout << "Results for " << search << "." << endl;
            st.search_text(search, 1);
            break;
        }

        case 9: {
            cout << "Search by grade" << endl;
            float grade;
            do {
                cout << "Enter grade" << endl;
                cin >> grade;

                if(cin.fail()) {
                    cout << "Please enter a number (No characters)" << endl;
                    cin.clear();
                    cin.ignore(256, '\n'); // Clear and ignore last line to prevent statement to be consta
                }
                else if(grade > 100) {
                    cout << "Grades can only be marked to 100" << endl;
                }
                else if(!is_pos(grade)){
                    cout << "Enter a positive value" << endl;
                } else {
                    cout << "Students with a grade greater or equal to " << grade << endl;
                    break; // Exit loop
                }

            } while(!is_pos(grade) || grade > 100 || !cin.fail());

            st.search_gpa(grade);
            break;
        }

        case 10: {

            char repeat = 'Y';
            while(repeat == 'Y' || repeat == 'y') {
                // Clear 'cin' of data
                cin.ignore();
                string teacher;
                cout << "Add students teacher" << endl;
                std::getline(cin, teacher); // Allow to read white spaces

                if(st.is_full(teacher)) {
                    cout << "Sorry, this class is full" << endl;
                } else {
                    const string name_regex = "[a-z\A-Z ,.'-]+$";

                    while(!is_correct(teacher, name_regex)) {
                        cout << "please try again" << endl;
                        cin.clear();
                        std::getline(cin, teacher);

                        if(is_correct(teacher, name_regex)) {
                            break;
                        }
                    }

                    cout << "Student name" << endl;
                    string student_name;
                    std::getline(cin, student_name);

                    while(!is_correct(student_name, name_regex)) {
                        cout << "please try again" << endl;
                        cin.clear();
                        std::getline(cin, student_name);

                        if(is_correct(student_name, name_regex)) {
                            break;
                        }
                    }
                    int age;
                    do {
                        cout << "Enter age" << endl;
                        cin >> age;

                        if(cin.fail()) {
                            cout << "Please enter a number (No characters)" << endl;
                            cin.clear();
                            cin.ignore(256, '\n');
                        } else if(age > 18) {
                            cout << "Students can't be older than 18" << endl;
                        } else if(age < 12) {
                            if(!is_pos(age)) {
                                cout << "Input must be a positive value" << endl;
                            } else {
                                cout << "Student is too young to attend this school" << endl;
                            }
                        } else {
                            break;
                        }
                    } while(!cin.fail() || age > 18 || age < 12);

                    cout << "Attendance" << endl;
                    int attendance;
                    do {
                        cout << "Enter attendance" << endl;
                        cin >> attendance;

                        if(cin.fail()) {
                            cout << "Please enter a number (No characters)" << endl;
                            cin.clear();
                            cin.ignore(256, '\n');
                        } else if(attendance > 100) {
                            cout << "Attendance is marked out of 100. Please enter again." << endl;
                        } else if(!is_pos(attendance)) {
                            cout << "Input must be a positive value" << endl;
                        } else {
                            break;
                        }
                    } while(!cin.fail() || !is_pos(attendance) || attendance > 100);


                    cout << "GPA" << endl;
                    float gpa;
                    do {
                        cout << "Enter grade" << endl;
                        cin >> gpa;

                        if(cin.fail()) {
                            cout << "Please enter a number (No characters)" << endl;
                            cin.clear();
                            cin.ignore(256, '\n'); // Clear and ignore last line to prevent statement to be consta
                        }
                        else if(gpa > 100) {
                            cout << "Grades can only be marked to 100" << endl;
                        }
                        else if(!is_pos(gpa)){
                            cout << "Enter a positive value" << endl;
                        } else {
                            break; // Exit loop
                        }

                    } while(!is_pos(gpa) || gpa > 100 || !cin.fail());

                    cout << "Comment" << endl;
                    string comment;
                    cin.ignore();
                    std::getline(cin,comment);

                    if(comment == "") {
                        comment = "No comment.";
                    }

                    // Put values into new student instance
                    const Student new_student(student_name,age,attendance,gpa,comment);

                    // add to map
                    st.add(teacher,new_student);

                    // print students first name only
                    cout << student_name.substr(0,student_name.find(' ')) << " has been added!" << endl;

                    cout << "Would you like to add another student? (Y/y: yes) (any other key: no)" << endl;
                    cin >> repeat;
                }
            }
            break;
        }

        case 11: {
            cin.ignore();
            cout << "Update student" << endl;
            cout << "Enter teacher name" << endl;
            string teacher;
            std::getline(cin, teacher);
            if(st.class_empty(teacher)) {
                cout << "Sorry, there are no students in " << teacher << "'s class yet." << endl;
            } else {
                const string name_regex = "[a-z\A-Z ,.'-]+$";

                while(!is_correct(teacher, name_regex)) {
                    cout << "please try again" << endl;
                    cin.clear();
                    std::getline(cin, teacher);

                    if(is_correct(teacher, name_regex)) {
                        break;
                    }
                }

                cout << "Enter student index" << endl;
                st.print_index(teacher);

                int student_at;
                cin >> student_at;

                while(cin.fail() || !is_pos(student_at) || !st.in_range(teacher, student_at)) {

                    if(!is_pos(student_at)) {
                        cout << "Please enter a positive index" << endl;
                    }
                    if(cin.fail()) {
                        cout << "Please enter a number" << endl;
                        cin.clear();
                        cin.ignore(256, '\n');
                    }
                    if(!st.in_range(teacher, student_at)) {
                        cout << "Index out of range" << endl;
                    }
                    cout << "Invalid input, please try again" << endl;
                    cin.clear();
                    cin >> student_at;
                }

                cin.ignore();

                cout << "Enter new student name (enter 's' for no change)" << endl;
                string name;
                std::getline(cin, name);

                while(!is_correct(name, name_regex)) {
                    cout << "please try again" << endl;
                    cin.clear();
                    std::getline(cin, name);

                    if(is_correct(name, name_regex)) {
                        break;
                    }
                }

                cout << "Change student age (enter -1 to leave the same)" << endl;
                int age;
                cin >> age;

                while(cin.fail() || age < -1 || age > 18) {
                    cout << "Invalid input, please try again" << endl;
                    if(age < -1) {
                        cout << "Please enter a positive index (or -1 to leave unchanged)" << endl;
                    }
                    if(cin.fail()) {
                        cout << "Please enter a number" << endl;
                        cin.clear();
                        cin.ignore(256, '\n');
                    }
                    if(age > 18) {
                        cout << "Sorry, students most be under 18 to attend" << endl;
                    }
                    cin.clear();
                    cin >> age;
                }

                cout << "Change student attendance (enter -1 to leave the same)" << endl;
                int attendance;
                cin >> attendance;

                while(cin.fail() || attendance < -1 || attendance > 100) {
                    cout << "Invalid input" << endl;

                    if(cin.fail()) {
                        cout << "No characters allowed" << endl;
                        cin.clear();
                        cin.ignore(256, '\n');
                    }
                    if(attendance < -1) {
                        cout << "Please enter a positive (or -1 to leave unchanged)" << endl;
                    }
                    if(attendance > 100) {
                        cout << "Attendance is only marked out of 100" << endl;
                    }
                    cin.clear();
                    cin >> attendance;
                }

                cout << "Change student gpa (enter -1 to leave the same)" << endl;
                float gpa;
                cin >> gpa;

                while(cin.fail() || gpa < -1 || gpa > 100) {
                    cout << "Invalid input" << endl;

                    if(cin.fail()) {
                        cout << "No characters allowed" << endl;
                        cin.clear();
                        cin.ignore(256, '\n');
                    }
                    if(gpa < -1) {
                        cout << "Please enter a positive (or -1 to leave unchanged)" << endl;
                    }
                    if(gpa > 100) {
                        cout << "Grades are only marked out of 100" << endl;
                    }
                    cin.clear();
                    cin >> gpa;
                }

                cin.ignore();
                cout << "Change comment (enter 's' for no change)" << endl;
                std::string comment;
                std::getline(cin, comment);

                st.update(teacher, student_at, name, age, attendance, gpa, comment);
            }
            break;
        }

        case 12: {
            cin.ignore();
            cout << "Create new Group" << endl;
            cout << "Please enter teacher name" << endl;
            string new_teacher;
            std::getline(cin,new_teacher);

            const string regex = "[a-z\A-Z ,.'-]+$";
            while(!is_correct(new_teacher, regex)) {
                cout << "Please try again" << endl;
                cin.clear();
                std::getline(cin, new_teacher);

                if(is_correct(new_teacher, regex)) {
                    break;
                }
            }
            st.create_group(new_teacher);
            break;
        }

        case 13: {
            st.create_webpage();
            cout << "Your webpage has been created" << endl;
            break;
        }

        case 14: {
            st.save(false);
            cout << "Data has been saved!" << endl;
            break;
        }

        case 15: {
            cout << "Please enter password to delete data" << endl;

            // Allows us to hide user input for password
            termios old_terminal; //
            tcgetattr(STDIN_FILENO, &old_terminal);
            termios new_terminal = old_terminal;
            new_terminal.c_lflag &= ~ECHO; // Erases characters on screen '&=' Combines assignment '=' and AND'&' bitwise operators
            tcsetattr(STDIN_FILENO, TCSANOW, &new_terminal);
            cin.ignore();
            string password;
            std::getline(cin, password);
            tcsetattr(STDIN_FILENO, TCSANOW, &old_terminal); // Resets masked user input

            if(is_password(password)) {
                char choice;
                cout << "Are you sure you want to delete all records?" << endl;
                cout << "Enter (Y | y) or Abort by entering any other key" << endl;

                cin >> choice;

                if(choice == 'y' || choice == 'Y') {
                    st.clear();
                    cout << "All data has been cleared" << endl;
                } else {
                    cout << "Deletion aborted" << endl;
                }
            } else {
                cout << "Access denied" << endl;
            }
            break;

        }

        case 16: {
            st.save(true);
            cout << "Your data has been saved" << endl;
            cout << "Goodbye!" << endl;
            exit(0);
            break;
        }

        case 17: {
            cin.ignore();
            cout << "Remove class" << endl;
            cout << "Please enter teacher name" << endl;
            string teacher;
            std::getline(cin, teacher);

            const string regex = "[a-z\A-Z ,.'-]+$";
            while(!is_correct(teacher, regex)) {
                cout << "Please try again" << endl;
                cin.clear();
                std::getline(cin, teacher);

                if(is_correct(teacher, regex)) {
                    break;
                }
            }
            st.remove_group(teacher);
            break;
        }

        case 18: {
            cin.ignore();
            cout << "Remove student" << endl;
            cout << "Enter students teacher" << endl;
            string teacher;
            std::getline(cin, teacher);

                const string regex = "[a-z\A-Z ,.'-]+$";
                while(!is_correct(teacher, regex)) {
                    cout << "please try again" << endl;
                    cin.clear();
                    std::getline(cin, teacher);

                    if(is_correct(teacher, regex)) {
                        break;
                    }
                }

                if(st.class_empty(teacher)) {
                    cout << "Class is empty" << endl;
                } else {
                    st.print_index(teacher);
                    cout << "Now enter index of student you wish to delete" << endl;
                    int index;
                    cin >> index;
                    while(!is_pos(index) || !st.in_range(teacher, index)) {

                        cout << "Invalid input, please try again" << endl;
                        cin >> index;

                    }
                    st.remove_student(teacher,index);
                }
            break;
        }

        case 19: {
            st.display_averages();
            break;
        }

        case 20: {
            std::vector<float> gpas = st.get_all_gpa();
            std::vector<int> attendances = st.get_all_attendance();

            float co_ef = st.pearson(attendances, gpas);
            cout << "Pearson coeffiecient of attendances and gpas: " << st.pearson(attendances, gpas) << endl;
            cout << school_status(co_ef) << endl;
            break;
        }

        case 21: {
            cout << "Add to student Records" << endl;
            st.save_school_records();
            st.clean_records();
            break;
        }

        case 22: {
            cout << "Read records" << endl;
            cout << "To read confidential school records please enter admin password" << endl;

            // Allows us to hide user input for password
            termios old_terminal; //
            tcgetattr(STDIN_FILENO, &old_terminal);
            termios new_terminal = old_terminal;
            new_terminal.c_lflag &= ~ECHO; // Erases characters on screen '&=' Combines assignment '=' and AND'&' bitwise operators
            tcsetattr(STDIN_FILENO, TCSANOW, &new_terminal);

            cin.ignore();
            string admin_password;
            std::getline(cin, admin_password);

            tcsetattr(STDIN_FILENO, TCSANOW, &old_terminal); // Resets masked user input

            if(admin_password == "admin_password") {
                st.read_records();
            } else {
                cout << "Incorrect admin password Access denied!" << endl;
            }
            break;
        }

        case 23: {
            cout << "View all alterations of data" << endl;
            st.print_log();
            break;
        }

        default: {
            cout << "Invalid input please try again." << endl;
            break;
        }
    }
}

string school_status(float& co_ef) {
    if(co_ef == 1) {
        return "There is a perfect correllation between attendance and results!";
    }
    else if(co_ef > 0.5) {
        return "Attendance and final grades are integrally linked";
    }
    else if(co_ef > 0) {
        return "There is some pattern between a student Attendance and his/her mark";
    }
    else if(co_ef == 0) {
        return "The two are completely unrelated";
    }
    else if(co_ef < 0) {
        return "It seems that students who don't go often attain higher grades";
    }
    else if(co_ef < 0.5) {
        return "Smart students don't go to school!";
    }
    else {
        return "This is an awful school!";
    }
}
