#include <string>
#include <stdexcept>
#include "handlers.hpp"
#include "drivers.hpp"
//#include <iostream>

using namespace handlers;
using namespace std;
using namespace drivers;

string SetLedStateHandler::process(StrArgs args) {
    //cout << "SetLedStateHandler::process RUN!!!" << "\n";
    if(args.size() != 1)
        throw HandlerBadParam();
    if(args[0] == "on")
        driver->setState(ON);
    else if (args[0] == "off")
        driver->setState(OFF);
    else
        throw HandlerBadParam();
    return "OK";
}

string SetLedColorHandler::process(StrArgs args) {
    if(args.size() != 1)
        throw HandlerBadParam();
    if(args[0] == "red")
        driver->setColor(RED);
    else if (args[0] == "green")
        driver->setColor(GREEN);
    else if (args[0] == "blue")
        driver->setColor(BLUE);
    else
        throw HandlerBadParam();
    return "OK";
}

string SetLedRateHandler::process(StrArgs args) {
    if(args.size() != 1)
        throw HandlerBadParam();
    try {
        driver->setRate(stoi(args[0]));
    }
    catch(invalid_argument) {
        throw HandlerBadParam();
    }
    return "OK";
}

string GetLedStateHandler::process(StrArgs args) {
    if(args.size() != 0)
        throw HandlerBadParam();
    string result;
    switch(driver->getState()) {
        case ON:
            result = "on";
            break;
        case OFF:
            result = "off";
            break;
        default:
            throw HandlerError();
            break;
    }
    return "OK " + result;
}

string GetLedColorHandler::process(StrArgs args) {
    if(args.size() != 0)
        throw HandlerBadParam();
    string result;
    switch(driver->getColor()) {
        case RED:
            result = "red";
            break;
        case GREEN:
            result = "green";
            break;
        case BLUE:
            result = "blue";
            break;
        default:
            throw HandlerError();
            break;
    }
    return "OK " + result;
}

string GetLedRateHandler::process(StrArgs args) {
    if(args.size() != 0)
        throw HandlerBadParam();
    return "OK " + to_string(driver->getRate());
}
