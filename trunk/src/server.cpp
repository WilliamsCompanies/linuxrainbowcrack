#include <unistd.h>
#include <stdio.h>
#include <netinet/in.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/signal.h>
#include "ChainWalkContext.h"
#include "DistGenTable.h"
#include <sys/socket.h>
#define MAX_CLIENTS 5
#define LOGFILE "server.log"
void usage(){
	printf("Usage:\n");
	printf("\t\t./server hash_algorithm plain_charset\n");
	printf("\t\t\tlen_min len_max index chain_length chain_count suffix <chains to send> <port> \n\n");
	
	CHashRoutine hr;
	printf("Available algorithms: %s\n",hr.GetAllHashRoutineName.s_str());
	printf("Available charsets  : Any that is available in charset.txt\n");
	printf("len_min / len_max   : The minimum and maximum plaintext length respectively\n");
	printf("Index				: The table index\n");
	printf("chain_length        : The length of one chain\n");
	printf("chain_count			: The number of chains to generate\n");
	printf("port                : The port to listen on for clients to connect to(default 9999)\n");
	printf("chains to send		: The number of chains to send to the client to generate\n\n");
	printf("Example\n");
	printf("./server md5 loweralpha 1 7 0 8000 4000000 test 100000 1234\n");
	printf("./server md5 loweralpha 1 7 0 5000 7963425 test 50000\n");
}

int main(int argc, char* argv[]){
	CDistGenTable dserver;
	
	//Initialize the server
	if(argc<9 || argc>10){
		usage();
		return 0;
	}
	string sHashRoutineName 	= argv[1];
	string sCharsetName			= argv[2];
	int nPlainLenMin			= atoi(argv[3]);
	int nplainLenMax			= atoi(argv[4]);
	int nrainbowChainIndex		= atoi(argv[5]);
	int nRainbowChainLen		= atoi(argv[6]);
	int nrainbowChainCount		= atoi(argv[7]);
	string sFileTitleSuffix		= argv[8];
	int nChainsToSend			= atoi(argv[9]);
	if(argc==9)
		int nPort				= 9999;
	if(argc==10)
		int nPort				= atoi(argv[10]);
	pid_t pid;
	int sockfd,connfd;
	struct sockaddr_in servaddr, cliaddr;
	time_t istime;
	FILE *logfd;
	memset(&servaddr,'\0',sizeof(servaddr));
	servaddr.sin_family	= AF_INET;
	servaddr.sin_port	= htons(nPort);
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	sockfd = socket(AF_INET,SOCK_STREAM,0);
	if(sockfd<0){
		printf("Cannot create socket\n");
		return -1;
	}
	bind(sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr));
	listen(sockfd,MAX_CLIENTS);
	len=sizeof(liaddr);
	//array for storing the clients max MAX_CLIENTS -> [client index][client ip]
	while(1==1){
		if(desrver.bCheckConnection()){
			//does the client has something to give to us?
			if(dserver.bHasWorkDone()){
				desrver.ReceiveResults();
				break;
			}
			else{
				//if client does not have an id add client to array send client it's id
				if(dserver.GetClientID()!=NULL){
					//send client the table parameters e.g md5 loweralpha 1 7 0 5000 
					if(!dserver.bInit(hashroutine,charsetname,min,max,host)
						printf("Error inititialising %s\n",host);
					//send client how many chains to generate
					dserver.SendWork(host,number of chains);
		}
		//chech if the number of hashes generated is less than that which is given
		desrver.GenChains(1000000);
	}
