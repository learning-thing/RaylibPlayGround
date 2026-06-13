//
// Created by benji on 13.06.26.
//

#ifndef RAYLIBPLAYGROUND_SERIAL_HPP
#define RAYLIBPLAYGROUND_SERIAL_HPP


#include <boost/asio.hpp>

// Strongly inspired by https://gist.github.com/kaliatech/427d57cb1a8e9a8815894413be337cf9
// @kaliatech


class Serial {
protected:
    boost::asio::io_context io;
    boost::asio::serial_port serial;
    std::string serialPath;
    int baud_rate;

public:

    Serial(std::string path, unsigned int baud_rate) : io(), serial(io, path), serialPath(path) {
        serial.set_option(boost::asio::serial_port_base::baud_rate(baud_rate));
        if (serial.is_open()) {
            std::cout << "Successfully opened serial port " << path << std::endl;
        }
    }

    Serial(Serial& s) : io(), serial(io, s.serialPath), baud_rate(s.baud_rate) {
        serial.set_option(boost::asio::serial_port_base::baud_rate(baud_rate));
        if (serial.is_open()) {
            std::cout << "Successfully opened serial port " << serialPath << std::endl;
        }
    }

    void write(const int integer) {
        const std::string asInt = std::to_string(integer)+"\n";
        boost::asio::write(serial, boost::asio::buffer(asInt.c_str(), asInt.length()));
    }

    void write(const std::string& msg) {
        const std::string asInt = msg+"\n";
        boost::asio::write(serial, boost::asio::buffer(asInt.c_str(), asInt.length()));
    }
};

#endif //RAYLIBPLAYGROUND_SERIAL_HPP
