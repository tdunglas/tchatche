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
#include <ctype.h>
#include <math.h>

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
			FONCTIONS DE DECODAGE
   --------------------------------------------- */

int char2int(char c) {
	return (int)(c-'0');
}

// (1) On suppose qu'on ne peut pas avoir de chiffres dans le type
// (2) On suppose que le type ne contient que des lettres majuscules
int decodeLength(protocol message) {
	int result = 0;
	int size_of_length = 0;
	int i = 0;
	while (isdigit(message[i])) {
		i++;
		size_of_length++;
	}
	int left_factor = (int)(pow(10.0, (double)(size_of_length-1)));
	for (i = 0; i < size_of_length; i++) {
		result += (char2int(message[i]))*left_factor;
		left_factor /= 10;
	}
	return result;
}

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

char* encodeNumber(int n, int length) {
	char* buffer = (char*)malloc(sizeof(char)*length);
	int i;
	int mod_factor = 10, div_factor = 1;
	for (i = length-1; i >= 0; i--) {
		buffer[i] = (char)('0'+((int)(n%mod_factor/div_factor)));
		mod_factor *= 10;
		div_factor *= 10;
	}
	return buffer;
}

void addLengthValue(protocol message, int value) {
	int messageLength = decodeLength(message);
	int newLength = messageLength+value;
	if (newLength <= 9999) {
		char* lengthEncoding = encodeNumber(newLength, 4);
		int i;
		for (i = 0; i < 4; i++)
			message[i] = lengthEncoding[i]; 
		free(lengthEncoding);
	}
	else if (newLength <= 99999999) {

	}
	else {
		perror("addLengthValue : length to large to encode");
		exit(0);
	}
}

protocol makeMessageHeader(int length, message_type type) {
	int length_nbchar = 0;
	char* numberEncoding;
	if (length <= 9999)
		length_nbchar = 4;
	else if (length <= 99999999)
		length_nbchar = 8;
	else {
		perror("initMessageHeader : length to large to encode");
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

protocol initMessageHeader(message_type type) {
	return makeMessageHeader(8, type);
}

char* encodeString(char* string, int length) {
	char* buffer;
	char* numberEncoding;
	if (length <= 9999) {
		buffer = (char*)calloc(sizeof(char), length+4+1);
		numberEncoding = encodeNumber(length, 4);
		buffer[length+4] = '\0';
	}
	else if (length <= 99999999) {
		buffer = (char*)calloc(sizeof(char), length+8+1);
		numberEncoding = encodeNumber(length, 8);
		buffer[length+8] = '\0';
	}
	else {
		perror("encodeString : length to large to encode");
		exit(0);
	}
	buffer = strcat(buffer, numberEncoding);
	buffer = strcat(buffer, string);
	free(numberEncoding);
	return buffer;
}

protocol addMessageString(protocol message, char* string) {
	char* stringEncoding = encodeString(string, strlen(string));
	int stringEncodingLength = strlen(stringEncoding);
	addLengthValue(message, stringEncodingLength);
	char* buffer = (char*)calloc(sizeof(char), decodeLength(message)+stringEncodingLength);
	buffer = strcat(buffer, message);
	buffer = strcat(buffer, stringEncoding);
	free(stringEncoding);
	free(message);
	return buffer;
}

protocol addMessageNumber(protocol message, int number) {
	int lengthEncoding = 0;

	if (number <= 9999)
		lengthEncoding = 4;
	else if (number <= 99999999)
		lengthEncoding = 8;
	else {
		perror("addMessageNumber : length to large to encode");
		exit(0);
	}
	addLengthValue(message, lengthEncoding);
	char* numberEncoding = encodeNumber(number, lengthEncoding);
	char* buffer = (char*)calloc(sizeof(char), decodeLength(message)+lengthEncoding);
	buffer = strcat(buffer, message);
	buffer = strcat(buffer, numberEncoding);

	free(numberEncoding);
	free(message);
	return buffer;
}

// ------------------- Connexion -------------------
protocol encodeConnexion(char* pseudo, char* tube) {
	char* header = initMessageHeader(HELO_t);
	header = addMessageString(header, pseudo);
	header = addMessageString(header, tube);
	return header;
}

protocol encodeConnexionConfirmation(int id) {
	char* header = initMessageHeader(OKOK_t);
	header = addMessageNumber(header, id);
	return header;
}

int main() {
	int i;
	protocol connexion = encodeConnexion("engboris", "s1");
	for (i = 0; i < decodeLength(connexion); i++) {
		printf("%c", connexion[i]);
	}
	printf("\n");
	protocol confirmation = encodeConnexionConfirmation(15);
	for (i = 0; i < decodeLength(confirmation); i++) {
		printf("%c", confirmation[i]);
	}
	return 0;
}
