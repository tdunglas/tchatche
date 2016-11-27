/* ___________________________________________
		  PROTOCOLE DE COMMUNICATION
   ___________________________________________

Format du protocole :
- On represente la taille du messages avec 4 ou 8 caracteres
- Le type avec 4 caracteres
- Le reste des donnees
  + Soit une taille de chaine suivie de la chaine concernee
  + Un entier (4 ou 8 caracteres)
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

const char* messagetype_to_string(message_type type) {
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

// 4 char : length <= 9999
// 8 char : length <= 9999 9999
protocol makeMessageHeader(int length, message_type type) {
	int length_nbchar = 0;
	if (length <= 9999)
		length_nbchar = 4;
	else if (length <= 99999999)
		length_nbchar = 8;
	else {
		perror("makeMessageHeader : length to large to encode");
		exit(0);
	}
	char* buffer = (char*)calloc(sizeof(char), length_nbchar+4);
	if (length_nbchar == 4)
		buffer = strcat(buffer, encodeNumber(length, 4));
	else
		buffer = strcat(buffer, encodeNumber(length, 8));
	buffer = strcat(buffer, messagetype_to_string(type));
	return buffer;
}

int main() {
	int length = 15;
	protocol header = makeMessageHeader(length, PRVT_t);
	int i;
	for (i = 0; i < length; i++) {
		printf("[%c]", header[i]);
	}
	return 0;
}