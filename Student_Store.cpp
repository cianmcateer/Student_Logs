#include "Student_Store.h"

/**
* Constructor reads saved data into map, also Initialises our record set and log stack
*/
Student_Store::Student_Store()
: school_data(Student_Store::read_file()), records(read_school_records()), logs(Student_Log()) {}

/**
* Destructor
*/
Student_Store::~Student_Store() {}

/**
* Returns a map of teachers and students that will be called if file read fails
* @return map
*/
std::map<std::string,std::vector<Student> > Student_Store::back_up_data() {

    std::map<std::string,std::vector<Student> > map;

    Student s1("Cian McAteer",13,15,44,"Good");
    Student s2("Matthew Fitzsimons",14,77,44,"Excellent");
    Student s3("Greg Hughes",16,11,33,"Bad attitude");
    std::vector<Student> vec1;
    vec1.push_back(s1);
    vec1.push_back(s2);
    vec1.push_back(s3);

    Student s4("Jamie samuel",10,13,22,"Excellent");
    Student s5("micky samuel",11,23,52,"good effort");
    Student s6("Jamie jones",12,12,11,"Wonderful");
    std::vector<Student> vec2;
    vec2.push_back(s4);
    vec2.push_back(s5);
    vec2.push_back(s6);

    map["Barbara Jones"] = vec1;
    map["John McDonald"] = vec2;

    return map;

}
/**
* Getter for school_data used in output_stream operator override
* @return school_data
*/
inline std::map<std::string, std::vector<Student> > Student_Store::get_map() const {
    return school_data;
}

/**
* Replaces characters where ever they occur. Used to read file data in
* @param s
* @param old_char
* @param new_char
*/
void Student_Store::replace_characters(Student& s, char old_char, char new_char) {
    std::string new_name = s.get_name();
    std::string new_comment = s.get_comment();

    replace(new_name.begin(),new_name.end(),old_char,new_char);
    replace(new_comment.begin(),new_comment.end(),old_char,new_char);

    s.set_name(new_name);
    s.set_comment(new_comment);
}

/**
* Adds new student to vector mapped to 'teacher'. (Will add new teacher if it does not exist)
* @param teacher
* @param s
*/
void Student_Store::add(std::string& teacher, const Student& s) {
    school_data[teacher].push_back(s);
    logs.add_log(s.get_name() + " was added on");
}

/**
* Deletes all data in map (Password protected in main)
*/
void Student_Store::clear() {
    school_data.clear();
    logs.add_log("School database cleared on");
}

/**
* Prints All students of a certain teacher along with index
* @param teacher
*/
void Student_Store::print_index(std::string& teacher) {
    for(unsigned int i = 0;i < school_data[teacher].size();++i) {
        std::cout << "INDEX: " << i << " " << school_data[teacher].at(i) << std::endl;
    }
}

/**
* Updates student details at certain index of a teachers class
* @param teacher
* @param index
* @param name
* @param age
* @param attendance
* @param gpa
* @param comment
*/
void Student_Store::update(std::string& teacher,int index,std::string name, int age, int attendance, float gpa, std::string comment) {

    if(name != "s") {
        school_data[teacher].at(index).set_name(name);
    }

    if(age != -1) {
        school_data[teacher].at(index).set_age(age);
    }

    if(attendance != -1) {
        school_data[teacher].at(index).set_attendance(attendance);
    }

    if(gpa != -1) {
        school_data[teacher].at(index).set_gpa(gpa);
    }

    if(comment != "s") {
        school_data[teacher].at(index).set_comment(comment);
    }

    logs.add_log("Student updated");
}

/**
* Returns whether or not a class is full
* @param teacher
* @return bool
*/
bool Student_Store::is_full(std::string& teacher) {
    if(school_data[teacher].size() > 30) {
        return true;
    }
    return false;
}

/**
* Takes all students from map and puts it into one vector
* @return students
*/
std::vector<Student> Student_Store::get_students() {
    std::vector<Student> students;
    for(const auto& key : school_data) {
        for(const auto& s : key.second) {
            students.push_back(s);
        }
    }
    return students;
}

