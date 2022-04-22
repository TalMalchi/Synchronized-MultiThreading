
client: Client

server: Server

Client: Client.o
	g++ -o client Client.o 

Server: Server.o
	g++ -o server Server.o -lpthread

Client.o: Client.cpp
	g++ -c Client.cpp  

Server.o: Server.cpp
	g++ -c Server.cpp -lpthread 



clean:
	rm -f *.o Server Client