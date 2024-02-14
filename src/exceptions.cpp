#include "exceptions.hpp"

namespace lngAr {

const char* divisionByZero::what() const noexcept {
    return "Деление на 0";
}

}