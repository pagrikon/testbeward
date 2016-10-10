#ifndef DRIVERS_H
#define DRIVERS_H

#include <string>

namespace drivers {

    using namespace std;

    enum Color {RED, GREEN, BLUE};

    enum State {ON, OFF};

    // Исключения
    class DriverError {
    public:
        string msg;
        DriverError() { msg = ""; }
        DriverError(string m) : msg(m) {}
    };

    class DriverBadRate: public DriverError {};
    // Конец блока исключений

    // эмулятор драйвера светодиода
    class LedDriver {
    private:
        Color color;
        State state;
        int rate;
        string ledFileName; //файл для вывода состояния светодиода
    protected:
        void apply(); // print led status in file
    public:
        LedDriver();
        void setState(State st);
        State getState();
        void setColor(Color st);
        Color getColor();
        void setRate(int rt);
        int getRate();
    };
}

#endif
