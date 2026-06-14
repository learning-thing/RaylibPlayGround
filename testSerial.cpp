//
// Created by benji on 13.06.26.
//
#include <iostream>
#include <serial.hpp>

std::string readed;

void onRead(std::string a) {
    readed = a;
    if (a.length() > 0)
    std::cout << a << std::endl;
}

int main() {
    Serial s("/dev/ttyUSB0", 9600);
    while (true) {
        const std::string line = s.readLineAsync();
        if (!line.empty()) {
            std::cout << "Received: " << line << '\n';
        }
    }
    return 0;
}