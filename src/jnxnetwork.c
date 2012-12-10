#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <net/if.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include "jnxnetwork.h"
#define true 1
#define false 0

int running = true;
void jnx_cancel_listener(void )
{
	if(running)
	running = false;
}
int jnx_setup_listener(int port,void (*Callback)(char*) )
{
    int sockfd, newsockfd, clilen;
    char buffer[MAXBUFFER];
    struct sockaddr_in serv_addr, cli_addr;
    int  n;
    int optval = 1;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("ERROR opening socket");
        exit(1);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(port);
    /* Now bind the host address using bind() call.*/
    // set SO_REUSEADDR on a socket to true (1):
    if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof optval) > 0)
    {
        perror("Set socketopts:");
        exit(1);
    }
    if (bind(sockfd, (struct sockaddr *) &serv_addr,
             sizeof(serv_addr)) < 0)
    {
        perror("ERROR on binding");
        exit(1);
    }
    /* Now start listening for the clients, here process will
    * go in sleep mode and will wait for the incoming connection
    */
	printf("Starting listener...\n");
    while(running)
    {
        clilen = sizeof(cli_addr);
        listen(sockfd,5);

        /* Accept actual connection from the client */
        newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr,
                           &clilen);
        if (newsockfd < 0)
        {
            perror("ERROR on accept");
            exit(1);
        }
        /* If connection is established then start communicating */
        bzero(buffer,MAXBUFFER);
		
        n = read( newsockfd,buffer,MAXBUFFER );
        if (n < 0)
        {
            perror("ERROR reading from socket");	
            exit(1);
        }
        n = write(newsockfd,"MESSAGE RECEIVED",16);
        if (n < 0)
        {
            perror("ERROR writing to socket");
            exit(1);
        }
        (*Callback)(buffer); //function pointer callback
    }
    close(newsockfd);
    return 0;
}
int jnx_send_message(char* host, int port, char* msg)
{
    struct hostent *send_server;
    struct sockaddr_in send_serv_addr;
    struct sockaddr_in remote_server_add;
    char send_transmission_buffer[MAXBUFFER];
    int send_port = port;

    int send_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(send_fd < 0)
    {
        printf("Couldn't open send port\n");
        return 1;
    }
    send_server = gethostbyname(host);
    if(send_server < 0)
    {
        printf("Couldnt resolve ip\n");
        return 1;
    }
    bzero((char *) &send_serv_addr, sizeof(send_serv_addr));

    send_serv_addr.sin_family = AF_INET;

    bcopy((char *)send_server->h_addr,(char *)&send_serv_addr.sin_addr.s_addr,send_server->h_length);

    send_serv_addr.sin_port = htons(port);

    printf("Attempting to connect...\n");

    if (connect(send_fd,(struct sockaddr *) &send_serv_addr,sizeof(send_serv_addr)) < 0)
    {
        printf("Error whilst connecting\n");
        return 1;
    }
    printf("Connected\n");

    bzero(send_transmission_buffer,MAXBUFFER);

    strcpy(send_transmission_buffer,msg);

    send_fd = write(send_fd,send_transmission_buffer,strlen(msg));

    if (send_fd < 0)
    {
        printf("Can't write to socket\n");
        return 1;
    }
    bzero(send_transmission_buffer,MAXBUFFER);	
    close(send_fd);

    printf("Message sent %s\n",send_transmission_buffer);
	printf("Freeing resources...\n");
	free(msg);
	//Note: Host is also taken care of gethostbyname
	//Note: typically the memory returned by gethostbyaddr
	//will be allocated and managed by the sockets library you are using - you don't free it yourself.
    return 0;
}
char* jnx_local_ip ( char* interface )
{
	int fd;
	struct if_nameindex *curif, *ifs;
	struct ifreq req;
	char *ipadd = NULL;
	
	if ( ( fd = socket ( PF_INET, SOCK_DGRAM, 0 ) ) != -1 )
    {
		ifs = if_nameindex();
		if ( ifs )
        {
			for ( curif = ifs; curif && curif->if_name; curif++ )
            {
				strncpy ( req.ifr_name, curif->if_name, IFNAMSIZ );
				req.ifr_name[IFNAMSIZ] = 0;
				if ( ioctl ( fd, SIOCGIFADDR, &req ) < 0 )
                perror ( "ioctl" );
				else
					if(strcmp(curif->if_name,interface) == 0)
					{
						ipadd = inet_ntoa ( ( ( struct sockaddr_in* ) &req.ifr_addr )->sin_addr );
					}       
            }
			if_freenameindex ( ifs );
			if ( close ( fd ) !=0 )
				perror ( "close" );
        }
		else
			perror ( "if_nameindex" );
    }
	else
		perror ( "socket" );
  return ipadd;
}

