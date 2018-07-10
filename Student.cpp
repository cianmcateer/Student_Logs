#include "Student.h"

/**
* Constructor uses member Initialisation to increase efficiency
* by Initialising values instead of assigning a new value to it
*/
Student::Student(std::string name, int age, int attendance, float gpa ,std::string comment)
: name(name) ,age(age) , attendance(attendance), gpa(gpa), comment(comment)  {}

/**
* Default Constructor
*/
Student::Student() : name("Default name") ,age(0) , attendance(0), gpa(0), comment("Default comment")  {}

/**
* Destructor
*/
Student::~Student(){}

/**
* Converts student information into a string with HTML tags
* Used for HTML function in student store
*/
std::string Student::to_html() {

    // ostringstream is used instead as we
    // don't need to convert floats and ints to strings
    std::ostringstream oss;

    // Streams HTML tags and student details into oss
    // We don't need to convert non string data types
    oss << "<td>" << this->name << "</td>"
    << "<td>" << this->age << "</td>"
    << "<td>" << this->attendance << "</td>"
    << "<td>" << this->gpa << "</td>"
    << "<td>" << this->comment << "</td>";

    // Convert stringstream to string
    return oss.str();
}

/**
* Used to display students
* @param output_stream
* @param s
* @return output_stream
*/
std::ostream& operator<<(std::ostream& output_stream, const Student& s) {

    output_stream << "NAME: " << s.get_name() << " AGE: " <<
    s.get_age() << " ATTENDANCE: " <<
    s.get_attendance() << " GPA: " << s.get_gpa() << " COMMENT: " << s.get_comment() << std::endl;

    return output_stream;
}

/**
* Overloaded operator allows us to read in data from text files easier
* @param input_stream
* @param s
* @return input_stream
*/
std::istream& operator>>(std::istream& input_stream, Student& s) {

    std::string name;
    input_stream >> name;
    s.set_name(name);

    int age;
    input_stream >> age;
    s.set_age(age);

    int attendance;
    input_stream >> attendance;
    s.set_attendance(attendance);

    float gpa;
    input_stream >> gpa;
    s.set_gpa(gpa);

    std::string comment;
    input_stream >> comment;
    s.set_comment(comment);

    return input_stream;
}

/**
* Standard sort for students.
* Compares by name then age then gpa
* @param s1
* @param s2
* @return bool
*/
bool operator<(const Student& s1, const Student& s2) {

    if(s1.get_name() < s2.get_name()) return true;
    if(s2.get_name() < s1.get_name()) return false;

    if(s1.get_age() < s2.get_age()) return true;
    if(s2.get_age() < s1.get_age()) return false;

    if(s1.get_gpa() < s2.get_gpa()) return true;
    if(s2.get_gpa() < s1.get_gpa()) return false;
        
    return false;

}

/**
* Will add get_gpa to x
* @param x
* @param s
* @return float
*/
float operator+=(float& x,Student& s) {
    return x += s.get_gpa();
}
