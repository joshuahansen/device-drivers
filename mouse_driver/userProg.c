#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

#define DEVICE "/sys/devices/platform/mouse/coordinates"

void clear_buffer();

int main(int argc, char *argv[])
{
	int sim_fd;
	int x, y;
	char buffer[10];
	char user_selection;

	sim_fd = open(DEVICE, O_RDWR);
	if(sim_fd < 0)
	{
		printf("Could not open Virtual Mouse file\n");
		exit(-1);
	}

	do
	{
		char *op_1 = "u = User Enter Coordinates (x,y)";
		char *op_2 = "r = Random Coordinates (x,y)";
		char *op_3 = "e = exit user program";
		printf("%s\n%s\n%s\nENTER SELECTION: ", op_1, op_2, op_3);
		scanf("%c", &user_selection);
		clear_buffer();

		switch(user_selection)
		{
			case 'u':
				printf("Enter Coordinates (x,y): ");
				
				scanf("%d %d", &x, &y);
				clear_buffer();
				
				sprintf(buffer, "%d %d %d", x, y, 0);

				write(sim_fd, buffer, strlen(buffer));
				printf("X = %d\nY = %d\n", x, y);
	
				fsync(sim_fd);
				break;
			case 'r':
				x = random()%20;
				y = random()%20;

				sprintf(buffer, "%d %d %d", x, y, 0);

				write(sim_fd, buffer, strlen(buffer));
				printf("X = %d\nY = %d\n", x, y);
	
				fsync(sim_fd);
				break;
			case 'e':
				printf("Close user program\n");
				break;
			defualt:
				printf("Please select one of the options\n");
		}
	}while(user_selection != 'e');

	close(sim_fd);
}

void clear_buffer()
{
	while(getchar() != '\n')
	{
		continue;
	}
}
