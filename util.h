#pragma once

#include <iostream>
#include <string>
#include <regex>
#include <fstream>

/**
* Returns true if input is positive
* can accept both floats and integer types
* @param t
* @return bool
*/
template <typename T> bool is_pos(T t) {
    return t >= 0;
}

bool is_correct(std::string text , const std::string regex);
void read_file(const std::string path);
bool is_password(std::string password);
std::string upper_case(std::string& x);
