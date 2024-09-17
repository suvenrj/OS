#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#define MAX_INPUT_SIZE 1024
#define MAX_TOKEN_SIZE 64
#define MAX_NUM_TOKENS 64

char **tokenize(char *line) {
    char **tokens = (char **)malloc(MAX_NUM_TOKENS * sizeof(char *));
    char token[MAX_TOKEN_SIZE];
    int i, tokenIndex = 0, tokenNo = 0;

    for (i = 0; i < strlen(line); i++) {
        char readChar = line[i];

        if (readChar == ' ' || readChar == '\n' || readChar == '\t') {
            if (tokenIndex != 0) {
                token[tokenIndex] = '\0';
                tokens[tokenNo] = (char *)malloc((tokenIndex + 1) * sizeof(char));
                strcpy(tokens[tokenNo++], token);
                tokenIndex = 0;
            }
        } else {
            token[tokenIndex++] = readChar;
        }
    }

    if (tokenIndex != 0) {
        token[tokenIndex] = '\0';
        tokens[tokenNo] = (char *)malloc((tokenIndex + 1) * sizeof(char));
        strcpy(tokens[tokenNo++], token);
    }

    tokens[tokenNo] = NULL;
    return tokens;
}

int main() {
    char line[MAX_INPUT_SIZE];
    char **tokens;
    int i;

    while (1) {
        printf("Hello>");
        bzero(line, MAX_INPUT_SIZE);
        fgets(line, MAX_INPUT_SIZE, stdin);
        line[strcspn(line, "\n")] = '\0';

        printf("Got command %s\n", line);
        tokens = tokenize(line);

        int command_counter = 0;
        char *command[MAX_NUM_TOKENS];
        int file_redirection = 0;
        int file_name_counter = 0;
        char *file_name;
        char *valid_executable_commands[5] = {"cat", "ls", "echo", "sleep", "kill"};
        int valid_executable_command_flag = 0;

        for (i = 0; tokens[i] != NULL; i++) {
            if (strcmp(tokens[i], ";;") != 0 && strcmp(tokens[i], ">") != 0 && !file_redirection) {
                command[command_counter]= tokens[i];
                command_counter++;
            } else if (strcmp(tokens[i], ">") == 0) {
                file_redirection = 1;
            } else if (strcmp(tokens[i], ";;") == 0) {
                char *argv[command_counter+1];
                for (int j = 0; j < command_counter; j++) {
                    argv[j]= command[j];
                }
                argv[command_counter] = NULL;

                for (int k = 0; k < 5; k++) {
                    if (strcmp(command[0], valid_executable_commands[k]) == 0) {
                        valid_executable_command_flag = 1;
                        break;
                    }
                }
                if (valid_executable_command_flag) {
                    const char *base_path = "/bin/";
                    const char *exec_name = command[0];
                    size_t path_length = strlen(base_path) + strlen(exec_name) + 1;
                    char *full_path = malloc(path_length);

                    if (full_path == NULL) {
                        perror("malloc failed");
                        return 1;
                    }

                    snprintf(full_path, path_length, "%s%s", base_path, exec_name);
                    printf("%s\n", full_path);
                    pid_t pid = fork();
                    if (pid == 0) {
                        if (file_redirection){
                            int fd = open(file_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
                            if (fd < 0) {
                                perror("open");
                                exit(1);
                            }
                            // Duplicate the file descriptor, replacing stdout (file descriptor 1)
                            if (dup2(fd, STDOUT_FILENO) < 0) {
                                perror("dup2");
                                exit(1);
                            }
                            // Close the file descriptor, it is no longer needed
                            close(fd);
                        }
                        execv(full_path, argv);
                        perror("execv failed");
                        exit(1);
                    } else if (pid > 0) {
                        wait(NULL);
                        command_counter = 0;
                        file_redirection = 0;
                        file_name_counter = 0;
                    } else {
                        perror("fork failed");
                    }

                    free(full_path);
                } else if (strcmp(command[0], "cd") == 0) {
                    if (command_counter != 2) {
                        printf("Incorrect params given to cd.\n");
                    } else {
                        if (chdir(command[1]) == -1) {
                            perror("chdir failed");
                        }
                    }
                } else {
                    printf("Provided command is not supported.\n");
                }
            } else {
                if (file_name_counter) {
                    printf("Error: More than 1 file for redirection\n");
                    break;
                }
                file_name = malloc(MAX_TOKEN_SIZE * sizeof(char));
                if (file_name == NULL) {
                    perror("malloc failed");
                    return 1;
                }
                file_name = tokens[i];
                printf("%s\n", file_name);
                file_name_counter++;
            }
        }

        // Freeing the allocated memory
        for (i = 0; tokens[i] != NULL; i++) {
            free(tokens[i]);
        }
        free(tokens);
    }

    return 0;
}