#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "protocol_primitives.h"
#include "protocol_instance.h"

int main() {
        int i;
	protocol_message connexion = encodeConnexion("engboris", "s1");
	for (i = 0; i < decodeLength(connexion); i++) {
		printf("[%c]", connexion[i]);
	}
	printf("\n");
	protocol_message confirmation = encodeConnexionConfirmation(15);
	for (i = 0; i < decodeLength(confirmation); i++) {
		printf("[%c]", confirmation[i]);
	}
	printf("\n");
	protocol_message fail = encodeFail();
	for (i = 0; i < decodeLength(fail); i++) {
		printf("[%c]", fail[i]);
		}
	printf("\n");
	protocol_message deconnexion = encodeDeconnexion(15);
	for (i = 0; i < decodeLength(deconnexion); i++) {
		printf("[%c]", deconnexion[i]);
	}
	printf("\n");
	printf("Type : %s\n", encodeType(decodeType(deconnexion)));
	return 0;
}
