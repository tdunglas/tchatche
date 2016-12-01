#ifndef protocol_instance_header
#define protocol_instance_header

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "protocol_primitives.h"

/* ---------------------------------------------
			 FONCTIONS D'ENCODAGE
   --------------------------------------------- */

// ------------------- Connexion -------------------
protocol_message encodeConnexion(char* pseudo, char* tube);

protocol_message encodeConnexionConfirmation(int id);

// ------------------- Echec -------------------
protocol_message encodeFail();

// ------------------- Deconnexion -------------------
protocol_message encodeDeconnexion(int id);

protocol_message encodeDeconnexionConfirmation(int id);

/* ---------------------------------------------
			FONCTIONS DE DECODAGE
   --------------------------------------------- */

#endif