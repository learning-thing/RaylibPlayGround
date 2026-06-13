//
// Created by benji on 13.06.26.
//
#include <iostream>
#include <serial.hpp>

int main() {
    Serial s("/dev/ttyUSB0", 9600);
    int a;
    while (true) {
        std::cin >> a;
        std::cout << "Writing " << a << std::endl;
        s.write(a);
    }
    return 0;
}