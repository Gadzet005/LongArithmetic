#pragma once

#include <stdexcept>

class divisionByZero : public std::exception {
public:
    const char* what() const noexcept;
};
