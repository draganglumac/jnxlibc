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
#define DEFAULTRESPONSE "Received"
int running = true;
void jnx_network_cancel_listener(void)
{
	if (running) {
		running = false;
	}
}
int jnx_network_setup_listener(int port, void (*jnx_network_listener_callback)(char*))
{
	int sockfd, newsockfd, clilen;
	char buffer[MAXBUFFER];
	struct sockaddr_in serv_addr, cli_addr;
	int  n;
	int optval = 1;
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) {
		perror("jnx_network_setup_listener error opening socket");
		return 1;
	}
	bzero((char*) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(port);
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof optval) > 0) {
		perror("jnx_network_setup_listener set socketopts:");
		return 1;
	}
	if (bind(sockfd, (struct sockaddr*) &serv_addr,
				sizeof(serv_addr)) < 0) {
		perror("jnx_network_setup_listener error on binding");
		return 1;
	}
	while (running) {
		clilen = sizeof(cli_addr);
		listen(sockfd, 5);
		newsockfd = accept(sockfd, (struct sockaddr*) &cli_addr,
				(socklen_t*)&clilen);
		if (newsockfd < 0) {
			perror("jnx_network_setup_listener error on accept");
			return 1;
		}
		bzero(buffer, MAXBUFFER);
		n = read(newsockfd, buffer, MAXBUFFER);
		if (n < 0) {
			perror("jnx_network_setup_listener error reading from socket");
			return 1;
		}
		n = write(newsockfd, DEFAULTRESPONSE, 16);
		if (n < 0) {
			perror("jnx_network_setup_listener error writing to socket");
			return 1;
		}
		(*jnx_network_listener_callback)(buffer);      //function pointer callback
	}
	close(newsockfd);
	return 0;
}
int jnx_network_send_message(char* host, int port, char* msg, void (*jnx_network_send_message_callback)(char*))
{
	if(host == NULL || port < 0 || msg == NULL) 
	{
		printf("jnx_network_send_message error in input values\n");
		return 1;
	}
	struct hostent* send_server;
	struct sockaddr_in send_serv_addr;
	int send_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (send_fd < 0) {
		printf("jnx_network_send_message couldn't open send port\n");
		return 1;
	}
	send_server = gethostbyname(host);
	if (send_server < 0) {
		printf(" jnx_network_send_message couldnt resolve ip\n");
		return 1;
	}
	bzero((char*) &send_serv_addr, sizeof(send_serv_addr));
	send_serv_addr.sin_family = AF_INET;
	bcopy((char*) send_server->h_addr, (char*) &send_serv_addr.sin_addr.s_addr, send_server->h_length);
	send_serv_addr.sin_port = htons(port);
	printf("jnx_network_send_message attempting to connect...\n");
	if (connect(send_fd, (struct sockaddr*) &send_serv_addr, sizeof(send_serv_addr)) < 0) {
		printf("jnx_network_send_message error whilst connecting\n");
		return 1;
	}
	printf("jnx_network_send_message connected\n");
	size_t n;
	char response_buffer[MAXBUFFER];
	n = write(send_fd, msg, strlen(msg));
	if (n < 0) {
		perror("jnx_network_send_message error writing to socket");
		return 1;
	}
	n = read(send_fd,response_buffer,MAXBUFFER -1 );	
	if (n < 0) {
		perror("jnx_network_send_message error reading from socket");
		return 1;
	}
	close(send_fd);
 	if(strlen(response_buffer) > 0)	
	(*jnx_network_send_message_callback)(response_buffer);
	return 0;
}
int jnx_network_send_broadcast(int port,char *broadcastgroup,char *message)
{
	struct sockaddr_in addr;
	int fd;
	if((fd=socket(AF_INET,SOCK_DGRAM,0)) < 0){
		perror(" jnx_network_send_broadcast broadcast");
		return 1;
	}
	memset(&addr,0,sizeof(addr));
	addr.sin_family=AF_INET;
	addr.sin_addr.s_addr=inet_addr(broadcastgroup);
	addr.sin_port=htons(port);
	if(sendto(fd,message,strlen(message),0,(struct sockaddr *) &addr, sizeof(addr)) < 0)
	{
		perror("jnx_network_send_broadcast sendto");
		return 1;
	}
	return 0;
}
void jnx_network_broadcast_listener(int port,char *broadcastgroup, void(*jnx_network_broadcast_callback)(char*))
{
	struct sockaddr_in addr;
	int fd, nbytes, addrlen;
	struct ip_mreq mreq;
	char buffer[MAXBUFFER];
	u_int yes=1;
	if((fd=socket(AF_INET,SOCK_DGRAM,0)) < 0){
		perror("jnx_network_broadcast_listener broadcast listener");
		return;
	}
	if(setsockopt(fd,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(yes)) < 0)
	{
		perror("jnx_network_broadcast_listener setsockopts");
		return;
	}
	memset(&addr,0,sizeof(addr));
	addr.sin_family=AF_INET;
	addr.sin_addr.s_addr=htonl(INADDR_ANY); /* N.B.: differs from sender */
	addr.sin_port=htons(port);
	if(bind(fd,(struct sockaddr*)&addr,sizeof(addr)) < 0){
		perror("jnx_network_broadcast_listener bind");
		return;
	}
	mreq.imr_multiaddr.s_addr=inet_addr(broadcastgroup);
	mreq.imr_interface.s_addr=htonl(INADDR_ANY);
	if(setsockopt(fd,IPPROTO_IP,IP_ADD_MEMBERSHIP,&mreq,sizeof(mreq)) < 0)
	{
		perror("jnx_network_broadcast_listener setsockopt");
		return;
	}    
	bzero(buffer,MAXBUFFER);
	while(1)
	{
		addrlen=sizeof(addr);
		if((nbytes=recvfrom(fd,buffer,MAXBUFFER,0,(struct sockaddr*)&addr,(socklen_t*)&addrlen)) < 0){
			perror("jnx_network_broadcast_listener recvfrom");
			return;
		}
		(*jnx_network_broadcast_callback)(buffer);
	}
}
char* jnx_network_local_ip(char* interface)
{
	int fd;
	char* ipadd = NULL;
	struct ifreq ifr;
	fd = socket(AF_INET, SOCK_DGRAM, 0);
	ifr.ifr_addr.sa_family = AF_INET;
	snprintf(ifr.ifr_name, IFNAMSIZ, interface);
	ioctl(fd, SIOCGIFADDR, &ifr);
	ipadd = inet_ntoa(((struct sockaddr_in*) &ifr.ifr_addr)->sin_addr);
	close(fd);
	return ipadd;
}

