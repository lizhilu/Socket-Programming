all:
    
compile:
	gcc client.c serverA.c serverB.c serverC.c serverD.c -lsocket -lnsl -lresolv -o socket
