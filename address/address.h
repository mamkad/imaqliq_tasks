#ifndef ADDRESS_H
#define ADDRESS_H

#include <sys/socket.h>
#include <arpa/inet.h>

void *get_in_addr(struct sockaddr *sa);

#endif
