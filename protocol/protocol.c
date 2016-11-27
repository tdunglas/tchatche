/* ___________________________________________
		  PROTOCOLE DE COMMUNICATION
   ___________________________________________

Format du protocole :
- On represente la taille du messages avec 4 ou 8 caracteres
- Le type avec 4 caracteres
- Le reste des donnees
  + Soit une taille de chaine suivie de la chaine concernee
  + Un entier (4 ou 8 caracteres)

Le header correspond a la taille + le type
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef char* protocol;

typedef enum message_type {
	BCST_t,
	PRVT_t,
	BADD_t,
	OKOK_t,
	BYEE_t,
	HELO_t,
	LIST_t,
	SHUT_t,
	DEBG_t,
	FILE_t
} message_type;

/* ---------------------------------------------
			 FONCTIONS D'ENCODAGE
   --------------------------------------------- */

const char* encodeType(message_type type) {
	switch (type) {
		case BCST_t: return "BCST"; break;
		case PRVT_t: return "PRVT"; break;
		case BADD_t: return "BADD"; break;
		case OKOK_t: return "OKOK"; break;
		case BYEE_t: return "BYEE"; break;
		case HELO_t: return "HELO"; break;
		case LIST_t: return "LIST"; break;
		case SHUT_t: return "SHUT"; break;
		case DEBG_t: return "DEBG"; break;
		case FILE_t: return "FILE"; break;
		default:
			perror("messagetype_to_string : No encoding for the given type");
			exit(0);
			break;
	}
}

// (length = 4) \/ (length = 8)
char* encodeNumber(int n, int length) {
	char* buffer = (char*)malloc(sizeof(char)*length);
	int i;
	int mod_factor = 10, div_factor = 1;
	for(i = length-1; i >= 0; i--) {
		buffer[i] = (char)('0'+((int)(n%mod_factor/div_factor)));
		mod_factor *= 10;
		div_factor *= 10;
	}
	return buffer;
}

int calculateMessageLength(int length_postheader) {
	int length_header = 4+4;
	int assumption = length_header+length_postheader;
	if (assumption > 9999) {
		if (assumption <= 99999999) {
			return 8+4+length_postheader;
		}
		else {
			perror("calculateMessageLength : length to large to encode");
			exit(0);
		}
	}
	return assumption;
}

protocol makeMessageHeader(int length, message_type type) {
	int length_nbchar = 0;
	char* numberEncoding;
	if (length <= 9999)
		length_nbchar = 4;
	else if (length <= 99999999)
		length_nbchar = 8;
	else {
		perror("makeMessageHeader : length to large to encode");
		exit(0);
	}
	char* buffer = (char*)calloc(sizeof(char), length_nbchar+4);
	if (length_nbchar == 4) {
		numberEncoding = encodeNumber(length, 4);
		buffer = strcat(buffer, numberEncoding);
	}
	else {
		numberEncoding = encodeNumber(length, 8);
		buffer = strcat(buffer, numberEncoding);
	}
	buffer = strcat(buffer, encodeType(type));
	free(numberEncoding);
	return buffer;
}

// ------------------- Connexion -------------------
protocol encodeConnexion(char* pseudo, char* tube) {
	int size_pseudo = strlen(pseudo);
	int size_tube = strlen(tube);
	int length_postheader = size_pseudo+size_tube;
	int total_length = calculateMessageLength(length_postheader);
	char* buffer = (char*)calloc(sizeof(char), total_length);
	char* header = makeMessageHeader(total_length, HELO_t);
	buffer = strcat(buffer, header);
	buffer = strcat(buffer, pseudo);
	buffer = strcat(buffer, tube);
	return buffer;
}

int main() {
	protocol connexion = encodeConnexion("engboris", "s1");
	int i;
	for (i = 0; i < calculateMessageLength(10); i++) {
		printf("[%c]", connexion[i]);
	}
	return 0;
}