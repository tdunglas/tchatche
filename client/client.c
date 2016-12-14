#include "client.h"

// Colors for text
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

char* network_path "../network/network.pipe";
char* my_pipe = "";
int id;
int* pipes = (int*) malloc (sizeof(int)*2);

void run()
{
  connexion();
}


void connexion()
{

	mode_t t = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;

	int mkfifo1 = 0;
	if(access (network_path, F_OK) == -1){
	  printf("le serveur n'existe pas");
	  exit(-1);
	}

	pipes[0] = open(network_path, O_WRONLY);
	pipes[1] = open(my_pipe, O_RDONLY);


	pipe(pipes[1]);
	size_t size = 16;
	char* buff
	do
	{
		printf("Veuillez choisir un pseudo : ");
		if((buff = get_message(size)) == NULL) // pas d'input de l'utilisateur
			continue;

	}while(connexion_approval(buff) == -1)

}

//FIXME : envoyer le msg encodé + récup id + chemin de tube perso et l'ouvrir en lecture.
int connexion_approval(char* pseudo)
{
        write(pipes[0], pseudo, strlen(pseudo) * sizeof(char));
	char buff[4];

	read(pipes[0], ?, ?);

	if(strcmp(buff, "OKOK") == 0)
	{
	  id = ?
	  return 1
	}

	return -1;
}

void deconnexion()
{
	char* msg = encode_deconnexion(id);
	write(pipes[0], msg, strlen(msg) + sizeof(char));

	close(pipes[0]);
	close(pipes[1]);
	free(pipes);
}

void read_message()
{
  int msg_length;
  char* tmp = (char*) malloc (1);
  char* msg;

  char c;

  while(c >= 48 && c <= 57) //number for ascii
  {
    read(pipes[1], &c, strlen(char));
  }
}

void send_message(const char* msg)
{
  char* code = encode(msg);
  write(pipes[0], code, strlen(code));
}

void print_message(const char* msg)
{
  printf("%s", parsing(msg));
}

// Coloriage du texte, gestion des pseudo pour les messages privé, etc.
char* parsing(char* msg)
{
  return NULL;
}
