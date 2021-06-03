#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <pthread.h>
#define PORT 5103

void  pwd(){
	char string[100];
	getcwd(string,sizeof(string));
	printf("Name of Current Directory: %s\n",string);
}
void ls(){
	char *exp[] = {"ls", ".", NULL};
	execvp("ls", exp);
	
}

void date(){
	execlp("date", "date", NULL);
}
void ps(){
	execlp("ps", "ps", NULL);
}
void whoami(){
char* name = getenv("USER");
		printf("Who forgets their name? anyways, you are:  %s\n",name);
}
void man(char *str){
	char *ls="Prints current files in Directory";
	char *pwd="Prints current work Directory";
	char *date="Prints current date";
	char *whoami="Prints current user name";
	char *man="Gives Information about supported commands in this shell";
	char *sleep="Sleeps the program for the given amount of seconds";
	char *ps="Prints the current running processes";
	char *pipe="Creates a pipe for interprocess communiation";
	char *makedir="Creates a new empty directory";
/*	
	scanf("%s",input); */
	
	char *command= strtok(str," ");
	command=strtok(NULL," ");
	
	if(strcmp(command,"ls")==0)
		printf("%s\n",ls );
	if(strcmp(command,"pwd")==0)
		printf("%s\n",pwd );
	if(strcmp(command,"date")==0)
		printf("%s\n",date );
	if(strcmp(command,"whoami")==0)
		printf("%s\n",whoami );
	if(strcmp(command,"man")==0)
		printf("%s\n",man );
	if(strcmp(command,"sleep")==0)
		printf("%s\n",sleep );
	if(strcmp(command,"pipe")==0)
		printf("%s\n",pipe );
	if(strcmp(command,"ps")==0)
		printf("%s\n",ps );
	if(strcmp(command,"makedir")==0)
		printf("%s\n",makedir );
	
}
void makedir(char *str){
	char *dirname= strtok(str," ");
	dirname=strtok(NULL," ");
	
	if (mkdir(dirname, 0777) == -1) 
      printf("Error in creation\n");
  
    else
       printf("Directory Created\n");

}

void sleeping(char *str){

char *slp= strtok(str," ");

slp=strtok(NULL," ");

int number =atoi(slp);

printf("Sleeping...");
fflush(stdout);
sleep(number);
printf("\n");
}

void piping(char *str){

  char *temp=strtok(str,"|");
  char *temp3=strtok(NULL,"|");
  pid_t pid;
  int fd[2];
  pipe(fd);
  
  pid = fork(); 
  
  
  if(pid == 0) {
    close(1);
    dup(fd[1]);
    close(fd[0]);
    close(fd[1]);
	
    char *exp[3];
    
    char temp2[20];
    memcpy(temp2,temp,strlen(temp));
    char *a=strtok(temp2," ");
    
    exp[0]=a;
    a=strtok(NULL," ");
    exp[1]=a;
    exp[2]=NULL;
    execvp(exp[0], exp);

  }

   if(pid > 0) {
    
    
    wait(NULL);
    close(0);
    dup(fd[0]);
    close (fd[1]);
    close(fd[0]);
    
    temp=strtok(NULL,"|");
    
    char *exp2[3];
    
    char temp4[20];
    memcpy(temp4,temp3,strlen(temp3));
    char *a=strtok(temp3," ");
    
    exp2[0]=a;
    a=strtok(NULL," ");
    exp2[1]=a;
    exp2[2]=NULL;
    
      
  //  char *exp[] = {temp, "-l", NULL};

  //  execvp(temp, exp);
  execvp(exp2[0], exp2);

  }

  close(fd[0]);
  close(fd[1]);
}

void dpiping(char *str)
{

char *temp1=strtok(str,"|");
char *temp2=strtok(NULL,"|");
char *temp3=strtok(NULL,"|");

pid_t pid;
int fd[2];
int fd2[2];

char *chld2[2];
char *chld1[3];
char *parent[3];
  
pipe(fd);
pipe(fd2);

int status=0;
pid = fork();


if (pid<0)
{
    perror("error at 1st fork");
    exit(1);
}
if (pid == 0){

    pid_t pid2;
    pid2=fork();
    
    
    if(pid2<0)
    {
        perror("error at 2nd fork");
        exit(1);
    }
    if (pid2 == 0)
    {
        //child 2
        
        //close the first pipe
        close (fd[1]);
        close (fd[0]);
        

        
        //redirect output of process to write end of second pipe (so it goes to child 1)
        close(1);
        dup(fd2[1]);
        close (fd2[0]);
        close (fd2[1]);
        
       
        
        //excecute command
        char*a1=strtok(temp1," ");
        chld2[0]=a1;
	a1=strtok(NULL," ");
	chld2[1]=a1;	
	a1=strtok(NULL," ");
	chld2[2]=a1;
	
 
        execvp(chld2[0], chld2);
        exit(0);
    }
    if(pid2>0)
    {
        //child 1
      //  wait(NULL);
        
        //redirect input of process to the read end of second pipe (so it comes from child 2)
        close(0);
        dup(fd2[0]);
        close (fd2[1]);
        close (fd2[0]);
        
  //redirect output of process to the write end of first pipe (so it goes to parent)
        close(1);
        dup(fd[1]);
        close (fd[0]);
        close (fd[1]); 
        
        
        
         //excecute command
        char*b1=strtok(temp2," ");
        chld1[0]=b1;
	b1=strtok(NULL," ");
	chld1[1]=b1;	
	b1=strtok(NULL," ");
	chld1[2]=b1;
	
        execvp(chld1[0], chld1);
        exit(0);

    }
    exit(0);
}
if(pid>0)
{

    //parent
  //  wait(NULL);
    
    //close the second pipe
        close (fd2[1]);
        close (fd2[0]);

    
    //redirect input of process to read end of first pipe (so it comes from child 1)
    close(0);
    dup(fd[0]);
        close (fd[1]);
    close (fd[0]);
    
    //close write end of first pipe

    
    //excecute command
    char*c1=strtok(temp3," ");
        parent[0]=c1;
	c1=strtok(NULL," ");
	parent[1]=c1;	
	c1=strtok(NULL," ");
	parent[2]=c1;
    execvp(parent[0], parent);   
       
        
}
close(fd[1]);
close(fd[0]);
close(fd2[1]);
close(fd2[0]);

}



