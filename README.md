
# Тестовое задание Junior/стажёр C/C++ linux developer
## Задание
*Необходимо реализовать клиент-серверное приложение под Linux. 
Клиент - программа, запускаемая из консоли. 
Сервер - демон, корректно завершающийся по сигналам SIGTERM и SIGHUP. 
Клиент должен передать содержимое текстового файла через TCP. 
Сервер должен принять и сохранить в файл.* 
## Решение
**Составные части программы**

TCP-сервер реализация: [ссылка](https://github.com/mamkad/imaqliq_tasks/tree/main/tcp_server)

Клиент реализация: [ссылка](https://github.com/mamkad/imaqliq_tasks/tree/main/client)

Запуск клиента либо сервера осуществляется в main.c с помощью специальных флагов. main.c: [ссылка](https://github.com/mamkad/imaqliq_tasks/blob/main/main.c)

В папке file находятся функции для чтения/записи в текстовый файл: [ссылка](https://github.com/mamkad/imaqliq_tasks/tree/main/file)

В папке address находится функция для получения sockaddr, IPv4 или IPv6: [ссылка](https://github.com/mamkad/imaqliq_tasks/tree/main/address)

Максимальный размер буфера и пути для file и address в geninfo.h [ссылка](https://github.com/mamkad/imaqliq_tasks/blob/main/geninfo.h)

**Сборка**

Для компиляции достаточно выполнить make-файл, исполняемый файл будет создан в папке bin. 

**Не составные части программы**

В папке util содержатся две небольшие программы:

myhost.c: [ссылка](https://github.com/mamkad/imaqliq_tasks/blob/main/util/myhost.c) - используется, чтобы узнать текущее имя хоста

throw_sig.c: [ссылка](https://github.com/mamkad/imaqliq_tasks/blob/main/util/throw_sig.c) - используется, чтобы посылать сигналы нужному процессу по pid (для тестирования завершения работы демона)

**Дполнительная информация**

При запуске сервер становится демоном, поэтому он не сможет использовать потоки ввода/вывода, чтобы сервер выводил сообщения во время работы, достаточно закомментировать следущие строки в tcp_server.с
```
//закрываем стандартные потоки ввода, вывода, вывода ошибок
fclose(stdin);
fclose(stdout);
fclose(stderr);
```
Также сервер переходит в корневую директорию, если это не нужно, то надо закомментировать следующую строку в tcp_server.с
```
chdir("/");
```
## Создание клиента/сервера
Чтобы создать сервер необохдимо при запуске из командной строки ввести следующие аргументы

`./app -s порт backlog имя_файла_для_сохранения_полученных_данных.txt`

Пример:

`./app -s 4545 10 ~/data/out.txt`

Чтобы запустить клиента и отправить сообщение серверу необохдимо при запуске из командной строки ввести следующие аргументы

`./app -c имя_хоста порт имя_файла_откуда_берётся_содержимое_для_отправки.txt`

Пример:

`./app -c Opres 4545 ~/data/in.txt`
