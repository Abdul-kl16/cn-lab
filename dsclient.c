#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<fcntl.h>
#include<stdlib.h>


struct message{

    int to_node;

    int from_node;

    int RT[20][20][20];
};

 struct message mesg, mesg_from;
main(int argc, char * argv[])
{
int i,j,n, h,k;

int num_nodes;

int sock_fd, max_fd, nready, fd[2];

char buffer[100], line[100];

struct sockaddr_in servaddr;

fd_set rset;

int node_sel, temp1;

int node, p;

int from_node_it;

int to_node_it, t;

int prev[20];

int cost[10][10];

int N[20][20];

temp1 = 999;

h=0;

printf("Enter number of nodes:");
scanf("%d", &num_nodes);

printf("Enter the cost matrix, For infinity enter 999\n");
for(i=0; i <num_nodes; i++)
for(j=0; j <num_nodes; j++)
scanf("%d", &cost[i][j]);

// DISTANCE VECTOR MATRIX FOR EACH NODE
for(i= 0; i < num_nodes; i++)
for(j=0; j < num_nodes; j++)
for(k=0; k < num_nodes; k++)
{
if(i==j)
mesg.RT[i][j][k] = cost[j][k];
else
mesg.RT[i][j][k] = 999;
}

// NEIGHBOURS
for(i=0; i < 20; i++)
for(j=0; j < 20; j++)
N[i][j] = -1;

// COST MATRIX
for(i = 0; i < num_nodes; i++)
for(j = 0; j < num_nodes; j++)
{
if(cost[i][j] < 999)
{
    N[i][j] =j;

}
}
        if(argc != 3)
{
fprintf(stderr, "Usage: ./client IPaddress_of_server port\n");

exit(1);

}

if((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
{

printf("Cannot create socket\n");

exit(1);

}
bzero((char*)&servaddr, sizeof(servaddr));

bzero(line, sizeof(line));

servaddr.sin_family = AF_INET;

servaddr.sin_port = htons(atoi(argv[2]));

inet_pton(AF_INET, argv[1], &servaddr.sin_addr);

if(connect(sock_fd, (struct sockaddr *)&servaddr, sizeof(servaddr))< 0)
{
perror("Connection failed:");
exit(1);
}
fd[0] = 0;
fd[1] = sock_fd;

for(; ; )
{
FD_ZERO(&rset);

FD_SET(0, &rset);

FD_SET(sock_fd, &rset);

bzero(line, sizeof(line));


max_fd = sock_fd;

nready = select(max_fd + 1, &rset, NULL, NULL, NULL);
  
  for ( j = 0; j <2 ; j++)
{
if(FD_ISSET(fd[j], &rset))
{
if(j==0)
{ // reading from std input

printf("Enter node number:");
scanf("%d", &node);

printf("Node = %d\n", node);
getchar();
getchar();

// send messages to its neighbours
  t=0;
 while(N[node][t] != -1)
{
 if(N[node][t] != node)
{
mesg.from_node = node;
 mesg.to_node = N[node][t];
  n = write(fd[j+1], &mesg, sizeof(mesg));
     }
 t++;
    }
 }
 else
{
read(fd[j], &mesg_from, sizeof(mesg_from));
to_node_it = mesg_from.to_node;
from_node_it = mesg_from.from_node;
 
 for(i =0; i<num_nodes; i++)
mesg.RT[to_node_it][from_node_it][i] = mesg_from.RT[from_node_it][from_node_it][i];

 //DISTANCE VECTOR OF to_node_it
 mesg.RT[to_node_it][to_node_it][to_node_it] = 0;
 for(i =0; i<num_nodes ; i++)
 prev[i] = mesg.RT[to_node_it][to_node_it][i];
 i = from_node_it;
 
 while(N[to_node_it][h] != -1)

 {
if(N[to_node_it][h] != 0)
{
if(N[to_node_it][h] == from_node_it)
{
node_sel = from_node_it;
if (temp1 > cost[to_node_it][N[to_node_it][h]] +

mesg_from.RT[node_sel][N[to_node_it][h]][i])

temp1 = cost[to_node_it][N[to_node_it][h]] +

mesg_from.RT[node_sel][N[to_node_it][h]][i];

}
else
{
node_sel = to_node_it;
if (temp1 > cost[to_node_it][N[to_node_it][h]] +

mesg.RT[node_sel][N[to_node_it][h]][i])

temp1 = cost[to_node_it][N[to_node_it][h]] +

mesg.RT[node_sel][N[to_node_it][h]][i];

}
}
  h++;
}
 mesg.RT[to_node_it][to_node_it][i] = temp1;
 h=0;

 for(i =0; i<num_nodes ; i++)
{
 if( prev[i] != mesg.RT[to_node_it][to_node_it][i])
{
p=0;
 while(N[to_node_it][p] !=-1 )
{
  if(p != to_node_it)
{
 mesg.to_node = N[to_node_it][p];
 mesg.from_node = to_node_it;
 write(fd[j], &mesg, sizeof(mesg));
}
p++;
}
break;
   }
}

temp1 =999;
}
 if(--nready == 0)
    break;
   }
}
 for(i = 0; i < num_nodes; i++)
{
for(j = 0; j < num_nodes; j++)
{
     
    printf("%d ", mesg.RT[node][i][j]);
  }
  
  printf("\n");
}

printf("ITERATION RESULT FOR ROUTING TABLE\n");

  }
}