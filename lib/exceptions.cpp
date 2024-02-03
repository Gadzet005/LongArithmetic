#include "exceptions.hpp"

const char* divisionByZero::what() const noexcept {
    return "Деление на 0";
}
