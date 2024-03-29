#include <gtest/gtest.h>
#include <tuple>
#include "long_number.hpp"

using std::vector, std::tuple, std::string;
using namespace lngAr;

TEST(Number, add) {
    using TestCase = tuple<string, string, string>;
    const vector<TestCase> cases = {
        {"999", "1", "1000"},
        {"1.0", "-1", "0"},
        {"0.000001", "0.01", "0.010001"},
        {"0.0", "0.00", "0"},
        {"-990.0000", "-10.00", "-1000.0"},
        {"-11.32", "11.0", "-0.32"},
        {"1.98", "9.0227", "11.0027"},
        {"13.0001", "12.0099", "25.01"},
        {"0.0001", "0.0001", "0.0002"},
        {"-0.0001", "-0.9999", "-1"},
        {
            "3131251289737817312398128921892130919823831213.18238192899213902390381289123324",
            "8984350922139218389239848324092341023102943085.213332525231932940219002391090132", 
            "12115602211877035701637977245984471942926774298.395714454224071964122815282323372"
        },
    };

    for (auto [s1, s2, s3] : cases) {
        EXPECT_EQ(LongNumber(s1) += LongNumber(s2), LongNumber(s3))
        << "Входные данные: " << s1 << ", " << s2 << ", " << s3;
    }
}

TEST(Number, sub) {
    using TestCase = tuple<string, string, string>;
    const vector<TestCase> cases = {
        {"1000", "1", "999"},
        {"1", "-1", "2"},
        {"0", "0", "0.0"},
        {"0.001", "0.01", "-0.009"},
        {"0.01", "0.0001", "0.0099"},
        {"-990.0000", "10.0", "-1000"},
        {"15", "13.001", "1.999"},
        {"-5.689", "1.011", "-6.7"},
        {"1.98", "-9.0227", "11.0027"},
        {"123.123", "72.56789", "50.55511"},
        {
            "9802344039820894890148938094143109123313221452.182381928992139023",
            "343454387892437471978187391238219004500394562.213332525231932940219002391090132", 
            "9458889651928457418170750702904890118812826889.969049403760206082780997608909868"
        },
    };

    for (auto [s1, s2, s3] : cases) {
        EXPECT_EQ(LongNumber(s1) -= LongNumber(s2), LongNumber(s3))
        << "Входные данные: " << s1 << ", " << s2 << ", " << s3;
    }
}

TEST(Number, mult) {
    using TestCase = tuple<string, string, string>;
    const vector<TestCase> cases = {
        {"0.001", "0.0005", "0.0000005"},
        {"-1001", "-0.1", "100.1"},
        {"1.001", "-1.001", "-1.002001"},
        {"-158.234", "-879.1", "139103.5094"},
        {"-21312.123", "1.0000000", "-21312.123"},
        {"-0.005", "-5000", "25"},
        {"21399.04", "0.0000", "0"},
        {"-8237.123523", "1", "-8237.123523"},
        {"0.0", "-0", "0.00"},
        {"1", "1", "1.0"},
        {"1", "9934201.231", "9934201.2310"},
        {"98249.1212", "1.2133246432", "119208.07992470355584"},
        {
            "8123129923942392304112389128989.123828948910211293120938",
            "919221.98219389120381239218389120",
            "7466959590304838547285393291390446476.3113933175456759204128974386759869660092570339456"
        }
    };

    for (auto [s1, s2, s3] : cases) {
        EXPECT_EQ(LongNumber(s1) *= LongNumber(s2), LongNumber(s3))
        << "Входные данные: " << s1 << ", " << s2 << ", " << s3;
    }
}

TEST(Number, div) {
    const int divPrec = 10;

    using TestCase = tuple<string, string, string>;
    const vector<TestCase> cases = {
        {"1", "7", "0.1428571428"},
        {"-1", "99", "-0.0101010101"},
        {"0.0001", "-0.0001", "-1"},
        {"-999999", "-999", "1001"},
        {"123129.213", "1000", "123.129213"},
        {"219.789", "4893.405", "0.044915350354201215717"},
        {"1.2421", "832784.1", "0.00000149150301981"},
        {"0", "21313.21321", "0"},
        {
            "-9289012312312893217429341091231231249.12938192389109381293243241",
            "-87832313147593.213123102311",
            "105758484314350900703765.77490296347985508642379892"
        }
    };

    for (auto [s1, s2, s3] : cases) {
        EXPECT_EQ(
            LongNumber::divide(LongNumber(s1), LongNumber(s2), divPrec), 
            LongNumber(s3, divPrec)
        ) << "[divide] Входные данные: " << s1 << ", " << s2 << ", " << s3;

        EXPECT_EQ(
            LongNumber(LongNumber(s1) /= LongNumber(s2), divPrec),
            LongNumber(s3, divPrec)
        ) << "[/=] Входные данные: " << s1 << ", " << s2 << ", " << s3;
    }

    EXPECT_THROW(LongNumber(1) / LongNumber(0), divisionByZero);
}

TEST(Number, comp) {
    EXPECT_EQ("123"_LN, "123.00000000000"_LN);
    EXPECT_EQ("0"_LN, "-0"_LN);
    EXPECT_GT("145.000000001"_LN, "145"_LN);
    EXPECT_GT("1.01"_LN, "-145"_LN);
    EXPECT_GE("-1.01"_LN, "-145"_LN);
    EXPECT_GE("-17.000"_LN, "-17"_LN);
    EXPECT_LT("123"_LN, "1225"_LN);
    EXPECT_LE("-123"_LN, "-122.999999999999999999"_LN);
}

TEST(Number, toString) {
    using TestCase = tuple<string, string>;
    const vector<TestCase> cases = {
        {"123.0000000000", "123.0"},
        {"123", "123"},
        {"-123.2131200000000", "-123.21312"},
        {"0", "0"},
        {"0.0000", "0.0"},
        {"-0.000010", "-0.00001"},
        {"-000214.213000", "-214.213"},
        {"10000.0000", "10000.0"},
        {"-10000", "-10000"}
    };

    for (auto [s1, s2] : cases) {
        EXPECT_EQ(LongNumber(s1).toString(), s2) 
        << "Входные данные: " << s1 << ", " << s2;
    }

    EXPECT_EQ(LongNumber().toString(), "0.0");
}

TEST(Number, other) {
    EXPECT_THROW(LongNumber("--13213"), std::invalid_argument);
    EXPECT_THROW(LongNumber("1123.213.213"), std::invalid_argument);
    EXPECT_THROW(LongNumber("-1123.213213@"), std::invalid_argument);
    EXPECT_THROW(LongNumber("-"), std::invalid_argument);
    EXPECT_THROW(LongNumber(".123"), std::invalid_argument);

    EXPECT_EQ(LongNumber(), LongNumber("0"));
    EXPECT_EQ(2324902394.5_LN, LongNumber(2324902394.5));
    EXPECT_EQ("-21983809213812089.2349894305909123"_LN, LongNumber("-21983809213812089.2349894305909123"));

    LongNumber n("-2938320123.213213");
    EXPECT_EQ(n.abs(), -n);
    EXPECT_EQ((-n).abs(), -n);
}
