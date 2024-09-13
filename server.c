#include <stdio.h>
#include <string.h>
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


	//array of clients
	int client_connections[MAX_Pending];
	//set client sockets =0
	for(int x=0;i<MAX_CLIENTS;i++)
		{
			client_connections[i]=0;
		}
	/*
 code DIRECTLY from geeks for geeks need to rearange and figure out what is even needed
 while (TRUE) {
        // clear the socket set
        FD_ZERO(&amp; readfds);

        // add master socket to set
        FD_SET(master_socket, &amp; readfds);
        max_sd = master_socket;

        // add child sockets to set
        for (i = 0; i & lt; max_clients; i++) {
            // socket descriptor
            sd = client_socket[i];

            // if valid socket descriptor then add to read
            // list
            if (sd & gt; 0)
                FD_SET(sd, &amp; readfds);

            // highest file descriptor number, need it for
            // the select function
            if (sd & gt; max_sd)
                max_sd = sd;
        }

        // wait for an activity on one of the sockets ,
        // timeout is NULL , so wait indefinitely
        activity = select(max_sd + 1, &amp;
                          readfds, NULL, NULL, NULL);

        if ((activity & lt; 0) & amp; &amp;
            (errno != EINTR)) {
            printf(&quot; select error & quot;);
        }

        // If something happened on the master socket ,
        // then its an incoming connection
        if (FD_ISSET(master_socket, &amp; readfds)) {
            if ((new_socket = accept(
                     master_socket, (struct sockaddr*)&amp;
                     address, (socklen_t*)&amp; addrlen))
                    & lt;
                0) {
                perror(&quot; accept & quot;);
                exit(EXIT_FAILURE);
            }

            // inform user of socket number - used in send
            // and receive commands
            printf(&quot;
                   New connection, socket fd is % d, ip is
                   :
                   % s, port
                   :
                   % d 
                  \n & quot;, new_socket,
                             inet_ntoa(address.sin_addr),
                             ntohs(address.sin_port));

            // send new connection greeting message
            if (send(new_socket, message, strlen(message),
                     0)
                != strlen(message)) {
                perror(&quot; send & quot;);
            }

            puts(&quot;
                 Welcome message sent successfully & quot;);

            // add new socket to array of sockets
            for (i = 0; i & lt; max_clients; i++) {
                // if position is empty
                if (client_socket[i] == 0) {
                    client_socket[i] = new_socket;
                    printf(&quot;
                           Adding to list of sockets as
                               % d\n
                           & quot;
                           , i);

                    break;
                }
            }
        }
 
 
 
 
 */
	/* wait for connection, then receive and print text */
	while(1) 
	{

		if ((command_socket = accept(s, (struct sockaddr *)&sin, &len)) < 0) 
		{
			perror("simplex-talk: accept");
			exit(1);
		}	

		
  		while (len = recv(command_socket, command_buf, sizeof(command_buf), 0))
		{
			if (strcmp(command_buf, "EXIT") == 0)
			{
				printf("Client has ended session...disconnecting...\n");
				break;
			}
			
			int counter = 0;
			char* token = strtok(command_buf, " ");
			char* command;
			char* fileName;

			while(token != NULL && counter < 2)
			{
				if (counter == 0)
				{
					command = token;
				}
				else
				{
					fileName = token;
				}

				token = strtok(NULL, " ");
				counter++;
			}

			if (strcmp(command, "get") == 0)
			{
				fp = fopen(fileName, "r");

				while(1)
				{
					if((file_socket = accept(s, (struct sockaddr *)&sin, &len)) < 0)
					{
						perror("simplex-talk: accept");
						exit(1);
					}
					
					while(fgets(file_buf, MAX_LINE, fp))
					{
						printf("%s", file_buf);
						send(file_socket, file_buf, sizeof(file_buf), 0);
						bzero(file_buf, MAX_LINE);
					}

					close(file_socket);
					break;
				}

				fclose(fp);

			}
			else if (strcmp(command, "put") == 0)
			{
				fp = fopen(fileName, "w");
					
				while(1)
				{
					if ((file_socket = accept(s, (struct sockaddr*)&sin, &len)) < 0)
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
				
				fclose(fp);
			}
			else
			{
				printf("Unknown command\n");
			}
		
		}
  	
		
		close(command_socket);
	}
}
