#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <ostream>

/**
* Class stores pupil details
*/
class Student {
private:
    // Instance variables
    std::string name;
    int age;
    int attendance;
    float gpa;
    std::string comment;

public:

    Student(std::string name, int age, int attendance, float gpa, std::string comment); // parameterised constructor
    Student(); // Default constructor
    ~Student(); // Destructor

    // Getters and setters are inlined to allow compiler
    // Getter methods are set to const to stop changes made to getters
    // as we use our setters to modify our instance variables
    inline std::string get_name() const {
        return name;
    }

    inline int get_age() const {
        return age;
    }

    inline int get_attendance() const {
        return attendance;
    }

    inline float get_gpa() const {
        return gpa;
    }

    inline std::string get_comment() const {
        return comment;
    }

    // inline methods tell the compiler to use the code in the body of the method
    // instead of calling the method, for one line methods this can use less overhead
    // Depending on what compiler is used
    inline void set_name(std::string name) {
        this->name = name;
    }

    inline void set_age(int age) {
        this->age = age;
    }

    inline void set_attendance(int attendance) {
        this->attendance = attendance;
    }

    inline void set_gpa(float gpa) {
        this->gpa = gpa;
    }

    inline void set_comment(std::string comment) {
        this->comment = comment;
    }

    // Returns student string in HTML tags (used for webpage function in Student_Store)
    std::string to_html();

    // Overridden operators, implementation is in 'Student.cpp'
    // friend keyword is used to allow implementation access to private members of Student.h
    friend std::ostream& operator<<(std::ostream& output_stream, const Student& s);
    friend std::istream& operator>>(std::istream& input_stream, Student& s);
    friend bool operator<(const Student& s1, const Student& s2);
    friend float operator+=(float& x,Student& s);
};
