#include "shell.h"

// Global Variables
char userManualPath[BUFFER_SIZE]; // Represent the absolute path to the user manual file
char line[BUFFER_SIZE]; // Represent the user input
char *command[BUFFER_SIZE]; // Represent the command line
int childCounter = 0; // Represent the number of child processes running in background
pid_t childPid[BUFFER_SIZE]; // Represent the child process IDs running in background
char *childName[BUFFER_SIZE]; // Represent the child process names (i.e. program name executed by the child process) running in background

// Function that starts the shell (CLI)
void startShell() {
  setUserManualPath(); // Get absolute path to the user manual file
  initialiseCommandArray(); // Initialise the command array with NULL

  // Keep iterating until the user enters the "quit" command
  while (strcmp(line, "quit") != 0) {
    promptAndGetLine(line); // Prompt the user for input and get the user input
    removeTrailingNewlineChar(line); // Remove trailing newline character from the user input

    // Check for valid user input (input must start with a lower case alphabet letter)
    // If the user input is valid then get command line from user input and execute command
    if (validCommandFromLine(line))
      interpretCommandFromLine(line, command);

    // Deallocate memory allocation pointed to by element(s) in command array
    freeCommandArray();
  }
  // Deallocate memory allocation pointed to by element(s) in childName array
  freeChildNameArray();
}

// Function that sets the "userManualPath" variable to the absolute path of the user manual file
// 1. Get reference to the current working directory path
// 2. Concatenate the name of the user manual file to the current working directory path
// Therefore, the user manual file can be accessed from any directory
void setUserManualPath() {
  getcwd(userManualPath, BUFFER_SIZE);
  strcat(userManualPath, "/manual.txt");
}

// Function that initialises each element in the command array with NULL
void initialiseCommandArray() {
  for (int i = 0; i < BUFFER_SIZE; i++) {
    command[i] = NULL;
  }
}

// Function that prompts the user for input and get the user input
void promptAndGetLine(char *line) {
  printf("$ ");
  fgets(line, BUFFER_SIZE, stdin);
}

// Function that removes trailing newline character from the user input ("line" variable) as a result of the fgets()
// 1. Replace the newline character with null character if there is a newline character at the end of the "line" variable
void removeTrailingNewlineChar(char *line) {
  size_t lineLength = strlen(line);
  if (line[lineLength - 1] == '\n')
    line[lineLength - 1] = '\0';
}

// Function that checks for valid user input (input must start with a lower case alphabet letter) by using ascii character value
// (a-z -> 97-122)
int validCommandFromLine(char *line) {
  return (line[0] >= 97 && line[0] <= 122);
}

// Function that gets command line from user input and executes the command
void interpretCommandFromLine(char *line, char *command[]) {
  getCommandFromLine(line, command);
  checkAndExecuteCommand(command);
}

// Function that gets command line from user input (assign the command and its argument(s) as element(s) of the command array)
void getCommandFromLine(char *line, char *command[BUFFER_SIZE]) {
  int counter = 0; // Counter to keep track of the number of command and its argument(s) to be assigned as element(s) of the command array
  char tmpLine[BUFFER_SIZE]; // Temporary variable that also holds the user input ("line" variable)
  char *token; // Represent each substring of the user input ("line" variable) delimited/separated by " "

  // Copy the content of the user input ("line" variable) to the temporary variable ("tmpLine" variable)
  // Temporary variable is required because when using strtok(), the content of the given string will get modified
  // Therefore, the temporary variable is required to preserve the original content of the string
  strcpy(tmpLine, line);

  // Get a substring of the user input delimited/separated by " "
  // Keep iterating until there is no more substring to be retrieved from the user input
  token = strtok(tmpLine, " ");
  while (token != NULL) {
    command[counter] = (char *) malloc(BUFFER_SIZE + 1); // Allocate memory in heap to store the substring
    strcpy(command[counter], token); // Copy the content of the substring to the appropriate element in the command array
    counter++; // Increment the counter accordingly
    token = strtok(NULL, " "); // Get the next substring
  }
}

