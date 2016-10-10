#ifndef HANDLERS_H
#define HANDLERS_H

#include <string>
#include <vector>
#include "drivers.hpp"

namespace handlers {

    using namespace std;
    using namespace drivers;

    //Exceptions
    class HandlerError {
    public:
        string msg;
        HandlerError() { msg = ""; }
        HandlerError(string m) { msg = m; }
    };

    class HandlerBadParam: public HandlerError {};
    //End Exceptions

    typedef const vector<string>& StrArgs;

    // базовый класс обработчиков
    class Handler {
    public:
        // метод - обработчик. Должен быть переопределен
        // в дочернем классе. На вход должен получить список (вектор)
        // строковых параметров.
        virtual string process(StrArgs args) = 0;
    };

    // базовый класс для обработчиков, работающих с драйвером LedDriver
    class LedDriverHandler: public Handler {
    protected:
        LedDriver* driver;
    public:
        LedDriverHandler(LedDriver* d) : driver(d) {}
    };

    // set-led-state command handler
    class SetLedStateHandler: public LedDriverHandler {
    public:
        SetLedStateHandler(LedDriver* d) : LedDriverHandler(d) {}
        virtual string process(StrArgs args);
    };

    // set-led-color command handler
    class SetLedColorHandler: public LedDriverHandler {
    public:
        SetLedColorHandler(LedDriver* d) : LedDriverHandler(d) {}
        virtual string process(StrArgs args);
    };

    // set-led-rate command handler
    class SetLedRateHandler: public LedDriverHandler {
    public:
        SetLedRateHandler(LedDriver* d) : LedDriverHandler(d) {}
        virtual string process(StrArgs args);
    };

    // get-led-state command handler
    class GetLedStateHandler: public LedDriverHandler {
    public:
        GetLedStateHandler(LedDriver* d) : LedDriverHandler(d) {}
        virtual string process(StrArgs args);
    };

    // get-led-color command handler
    class GetLedColorHandler: public LedDriverHandler {
    public:
        GetLedColorHandler(LedDriver* d) : LedDriverHandler(d) {}
        virtual string process(StrArgs args);
    };

    // get-led-rate command handler
    class GetLedRateHandler: public LedDriverHandler {
    public:
        GetLedRateHandler(LedDriver* d) : LedDriverHandler(d) {}
        virtual string process(StrArgs args);
    };
}

#endif
