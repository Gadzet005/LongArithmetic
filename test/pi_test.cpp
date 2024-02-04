#include <gtest/gtest.h>
#include <fstream>
#include <array>
#include "utils.hpp"


TEST(Pi, calc) {
    std::ifstream piFile("data/pi.txt");
    std::string piString;
    piFile >> piString;
    piFile.close();

    std::array<unsigned int, 5> precArr = {0, 4, 50, 100, 200};
    for (unsigned int prec : precArr) {
        LongNumber calculatedPi = calculatePi(prec);
        LongNumber realPi = LongNumber(piString, prec);
        EXPECT_EQ(calculatedPi, realPi);
        EXPECT_EQ(calculatedPi.getPrecision(), prec);
    }
}
