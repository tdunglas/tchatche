#ifndef protocol_primitives_header
#define protocol_primitives_header

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

typedef char* protocol_message;

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

typedef struct data_element {
	char* resource;
	struct data_element* next;
} data_element;

typedef struct protocol_data {
	int total_length;
	message_type type;
	data_element* data;
} protocol_data;

/* ---------------------------------------------
			 FONCTIONS D'ENCODAGE
   --------------------------------------------- */

const char* encodeType(message_type type);

char* encodeNumber(int n, int length);

char* encodeString(char* string, int length);

protocol_data* initMessageHeader(message_type type);

void freeProtocolData(protocol_data d);

protocol_message encodeProtocolData(protocol_data* d);

void insertData(protocol_data* d, char* string);

void addMessageString(protocol_data* d, char* string);

void addMessageNumber(protocol_data* d, int number);

/* ---------------------------------------------
			FONCTIONS DE DECODAGE
   --------------------------------------------- */

int char2int(char c);

int decodeLength(protocol_message message);

protocol_data* decodeProtocolData(protocol_message message);

#endif