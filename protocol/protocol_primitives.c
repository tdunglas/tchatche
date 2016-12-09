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

Si on oublie de mettre a jour les fonctions lors de la creation
d'un nouveau type, on est prevenu par une erreur lors de l'execution.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "protocol_primitives.h"

#define TYPE_LENGTH 4

/* ---------------------------------------------
		    MANIPULATION DE DONNEES
   --------------------------------------------- */

char* stringWithoutLength(char* string) {
	int length = strlen(string);
	int offset = 0;
	int i = 0;
	while (isdigit(string[i])) {
		offset++;
		i++;
	}
	char* buffer = (char*)malloc(sizeof(char)*(length-offset+1));
	buffer = strcpy(buffer, string+offset);
	free(string);
	return buffer;
}

void freeProtocolContent(content_data* d) {
	if (d != NULL) {
		if (d->is_string == 1 && d->data_union->string != NULL) {
			free(d->data_union->string);
		}
		free(d->data_union);
	}
}

void freeProtocolData(protocol_data* d) {
	data_element* e = d->data;
	while (e != NULL) {
	        freeProtocolContent(e->resource);
		data_element* tmp = e;
		e = e->next;
		free(tmp);
	}
	free(d);
}

void insertData(protocol_data* d, content_data* content) {
	data_element* es = (data_element*)malloc(sizeof(data_element));
	es->resource = content;
	es->next = NULL;

	data_element* e = d->data;
	if (e != NULL) {
		while (e->next != NULL)
			e = e->next;
		e->next = es;
	}
	else {
		d->data = es;
	}
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
			perror("messagetype_to_string : unknown or unimplemented type");
			exit(0);
			break;
	}
}

char* encodeNumber(long int n, int length) {
	char* buffer = (char*)malloc(sizeof(char)*length+1);
	int i;
	int mod_factor = 10, div_factor = 1;
	for (i = length-1; i >= 0; i--) {
		buffer[i] = (char)('0'+((int)(n%mod_factor/div_factor)));
		mod_factor *= 10;
		div_factor *= 10;
	}
	return buffer;
}

