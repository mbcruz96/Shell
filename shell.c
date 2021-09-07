#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
	int size;
	char **items;
} tokenlist;

char *get_input(void);
tokenlist *get_tokens(char *input);
void echo(char *input);
void tilde_expand(char *input);

tokenlist *new_tokenlist(void);
void add_token(tokenlist *tokens, char *item);
void free_tokens(tokenlist *tokens);




int main()
{
	
	while (1) {
		printf("%s@%s:%s>", getenv("USER"), getenv("MACHINE"), getenv("PWD"));

		/* input contains the whole command
		 * tokens contains substrings from input split by spaces
		 */

		char *input = get_input();
		char *echo_resp;

		tokenlist *tokens = get_tokens(input);
		for (int i = 0; i < tokens->size; i++) {

		}
		if (strcmp(tokens->items[0],"echo") == 0){
			echo(tokens->items[1]);
		}
		tilde_expand(tokens->items[0]);


		free(input);
		free_tokens(tokens);
	}

	return 0;
}

void tilde_expand(char *input){
	char *home = getenv("HOME");
	char *position;
	char answer[strlen(home) + strlen(input)];// = home;
	int index;
	int x = strlen(home);

	if(strchr(input,'~') != NULL){
		position  = strchr(input,'~');
		index = (int)(position - input);
		for( int i = 0; i < x; i++){
			answer[i] = home[i];
		}
		for( int i = index; i < strlen(input); i++)
		{
			//answer[variable] = input[i]
			answer[x] = input[i];
			x += 1; 
		}
	}
	echo(answer);
}

void echo(char* input){
	char* answer = input;
	char* home;
	int index;
	int counter = 0;
	const char ch = '$';

	if(strchr(input,ch) == NULL){
		printf(input, "\n");
	}
	else{
		while(counter < strlen(answer))
		{
			answer[counter] = answer[counter+1];
			counter += 1;
		}
		answer = getenv(input);
		if(answer == NULL)
			printf("Error");
		else
			printf(answer, "\n");
	}



}

tokenlist *new_tokenlist(void)
{
	tokenlist *tokens = (tokenlist *) malloc(sizeof(tokenlist));
	tokens->size = 0;
	tokens->items = (char **) malloc(sizeof(char *));
	tokens->items[0] = NULL; /* make NULL terminated */
	return tokens;
}

void add_token(tokenlist *tokens, char *item)
{
	int i = tokens->size;

	tokens->items = (char **) realloc(tokens->items, (i + 2) * sizeof(char *));
	tokens->items[i] = (char *) malloc(strlen(item) + 1);
	tokens->items[i + 1] = NULL;
	strcpy(tokens->items[i], item);

	tokens->size += 1;
}

char *get_input(void)
{
	char *buffer = NULL;
	int bufsize = 0;

	char line[5];
	while (fgets(line, 5, stdin) != NULL) {
		int addby = 0;
		char *newln = strchr(line, '\n');
		if (newln != NULL)
			addby = newln - line;
		else
			addby = 5 - 1;

		buffer = (char *) realloc(buffer, bufsize + addby);
		memcpy(&buffer[bufsize], line, addby);
		bufsize += addby;

		if (newln != NULL)
			break;
	}

	buffer = (char *) realloc(buffer, bufsize + 1);
	buffer[bufsize] = 0;

	return buffer;
}

tokenlist *get_tokens(char *input)
{
	char *buf = (char *) malloc(strlen(input) + 1);
	strcpy(buf, input);

	tokenlist *tokens = new_tokenlist();

	char *tok = strtok(buf, " ");
	while (tok != NULL) {
		add_token(tokens, tok);
		tok = strtok(NULL, " ");
	}

	free(buf);
	return tokens;
}

void free_tokens(tokenlist *tokens)
{
	for (int i = 0; i < tokens->size; i++)
		free(tokens->items[i]);
	free(tokens->items);
	free(tokens);
}