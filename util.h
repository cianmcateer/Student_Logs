#pragma once

#include <iostream>
#include <string>
#include <regex>
#include <fstream>
#include <vector>

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

/**
* Calculates the sum of a templated vector
*/
template <typename T>
float sum(std::vector<T>& vec) {
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
float mean(std::vector<T>& vec) {
    return sum(vec) / vec.size();
}

/**
*
*/
template <typename T>
float st_dev(std::vector<T>& vec) {
    float st_dev = 0.0;

    for(unsigned int i = 0; i < vec.size(); ++i) {
        st_dev += pow(vec[i] - mean(vec), 2);
    }

    return sqrt(st_dev / vec.size());
}

template <typename T>
std::vector<T> vec_minus_mean(std::vector<T>& vec) {
    std::vector<T> vec_minus_mean;
    for(unsigned int i = 0;i < vec.size();++i) {
        vec_minus_mean.push_back(vec.at(i) - mean(vec));
    }
    return vec_minus_mean;
}