void checking(char *string){
	char *pipedash="|";
	char *ret;
	char *ret2;
	
	//fgets(string,sizeof(char)*1024,stdin);
	//string[strlen(string)-1]='\0';
	
	// command
	char str2[50];
	memcpy(str2,string,strlen(string)-1);
	char *a=strtok(str2," ");
	ret=strpbrk(string,pipedash);

	
	// checking if double pipe
	if(ret){
	char str3[50];
	memcpy(str3,string,strlen(string));
	char *b=strtok(str3,pipedash);
	b=strtok(NULL,"");
	ret2=strpbrk(b,pipedash); 
	}
	
	if(strcmp(string,"pwd")==0)
		pwd();
	
	else if(strcmp(string,"ls")==0){
	pid_t pid;
	pid=fork();
		if(pid==0)
		ls();
		if(pid>0)
			wait(NULL);
	}
	else if(strcmp(string,"date")==0){
	pid_t pid;
	pid=fork();
		if(pid==0){
			printf("\nCurrent Date is:\n ");
			date();
	}
		if(pid>0)
			wait(NULL);
		}
	
	else if(strcmp(string,"whoami")==0){
		whoami();
	}
	else if(strcmp(a,"man")==0){
		man(string);}
		
	else if(strcmp(a,"makedir")==0)
		makedir(string);
		
	else if(strcmp(a,"sleep")==0)
		sleeping(string);
		
	else if(strcmp(string,"ps")==0){
	pid_t pid;
	pid=fork();
		if(pid==0)
		ps();
		if(pid>0)
			wait(NULL);
	}
	else if(ret && !ret2){
	pid_t pid;
	pid=fork();
		if(pid==0)
		piping(string);
		if(pid>0)
			wait(NULL);
	}
	else if(ret && ret2){
	pid_t pid;
	pid=fork();
		if(pid==0)
		dpiping(string);
		if(pid>0)
			wait(NULL);
	}
	
	
	else{
	pid_t pid;
	pid=fork();
		if(pid==0){
		
		char str2[1024];
		memcpy(str2,string,strlen(string));
  		char *a=strtok(str2,"/");
		a=strtok(NULL,"/");
		
		execlp(string,a,NULL);

		}
		if(pid>0){
			wait(NULL);
			}

	} 
}

void* runner(void*);
void* runner(void* socket){
//cast socket
int *sock=(int*) socket;
int s=*sock;


while(1){
int valread;

//recieve buffer
char buffer[1024]={0};
valread=recv(s,buffer,sizeof(buffer),0);
printf("Received: %s\n",buffer);

pid_t pid;
pid=fork();

if(pid<0)
perror("1");
if(pid==0){

//dup to file
int file=open("output",O_WRONLY | O_CREAT | O_TRUNC,0644);
close(1);
dup(file);

//executing	
checking(buffer);
exit(0);
}
if(pid>0){
wait(NULL);
FILE *fp=fopen("output","r");
char buffer1[1024]={0};
char buffer2[1024]={0};
			
while(fgets(buffer2,1024,fp)!=NULL){
strcat(buffer1,buffer2);
}

//send buffer to client 
send(s,buffer1,sizeof(buffer1),0);
printf("sent result\n");


}

}
close(s);
}


int main(int argc, char **argv){ 

int server_sock,new_sock,valread;
 
struct sockaddr_in address;
int addrlen=sizeof(address);
server_sock=socket(AF_INET, SOCK_STREAM,0);

address.sin_family=AF_INET;
address.sin_addr.s_addr= htonl(INADDR_ANY);
address.sin_port= htons(PORT);

bind(server_sock,(struct sockaddr *)&address,sizeof(address));

struct sockaddr_in newaddr;

listen(server_sock,1);

while(1){


new_sock= accept(server_sock,(struct sockaddr*)&address,(socklen_t*)&addrlen);


//thread initialization
pthread_t th;
pthread_attr_t attr;
pthread_attr_init(&attr);
//pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);


//thread creation
pthread_create(&th,&attr,runner,&new_sock);

}
//close(new_sock);
close(server_sock);
return 0;
}