/**
* Prints all values in student vector
* @param students
*/
void Student_Store::print(std::vector<Student>& students) {
    std::cout << "Total number of students: " << students.size() << std::endl;
    for(auto& s : students) {
        std::cout << s << std::endl;
    }
}

/**
* Reads data from file line by line, creating a vector of string
* Using the overridden input stream to convert the values into students (apart from the first value)
* teacher and student objects are then added to 'map'
* If file read fails, a Backup method will add run as a failsafe feature
* @return map
*/
std::map<std::string,std::vector<Student> > Student_Store::read_file() {


    std::ifstream student_file;
    // Set 'FILE' variable to false to use test2.txt
    FILE ? student_file.open("test1.txt") : student_file.open("test2.txt"); // test2.txt // test1.txt

    std::vector<Student> students;

    std::map<std::string,std::vector<Student> > map;
    if(student_file.is_open()) {

        std::vector<std::string> student_lines;
        std::string student_line;
        while(getline(student_file,student_line)) {
            student_lines.push_back(student_line);
        }
        for(unsigned int i = 0;i < student_lines.size();++i) {
            std::stringstream student_stream(student_lines[i]);

            std::string teacher;
            Student s;

            while(student_stream >> teacher) {
                replace(teacher.begin(), teacher.end(), '-', ' ');
                while(student_stream >> s) {
                    replace_characters(s,'-',' ');
                    students.push_back(s);
                }
                map[teacher] = students;
                students.clear();
            }
        }

        student_file.close();
    } else {
        map = back_up_data();
        std::cerr << "Error: Could access file, Backup data has been implemented." << std::endl;
    }
    return map;
}

/**
* Displays all students in a certain teachers class
* @param teacher
*/
void Student_Store::display_group(std::string& teacher) {
    if(school_data[teacher].empty()) {
        std::cout << "No students have been assigned to "
        << teacher << " yet" << std::endl;
    } else {
        for(auto& s : school_data[teacher]) {
            std::cout << s << std::endl;
        }
    }
}

/**
* tests whether a teachers name is in our map
* @param teacher
*/
bool Student_Store::teacher_exists(std::string teacher) {
    std::string lower_teacher = lower_case(teacher);
    for(auto& key : school_data) {
        std::string lower_key = lower_case(key.first);
        if(lower_key == lower_teacher) {
            return true;
        }
    }

    return false;
}

/**
* Converts 'word' to lower case
* @param word
*/
std::string Student_Store::lower_case(std::string word) {
    for(char& c : word) {
        c = tolower(c);
    }
    return word;
}

/**
* Adds a new empty key to our map, will ignore if key is already inserted
* @param teacher
*/
void Student_Store::create_group(std::string& teacher) {

    if(teacher_exists(teacher)) {
        std::cout << teacher << " has already been added." << std::endl;
    } else {
        std::vector<Student> new_group;
        school_data[teacher] = new_group;
        std::cout << teacher.substr(0,teacher.find(' ')) << " class has been created" << std::endl;
    }
    logs.add_log(teacher + "'s Group was created on");
}

/**
* Uses an iterator to remove a key from map
* @param teacher
*/
void Student_Store::remove_group(std::string& teacher) {

    // Create iterator of map
    std::map<std::string,std::vector<Student> >::iterator iter;
    // Find matching key and delete
    iter = school_data.find(teacher);

    if(iter != school_data.end()) {
        school_data.erase(iter);
        logs.add_log(teacher + "'s group was removed on ");
    } else {
        std::cout << "Teacher not found" << std::endl;
    }

}

/**
* Tests whether a teacher has students
* @param teacher
* @return bool
*/
bool Student_Store::class_empty(std::string& teacher) {
    return school_data[teacher].size() == 0;
}

/**
* Tests whether the index inputted is out of bounds
* @param teacher
* @param index
*/
bool Student_Store::in_range(std::string& teacher, int& index) {
    return index <= (school_data[teacher].size()-1);
}

/**
* Deletes student from map
* @param teacher
* @param index
*/
void Student_Store::remove_student(std::string& teacher, int& index) {

    for(unsigned int i = 0;i < school_data[teacher].size();++i) {
        if(index == i) {
            school_data[teacher].erase(school_data[teacher].begin() + i);
            logs.add_log("A student in " + teacher + "'s group was removed on");
        }
    }
}

