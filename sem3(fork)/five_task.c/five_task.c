#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/wait.h>

enum consts {
    MAX_COMMAND_SIZE = 10
};

size_t file_size(FILE* file) {
	fseek(file, 0, SEEK_END);
	size_t ret = ftell(file);
	fseek(file, 0, SEEK_SET);
	return ret;
}

size_t split(char* input, char* delims, char** split_array) {
    char* lexem = strtok(input, delims);
    size_t i = 0;
    for (i = 0; lexem != NULL; i++) {
        split_array[i] = lexem;
        lexem = strtok(NULL, delims);
    }
    return i;
}

struct command_t {
    size_t delay;
    char** command_and_args;
    size_t caa_number;
};

int commands_cmp(const void* lhs, const void* rhs) {
    return ((struct command_t*)lhs)->delay - ((struct command_t*)rhs)->delay;
}

#ifdef __DEBUG_FIVE_TASK__
void print_command(struct command_t command) {
    printf("delay: %lu \n", command.delay);
    for(size_t i = 0; i < command.caa_number; ++i) {
        printf("%s ", command.command_and_args[i]);
    }
}
#endif

void parse_command_lines(char** command_lines, size_t command_count ,struct command_t* commands) {
    for(size_t i = 0; i < command_count; ++i) {
        commands[i].delay = atoi(command_lines[i]);

		commands[i].command_and_args = (char**)calloc(MAX_COMMAND_SIZE + 1, sizeof(char*));
        size_t k = 0;
        while(isdigit(command_lines[i][k]) || isspace(command_lines[i][k])) {++k;}

		commands[i].caa_number = split(command_lines[i] + k, " ", commands[i].command_and_args);
    }
} 

int main(int argc, char** argv) {
    if(argc != 2) {
        fprintf(stderr, "Expected only input file\n");
        exit(1);
    }

    FILE* input_file = fopen(argv[1], "r");
    if(input_file == NULL) {
        fprintf(stderr, "Can't open file\n");
        exit(1);
    }

    size_t input_file_size = file_size(input_file);

    char* input_buffer = (char*)calloc(input_file_size + 1, sizeof(char));
    if(input_buffer == NULL) {
        fprintf(stderr, "Alloc error\n");
        exit(1);
    }
    size_t tmp = fread(input_buffer, sizeof(char), input_file_size, input_file);
    if(tmp != input_file_size) {
        fprintf(stderr, "Can't read file\n");
        exit(1);
    }

    
    char** command_lines = (char**)calloc(input_file_size, sizeof(char*));
    if(command_lines == NULL) {
        fprintf(stderr, "Alloc error\n");
        exit(1);
    }

    size_t commands_count = split(input_buffer, "\n", command_lines);
    command_lines = (char**)realloc(command_lines, commands_count * sizeof(char*));

    struct command_t* commands = (struct command_t*)calloc(commands_count, sizeof(struct command_t));
    parse_command_lines(command_lines, commands_count, commands);

    qsort(commands, commands_count, sizeof(struct command_t), commands_cmp);

    for (size_t i = 1; i < commands_count; i++)
		commands[i].delay -= commands[i - 1].delay;

    for(size_t i = 0; i < commands_count; ++i) {
        sleep(commands[i].delay);
        pid_t pid = fork();
		if (pid == 0) {
			execvp(commands[i].command_and_args[0], commands[i].command_and_args);
				
			printf("Incorrect execvp\n");
			exit(1);
		}
		else {
			int status = 0;
			wait(&status);
		}
    }

    for(size_t i = 0; i < commands_count; ++i) {
        free(commands[i].command_and_args);
    }
    free(input_buffer);
    free(command_lines);
    free(commands);
    fclose(input_file);
}