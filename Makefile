CC = gcc

app: main.o filerw.o tcp_server.o client.o address.o
	$(CC) -o bin/app bin/main.o bin/filerw.o bin/tcp_server.o bin/client.o bin/address.o
	
main.o: main.c file/filerw.h tcp_server/tcp_server.h client/client.h address/address.h
	$(CC) -c main.c -o bin/main.o
	
filerw.o: file/filerw.c file/filerw.h
	$(CC) -c file/filerw.c -o bin/filerw.o
	
tcp_server.o: tcp_server/tcp_server.c tcp_server/tcp_server.h
	$(CC) -c tcp_server/tcp_server.c -o bin/tcp_server.o

client.o: client/client.c client/client.h
	$(CC) -c client/client.c -o bin/client.o
	
address.o: address/address.c address/address.h
	$(CC) -c address/address.c -o bin/address.o
	
clean:
	rm -rf *.s *.o *.exe
	
