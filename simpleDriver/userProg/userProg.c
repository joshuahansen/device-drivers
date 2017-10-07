#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define DEVICE "/dev/s3589185Device"
#define BUFFER 100

void clear_buffer();

int main()
{
	int bytes_written, bytes_read, device_file;
	char user_selection, write_buffer[BUFFER], read_buffer[BUFFER];

	device_file = open(DEVICE, O_RDWR);

	if(device_file == -1)
	{
		printf("File %s does not exist or is locked by another program\n", DEVICE);
		exit(-1);
	}
	do
	{
		char *op_1 = "r = read from device";
		char *op_2 = "w = write to device";
		char *op_3 = "e = exit user program";
		printf("%s\n%s\n%s\nENTER SELECTION: ", op_1, op_2, op_3);
		scanf("%c", &user_selection);
		clear_buffer();

		switch(user_selection)
		{
			case 'r':
				bytes_read = read(device_file, read_buffer, sizeof(read_buffer));
				if(bytes_read < 0)
				{
					printf("An Error occurred when reading the file\n");
				}
				printf("Device: %s\n", read_buffer);
				break;
			case 'w':
				printf("Enter Data: ");
				scanf("%[^\n]s", write_buffer);
				clear_buffer();
				bytes_written = write(device_file, write_buffer, sizeof(write_buffer));
				if(bytes_written < 0)
				{
					printf("An Error occurred when writing\n");
				}
				break;
			case 'e':
				printf("Close user program\n");
				break;
			defualt:
				printf("Please select one of the options\n");
		}
	}while(user_selection != 'e');
	
	if(close(device_file) < 0)
	{
		printf("File %s was not closed\n", DEVICE);
	}
	return 0;
}

void clear_buffer()
{
	while(getchar() != '\n')
	{
		continue;
	}
}


