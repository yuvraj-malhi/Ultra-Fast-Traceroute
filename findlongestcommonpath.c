#include<time.h>
#include<stdio.h>
#include<error.h>
#include<errno.h>
#include<netdb.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<signal.h>
#include<sys/wait.h>
#include<sys/time.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<arpa/inet.h>
#include<sys/socket.h>



#define DOMAIN_FILE "domain.txt"		// File to store domain names
#define MAX_DOMAINS 30				// Maximum number of domains in domain.txt
////////////////////////////////


#define SEQ "NetProgSEQ"	//Last domain to denote end of sequence
#define L_PORT 4400		// Port to connect with traceroute
int FD;				// Contact with traceroute


int max_ttl;			// In traceroute
int nprobes;			// In traceroute
int DSIZE;			// Store total number of request
char data[MAX_DOMAINS][25];

//////////////////////////////

struct Result{
	int RQ;
	char result[500][16];
};
struct Result *R;                 // Store results


int fill (char data[MAX_DOMAINS][25])
{

	printf("\n--------------------- READ AND REQUEST ----------------------\n\n");
	printf("DATA: Reading  from   file - %s\n",DOMAIN_FILE);

	FILE* DATA_F = fopen (DOMAIN_FILE, "r");
	if(DATA_F == NULL)
	{
		perror("DATA: Error opening");
		exit(0);
	}

	int i = 0, size = 0;

	fscanf (DATA_F, "%s", data[0]);
	while (!feof (DATA_F))
	{
	      size++;
	      fscanf (DATA_F, "%s", data[size]);
	}
	fclose ( DATA_F );

	printf("DATA: File opened\n");


	return size;

}


void add(struct Result R)
{
	printf("Following is the result of %s:\n",data[R.RQ-1]);

	int dd=0;
	int i=0;
	
	for(int ee=0; (ee<max_ttl)&&(dd==0); ee++)
	{
		printf(" TTL = %d",ee);
		for(int j=0; j<nprobes; j++)
		{
			if(R.result[i][0]=='\0')
			{
				dd=1;
				break;
			}
			else
				printf("\t%16s",R.result[i++]);
		}
		printf("\n");
	}


}



void end (int sig) ///////////////////////// Sig handler
{}

void main()
{
	
	signal(SIGINT, end);


	printf("\n------------------------- INITIALIZE -------------------------\n\n");

	struct sockaddr_in conn;
	FD = socket(AF_INET, SOCK_STREAM, 0);
	if( FD == -1 )
	{
		printf("CONNECT: Error creating sokcet\n");
	        exit(0);
        }

	conn.sin_port   = htons(L_PORT);
	conn.sin_family = AF_INET;
	conn.sin_addr.s_addr  =  htonl(INADDR_ANY);

	printf("CONNECT: Connecting with fastertraceroute.c ....\n");

	if (connect(FD, (struct sockaddr *)&conn, sizeof(conn)) < 0)
	{
		perror("CONNECT"); 
		exit(0);
	}

	printf("CONNECT: Connection successful\n");


	char buff[6];
	printf("SYNC: Getting secondary data...\n");

	read(FD, buff, sizeof(buff));
	max_ttl=atoi(buff);

	read(FD, buff, sizeof(buff));
	nprobes=atoi(buff);
	

	printf("SYNC: Value for:\tTTL (Time to live) -\t%d\n\t\t\tNumber of probes   -\t%d\n",max_ttl,nprobes);




	DSIZE = fill(data);

	
	printf("\n-------------------------- PROCESS ---------------------------\n\n");
	printf("SEND: Sending data and recieving results from fastertraceroute.c ....\n");

	R = (struct Result*)malloc(sizeof(struct Result)*DSIZE);

	
	fd_set SET;
	struct timeval tt;
	tt.tv_usec = 500000;
	tt.tv_sec=0;


	int value, tmp, lst=0;
	int send=0, recv=0;		//counters
	while(1)
	{
		if( (send==DSIZE)&&(lst==0) )
		{	
			char last[25];
			strcpy(last,SEQ);
			write(FD,last,sizeof(last));
			lst=1;
			continue;
		}

		if( (recv==DSIZE) && (send==DSIZE) )
			break;

		if(recv!=DSIZE)
		{
			FD_ZERO(&SET);
			FD_SET(FD,&SET);
			
			value = select(FD+1, &SET, NULL, NULL, &tt);
			if(value==-1)
				continue;

			if(value > 0)
			{
				printf("SERVER has sent a message\n");
				read(FD,R+recv,sizeof(R[0]));
				printf("\t\t\t\tRECV RQ %d: Result taken\n",R[recv].RQ);
				add(R[recv]);
				recv++;
			}
		}

		if(send!=DSIZE)
		{
			printf("\t\t\t\tSENT RQ %d: %s\n",send+1, data[send]);
			write(FD,data[send],sizeof(data[send]));
		//	printf("Read - %d\n",R[0].RQ);
			send++;
		}
	}

	printf("PROCESS: All data has been processed\n");
	
	printf("Showing final result....\n");

}

/*
int* arr; // global
int n;

f(){
	printf("%d\n", arr[n-1]);
}

main(){
	int brr[10];
	arr = brr;
	n = 10;
}

free malloc variables

signal handlers :-
	- close socket fds
	- kill children
	- different signal handler for parent and child
*/
