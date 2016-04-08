#include <stdio.h>
#include <stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<errno.h>
#include <unistd.h>
#include <arpa/inet.h>
#include<string.h>
#include<netdb.h>

/* paddr: print the IP address in a standard decimal dotted format */
void
paddr(unsigned char *a)
{
    printf("%d.%d.%d.%d\n", a[0], a[1], a[2], a[3]);
}

/*main method*/
int main(){
    
    int sock=0;
    struct hostent *hp;
    char *host= "localhost";
    struct sockaddr_in myaddr;

    
    if((sock = socket(AF_INET, SOCK_STREAM,0)) < 0){
        perror("cannot create socket");
        exit(1);
    }
    memset((char*)&myaddr, 0, sizeof(myaddr));
    myaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    myaddr.sin_port = htons(0);
    hp = gethostbyname(host);
    
    if (!hp) {
        fprintf(stderr, "could not obtain address of %s\n", host);
        return 0;
    }
    
    memcpy((void *)&myaddr.sin_addr, hp->h_addr_list[0], hp->h_length);

    
    if (bind(sock, (struct sockaddr *)&myaddr, sizeof(myaddr)) < 0) {
        perror("bind failed");
        return 0;
    }
    socklen_t addrlen = sizeof(myaddr);
    getsockname(sock,(struct sockaddr *)&myaddr,&addrlen);
    
    struct sockaddr_in server;
    
    memset((char*)&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(25842);
    hp = gethostbyname(host);
    
    if (!hp) {
        fprintf(stderr, "could not obtain address of %s\n", host);
        return 0;
    }
    
    memcpy((void *)&server.sin_addr, hp->h_addr_list[0], hp->h_length);
    
    int new=0;
    if ((new=connect(sock, (struct sockaddr *)&server, sizeof(server))) < 0) {
        perror("connect failed");
        exit(1);
    }
    getsockname(new,(struct sockaddr *)&myaddr,&addrlen);
    printf("The Server B is up and running.\n");
    
    int sent=0;
    FILE *fp;
    char str[1000]="";
    char trans[1000]="";
    char *token;
    
    fp = fopen("serverB.txt","r");
    if(fp == NULL){
        perror("Error opening file");
        return(-1);
    }
    printf("The Server B has the following neughbor information:\n");
    printf("Neighbor-----Cost\n");
    while( fgets(str,1000, fp)!= NULL ){
        strcat(trans,str);
        strcat(trans," ");
        token = strtok(str," ");
        printf("%s       ", token);
        token = strtok(NULL," ");
        printf("%s", token);
    }
    printf("\n");
    sent = send(sock, trans, strlen(trans),0);
    
    printf("The Server B finishes sending its neighbor information to the Client with TCP port number %d and IP address ",ntohs(server.sin_port));
    for (int k=0; hp->h_addr_list[k]!= 0; k++){
        paddr((unsigned char*) hp->h_addr_list[k]);
    }
    printf("\n");
    printf("For this connection with the Client, the Server B has T​CP port number %d and IP address %s.\n", ntohs(myaddr.sin_port), inet_ntoa(myaddr.sin_addr));
    

    fclose(fp);
    close(sock);
    
    //UDP
    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("cannot create socket");
        exit(1);
    }
    
    struct sockaddr_in client;
    
    memset((char *)&client, 0, sizeof(client));
    client.sin_family = AF_INET;
    client.sin_port = htons(22842);
    hp = gethostbyname(host);
    
    if (!hp) {
        fprintf(stderr, "could not obtain address of %s\n", host);
        return 0;
    }
    
    memcpy((void *)&client.sin_addr, hp->h_addr_list[0], hp->h_length);

    
    if (bind(sock, (struct sockaddr *)&client, sizeof(client)) < 0) {
        perror("bind failed");
        return 0;
    }
    
    int recvlen=0;
    char receive[1000]="";
    struct sockaddr_in remote;
    socklen_t remotelen = sizeof(remote);
    
    recvlen = recvfrom(sock, receive, 1000, 0, (struct sockaddr *)&remote, &remotelen);
    printf("\n");
    printf("The server B has received the network topology from the Client with UDP port number %d and IP address %s\n", ntohs(remote.sin_port), inet_ntoa(remote.sin_addr));
    printf("Client’s UDP port number and IP address) as follows:\n");
    printf("Edge-----Cost\n");
    token = strtok(receive," ");
    while(token!= NULL){
        printf("%s        ", token);
        token = strtok(NULL," ");
        printf("%s\n", token);
        token = strtok(NULL," ");
    }
    printf("For this connection with Client, The Server B has UDP port number %d and IP address %s.\n",ntohs(client.sin_port), inet_ntoa(client.sin_addr));
    
    return (0);
    
    
}





