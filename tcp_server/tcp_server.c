/*
** tcp_server.c -- сервер потокового сокета TCP
*/

#include "../address/address.h"
#include "../file/filerw.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>

#define FILELENGTHSIZE 17

// Функция для обработки сигналов SIGTERM и SIGHUP
static void signalof_exit(int signal)
{
	printf("%s\n","server is dead");
	exit(0);
}

// Функция для обработки сигналов дочерних процессов
// Ждём завершение родительского процесса
// Ветвь дочернего процесса автоматически очищается при завершении
static void sigchld_handler(int signal)
{
	while(waitpid(-1, NULL, WNOHANG) > 0);
}

// Функция для получения данных от клиента
static int recv_spec(int soc_fd, char** buff)
{
	char size_number[FILELENGTHSIZE];
    size_number[0] = '\0';
    
	if(recv(soc_fd, size_number, FILELENGTHSIZE, 0) == -1) {
		perror("recv1");
		return -1;	
	}
	
	int size = atoi(size_number);
	if (!size) {
		perror("wrong convertation");
	}
	
	*buff = (char*) malloc(sizeof(char) * (size + 1));
	if (buff == NULL) {
    	return -1;	
    }  
	
	if(recv(soc_fd, *buff, size, 0) == -1) {
		perror("recv2");
		return -1;	
	}
	
	return size;
}

//tcp сервер
static int tcp_server(char const* port, int backlog, char const* file_name_out)
{
	int sockfd, new_fd; 				// слушать на sock_fd, новое подключение на new_fd
	struct addrinfo hints, *servinfo, *p;
	struct sockaddr_storage their_addr; // адресная информация подключившегося
	
	memset(&hints, 0, sizeof hints); // очистка структуры
	hints.ai_family = AF_UNSPEC;     // IPv4 либо IPv6
	hints.ai_socktype = SOCK_STREAM; // потоковый сокет TCP
	hints.ai_flags = AI_PASSIVE; 	 // использовать мой IP

	int rv;  // возращаемое значение функции getaddrinfo
	// servinfo будет указывать на связанный список из 1 или более struct addrinfo
	if ((rv = getaddrinfo(NULL, port, &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}
	
	int yes = 1;
	// цикл по всем результатам и связывание с первым возможным
	for(p = servinfo; p != NULL; p = p->ai_next) {
		// создать сокет:
		if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
			perror("server: socket");
			continue;
		}
		// устранить проблему "Address already in use"
		if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
			perror("setsockopt");
			exit(1);
		}
		// связать с портом, полученным из getaddrinfo()
		if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			close(sockfd);
			perror("server: bind");
			continue;
	    }
		break;
	}
	
	//не получилось привязать сокет ни к одному ip
	if (p == NULL) {
		fprintf(stderr, "server: failed to bind\n");
		return 2;
	}
	
	freeaddrinfo(servinfo); // со структурой закончили
	
	//начинаем слушать порт
	if (listen(sockfd, backlog) == -1) {
		perror("listen");
		exit(1);
	}
	
	// изменение действий обработки, связанных с указанным сигналом
	struct sigaction sa;
	sa.sa_handler = sigchld_handler; // жатва всех мёртвых процессов
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	
	// устанавливаем функцию-обработчик для сигнала SIGCHLD
	// проверяем на ошибки
	if (sigaction(SIGCHLD, &sa, NULL) == -1) {
		perror("sigaction");
		exit(1);
	}
	
	//закрываем стандартные потоки ввода, вывода, вывода ошибок
	fclose(stdin);
	fclose(stdout);
	fclose(stderr);

	int numbytes;		      // кол-во принятых байтов
	socklen_t sin_size;       // содержит размер struct sockaddr_storage до того как её адрес будет передан accept()
	char s[INET6_ADDRSTRLEN]; // адресс входящего подключения подключения
	
	// главный цикл accept()
	while(1) { 
		sin_size = sizeof their_addr;
		
		new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
		if (new_fd == -1) {
			perror("accept");
			continue;
		}
		
		//уведомляем о  входящем подключении
		inet_ntop(their_addr.ss_family, get_in_addr((struct sockaddr *)&their_addr),s, sizeof s);
		printf("server: got connection from %s\n", s);
		
		// это порождённые процессы
		// сервер не будет завершаться после обработки одного подключения от клиента
		// также не будет завершаться в случае возникноения ошибок во время подключения с клиентом
		// он лишь завершит процесс для данного подключения и будет обрабатывать следущие
		if (!fork()) {
			close(sockfd); // его слушать не нужно
			// получаем данные от клиента
			// проверяем, чтобы чтение прошло успешно
			char *buf; // буфер для полученных данных
			if((numbytes = recv_spec(new_fd, &buf)) == -1) {
				exit(1);
			}
			// сохраняем полученные данные в файл
			// проверяем, что запись в файл прошла успешно
			buf[numbytes] = '\0';
			if(save_file(file_name_out, buf, numbytes) == -1) {
				free(buf);
				perror("file");
				exit(1);
			}
			//закрываем соект
			//завершаем процесс
			free(buf);
			close(new_fd);
			exit(0);
		}
 		close(new_fd); // родителю это не нужно 
 	}
 	return 0;
}

// вызывается перед tcp_server()
// служит для установки функций-обработчиков на нужные сигналы для сервера
// создание процесса-демона
void tcp_server_start(char const* port, int backlog, char const* file_name_out)
{
	// изменение действий обработки, связанных с указанными сигналами
	// с использоваеним структуры sigaction
	struct sigaction act;
	memset(&act, 0, sizeof(act));
	act.sa_handler = signalof_exit; 
	
	sigemptyset(&act.sa_mask);

	// устанавливаем функцию-обработчик для сигнала SIGTERM
	// проверяем на ошибки
	if (sigaction(SIGTERM, &act, NULL) == -1) {
		perror("sigaction");
		exit(0);
	}
	
	// устанавливаем функцию-обработчик для сигнала SIGHUP
	// проверяем на ошибки
	if (sigaction(SIGHUP, &act, NULL) == -1) {
		perror("sigaction");
		exit(0);
	}
	
	// создаём процесс-демон
	if(!fork())
	{
		printf("%s\n","server: start of init");
		printf("pid = %d\n",setsid());
		printf("%s\n", "server: waiting for connections...\n");
		chdir("/");
		tcp_server(port, backlog, file_name_out);	
		exit(0); // завершить процесс
	}
	else {
		exit(0); // завершить процесс
	}
}
