
# Тестовое задание Junior/стажёр C/C++ linux developer/
## Задание
*Необходимо реализовать клиент-серверное приложение под Linux. 
Клиент - программа, запускаемая из консоли. 
Сервер - демон, корректно завершающийся по сигналам SIGTERM и SIGHUP. 
Клиент должен передать содержимое текстового файла через TCP. 
Сервер должен принять и сохранить в файл.* 
## Решение
Составные части программы

TCP-сервер реализация: [ссылка](https://github.com/mamkad/imaqliq_tasks/tree/main/tcp_server)
Клиент реализация: [ссылка](https://github.com/mamkad/imaqliq_tasks/tree/main/client)
Запуск клиента либо сервера осуществляется в main.c с помощью специальных флагов: [ссылка](https://github.com/mamkad/imaqliq_tasks/blob/main/main.c)
в папке file находятся фукнции для чтения/записи в текстовый файл
в папке address находится фукнция для получения sockaddr, IPv4 или IPv6
максимальный размер буфера и пути для file и address в geninfo.h [](https://github.com/mamkad/imaqliq_tasks/blob/main/geninfo.h)

Сборка 

Для компиляции достаточно выполнить make-файл, исполняемый файл будет создан в папке bin. 

Несоставные части программы

в папке util содержаться две небольшие программы:
myhost.c [ссылка](https://github.com/mamkad/imaqliq_tasks/blob/main/util/myhost.c) - используется, чтобы узнать текущее имя хоста
throw_sig.c [ссылка](https://github.com/mamkad/imaqliq_tasks/blob/main/util/throw_sig.c) - используется, чтобы посылать сигналы нужному процессу по pid (для тестирования завершения работы демона)

## Создание клиента/сервера
Чтобы создать сервер необохдимо при запуске из командной строки ввести следующие аргументы
`./app -s порт backlog имя_файла_для_сохранения_полученных_данных.txt`
пример
`./app -s 4545 10 ~/data/out.txt`

Чтобы запустить клиента и отправить сообщение серверу необохдимо при запуске из командной строки ввести следующие аргументы
`./app -c имя_хоста порт имя_файла_откуда_берётся_содержимое_для_отправки.txt`
пример
`./app -c Opres 4545 ~/data/in.txt`
