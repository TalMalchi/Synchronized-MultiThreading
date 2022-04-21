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
#include <pthread.h>
#include <iostream>
using namespace std;
#include "stack.hpp"


#define MAXDATASIZE 100

#define PORT "3490"  // the port users will be connecting to

#define BACKLOG 10	 // how many pending connections queue will hold

stack <string> ss;
	pthread_mutex_t mutex;
	int numbytes;
	

void sigchld_handler(int s)
{
	(void)s; // quiet unused variable warning

	// waitpid() might overwrite errno, so we save and restore it:
	int saved_errno = errno;

	while(waitpid(-1, NULL, WNOHANG) > 0);

	errno = saved_errno;
}




// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}

	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

void *send_function(void *ptr){
		char buffer[1024];
    
        // int sockfd= malloc(sizeof(((int*)ptr)));   
        int sockfd = *((int*)ptr);


        // if(send(sockfd, "Hello, world!", 13, 0) == -1){
        //         printf("im here\n");
        //         perror("send");
        //         exit(1);
        // }

	string line, cmd, data;
	while(true){
		try{
		//sleep(10);
		memset(buffer,0,1024);
        if ((numbytes = recv(sockfd, buffer, sizeof(buffer), 0)) == -1) {
	    cout << "fail in recieve" << endl;
		perror("recv");
	    exit(1);
		
	buffer[numbytes] = '\0';
	printf("Server: received '%s'\n",buffer);
		}	
	line = buffer;
	cmd= line.substr(0, line.find_first_of(" "));

	if (cmd.size() < line.size())
        {
            // rest line is data from the user
            data = line.substr(line.find_first_of(" ") + 1);
        }
	//memcpy((void*)cmd, buf, strlen(buf));
		
	if (cmd== "PUSH")
	{

		pthread_mutex_lock(&mutex);
		cout<< "Data to Push: " << data <<endl;
		cout<< "Inside Push Server.cpp"<<endl;
		ss.push(data);
		pthread_mutex_unlock(&mutex);

		
	}
	else if (cmd== "POP"){
		pthread_mutex_lock(&mutex);
		cout<< "Inside Pop Server.cpp"<<endl;
		if(!ss.empty()){
			ss.pop();
		}
		else if(ss.empty()){
			cout<< "Error- Stack is empty!"<<endl;
			//break;
		}
		pthread_mutex_unlock(&mutex);

	}
	else if (cmd== "TOP"){
		string empty_msg= "Error- Stack is empty!" ;
		pthread_mutex_lock(&mutex);
		cout<< "Inside Top Server.cpp"<<endl;
		if(ss.empty()){
			cout<< empty_msg <<endl;
			try{
			if(send(sockfd, empty_msg.c_str(), empty_msg.size()+1, 0) == -1){        
					perror("send");
					exit(1);
			}
			printf("send-Empty-MSG-to-Client\n");
			pthread_mutex_unlock(&mutex);
		}
		catch(exception& e){
			cout<<"exception: Can't send message to Client" << endl;
			
		}
		}
		else if(!ss.empty()){
		string top_msg;
		top_msg= ss.top();	
		printf("TOP_MSG: '%s'\n" ,top_msg.c_str());	
		try{
			if(send(sockfd, top_msg.c_str(), top_msg.size()+1, 0) == -1){        
					perror("send");
					exit(1);
			}
			printf("send-top-to-client\n");
			pthread_mutex_unlock(&mutex);
		}
		catch(exception& e){
			cout<<"exception: Can't send message to Client" << endl;
	}
	}
	}
	}
	catch(exception& e){
		cout<<"exception: didnt recieve msg"<<endl;
	}
        //close(sockfd) ;     
	
}
}

int main(void)
{
	int sockfd, new_fd;  // listen on sock_fd, new connection on new_fd
	struct addrinfo hints, *servinfo, *p;
	struct sockaddr_storage their_addr; // connector's address information
	socklen_t sin_size;
	struct sigaction sa;
	int yes=1;
    
	char s[INET6_ADDRSTRLEN];
	int rv;
    //stack_mutex <string> check;
	

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE; // use my IP

	if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}

	// loop through all the results and bind to the first we can
	for(p = servinfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype,
				p->ai_protocol)) == -1) {
			perror("server: socket");
			continue;
		}

		if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,
				sizeof(int)) == -1) {
			perror("setsockopt");
			exit(1);
		}

		if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			close(sockfd);
			perror("server: bind");
			continue;
		}

		break;
	}

	freeaddrinfo(servinfo); // all done with this structure

	if (p == NULL)  {
		fprintf(stderr, "server: failed to bind\n");
		exit(1);
	}

	if (listen(sockfd, BACKLOG) == -1) {
		perror("listen");
		exit(1);
	}

	sa.sa_handler = sigchld_handler; // reap all dead processes
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	if (sigaction(SIGCHLD, &sa, NULL) == -1) {
		perror("sigaction");
		exit(1);
	}

	printf("server: waiting for connections...\n");

	while(true) {  // main accept() loop
		sin_size = sizeof their_addr;
		new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
		if (new_fd == -1) {
			perror("accept");
			continue;
		}

		inet_ntop(their_addr.ss_family,
			get_in_addr((struct sockaddr *)&their_addr),
			s, sizeof s);
		printf("server: got connection from %s\n", s);
	
	




		// if (!fork()) { // this is the child process
		// 	close(sockfd); // child doesn't need the listener
		// 	if (send(new_fd, "Hello, world!", 13, 0) == -1)
		// 		perror("send");
		// 	close(new_fd);
		// 	exit(0);
		// }
		//signal(SIGINT,siginit_handler);
		pthread_t thread;
       pthread_create(&thread, NULL,send_function, (void *)&new_fd);

		//close(new_fd);  // parent doesn't need this
		
}
return 0;
}