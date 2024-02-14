#include <iostream>
#include <fstream>
#include <ctime>

#include "utils.hpp"

int main() {
    using namespace std;
    using namespace lngAr;

    unsigned int prec;
    cout << "Введите точность: " << endl;
    cin >> prec;

    ifstream piFile("data/pi.txt");
    string piString;
    piFile >> piString;
    piFile.close();

    const LongNumber pi(piString, prec);

    const clock_t start = clock();
    LongNumber myPi = calculatePi(prec);
    const clock_t end = clock();

    cout << "Рассчитанное пи:\n" << myPi << endl;
    cout << "Реальное пи:\n" << pi << endl;
    cout << "Разница:\n" << myPi - pi << endl;
    cout << "Время: " << 1000.0 * (end - start) / CLOCKS_PER_SEC << "ms" << endl;
}
