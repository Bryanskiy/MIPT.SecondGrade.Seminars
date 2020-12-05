#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

enum {
     max_message_len = 1000
}; 

struct channel_t {
    int fifo0;
    int fifo1;
    char fifo0_name[2]; 
    char fifo1_name[2];
};

void create_connection(struct channel_t* channel) {
    channel->fifo0_name[0] = '0';
    channel->fifo1_name[0] = '1';

    channel->fifo0_name[1] = '\0';
    channel->fifo1_name[1] = '\0';

    (void)umask(0);
	channel->fifo0 = mknod(channel->fifo0_name, S_IFIFO | 0666, 0);
    channel->fifo1 = mknod(channel->fifo1_name, S_IFIFO | 0666, 0);

	if (channel->fifo0 < 0 && errno != EEXIST) {
		fprintf(stderr, "Cant create %s FIFO\n", channel->fifo0_name);
		exit(-1);
	}

	if (channel->fifo1 < 0 && errno != EEXIST) {
		fprintf(stderr, "Cant create %s FIFO\n", channel->fifo1_name);
		exit(-1);
	}
}

void read_msg(struct channel_t channel, int user_id)
{
	char* fifo_read;

	if (user_id == 0) {
		fifo_read = channel.fifo1_name;
	}
	else {
		fifo_read = channel.fifo0_name;
	}

	int fd;
	if ((fd = open(fifo_read, O_RDONLY)) < 0) {
		fprintf(stderr, "Can't open FIFO for reading\n");
		exit(-1);
	}

	char message[max_message_len + 1] = {};

	while(1) {
		int size = read(fd, message, max_message_len);
		if (size <= 0) {
			fprintf(stderr, "Interlocutor leaved\n");
			close(fd);
			exit(0);
		}
		printf("[interlocutor]: %s", message);
		memset(message, '\0', size);
	}
}

void write_msg(struct channel_t channel, int user_id) {
	char* fifo_write;

	if (user_id == 0) {
		fifo_write = channel.fifo0_name;
	}
	else {
		fifo_write = channel.fifo1_name;
	}

    int fd;
	    if ((fd = open(fifo_write, O_WRONLY)) < 0) {
		    printf("Can't open FIFO for writting\n");
		    exit(-1);
    	}

		char message[max_message_len + 1] = {};

	    while(1) {
		    fgets(message, max_message_len, stdin);
		    write(fd, message, max_message_len);
		    memset(message, '\0', max_message_len);
	    }
}

int main(int argc, char** argv) {
    if(argc != 2) {
        fprintf(stderr, "Expected user id\n");
        exit(-1);
    }

    struct channel_t channel;
    create_connection(&channel);

    int user_id = atoi(argv[1]);

	pid_t pid = fork();

	if (pid < 0) {
		fprintf(stderr, "Can't fork\n");
		exit(-1);
	}
	else if (pid == 0) {
        write_msg(channel, user_id);
    } else {
		read_msg(channel, user_id);
    }    

    close(channel.fifo0);
    close(channel.fifo1);
	return 0;
}