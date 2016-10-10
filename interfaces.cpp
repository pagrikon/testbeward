#include <string>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <fcntl.h>
#include <thread>
#include <unistd.h>
#include <sys/stat.h>
#include "interfaces.hpp"
#include "drivers.hpp"
#include "handlers.hpp"

using namespace interfaces;

NamedPipe::NamedPipe(string inName, string outName) : inPipeName(inName), outPipeName(outName) {
    // создание pipe
    unlink(inPipeName.c_str());
    if(mkfifo(inPipeName.c_str(), 0666) == -1)
        throw InterfaceError("Can not create input pipe");
    unlink(outPipeName.c_str());
    if(mkfifo(outPipeName.c_str(), 0666) == -1)
        throw InterfaceError("Can not create output pipe");
}

void NamedPipe::addHandler(string command, Handler* handler) {
    handlerMap[command] = handler;
}

bool waitReadPipe = false; // признак ожидания чтения выходного канала в треде
bool threadError = false; // признак ошибки в треде
string threadErrorMsg; // сообщение об ошибке в треде

// записать в выходной канал. реализована для запуска в треде
void writePipe(string const& pipeName, string const& str) {
    try {
        std::ofstream outPipe;
        waitReadPipe = true;
        outPipe.open(pipeName.c_str(), std::ios::out);
        if(!outPipe.is_open()) {
            threadError = true;
            threadErrorMsg = "Can not open output pipe";
            return;
        }
        outPipe << str;
        outPipe.close();
        waitReadPipe = false;
    }
    catch(const std::exception &e) {
        threadError = true;
        threadErrorMsg = e.what();
        return;
    }
}

// Очистить выходной канал (прочитать и закрыть)
void clearPipe(string const& pipeName) {
    ifstream inPipe(pipeName);
    if(inPipe) {
        string temp;
        std::getline(inPipe, temp);
        inPipe.close();
    }
}

void NamedPipe::run() {
    std::ifstream inPipe;
    std::ofstream outPipe;
    string line;
    string result;
    string command;
    string argument;
    vector<string> args;
    map<string, Handler*>::iterator handlerIter;
    std::thread writePipeThread; // писать в выходной канал будем в треде
    waitReadPipe = false;
    while(true) {
        inPipe.open(inPipeName.c_str(), std::ios::in);
        if(!inPipe.is_open())
            throw InterfaceError("Can not open input pipe");
        std::getline(inPipe, line);
        inPipe.close();
        // преобразуем полученную из входного пайпа строку в stream
        // (потом будет проще обрабатывать
        istringstream streamLine(line); 
        // первое слово - команда. По ней потом найдем обработчик
        streamLine >> command;
        args.clear();
        // остальные слова - аргументы обработчика
        while(streamLine >> argument) {
            args.push_back(argument);
        }
        // запускаем обрабочик, если найдем. Иначе - ошибка
        try {
            handlerIter = handlerMap.find(command);
            if(handlerIter != handlerMap.end())
                result = handlerIter->second->process(args);
            else
                result = "FAILED";
        }
        catch(HandlerError) {
            result = "FAILED";
        }
        catch(DriverError) {
            result = "FAILED";
        }
        result += "\n";
        // Если тред вывода в канал запущен - ждем завершения
        if(writePipeThread.joinable()) {
            // Если тред вывода ждет чтения из канала, значит процесс клиента
            // умер, не успев его прочитать. Тогда читаем из канала сами
            // (чистим канал и освобождаем тред)
            if(waitReadPipe)
                clearPipe(outPipeName);
            writePipeThread.join();
            if(threadError)
                throw InterfaceError(threadErrorMsg);
        }
        // Чтобы в случае крэша клиента, сервер не завис при выводе
        // резальтата в канал, вывод делаем в отдельном треде
        writePipeThread = std::thread(writePipe, outPipeName, result);
    }
}