// Function that checks and executes the appropriate command
// 1. Check the given command and execute the appropriate command with the corresponding argument(s)
// command[0] -> command name
// command[1] -> argument1
// command[2] -> argument2
void checkAndExecuteCommand(char *command[BUFFER_SIZE]) {
  if (strcmp(command[0], "clear") == 0)
    executeClearCommand();
  else if (strcmp(command[0], "cd") == 0)
    executeCdCommand(command[1]);
  else if (strcmp(command[0], "ls") == 0)
    executeLsCommand(command[1]);
  else if (strcmp(command[0], "echo") == 0)
    executeEchoCommand(command[1]);
  else if (strcmp(command[0], "help") == 0)
    executeHelpCommand();
  else if (strcmp(command[0], "create") == 0)
    executeCreateCommand(command[1]);
  else if (strcmp(command[0], "copy") == 0)
    executeCopyCommand(command[1], command[2]);
  else if (strcmp(command[0], "search") == 0)
    executeSearchCommand(command[1], command[2]);
  else if (strcmp(command[0], "run") == 0)
    executeRunCommand(command[1]);
  else if (strcmp(command[0], "halt") == 0)
    executeHaltCommand(command[1]);
}

// Function that checks for empty command argument
// 1. Check whether the given command argument is NULL or has a length of 0 character
int emptyCommandArgument(char *argument) {
  return (argument == NULL || strlen(argument) == 0);
}

// Function that executes the "clear" command
// 1. Position the cursor to point (1,1) and clear the screen
void executeClearCommand() {
  printf("\e[1;1H\e[2J");
}

// Function that executes the "cd" command
void executeCdCommand(char *argument1) {
  int status; // Represent the chdir() return value
  char newPWD[BUFFER_SIZE]; // Represent the new "PWD" environment variable

  // Check whether argument1 is empty or not
  // If argument1 is empty then change directory to the directory given by the "HOME" environment variable
  // Else, change directory to the given directory (argument1) and print the appropriate error message if the given directory does not exist
  if (emptyCommandArgument(argument1)) {
    chdir(getenv("HOME"));
  }
  else {
    status = chdir(argument1);

    if (status != 0)
      printf("cd: %s: No such directory\n", argument1);
  }

  // Get reference to the current working directory path
  getcwd(newPWD, BUFFER_SIZE);

  // Update the "PWD" environment variable
  // 1 -> overwrite
  setenv("PWD", newPWD, 1);
}

// Function that executes the "ls" command
void executeLsCommand(char *argument1) {
  char path[BUFFER_SIZE]; // Represent the directory path

  // Check whether argument1 is empty or not
  // If argument1 is empty then assign the directory path to the current working directory path
  // Else, assign the directory path to the given directory path (argument1)
  if (emptyCommandArgument(argument1)) {
    getcwd(path, BUFFER_SIZE);
  }
  else {
    strcpy(path, argument1);
  }

  // Get reference to the directory by opening the directory using the directory path
  DIR *dir = opendir(path);

  // Check whether the directory can be opened or not
  // If the directory can be opened then print each file's name (excluding hidden file) and close the directory
  // Else, print the appropriate error message
  if (dir != NULL) {
    struct dirent *file; // Represent each file in the directory

    // Keep iterating until there is no more file to be retrieved from the directory
    while((file = readdir(dir)) != NULL)
    {
      char *fileName = file->d_name; // Get the file name

      // Only print file name that is not a hidden file (i.e. exclude file name that starts with '.')
      if (fileName[0] != '.') {
        printf("%s  ", fileName);
      }
    }
    printf("\n");

    // Close the directory
    // Note: Only close the directory if it can be opened to prevent segmentation fault error
    closedir(dir);
  }
  else {
    printf("ls: %s: No such directory\n", path);
  }
}

// Function that executes the "echo" command
void executeEchoCommand(char *argument1) {
  char *argument = ""; // Represent statement to be printed/echoed

  // Check whether argument1 is empty or not
  // If argument1 is not empty then get a substring of the user input delimited/separated by double quote ("")
  if (!emptyCommandArgument(argument1)) {
    char tmpLine[BUFFER_SIZE]; // Temporary variable that also holds the user input ("line" variable)

    strcpy(tmpLine, line); // Copy the content of the user input ("line" variable) to the temporary variable ("tmpLine" variable)

    // Get the substring of the user input delimited/separated by double quote ("")
    argument = strtok(tmpLine, "\"");
    argument = strtok(NULL, "\"");
  }

  // Print the statement on the output screen
  printf("%s\n", argument);
}

