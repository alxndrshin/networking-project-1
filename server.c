#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define SERVER_PORT 5432
#define MAX_PENDING 5
#define MAX_LINE 256

int main()
{
	FILE *fp;
	struct sockaddr_in sin;
	char command_buf[MAX_LINE];
	char file_buf[MAX_LINE];
	int len;
	int s, command_socket, file_socket;
	
	/* build address data structure */
	bzero((char *)&sin, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = INADDR_ANY;
	sin.sin_port = htons(SERVER_PORT);
	
	/* setup passive open */
	if ((s = socket(PF_INET, SOCK_STREAM, 0)) < 0) 
	{
		perror("simplex-talk: socket");
		exit(1);
	}
	
	if ((bind(s, (struct sockaddr *)&sin, sizeof(sin))) < 0) 
	{
		perror("simplex-talk: bind");
		exit(1);
	}
	
	listen(s, MAX_PENDING);

	/* wait for connection, then receive and print text */
	while(1) 
	{
		if ((command_socket = accept(s, (struct sockaddr *)&sin, &len)) < 0) 
		{
			perror("simplex-talk: accept");
			exit(1);
		}
		
		fp = fopen("itmadeit.txt", "a");	

		while (len = recv(command_socket, command_buf, sizeof(command_buf), 0))
		{
			while(1)
			{
				if ((file_socket = accept(s, (struct sockaddr *)&sin, &len)) < 0) 
				{
					perror("simplex-talk: accept");
					exit(1);
				}
		
				while((len = recv(file_socket, file_buf, sizeof(file_buf), 0)))
				{
					printf("%s", file_buf);
					fprintf(fp, "%s", file_buf);
					bzero(file_buf, MAX_LINE);
				}

				close(file_socket);
				break;
			}	
		
		}
		
		fclose(fp);
		break;

	}
	close(command_socket);
}