/**
* Writes all map data to a text file using ofstream
* First whitespaces are replaced by '-' so we don't need to use a different delimeter
*/
void Student_Store::save(bool is_exit) {
    std::ofstream save_file;
    FILE ? save_file.open("test1.txt") : save_file.open("test2.txt"); // test2.txt // test1.txt

    if(save_file.is_open()) {
        for(auto& str : school_data) {
            std::string teacher = str.first;
            // Replace spaces with spaces before sending to file
            std::replace(teacher.begin(), teacher.end(), ' ', '-');
            save_file << teacher << " ";
            for(auto& s : str.second) {
                replace_characters(s,' ','-');

                save_file << s.get_name() << " " << s.get_age() << " "
                << s.get_attendance() << " " << s.get_gpa()
                << " " << s.get_comment() << " ";

                if(!is_exit) {
                    replace_characters(s,'-',' ');
                }
            }

            save_file << std::endl;
        }
        save_file.close();
    } else {
        std::cerr << "Save file could not open" << std::endl;
    }

}

/**
* Creates a HTML page that displays all current students in map
*
*/
void Student_Store::create_webpage() {
    std::ofstream html_page("e-school.html");
    // HTML header tags
    html_page << "<!DOCTYPE html><html><head>"; // Header tags
    html_page << "<link href=\"school_page.css\" rel=\"stylesheet\">"; // Style sheet link
    html_page << "</head><body>";
    html_page << "<h1>E-School.ie</h1>";
    html_page << "<img id='titleImage' src=\"https://www.lusd.org//cms/lib/CA01001399/Centricity/Domain/1/School-backgrounds-8.jpg\" alt = \"School books\">";
    html_page << "<table border='1'>";
    html_page << "<h2>Current students</h2>";
    html_page << "<tr><td class='title'>Teacher</td><td class='title'>Student name</td><td class='title'>Age</td><td class='title'>Attendance</td><td class='title'>GPA</td><td class='title'>Comment</td></tr>";
    for(auto& key : school_data) {

        for(auto& s : key.second) {
            html_page << "<tr>";
            html_page << "<td>" << key.first << "</td>" << s.to_html(); // Converts to HTML string
            html_page << "</tr>";
        }

    }
    html_page << "</table>";
    // Close off page and end connection
    html_page << "</body></html>";
    html_page.close();

}

/**
* Uses a priority queue to display the ten best students
* If there are less than 10 students then it will display all
*/
void Student_Store::top_ten() {

    // Use to sort priority queue
    struct by_gpa {
        bool operator()(const Student& s1, const Student& s2) {
            return s1.get_gpa() < s2.get_gpa();
        }
    };

    std::priority_queue<Student,std::vector<Student>, by_gpa> pq; // Initialise priority queue sorted by gpa
    for(auto& key : school_data) {
        for(const auto& s : key.second) {
            pq.push(s); // Fill students
        }
    }

    int top_students = 10;
    // Print Ten highest scoring students
    if(pq.size() < top_students) {
        top_students = pq.size();
    }
    for(unsigned int i = 0;i < top_students;++i) {
        // Print highest priority object
        std::cout << "Name: " << pq.top().get_name() << " GPA: " << pq.top().get_gpa() << std::endl;
        pq.pop(); // Take off highest priority so we can print the next highest priority student
    }

}

/**
* Prints all students who have failed their overall results
* @param students
*/
void Student_Store::has_failed(std::vector<Student>& students) {
    const unsigned int pass = 40;
    for(auto& s : students) {
        if(s.get_gpa() < pass) {
            std::cout << "Name: " << s.get_name() << " Result: " << s.get_gpa() << std::endl;
        }
    }
}
/**
* Returns Message based on i
* @param i
*
*/
void Student_Store::get_count(int& i) {
    if(i == 0) {
        std::cout << "No results" << std::endl;
    } else if(i == 1) {
        std::cout << "1 result found" << std::endl;
    } else {
        std::cout << i << " results found" << std::endl;
    }
}

