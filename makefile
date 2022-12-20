all:
	gcc P1SocketServer.c -o P1SocketServer
	gcc P2SocketClient.c -o P2SocketClient
	gcc P1FIFOServer.c -o P1FIFOServer
	gcc P2FIFOClient.c -o P2FIFOClient