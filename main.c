/*
** main.c -- создание сервера/клиента. Тестирование
*/

#include "tcp_server/tcp_server.h"
#include "client/client.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define SERVER 's'
#define CLIENT 'c'

int main(int argc, char** argv) 
{
	//проверяем, что было введено 4 аргумента
	if (argc != 5) {
		printf("%s\n", "incorrect number of arguments");
		return 0;
	}
	
	//ищем префикс флага
	char * prefix = strchr(argv[1], '-');
	if (!prefix) {
		printf("%s\n", "flag prefix is missing");
		return 0;		
	}
	
	//проверяем, что после префикса был указан флаг
	char flag = *(prefix + 1);
	if(flag == '\0') {
		printf("%s\n", "missing flag");
		return 0;
	}
	
	int backlog; //backlog
	// обрабатываем флаги
	switch(flag) {
		case SERVER: //создание сервера
			backlog = atoi(argv[3]);
			if(!backlog) {
				printf("%s\n", "backlog was entered incorrectly");
				return 0;
			}
			tcp_server_start(argv[2], backlog, argv[4]); //аргументы: порт, размер очереди ожидающих подключений, имя файла для сохранения принятых от клиента данных
			return 0;
		case CLIENT: //создание клиента
			client(argv[2], argv[3], argv[4]); //аргументы: имя хоста, порт и имя файла для открытия
			return 0;
		default: //был введён неизвестный флаг
			printf("%s\n", "you must specify creation flag. [-s] - server, [-c] - client");
			return 0;	
	}
}