/**
* Displays all students aged the same as user argument
* calls get_count() to display the number of results
* @param age
*/
void Student_Store::search_age(int& age) {

    int count = 0;

    for(auto& key : school_data) {
        for(const auto& s : key.second) {
            if(s.get_age() == age) {
                std::cout << s << std::endl;
                ++count;
            }
        }
    }
    get_count(count);
}

/**
* Prints all students that contain characters in 'text'
* User can choose whether to search by comment or name
*
* @param text
* @param choice
**/
void Student_Store::search_text(std::string& text, int choice) {
    int count = 0;

    std::string upper_text = upper_case(text);

    if(choice == 0) {
        int count = 0;
        for(auto& key : school_data) {
            for(const auto& s : key.second) {
                std::string upper_name = s.get_name();
                for(auto& c : upper_name) c = toupper(c);

                if(upper_name.find(upper_text) != std::string::npos) {
                    std::cout << s << std::endl;
                    ++count;
                }
            }
        }
        get_count(count);

    } else {
        for(auto& key : school_data) {
            for(const auto& s : key.second) {
                std::string upper_comment = s.get_comment();

                for(auto& c : upper_comment) c = toupper(c);
                if(upper_comment.find(upper_text) != std::string::npos) {
                    std::cout << s << std::endl;
                    ++count;
                }
            }
        }
        get_count(count);
    }
}

/**
* Prints out all Students that have a
* grade greater or equal to
* the user argument
*
* @param higher_than
*/
void Student_Store::search_gpa(float& higher_than) {

    int count = 0;
    for(auto& key : school_data) {
        for(const auto& s : key.second) {
            if(s.get_gpa() >= higher_than) {
                std::cout << s << std::endl;
                ++count;
            }
        }
    }
    get_count(count);
}

/**
* Overrides the output stream operator so our objects values can be viewed
* Teacher name is in a box while Student class has its own override
* @param output_stream
* @param st
* @return output_stream
*
*/
std::ostream& operator<<(std::ostream& output_stream, const Student_Store& st) {
    output_stream << "Number of current teachers is " << st.get_map().size() << "." << std::endl;


    for(auto& key : st.get_map()) {

        std::string border(key.first.length()+4, '*');
        std::string padding(border.length() - key.first.length(), ' ');

        output_stream << border << std::endl;
        output_stream << '*' << ' ' << key.first << ' ' << '*' << std::endl;
        output_stream << border << std::endl;
        std::vector<Student> students = key.second;
        if(students.empty()) {
            output_stream << '\t' << "Class is empty" << std::endl;
            output_stream << std::endl;
        }
        std::sort(students.begin(), students.end());

        for(const auto& s : students) {
            output_stream << '\t' << s << std::endl;
        }

    }
    return output_stream;
}

/**
* Returns a map of teachers and their classes overall gpa
* @return class_averages
*/
std::map<std::string,float> Student_Store::get_means() {
    std::map<std::string,float> class_averages;
    for(auto& key : school_data) {
        float total = 0;
        // Add up gpa's
        for(auto& s : key.second) {
            total += s.get_gpa();
        }
        // Will not add empty classes
        if(!isnan(total / key.second.size())) {
            // Uses overriden '+=' from student class, will append 'get_gpa()' to variable
            class_averages[key.first] = mean(key.second); // Our average class GPA is mapped to its teacher (key)
        }
    }
    return class_averages;
}

/**
* Prints "get_means" map
*/
void Student_Store::display_averages() {
    std::map<std::string,float> class_averages = get_means();

    for(auto& key : class_averages) {
        std::cout << "Teacher: " << key.first << " Class average: " << key.second << std::endl;
    }
}

/**
* Returns a vector of all students attendances
*/
std::vector<int> Student_Store::get_all_attendance() {
    std::vector<int> attendances;
    for(const auto& s : get_students()) {
        attendances.push_back(s.get_attendance());
    }
    return attendances;
}

/**
* Returns a vector of all students gpa's
* @return gpas
*/
std::vector<float> Student_Store::get_all_gpa() {
    std::vector<float> gpas;
    for(const auto& s : get_students()) {
        gpas.push_back(s.get_gpa());
    }
    return gpas;
}

