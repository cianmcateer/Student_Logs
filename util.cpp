#include "util.h"

/**
* Returns true if 'text' matches regex validation
* @param text
* @param regex
* @return bool
*/
bool is_correct(std::string text , const std::string regex) {
    std::regex syntax(regex);
    if(std::regex_match(text, syntax)) {
        return true;
    }
    return false;
}

/**
* Displays the contents of the file path passed in
* @param path
*/
void read_file(const std::string path) {
    // Fix file path problem
    std::ifstream help_menu(path);
    if(help_menu.is_open()) {
        std::string line;
        // Will read line until new line is detected
        while(std::getline(help_menu,line)) {
            std::cout << line << std::endl;
        }
    } else {
        std::cerr << "Error opening menu file" << std::endl;
    }

}

/**
* Returns true if argument matches password
* @param password
* @return bool
*/
bool is_password(std::string password) {
    const std::string user_password = "password";
    return user_password == password;
}
/**
* Converts string to all upper_case
* @param x
* @return x
*/
std::string upper_case(std::string& x) {
    for(auto& c : x) c = toupper(c);
    return x;
}