// Function that executes the "help" command
void executeHelpCommand() {
  char *path = "/usr/bin/less"; // Represent the absolute path to the "less" executable file
  char *params[] = {path, userManualPath, 0}; // Represent the parameters to be executed by the child process
  spawnAndExecuteChildProcess(path, params, 0); // Spawn a child process and execute the "less" command with the appropriate parameters in foreground
}

// Function that spawns a child process and execute the appropriate command
void spawnAndExecuteChildProcess(char *path, char *params[], int runInBackground) {
  pid_t pid, status; // Represent the process ID and status of the child process

  // Check whether fork() successfully created a child process or not
  // If fork() failed to create a child process then display the appropriate error message and exit with status code of 1 (indicating error has occured)
  // If fork() successfully created a child process then THE CHILD PROCESS will execute the appropriate command with the corresponding parameters
  //    If the child process does NOT run in background then:
  //        1. The child process exits with status code of 0 (indicating no error has occured)
  // If fork() successfully created a child process then THE PARENT PROCESS will:
  //    If the child process runs in background then:
  //        1. Get a reference to the child process's ID and name
  //        2. The parent process will continue its execution without waiting for the termination of the child process
  //    If the child process does NOT run in background then the parent process will wait for the termination of the child process
  if ((pid = fork()) < 0) {
    perror("fork error");
    exit(1);
  }
  else if (pid == 0) {
    execv(path, params);
    if (!runInBackground)
      exit(0);
  }
  else {
    if (runInBackground) {
      childPid[childCounter] = pid; // Get a reference to the child process's pid
      childName[childCounter] = (char *) malloc(sizeof(path) + 1); // Allocate memory in heap to store the child process's name
      strcpy(childName[childCounter], path); // Get a reference to the child process's name
      printf("Background: \"%s\" (pid->%i)\n", childName[childCounter], childPid[childCounter]);
      childCounter++; // Increment the counter accordingly
      sleep(1); // Sleep for 1 second
      waitpid(-1, &status, WNOHANG); // Specify that the parent process will continue its execution without waiting for the termination of the child process
    }
    else {
      wait(NULL);
    }
  }
}

// Function that executes the "create" command
void executeCreateCommand(char *argument1) {
  int outfile; // Represent the output file descriptor

  // Check whether argument1 is empty or not
  // If argument1 is NOT empty then attempt to create a new file with the name specified in argument1 and with file permission of rw-r--r--
  //    If there is an existing file with the same name then the existing file will be truncated with the new one
  // If argument1 is empty then print the appropriate error message
  if (!emptyCommandArgument(argument1)) {
    if ((outfile = open(argument1, O_CREAT | O_WRONLY | O_TRUNC, 0644)) < 0) {
      perror(argument1); // Print the appropriate error message if an error occurs when attempting to create the file
    }

    close(outfile); // Close the output file
  }
  else {
    printf("Usage: create <file>\n");
  }
}

// Function that executes the "copy" command
void executeCopyCommand(char *argument1, char *argument2) {
  FILE *infile; // Represent the input file
  FILE *outfile; // Represent the output file
  char fileLine[FILE_BUFFER_SIZE]; // Represent each line in the input file

  // Check whether argument1 and argument2 are empty or not
  // If argument1 and argument2 are NOT empty then copy the content of file specified by argument1 to file specified by argument2
  // If argument1 and argument2 are empty then print the appropriate error message
  if (!emptyCommandArgument(argument1) && !emptyCommandArgument(argument2)) {
    char *infileName = argument1; // Represent the input file name
    char *outfileName = argument2; // Represent the output file name

    if ((infile = fopen(infileName, "r")) == NULL) {
      perror(infileName); // Print the appropriate error message if an error occurs when attempting to open for reading the input file
    }
    else if ((outfile = fopen(outfileName, "w")) == NULL) {
      perror(outfileName); // Print the appropriate error message if an error occurs when attempting to open for writing to the output file
    }
    else {
      // Keep iterating until there is no more line to be read from the input file
      while (fgets(fileLine, FILE_BUFFER_SIZE, infile)) {
        fputs(fileLine, outfile); // Put each line in input file to the output file
      }

      // Close the input and output files
      fclose(infile);
      fclose(outfile);
    }
  }
  else {
    printf("Usage: copy <old_file> <new_file>\n");
  }
}