char* encodeString(char* string, long int length) {
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

protocol_data* initMessageHeader(message_type type) {
	protocol_data* p = (protocol_data*)malloc(sizeof(protocol_data));
	p->total_length = 8;
	p->type = type;
	p->data = NULL;
	return p;
}

protocol_message encodeProtocolData(protocol_data* d) {
	// Header composition
	int length_nbchar = 0;
	char* numberEncoding;
	if (d->total_length <= 9999)
		length_nbchar = 4;
	else if (d->total_length <= 99999999)
		length_nbchar = 8;
	else {
		perror("encodeProtocolData : length to large to encode");
		exit(0);
	}
	char* buffer_message = (char*)calloc(sizeof(char), length_nbchar+TYPE_LENGTH+1);
	if (length_nbchar == 4) {
		numberEncoding = encodeNumber(d->total_length, 4);
		buffer_message = strcat(buffer_message, numberEncoding);
	}
	else {
		numberEncoding = encodeNumber(d->total_length, 8);
		buffer_message = strcat(buffer_message, numberEncoding);
	}
	buffer_message = strcat(buffer_message, encodeType(d->type));
	buffer_message[length_nbchar+TYPE_LENGTH] = '\0';

	// Data composition
	data_element* e = d->data;
	while (e != NULL) {
		if (e->resource->is_string == 1) {
			long int string_length = strlen(e->resource->data_union->string);
			buffer_message = (char*)realloc(buffer_message, sizeof(char)*(length_nbchar+TYPE_LENGTH+string_length));
			buffer_message = strcat(buffer_message, e->resource->data_union->string);
		}
		else {
			long int string_length;
			if (e->resource->data_union->integer <= 9999)
				string_length = 4;
			else if (e->resource->data_union->integer <= 99999999)
				string_length = 8;
			else {
				perror("encodeProtocolData : depassement de limite d'entier.");
				exit(0);
			}
			char* numberEncoding = encodeNumber(e->resource->data_union->integer, string_length);
			buffer_message = (char*)realloc(buffer_message, sizeof(char)*(length_nbchar+TYPE_LENGTH+string_length));
			buffer_message = strcat(buffer_message, numberEncoding);
			free(numberEncoding);
		}
		e = e->next;
	}

	freeProtocolData(d);

	return buffer_message;
}

void addMessageString(protocol_data* d, char* string) {
	content_data* cd = (content_data*)malloc(sizeof(content_data));
	content_union* cu = (content_union*)malloc(sizeof(content_union));
	// String encoding
	long int string_length = strlen(string);
	char* stringEncoding = encodeString(string, string_length);
	int stringEncodingLength = strlen(stringEncoding);
	d->total_length += stringEncodingLength;

	// Creation
	char* buffer = (char*)calloc(sizeof(char), stringEncodingLength);
	buffer = strcat(buffer, stringEncoding);

	cu->string = buffer;
	cd->is_string = 1;
	cd->data_union = cu;
	insertData(d, cd);
}

void addMessageNumber(protocol_data* d, long int number) {
        if (number <= 9999)
            d->total_length += 4;
        else if (number <= 99999999)
            d->total_length += 8;
        else {
            perror("addMessageNumber : depassement de limite d'entier.");
	    exit(0);
        }
	content_data* cd = (content_data*)malloc(sizeof(content_data));
	content_union* cu = (content_union*)malloc(sizeof(content_union));
	cu->integer = number;
	cd->is_string = 0;
	cd->data_union = cu;
	insertData(d, cd);
}

/* ---------------------------------------------
			FONCTIONS DE DECODAGE
   --------------------------------------------- */

int char2int(char c) {
	return (int)(c-'0');
}

// (1) On suppose qu'on ne peut pas avoir de chiffres dans le type
// (2) On suppose que le type ne contient que des lettres majuscules
long int decodeNumber(char* message) {
	long int result = 0;
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

long int decodeLength(protocol_message message) {
	return decodeNumber(message);
}

const char* getTypeStructure(message_type type) {
	switch (type) {
		case BADD_t: return ""; break;
		case BYEE_t: return "I"; break;
		case HELO_t: return "SS"; break;
		default:
			perror("getTypeStructure : unknown or unimplemented type");
			exit(0);
			break;
	}
}

message_type decodeType(protocol_message message) {
	int i = 0;
	while (isdigit(message[i]))
		i++;
	char* buffer = (char*)malloc(sizeof(char)*4);
	sprintf(buffer, "%c%c%c%c", message[i], message[i+1], message[i+2], message[i+3]);
	     if (strcmp("BCST", buffer) == 0) { return BCST_t; }
	else if (strcmp("PRVT", buffer) == 0) { return PRVT_t; }
	else if (strcmp("BADD", buffer) == 0) { return BADD_t; }
	else if (strcmp("OKOK", buffer) == 0) { return OKOK_t; }
	else if (strcmp("BYEE", buffer) == 0) { return BYEE_t; }
	else if (strcmp("HELO", buffer) == 0) { return HELO_t; }
	else if (strcmp("LIST", buffer) == 0) { return LIST_t; }
	else if (strcmp("SHUT", buffer) == 0) { return SHUT_t; }
	else if (strcmp("DEBG", buffer) == 0) { return DEBG_t; }
	else if (strcmp("FILE", buffer) == 0) { return FILE_t; }
	else { perror("decodeType : unknown or unimplemented type"); exit(0); }
	free(buffer);
}

protocol_data* extractMessageContent(protocol_data* data, const char* codeStructure) {
	/* TODO */
	return NULL;
}

int headerLength(protocol_message message) {
        int l = 0;
	while (isdigit(message[l]))
	  l++;
	return l+4;
}

protocol_data* decodeProtocolData(protocol_message message) {
	protocol_data* protocolData = (protocol_data*)malloc(sizeof(protocol_data));
	protocolData->total_length = decodeLength(message);
	protocolData->type = decodeType(message);
	const char* codeStructure = getTypeStructure(protocolData->type);
	return extractMessageContent(protocolData, codeStructure);
}