/**
* Calculates the sum of a templated vector
*/
template <typename T>
float Student_Store::sum(std::vector<T>& vec) {
    float total = 0;
    for(unsigned int i = 0;i < vec.size();++i) {
        total += vec.at(i);
    }
    return total;
}

/**
* Returns the mean of a templated vector
* @param vec
* @return float
*/
template <typename T>
float Student_Store::mean(std::vector<T>& vec) {
    return sum(vec) / vec.size();
}

/**
*
*/
template <typename T>
float Student_Store::st_dev(std::vector<T>& vec) {
    float st_dev = 0.0;

    for(unsigned int i = 0; i < vec.size(); ++i) {
        st_dev += pow(vec[i] - mean(vec), 2);
    }

    return sqrt(st_dev / vec.size());
}

template <typename T>
std::vector<T> Student_Store::vec_minus_mean(std::vector<T>& vec) {
    std::vector<T> vec_minus_mean;
    for(unsigned int i = 0;i < vec.size();++i) {
        vec_minus_mean.push_back(vec.at(i) - mean(vec));
    }
    return vec_minus_mean;
}

/**
* Calculates pearson coeffiecient of two vectors
* both int and float vectors use the same template methods
* @param vec1
* @param vec2
* @return float
*/
float Student_Store::pearson(std::vector<int>& vec1, std::vector<float>& vec2) {

    std::vector<int> vec1_minus_mean = vec_minus_mean(vec1);
    std::vector<float> vec2_minus_mean = vec_minus_mean(vec2);

    float sum = 0;
    for(unsigned int i = 0; i < vec1_minus_mean.size();++i) {
        sum += vec1_minus_mean.at(i) * vec2_minus_mean.at(i);
    }
    return sum / (vec1.size() * st_dev(vec1) * st_dev(vec2));
}

/**
* Appends all students to a text file
* records.txt is used as a permenant history for the school
*/
void Student_Store::save_school_records() {

    // Append data instead of replacing data
    std::ofstream save_records("records.txt",std::fstream::in | std::ios::out | std::ios::app);

    if(save_records.is_open()) {
        for(auto key : school_data) {
            for(auto s : key.second) {
                replace_characters(s,' ','-');
                save_records << s.get_name() << " " << s.get_age() << " " << s.get_attendance()
                << " " << s.get_gpa() << " " << s.get_comment() << std::endl;
            }
        }
        std::cout << "Records added" << std::endl;
        save_records.close();
    } else {
        std::cerr << "Couldn't open records" << std::endl;
    }
}

/**
* Reads records into a set, eliminating duplicates
* @return records
*/
std::set<Student> Student_Store::read_school_records() {
    std::ifstream read_records("records.txt");

    std::set<Student> records;
    if(read_records.is_open()) {
        std::vector<std::string> lines;
        std::string line;

        while(getline(read_records,line)) {
            lines.push_back(line);
        }

        Student s;
        for(unsigned int i = 0;i < lines.size();++i) {
            std::stringstream record_stream(lines[i]);
            while(record_stream >> s) {
                replace_characters(s,'-',' ');
                records.insert(s);
            }
        }
    } else {
        std::cerr << "Unable to read records" << std::endl;
    }

    return records;
}

/**
* Will read lines from records.txt and insert them into a set
* Getting rid of duplicates and freeing up space
*/
void Student_Store::clean_records() {
    std::set<Student> records = read_school_records();

    // Default ofstream constructor Overrides text file all data in text file
    std::ofstream save_records("records.txt");
    if(save_records.is_open()) {
        for(auto s : records) {
            // None duplicated students are added back
            replace_characters(s,' ','-');
            save_records << s.get_name() << " " << s.get_age() << " " << s.get_attendance()
            << " " << s.get_gpa() << " " << s.get_comment() << std::endl;
        }
    } else {
        std::cerr << "Unable to clean records" << std::endl;
    }

}

/**
* Prints all students who have attended the school
*/
void Student_Store::read_records() {
    std::cout << records.size() << " students have/are attending this school." << std::endl;

    for(auto it = records.begin();it != records.end();++it) {
        std::cout << *it << std::endl;
    }
}

/**
* Prints logs First in last out
* values reassigned after all values are popped off
*/
void Student_Store::print_log() {
    std::cout << logs << std::endl;
}
