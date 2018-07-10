#include <stack>
#include <string>
#include <fstream>

#include "Student.h"

class Student_Log {
private:
    std::stack<std::string> logs;
    std::stack<std::string> read_log();
    std::string get_time();

public:

    Student_Log();
    std::string top();
    void pop();
    bool empty();
    ~Student_Log();
    void add_log(std::string message);
    friend std::ostream& operator<<(std::ostream& output_stream, const Student_Log sl);
};
