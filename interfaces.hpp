#ifndef INTERFACES_H
#define INTERFACES_H

#include <string>
#include <map>
#include "handlers.hpp"

namespace interfaces {

    using namespace std;
    using namespace handlers;

    //Exception
    class InterfaceError {
    public:
        string msg;
        InterfaceError() { msg = ""; }
        InterfaceError(string m) { msg = m; }
    };

    // Реализация интерфейса на именованных каналах
    class NamedPipe {
    private:
        string inPipeName;
        string outPipeName;
        map<string, Handler*> handlerMap;
    public:
        NamedPipe(string inName, string outName);
        // Добавить обработчки (command - команда, при которой
        // обработчик будет вызываться)
        void addHandler(string command, Handler* handler);
        // Запустить интрефейс
        void run();
    };
}


#endif
