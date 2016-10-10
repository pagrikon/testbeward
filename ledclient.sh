#!/bin/bash

LOCKFILE=var/client.lock
SERVER_INPUT_PIPE=var/ipipe
SERVER_OUTPUT_PIPE=var/opipe
SERVER_PID=var/ledserver.pid

# Захват сеанса обмена с серверов
lock() {
    until mkfifo $LOCKFILE >> /dev/null 2>> /dev/null
    do
        read none < $LOCKFILE
    done
}

# Освобождение сеанса обмена с серверов
unlock() {
    mv $LOCKFILE $LOCKFILE.$$
    : <> $LOCKFILE.$$
    rm -f $LOCKFILE.$$
}

# Сеанс с сервером
server_processing() {
    lock
    echo "$1" > $SERVER_INPUT_PIPE
    cat < $SERVER_OUTPUT_PIPE
    unlock
}

# Вывод помощи
help_cmd() {
    echo ""
    echo "-------------------HELP--------------------"
    echo "help                     - this help"
    echo "exit                     - exit"
    echo "set state on|off         - change led state"
    echo "set color red|green|blue - change led color"
    echo "set rate 0..5            - change led rate"
    echo "get state                - get state"
    echo "get color                - get color"
    echo "get rate                 - get rate"
    echo "-------------------------------------------"
    echo ""
}

# Пользователь ввел неверную команду
type_help() {
    echo "Invalid command. Type 'help' for help"
}

# set-led-rate - Отдельная ф-ция обарботки из-за числового параметра
set_led_rate() {
    if [[ "0" -le "$1" && "$1" -le "5" ]]
    then
        if [[ $(( $1+0 )) == "0" && "$1" != "0" ]]
        then
            type_help # it's not number
        else
            server_processing "set-led-rate $1"
        fi
    else
        type_help # not in range
    fi
}

# Сюда добавлять навоые команды. Основная логика обработки команд пользователя
processing() {
    case "$1" in
        set)
            case "$2" in
                state)
                    case "$3" in
                        on)
                            server_processing "set-led-state on";;
                        off)
                            server_processing "set-led-state off";;
                        *)
                            type_help;;
                    esac;;
                color)
                    case "$3" in
                        red)
                            server_processing "set-led-color red";;
                        green)
                            server_processing "set-led-color green";;
                        blue)
                            server_processing "set-led-color blue";;
                        *)
                            type_help;;
                    esac;;
                rate)
                    set_led_rate "$3";;
                *)
                    type_help;;
            esac;;
        get)
            case "$2" in
                state)
                    server_processing "get-led-state";;
                color)
                    server_processing "get-led-color";;
                rate)
                    server_processing "get-led-rate";;
                *)
                    type_help;;
            esac;;
        help)
            help_cmd;;
        exit)
            exit 0;;
        *)
            type_help;;
    esac
}

if [ -e $SERVER_PID ]
then
    echo ""
    echo "|------------------------------|"
    echo "|     LED CONTROL CLIENT       |"
    echo "|    Type 'help' for help      |"
    echo "|------------------------------|"
    echo ""
else
    echo "-------------------------------"
    echo "|   Server not started. Use:  |"
    echo "|   ./ledserver.sh start      |"
    echo "|   for start server          |"
    echo "-------------------------------"
    exit 0
fi

while [ 1 ]
do
    COMMAND="help"
    read -p "ledclient> " COMMAND
    processing $COMMAND
done
