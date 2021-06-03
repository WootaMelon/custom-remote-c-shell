#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <sys/wait.h>
#define PORT 5103 

void start_shell(){

	printf("Hello, \nPlease wait while the shell finishes startup...\n");
	printf("\n");
	printf("This shell currently supports the following commands: \n");
	printf(">pwd\n");
	printf(">ls\n");
	printf(">date\n");
	printf(">whoami\n");
	printf(">man\n");
	printf(">ps\n");
	printf(">makedir\n");
	printf(">sleep\n");
	printf(">piping\n");
	printf(">executing a program (./name)\n");
	printf( "Enter a command: " );
	printf("\n\n");
}

int main(int argc, char **argv){ 

int sock=0,valread;

struct sockaddr_in serv_addr;

sock=socket(AF_INET, SOCK_STREAM,0);

serv_addr.sin_family=AF_INET;
serv_addr.sin_port= htons(PORT);
inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);

connect(sock,(struct sockaddr *)&serv_addr,sizeof(serv_addr));

start_shell();


while(1){

//scan the string
char string[100];
fgets(string,sizeof(char)*1024,stdin);
string[strlen(string)-1]='\0';


//send the string
send(sock,string,sizeof(string),0);


char buffer[1024]={0};
valread=recv(sock,buffer,sizeof(buffer),0);


printf("%s",buffer);
}

close(sock);

return 0;
}
