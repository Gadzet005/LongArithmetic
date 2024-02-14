#pragma once

#include <stdexcept>

namespace lngAr {

class divisionByZero : public std::exception {
public:
    const char* what() const noexcept;
};

}