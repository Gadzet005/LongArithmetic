#include <iostream>
#include <fstream>

#include "src/utils.hpp"

int main() {
    using namespace std;

    unsigned int prec;
    cout << "Введите точность: " << endl;
    cin >> prec;

    ifstream piFile("data/pi.txt");
    string piString;
    piFile >> piString;
    piFile.close();

    const LongNumber pi(piString, prec);
    LongNumber myPi = calculatePi(prec);

    cout << "Рассчитанное пи:\n" << myPi << endl;
    cout << "Реальное пи:\n" << pi << endl;
    cout << "Разница:\n" << myPi - pi << endl;

}
