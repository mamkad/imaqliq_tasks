#ifndef TCPSERVER_H
#define TCPSERVER_H

//аргументы: порт, размер очереди ожидающих подключений, имя файла для сохранения принятых от клиента данных
void tcp_server_start(char const* port, int backlog, char const* file_name_out);

#endif
