#include "client.h"

// Colors for text
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

int pipes[2][2]; // 1 - client read / write | 2 - serveur read / write
int id;

void run()
{
	while(connexion());
}


char* get_message(size_t size)
{
	char* string = (char*) malloc (size);

	if(fgets(string, size, stdin) == NULL)
	{
		printf("pas de saisie clavier");
		return NULL;
	}
	string[strlen(string) - 1] = '0';
	return string;
}

int connexion()
{
	pipe(pipes[1]);
	size_t size = 16;
	char* buff
	do
	{
		printf("Veuillez choisir un pseudo : ");
		if((buff = get_message(size)) == NULL) // pas d'input de l'utilisateur
			continue;

	}while(verification_validation(buff) == -1)
}

int verification_validation(char* pseudo)
{
	//FIXME : envoyer le message encodé
	write(pipes[2][2], pseudo, strlen(pesudo)*sizeof(char)); 

	char buff[4];

	read(pipes[1][1], buff, sizeof(buff));

	if(strcmp(buff, "OKOK") == 0){
		//TODO :  récupération de l'ID
		return 1
	}

	return -1;
}

void deconnexion()
{
	char* msg = encode_deconnexion(id);
	write(pipes[2][2], msg, strlen(msg) + sizeof(char));

	int i, j;
	for(i = 0 ; i < 2 ; i++)
		for(j = 0 ; j < 2 ; j++)
			close(pipes[i][j])
}

void recevoir_message();
void envoyer_message()
{

}
void afficher_message(char* msg)
{

}

void coloriage(char* msg)
{
	
}