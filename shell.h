#define MAXLINE 1048576 /* kernel dependent */
#define MAXARGS 128
#define HISTORY_FILE ".seashell_history.txt" /* hidden file */

// Move all the text to the top of the terminal and sets the cursor position to (1,1)
#define clear() printf("\033[H\033[J");

// Shell greeting 
void init_shell();

// Evaluate a command line
void eval (char *cmdline);

// Parse the command line and build the argv array
int parseline (char *charbuf, char **argv);

// Insert
int builtin_command(char **argv);