//TODO Писать в outPipe в отдельном треде
//TODO error logging
//TODO README
//TODO закрывать тред в деструкторе интрефейса если выполняется

#include <string>
#include <unistd.h>
#include <cstring>
#include <sys/stat.h>
#include "interfaces.hpp"
#include "handlers.hpp"
#include "drivers.hpp"
#include <signal.h>

using namespace drivers;
using namespace handlers;
using namespace interfaces;

string pidFile = "var/ledserver.pid";

void sigTermHandler(int sig) {
    unlink(pidFile.c_str());
    exit(0);
}

void setPidFile(const char* Filename)
{
    FILE* f;

    f = fopen(Filename, "w+");
    if (f) {
        fprintf(f, "%u", getpid());
        fclose(f);
    }
}

int runServer() {
    LedDriver ledDriver;
    NamedPipe pipe("var/ipipe", "var/opipe");
    // ---------handler registration----------
    SetLedStateHandler set_led_state(&ledDriver);
    pipe.addHandler("set-led-state", &set_led_state);
    SetLedColorHandler set_led_color(&ledDriver);
    pipe.addHandler("set-led-color", &set_led_color);
    SetLedRateHandler set_led_rate(&ledDriver);
    pipe.addHandler("set-led-rate", &set_led_rate);
    GetLedStateHandler get_led_state(&ledDriver);
    pipe.addHandler("get-led-state", &get_led_state);
    GetLedColorHandler get_led_color(&ledDriver);
    pipe.addHandler("get-led-color", &get_led_color);
    GetLedRateHandler get_led_rate(&ledDriver);
    pipe.addHandler("get-led-rate", &get_led_rate);
    // run interface
    pipe.run();
}

int main(int argc, char* argv[]) {
    int status;
    int pid;

    system("mkdir var 2> /dev/null");

    // Демонизируем сервер
    pid = fork();

    if (pid == -1) { // Не удалось форкнуть процесс
        printf("Error: Start led server failed (%s)\n", strerror(errno));
    }
    else if (!pid) { // Это потомок - запускаем сервер
        signal(SIGTERM, sigTermHandler);
        setPidFile(pidFile.c_str());
        umask(0);
        setsid();
        // На реальном сервере демон должен работать в рутовой дирректории
        // Но так как это тестовая задача - будем работать в текущей
        //chdir("/");
        close(STDIN_FILENO);
        close(STDOUT_FILENO);
        close(STDERR_FILENO);
        runServer(); // Запускаем сервер
        return 0;
    }
    else { // Это родитель - выходим
        return 0;
    }
}