// Function that executes the "search" command
void executeSearchCommand(char *argument1, char *argument2) {
  FILE *infile; // Represent the input file
  char fileLine[FILE_BUFFER_SIZE]; // Represent each line in the input file
  int patternCounter = 0; // Represent the number of occurrences of pattern in input file

  // Check whether argument1 and argument2 are empty or not
  // If argument1 and argument2 are NOT empty then count and print the number of occurrences of pattern in input file
  // If argument1 and argument2 are empty then print the appropriate error message
  if (!emptyCommandArgument(argument1) && !emptyCommandArgument(argument2)) {
    char *pattern = argument1; // Represent the pattern to be searched
    char *fileName = argument2; // Represent the input file name

    if ((infile = fopen(fileName, "r")) == NULL) {
      perror(fileName); // Print the appropriate error message if an error occurs when attempting to open for reading the input file
    }
    else {
      // Keep iterating until there is no more line to be read from the input file
      while (fgets(fileLine, FILE_BUFFER_SIZE, infile)) {
        removeTrailingNewlineChar(fileLine); // Remove trailing newline character from the file line
        const char *tmp = fileLine; // Temporary variable that holds the file line

        // Keep iterating until there is no more occurrences of pattern in file line
        while ((tmp = strstr(tmp, pattern))) {
          patternCounter++; // Increment the counter accordingly
          tmp += strlen(pattern); // Attempt to get the next occurrences of pattern in file line
        }
      }

      // Print the number of occurrences of pattern in input file
      printf("%i\n", patternCounter);
    }

    // Close the input file
    fclose(infile);
  }
  else {
    printf("Usage: search <pattern> <file>\n");
  }
}

// Function that executes the "run" command
void executeRunCommand(char *argument1) {
  int runInBackground = 0; // Represent whether the executable program should run in foreground or background (default -> foreground)

  // Check whether argument1 is empty or not
  // If argument1 is NOT empty then spawn a child process and execute the executable program with the appropriate parameters
  // If argument1 is empty then print the appropriate error message
  if (!emptyCommandArgument(argument1)) {
    char *path = argument1; // Represent the path to the executable program
    char *params[] = {path, 0}; // Represent the parameters to be executed by the child process

    // If the '&' symbol is included at the end of the path then the executable program will run in background
    if (path[strlen(path)-1] == '&') {
      path[strlen(path)-1] = '\0'; // Remove the '&' symbol at the end of the path
      runInBackground = 1; // Indicate that the executable program will run in background
    }

    // Spawn a child process and execute the executable program with the appropriate parameters
    spawnAndExecuteChildProcess(path, params, runInBackground);
  }
  else {
    printf("Usage: run <program>\n");
  }
}

// Function that executes the "halt" command
void executeHaltCommand(char *argument1) {
  // Check whether argument1 is empty or not
  // If argument1 is NOT empty then stop the execution of the executable program named argument1
  // If argument1 is empty then print the appropriate error message
  if (!emptyCommandArgument(argument1)) {
    // Loop through all child processes
    for (int i = 0; i < childCounter; i++) {
      // If a child process has the same name as argument1
      if (strcmp(childName[i], argument1) == 0) {
        kill(childPid[i], 9); // Kill the child process with SIGKILL (9) signal (SIGKILL cannot be ignored)
        printf("Killed: \"%s\" (pid->%i)\n", childName[i], childPid[i]); // Print the appropriate feedback message
        childName[i] = realloc(childName[i], 1); // Reallocate memory in heap to size 1 (empty string with \0) for the child process's name
        strcpy(childName[i], ""); // Copy an empty string to the child process's name
      }
    }
  }
  else {
    printf("Usage: halt <program>\n");
  }
}

// Function that deallocates memory allocation pointed to by element(s) in command array
// 1. Only deallocate memory allocation if the element in command array is not NULL (i.e. contains a memory address)
// 2. Assign NULL to char pointer that has been deallocated
void freeCommandArray() {
  for (int i = 0; i < BUFFER_SIZE; i++) {
    if (command[i] != NULL) {
      free(command[i]);
      command[i] = NULL;
    }
  }
}

// Function that deallocates memory allocation pointed to by element(s) in childName array
void freeChildNameArray() {
  for (int i = 0; i < childCounter; i++) {
    free(childName[i]);
  }
}
