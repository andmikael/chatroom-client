#include <unistd.h>

extern "C" {
    #include "client.h"
}



int main() {
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
    pthread_create(&listener_worker, NULL, reader, (void*)socketFD);

    char buffer[MAXBUF];	//Temporary buffer to read and write data
    int n;

    while(1){
       //read the command
        if ((fgets(buffer, MAXBUF, stdin) == NULL) && ferror(stdin)) {
            perror("fgets error");
            break;
        }

      // send the request to the server
      if (rio_writen(socketFD,buffer,strlen(buffer)) == -1){
          perror("not able to send the data");
          break;
        }

        if (!strcmp(buffer, "quit\n")) {
            break;
        }

    }


    pthread_join(listener_worker, NULL);
    close(socketFD);
    return 0;
}