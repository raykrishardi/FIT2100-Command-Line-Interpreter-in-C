#ifndef SHELL_H
#define SHELL_H

// Constants
#define BUFFER_SIZE 100 // Represent the size of the buffer
#define FILE_BUFFER_SIZE 1024 // Represent the size of the buffer for file I/O

// Libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <dirent.h>
#include <signal.h>

// Functions
void startShell(); // Start the shell (CLI)
void setUserManualPath(); // Get absolute path to the user manual file
void initialiseCommandArray(); // Initialise the command array with NULL
void promptAndGetLine(char *line); // Prompt the user for input and get the user input
void removeTrailingNewlineChar(char *line); // Remove trailing newline character from the user input
int validCommandFromLine(char *line); // Check for valid user input (input must start with a lower case alphabet letter)
void interpretCommandFromLine(char *line, char *command[]); // Get command line from user input and execute command
void getCommandFromLine(char *line, char *command[]); // Get command line from user input
void checkAndExecuteCommand(char *command[]); // Check and execute the appropriate command
int emptyCommandArgument(char *argument); // Check for empty command argument
void executeClearCommand(); // Execute clear command
void executeCdCommand(char *argument1); // Execute cd command
void executeLsCommand(char *argument1); // Execute ls command
void executeEchoCommand(char *argument1); // Execute echo command
void executeHelpCommand(); // Execute help command
void spawnAndExecuteChildProcess(char *path, char *params[], int runInBackground); // Spawn a child process and execute the appropriate command in foreground/background
void executeCreateCommand(char *argument1); // Execute create command
void executeCopyCommand(char *argument1, char *argument2); // Execute copy command
void executeSearchCommand(char *argument1, char *argument2); // Execute search command
void executeRunCommand(char *argument1); // Execute run command
void executeHaltCommand(char *argument1); // Execute halt command
void freeCommandArray(); // Deallocate memory allocation pointed to by element(s) in command array
void freeChildNameArray(); // Deallocate memory allocation pointed to by element(s) in childName array

#endif