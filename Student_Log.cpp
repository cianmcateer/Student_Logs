#include "Student_Log.h"

Student_Log::Student_Log() : logs(read_log()) {}

Student_Log::~Student_Log() {}

std::stack<std::string> Student_Log::read_log() {
    std::ifstream user_log("logs.txt");
    std::stack<std::string> logs;
    if(user_log.is_open()) {
        std::string line;
        while(getline(user_log, line)) {
            logs.push(line);
        }
    } else {
        std::cerr << "Could not read log file" << std::endl;
    }
    return logs;
}

void Student_Log::add_log(std::string message) {
    // Append students instead of overwritting students
    std::ofstream user_log("logs.txt",std::fstream::in | std::ios::out | std::ios::app);
    if(user_log.is_open()) {
       user_log << message << " " << get_time();
       std::cout << std::endl;
    } else {
        std::cerr << "Unable to open log file" << std::endl;
    }
}

/**
* Returns current GMT time as string
* @return ss
*/
std::string Student_Log::get_time() {
    // current date/time based on current system
   time_t now = time(0);

   // convert now to string form
   char* dt = ctime(&now);

   // convert now to tm struct
   tm *gmtm = gmtime(&now);
   dt = asctime(gmtm);

   // Add to string stream so we can convert to string
   std::stringstream ss;
   ss << dt;

   // Convert to string before return
   return ss.str();
}

std::string Student_Log::top() {
    return logs.top();
}

void Student_Log::pop() {
    logs.pop();
}

bool Student_Log::empty() {
    return logs.empty();
}

std::ostream& operator<<(std::ostream& output_stream, Student_Log sl) {

    while(!sl.empty()) {
        output_stream << sl.top() << std::endl;
        sl.pop();
    }
    return output_stream;
}
