//
// Created by benji on 13.06.26.
//

#ifndef RAYLIBPLAYGROUND_SERIAL_HPP
#define RAYLIBPLAYGROUND_SERIAL_HPP


#include <boost/asio.hpp>

// Strongly inspired by https://gist.github.com/kaliatech/427d57cb1a8e9a8815894413be337cf9
// @kaliatech

inline void asnyReadHandler(const boost::system::error_code& error, std::size_t bytes_transferred) {

}

class Serial {
protected:
    boost::asio::io_context io;
    boost::asio::serial_port serial;
    std::array<char, 64> readBuf{};
    std::string serialPath;
    std::string buffer;
    int baud_rate;
    void startAsyncRead() {
        serial.async_read_some(
            boost::asio::buffer(readBuf),
            [this](const boost::system::error_code& ec, std::size_t bytes) {
                if (!ec) {
                    buffer.append(readBuf.data(), bytes);
                    startAsyncRead(); // keep reading
                }
            });
    }
public:
    Serial(const std::string path, const unsigned int baud_rate) : io(), serial(io, path), serialPath(path), baud_rate(baud_rate) {
        serial.set_option(boost::asio::serial_port_base::baud_rate(baud_rate));
        if (serial.is_open()) {
            std::cout << "Successfully opened serial port" << path << std::endl;
            startAsyncRead();
        } else {
            std::cerr << "Failed to open serial port, you can retry with SerialRetry();" << path << std::endl;
        }
    }

    Serial(Serial& s) : io(), serial(io, s.serialPath), baud_rate(s.baud_rate) {
        if (serial.is_open()) {
            std::cout << "Successfully opened serial port " << serialPath << std::endl;
        } else {
            std::cerr << "Failed to open serial port, you can retry with SerialRetry();" << s.serialPath << std::endl;
        }
        serial.set_option(boost::asio::serial_port_base::baud_rate(baud_rate));
    }

    void write(const int integer) {
        const std::string asInt = std::to_string(integer)+"\n";
        boost::asio::write(serial, boost::asio::buffer(asInt.c_str(), asInt.length()));
    }

    void write(const std::string& msg) {
        const std::string asInt = msg+"\n";
        boost::asio::write(serial, boost::asio::buffer(asInt.c_str(), asInt.length()));
    }
    bool isOpen() const {
        return serial.is_open();
    }
    void retry() {
        if (serial.is_open()) {
            std::cout << "Serial already opened\n";
        } else {
            std::cout << "Trying again to open serial\n";
            serial = boost::asio::serial_port(io, serialPath);
            serial.set_option(boost::asio::serial_port_base::baud_rate(baud_rate));
        }
    }
    std::string readLine() {
        //Reading data char by char, code is optimized for simplicity, not speed
        using namespace boost;
        char c;
        std::string result;
        for(;;) {
            asio::read(serial, asio::buffer(&c,1));
            switch(c) {
                case '\r':
                    break;
                case '\n':
                    return result;
                default:
                    result+=c;
            }
        }
    }

    std::string readLineAsync() {
        io.poll(); // handle finished async stuff

        const auto pos = buffer.find('\n');
        if (pos == std::string::npos) return "";

        std::string line = buffer.substr(0, pos);

        if (!line.empty() && line.back() == '\r') line.pop_back();

        buffer.erase(0, pos + 1);

        return line;
    }
};

#endif //RAYLIBPLAYGROUND_SERIAL_HPP
