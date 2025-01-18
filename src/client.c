#include "socketutil.h"
#include "helper.h"

#define MAXBUF 1024 /*max line limit for message*/

// read server response

void reader(void* sockFD) {
    char buf[MAXBUF];
    rio_t rio;
    int status;
    int connID=(int)sockFD;

    // init buffers for client socket
    rio_readinitb(&rio, connID);

    while(1) {
        // read a buffered line 
        while((status=rio_readlineb(&rio,buf,MAXBUF)) > 0) {
            //error
            if(status == -1)
                exit(1);
            if(!strcmp(buf,"\r\n")){
                break;
            }
            // exit from the server
            if (!strcmp(buf,"exit")){
                exit(0);
            }
            if (!strcmp(buf,"start\n")){
                printf("\n");
            } else {
                printf("%s",buf);
            }
        // print the Chatroom prompt
        fflush(stdout);
        }
    }
}

/*void reader(void* var){
  char buf[MAXBUF];
  rio_t rio;
  int status;
  int connID=(int)var;
  // initialise rio data structure
  rio_readinitb(&rio, connID);

  // start recieving messages from server
  while(1){
     while((status=rio_readlineb(&rio,buf,MAXBUF)) >0){
          //error
          if(status == -1)
            exit(1);
          if(!strcmp(buf,"\r\n")){
              break;
            }
          // exit from the server
          if (!strcmp(buf,"exit")){
              close(connID);
              exit(0);
            }
          if (!strcmp(buf,"start\n")){

               printf("\n");
            }

          else
             printf("%s",buf);
      }
      fflush(stdout);
  }
}*/

/*int main() {
    // thread for reading server responses
    pthread_t listener_worker;
    pthread_t sender_worker;

    int socketFD = CreateIpv4Socket();
    struct sockaddr_in* address = CreateIpv4Address("127.0.0.1", 6633);
    int result = connect(socketFD, address, sizeof(*address));

    if (result == 0) {
        printf("result was successful\n");
    }

    // a thread for reading server response
    pthread_create(&listener_worker,NULL,reader, (void*)socketFD);

    char line[MAXBUF];
    size_t lineSize = 0;

    char buffer[MAXBUF];	//Temporary buffer to read and write data
    int n;

    while(true) {
        /*ssize_t charCount = getline(&line, &lineSize, stdin);
        printf("%d\n", charCount);

        if (charCount > 0) {
            if (strcmp(line, "exit\n") == 0) {
                break;
            } else {
                printf("sent message\n");
                ssize_t amountSent = send(socketFD, charCount, lineSize, 0);
                printf("result was: %d\n", amountSent);

            }
        }
       	bzero(buffer, 256);
		printf("Me: ");
		fgets(buffer, 255, stdin);
		n = send(socketFD, buffer, strlen(buffer), 0);	//Write to server
		if(n<0)
			error("Error writing to server\n");
		n = read(socketFD, buffer, 255, 0);	//Read incoming data streams
		if(n<0)
			error("Error reading from Server");
		printf("Server : %s \n", buffer);
		if(strncmp(buffer, "Bye", 3) == 0)
			break;
    }

    while(1){
      // read the command
      if ((fgets(buffer, MAXBUF, stdin) == NULL) && ferror(stdin)) {
            perror("fgets error");
            close(socketFD);
            exit(1);
        }


      // send the request to the server
      if (rio_writen(socketFD,buffer,strlen(buffer)) == -1){
          perror("not able to send the data");
          close(socketFD);
          exit(1);
        }

    }



    close(socketFD);
    return 0;
}*/