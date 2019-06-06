#include <unistd.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <signal.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/tcp.h>

union val {
	int i_val;
	long l_val;
	struct linger linger_val;
	struct timeval timeval_val;
};


void gotoEnd() {
	printf("Time Out CallBack Function.\n");
	return ;
}

int main(int argc, char** argv) {
	int client;
	if ((client = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("Socket Error");
	}
	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(struct sockaddr_in));
	addr.sin_family = AF_INET;
	char* address = "192.168.26.83";
	inet_pton(AF_INET, address, &addr.sin_addr);
	addr.sin_port = htons(4321);

	/*int sendBuffSize = ;*/
	/*socklen_t len = sizeof(sendBuffSize);*/

	/*int status = setsockopt(client, SOL_SOCKET, SO_SNDBUF, &sendBuffSize, len);*/
	/*if (status < 0) {*/
		/*perror("Set Opt Error");*/
		/*exit(1);*/
	/*}*/

	int sendBuffSize;
	socklen_t len = sizeof(sendBuffSize);
	int s = getsockopt(client, SOL_SOCKET, SO_SNDBUF, &sendBuffSize, &len);
	if (s < 0) {
		perror("Get Opt Error");
		exit(1);
	}
	printf("The send buff size is : %d.\n", sendBuffSize);

	int status = connect(client, (struct sockaddr*)&addr, sizeof(struct sockaddr_in));
	if (status < 0) {
		perror("Connect Error");
		exit(1);
	}

	char buf[512];
	int n, wn;
	/*int fd = open("/home/lishigang/current/Rapunzel/rapunzel_server/annotation/annotation", O_RDONLY);*/
	int fd = open("/home/lishigang/lishigang/c10k/client/1.json", O_RDONLY);
	if (fd < 0) {
		perror("Read Error");
		exit(1);
	}

	int flag = fcntl(client, F_GETFL, 0);
	flag |= O_NONBLOCK;
	int ss = fcntl(client, F_SETFL, flag);

	union val curVal;
	socklen_t valLen = sizeof(curVal);
	int delayFlag = 1;

	int sss = setsockopt(client, IPPROTO_TCP, TCP_NODELAY, (const void*)&delayFlag, sizeof(delayFlag));
	if (sss != 0) {
		perror("Set sock opt Error");
	} else {
		printf("0........%d.\n", sss);
	}
	int afterFlag = -1;
	int afterLen = sizeof(afterFlag);
	sss = getsockopt(client, IPPROTO_TCP, TCP_NODELAY, (void*)&afterFlag, &afterFlag);
	if (sss != 0) {
		perror("Get sock opt Error");
	} else {
		printf("1.........%d.\n", afterFlag);
	}

	char littleBuf[1] = {0};
	char* curStr = "Fuck University.\n";
	int strLen = strlen(curStr);
	char* iter = curStr;
	for (int i = 0; i < strLen; ++i) {
		littleBuf[0] = *iter++;
		write(client, littleBuf, 1);
	}

	/*while ((n = read(fd, buf, sizeof(buf))) > 0) {*/
	/*if ((n = read(fd, buf, sizeof(buf))) > 0) {*/
		/*wn = write(client, buf, n);*/
		/*sleep(1);*/
		/*printf("Write %d bytes.\n", wn);*/
	/*}*/

	if (n < 0) {
		perror("Write Error");
	} else {
		printf("EOF.\n");
	}

	char wBuff[1024];
	memset(wBuff, 0, sizeof(wBuff));

	/*struct sigaction action, oldAction;*/
	/*memset(&action, 0, sizeof(struct sigaction));*/
	/*action.sa_handler= gotoEnd;*/
	/*action.sa_flags = 0;*/
	/*sigaction(SIGALRM, &action, &oldAction);*/

	/*alarm(5);*/

	/*sleep(1);*/
	while (1) {
		while ((n = read(client, wBuff, sizeof(wBuff))) > 0) {
			/*char curString[1024];*/
			/*sprintf(curString, "The Msg is : %s\n", wBuff);*/
			/*printf("The string is : %s\n", curString);*/
			/*printf("----------%d.\n", n);*/
			printf("%s\n", wBuff);
		}
		if ((n < 0 && errno == EAGAIN) || n == 0) {
			break;
		} else if (n < 0 && errno != ERANGE) {
			perror("Read Error");
		}
	}
	
	/*if (n < 0) {*/
		/*perror("Read Error");*/
	/*}*/

	/*const char* str = "Monster Hunter World.";*/
	/*memcpy(buf, str, strlen(str));*/
	/*for (int i = 0; i < 2; ++i) {*/
		/*if ((wn = write(client, buf, sizeof(buf))) < 0) {*/
			/*perror("Write Error");*/
			/*break;*/
		/*}*/
		/*sleep(2);*/
	/*}*/
	/*while (1) {}*/

	/*shutdown(client, SHUT_WR);*/
	close(client);
	return 0;
}
