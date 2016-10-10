#include "drivers.hpp"
#include <fstream>
#include <unistd.h>

using namespace drivers;

LedDriver::LedDriver() {
    ledFileName = "var/led";
    unlink(ledFileName.c_str());
    color = GREEN;
    state = OFF;
    rate = 0;
    apply();
}

void LedDriver::setState(State st) {
    state = st;
    apply();
}

State LedDriver::getState() {
    return state;
}

void LedDriver::setColor(Color cl) {
    if(state == OFF)
        throw DriverError("Can not change the color of the led switched off");
    color = cl;
    apply();
}

Color LedDriver::getColor() {
    return color;
}

void LedDriver::setRate(int rt) {
    if(rt < 0 || rt > 5)
        throw DriverBadRate();
    if(state == OFF)
        throw DriverError("Can not change the rate of the led switched off");
    rate = rt;
    apply();
}

int LedDriver::getRate() {
    return rate;
}

void LedDriver::apply() {
    std::ofstream file;
    file.open(ledFileName.c_str(), std::ios::app);
    if(!file.is_open())
        throw DriverError("Can not open led file");
    string colorStr;
    switch(color) {
        case RED:
            colorStr = "RED";
            break;
        case GREEN:
            colorStr = "GREEN";
            break;
        case BLUE:
            colorStr = "BLUE";
            break;
        default:
            colorStr = "BAD";
            break;
    }
    string stateStr;
    switch(state) {
        case ON:
            stateStr = "ON";
            break;
        case OFF:
            stateStr = "OFF";
            break;
        default:
            stateStr = "BAD";
            break;
    }
    file << "State:" << stateStr << "; Color:" << colorStr << "; Rate:" << rate << "\n";
    file.close();
}
