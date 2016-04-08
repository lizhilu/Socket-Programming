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

int main(){
    int sock=0;
    struct sockaddr_in  client;
    struct sockaddr_in sender;
    int rqst=0;
    socklen_t alen;
    char receive[1000]="";
    int length=0;
    char *host= "localhost";
    struct hostent *hp;
    
    if((sock = socket(AF_INET, SOCK_STREAM,0)) < 0){
        perror("cannot create socket");
        exit(-1);
    }
    
    memset((char *)&client, 0, sizeof(client));
    client.sin_family = AF_INET;
    client.sin_port = htons(25842);
    hp = gethostbyname(host);
    
    if (!hp) {
        fprintf(stderr, "could not obtain address of %s\n", host);
        return 0;
    }
    
    memcpy((void *)&client.sin_addr, hp->h_addr_list[0], hp->h_length);
    
    if (bind(sock, (struct sockaddr *)&client, sizeof(client)) < 0) {
        perror("bind failed");
        exit(-1);
    }
    
    if((listen(sock, 5))<0){
        perror("listen failed");
        exit(-1);
    }
    alen = sizeof(client);
    getsockname(sock,(struct sockaddr *)&client,&alen);
    printf("The Client has TCP port number %d and IP address %s.", ntohs(client.sin_port), inet_ntoa(client.sin_addr));
    printf("\n");
    
    char *token;
    int inf[4][4]={};
    int i=0;
    int j=0;
    char adjacent[1000]="";
    
    while(j<4){
        
    if((rqst = accept(sock, (struct sockaddr *)&sender, &alen)) < 0) {
            perror("accept failed");
            exit(-1);
    }
    length = recv(rqst, receive,1000, 0);
    struct sockaddr_in peer;
    socklen_t len= sizeof(peer);
    getpeername(rqst,(struct sockaddr *)&peer,&len);
    if(j==0){
    printf("The Client receives neighbor information from the Server A with TCP port number %d and IP address %s\n",ntohs(peer.sin_port), inet_ntoa(peer.sin_addr));
    printf("The Server A has the following neighbor information:\n");
    }
    if(j==1){
    printf("The Client receives neighbor information from the Server B with TCP port number %d and IP address %s\n",ntohs(peer.sin_port), inet_ntoa(peer.sin_addr));
    printf("The Server B has the following neighbor information:\n");
    }
    if(j==2){
    printf("The Client receives neighbor information from the Server C with TCP port number %d and IP address %s\n",ntohs(peer.sin_port), inet_ntoa(peer.sin_addr));
    printf("The Server C has the following neighbor information:\n");
    }
    if(j==3){
    printf("The Client receives neighbor information from the Server D with TCP port number %d and IP address %s\n",ntohs(peer.sin_port), inet_ntoa(peer.sin_addr));
    printf("The Server D has the following neighbor information:\n");
    }
    
    printf("Neighbor-----Cost\n");
        
    token = strtok(receive," ");
    while(token!= NULL){
        printf("%s       ", token);
        if(strcmp(token,"serverA")==0){
            i=0;
        }
        else if(strcmp(token,"serverB")==0){
            i=1;
        }
        else if(strcmp(token,"serverC")==0){
            i=2;
        }
        else if(strcmp(token,"serverD")==0){
            i=3;
        }else{
            inf[j][i] = inf[j][i]+atoi(token);
            inf[i][j] = inf[i][j]+atoi(token);
        }
        token = strtok(NULL," ");
        if(strcmp(token,"serverA")==0){
            i=0;
        }
        else if(strcmp(token,"serverB")==0){
            i=1;
        }
        else if(strcmp(token,"serverC")==0){
            i=2;
        }
        else if(strcmp(token,"serverD")==0){
            i=3;
        }else{
            inf[j][i] = inf[j][i]+atoi(token);
            inf[i][j] = inf[i][j]+atoi(token);
        }
        printf("%s", token);
        token = strtok(NULL," ");

    }
        printf("\n");
        getsockname(rqst,(struct sockaddr *)&client,&alen);
        if(j==0){
            printf("For this connection with Server A, The Client has TCP port number %d and IP address %s.\n",ntohs(client.sin_port), inet_ntoa(client.sin_addr));
        }
        if(j==1){
            printf("For this connection with Server B, The Client has TCP port number %d and IP address %s.\n",ntohs(client.sin_port), inet_ntoa(client.sin_addr));
        }
        if(j==2){
            printf("For this connection with Server C, The Client has TCP port number %d and IP address %s.\n",ntohs(client.sin_port), inet_ntoa(client.sin_addr));
        }
        if(j==3){
            printf("For this connection with Server D, The Client has TCP port number %d and IP address %s.\n",ntohs(client.sin_port), inet_ntoa(client.sin_addr));
        }
        printf("\n");
        close(rqst);
     
       j++;
    }
  
   for(i=0;i<4;i++){
       for(j=0;j<4;j++){
            inf[i][j]=inf[i][j]/2;
        }
    }
    printf("\n");
    
    char temp[100]="";
    for(i=0;i<4;i++){
        for(j=i;j<4;j++){
            if(inf[i][j]!=0){
                if(i==0){
                  strcat(adjacent,"A");
                }
                if(i==1){
                  strcat(adjacent,"B");
                }
                if(i==2){
                  strcat(adjacent,"C");
                }
                if(i==3){
                  strcat(adjacent,"D");
                }
                if(j==0){
                  strcat(adjacent,"A");
                  strcat(adjacent," ");
                }
                if(j==1){
                    strcat(adjacent,"B");
                    strcat(adjacent," ");
                }
                if(j==2){
                    strcat(adjacent,"C");
                    strcat(adjacent," ");
                }
                if(j==3){
                    strcat(adjacent,"D");
                    strcat(adjacent," ");
                }
                sprintf(temp,"%d",inf[i][j]);
                strcat(adjacent, temp);
                strcat(adjacent, " ");
            }
        }
    }
    close(sock);
    
    char adjacent1[1000]="";
    char adjacent2[1000]="";
    char adjacent3[1000]="";
    strncpy(adjacent1, adjacent, 1000);
    strncpy(adjacent2, adjacent, 1000);
    strncpy(adjacent3, adjacent, 1000);
    //UDP send to serverA
    struct sockaddr_in myaddr;
    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("cannot create socket");
        exit(1);
    }
    memset((char*)&myaddr, 0, sizeof(myaddr));
    myaddr.sin_family = AF_INET;
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
    
    
    struct sockaddr_in serverA;
    
    memset((char*)&serverA, 0, sizeof(serverA));
    serverA.sin_family = AF_INET;
    serverA.sin_port = htons(21842);
    hp = gethostbyname(host);
    
    if (!hp) {
        fprintf(stderr, "could not obtain address of %s\n", host);
        return 0;
    }
    
    memcpy((void *)&serverA.sin_addr, hp->h_addr_list[0], hp->h_length);
    
    if (sendto(sock, adjacent, strlen(adjacent), 0, (struct sockaddr *)&serverA, sizeof(serverA)) < 0) {
        perror("sendto failed");
        return 0;
    }
    printf("The Client has sent the network topology to the network topology to the Server A with UDP port number %d and IP address %s (Server A’s UDP port number and IP address) as follows:\n",ntohs(serverA.sin_port), inet_ntoa(serverA.sin_addr));
    printf("Edge------Cost\n");
    token = strtok(adjacent," ");
    while(token!= NULL){
        printf("%s        ", token);
        token = strtok(NULL," ");
        printf("%s\n", token);
        token = strtok(NULL," ");
    }
    
    socklen_t addrlen = sizeof(myaddr);
    getsockname(sock,(struct sockaddr *)&myaddr,&addrlen);
    printf("For this connection with Server A, The Client has UDP port number %d and IP address %s.", ntohs(myaddr.sin_port), inet_ntoa(myaddr.sin_addr));
    printf("\n");
    
    // UDP send to serverB
    struct sockaddr_in serverB;
    
    
    memset((char*)&serverB, 0, sizeof(serverB));
    serverB.sin_family = AF_INET;
    serverB.sin_port = htons(22842);
    hp = gethostbyname(host);
    
    if (!hp) {
        fprintf(stderr, "could not obtain address of %s\n", host);
        return 0;
    }
    
    memcpy((void *)&serverB.sin_addr, hp->h_addr_list[0], hp->h_length);
    
    if (sendto(sock, adjacent1, strlen(adjacent1), 0, (struct sockaddr *)&serverB, sizeof(serverB)) < 0) {
        perror("sendto failed");
        return 0;
    }
    
    printf("The Client has sent the network topology to the network topology to the Server B with UDP port number %d and IP address %s (Server B’s UDP port number and IP address) as follows:\n",ntohs(serverB.sin_port), inet_ntoa(serverB.sin_addr));
    printf("Edge------Cost\n");
    token = strtok(adjacent1," ");
    while(token!= NULL){
        printf("%s        ", token);
        token = strtok(NULL," ");
        printf("%s\n", token);
        token = strtok(NULL," ");
    }
    printf("For this connection with Server B, The Client has UDP port number %d and IP address %s.", ntohs(myaddr.sin_port), inet_ntoa(myaddr.sin_addr));
    printf("\n");


    //UDP send to serverC
    
    
    struct sockaddr_in serverC;
    
    memset((char*)&serverC, 0, sizeof(serverC));
    serverC.sin_family = AF_INET;
    serverC.sin_port = htons(23842);
    hp = gethostbyname(host);
    
    if (!hp) {
        fprintf(stderr, "could not obtain address of %s\n", host);
        return 0;
    }
    
    memcpy((void *)&serverC.sin_addr, hp->h_addr_list[0], hp->h_length);
    
    if (sendto(sock, adjacent2, strlen(adjacent2), 0, (struct sockaddr *)&serverC, sizeof(serverC)) < 0) {
        perror("sendto failed");
        return 0;
    }
    
    printf("The Client has sent the network topology to the network topology to the Server C with UDP port number %d and IP address %s (Server C’s UDP port number and IP address) as follows:\n",ntohs(serverC.sin_port), inet_ntoa(serverC.sin_addr));
    printf("Edge------Cost\n");
    token = strtok(adjacent2," ");
    while(token!= NULL){
        printf("%s        ", token);
        token = strtok(NULL," ");
        printf("%s\n", token);
        token = strtok(NULL," ");
    }
    
    printf("For this connection with Server C, The Client has UDP port number %d and IP address %s.", ntohs(myaddr.sin_port), inet_ntoa(myaddr.sin_addr));
    printf("\n");


    //UDP send to serverD
    
    
    struct sockaddr_in serverD;
    memset((char*)&serverD, 0, sizeof(serverD));
    serverD.sin_family = AF_INET;
    serverD.sin_port = htons(24842);
    hp = gethostbyname(host);
    
    if (!hp) {
        fprintf(stderr, "could not obtain address of %s\n", host);
        return 0;
    }
    
    memcpy((void *)&serverD.sin_addr, hp->h_addr_list[0], hp->h_length);
    
    if (sendto(sock, adjacent3, strlen(adjacent3), 0, (struct sockaddr *)&serverD, sizeof(serverD)) < 0) {
        perror("sendto failed");
        return 0;
    }
    
    printf("The Client has sent the network topology to the network topology to the Server D with UDP port number %d and IP address %s (Server D’s UDP port number and IP address) as follows:\n",ntohs(serverD.sin_port), inet_ntoa(serverD.sin_addr));
    printf("Edge------Cost\n");
    token = strtok(adjacent3," ");
    while(token!= NULL){
        printf("%s        ", token);
        token = strtok(NULL," ");
        printf("%s\n", token);
        token = strtok(NULL," ");
    }
    
    printf("For this connection with Server D, The Client has UDP port number %d and IP address %s.", ntohs(myaddr.sin_port), inet_ntoa(myaddr.sin_addr));
    printf("\n");
    
    close(sock);
    
    
    
    //calculate minimum spanning tree
    int total=0;
    char value[1000]="";
    char tep[100]="";
    int n = 4;
    int u=0;
    int v=0;
    int visited[4]={0};
    int min=0;
    for(i=0;i<4;i++){
        for(j=0;j<4;j++){
            if(inf[i][j]==0){
                inf[i][j]=1000;
            }
        }
    }
    
    visited[0]=1;
    for(int counter=0;counter<n-1;counter++){
         min=1000;
        for(i=0;i<4;i++){
            if(visited[i]==1){
                for(j=0;j<4;j++){
                    if(visited[j]!=1){
                        if(min>inf[i][j]){
                            min=inf[i][j];
                            u=i;
                            v=j;
                        }
                    }
                }
            }
        }
        visited[j]=1;
        visited[v]=1;
        total+=min;
        if(u==0){
           strcat(value,"A");
        }
        if(u==1){
            strcat(value,"B");
        }
        if(u==2){
            strcat(value,"C");
        }
        if(u==3){
            strcat(value,"D");
        }
        if(v==0){
            strcat(value,"A");
            strcat(value," ");
        }
        if(v==1){
            strcat(value,"B");
            strcat(value," ");
        }
        if(v==2){
            strcat(value,"C");
            strcat(value," ");
        }
        if(v==3){
            strcat(value,"D");
            strcat(value," ");
        }
        sprintf(tep,"%d",inf[u][v]);
        strcat(value, tep);
        strcat(value," ");
        inf[u][v]=1000;
        inf[v][u]=1000;
    }
    printf("The Client has calculated a minimum spanning tree. The tree cost is %d\n",total);
    printf("Edge-----Cost\n");
    token = strtok(value," ");
    while(token!= NULL){
        printf("%s       ", token);
        token = strtok(NULL," ");
        printf("%s\n", token);
        token = strtok(NULL," ");
    }
    return (0);
}
