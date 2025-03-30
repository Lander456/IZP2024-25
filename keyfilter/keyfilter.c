#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LEN 102	//max delka jedne adresy + prostor pro \n, znamo ze zadani
#define MAX_AMT_KEYS 128	//pocet nalezitelnych chars == velikost zakladni ASCII

int main (int argc, char *argv[]){

	if (argc > 2){
		fprintf(stderr, "ERROR: Invalid argcount\n");	//input stanoven v diskuzich na max 1 string, nikdy vic
		return 1;
	}
								//v kazde deklaraci byte navic, nebot potrebuji misto na \0
	char city[MAX_LINE_LEN + 1] = "";			//deklarace arraye pro tahani z adresy.txt
	char outString[MAX_LINE_LEN + 1] = "";			//deklarace output arraye pro output nalezeneho mesta
	char keysFound[MAX_AMT_KEYS + 1] = "";			//deklarace arraye pro nalezene potencialni pokracovani inputu
	int hit = 0;						//hit == kolik jsem nasel shod cities s inputem

	char input[strlen(argv[argc - 1])];
	strcpy(input, argv[argc - 1]);
	for (unsigned int i = 0; i < strlen(input); i++){	//cyklus pro konverzi vsech znaku v argumentu na uppercase pomoci toupper
		input[i] = toupper(input[i]);
		}

	while(fgets(city, MAX_LINE_LEN, stdin) != NULL){	// podminka pro ukonceni programu, jakmile je fgets NULL jsem na konci adresy.txt
	
		/*errorcheck delky adres*/

		if(city[strlen(city)-1] != '\n'){				//pokud neni poslednim znakem \n, adresa je delsi, nez je zadano == error
			fprintf(stderr, "ERROR, city name too long\n");
			return 2;
		}

		/*errorcheck delky 1 adresy*/

		for (unsigned int i = 0; i < strlen(city); i++){	//cyklus pro konverzi vsech znaku v momentalne pullnutem meste z adresy.txt na uppercase pomoci toupper
			city[i] = toupper(city[i]);
			if (city[i] == '\n'){				//prepsani newline na NULL
				city[i] = '\0';
			}
		}

		/*zadny input, potrebuji vypsat pouze prvni znaky vsech available mest jako zakladni nabidku inputu*/
	
		if (argc == 1){
	
			if (strchr(keysFound, city[0]) == NULL){	//pred jakymkoliv zapisem do keysfound kontrola, jestli nahodou tento znak jiz v keysfound neni
				hit++;
				char key = city[0];
				strncat(keysFound, &key, 1);		//pokud 1. pismeno prave nacteneho mesta neni v keysfound, zapis do keysfound

			}
		}

		/*zadny input*/

		/*input args, resim pismeno na poizici za delkou inputu*/
 
		if (argc !=1){
			
			/*mesto nalezeno*/

			if (strncmp(input, city, strlen(input)) == 0){	//pokud je dane mesto shodne v libovolnem rozsahu s inputem, zapisu do outStringu, zapisu si hit == nalezeni potencialniho final outputu
				hit++;					//napr. u inputu BR je shodne BRno, BReclav, BRuntal, ponevadz jsou 3, budou 3 hity == vice moznosti, nemam final output
				strncpy(outString, city, MAX_LINE_LEN);
			
			/*mesto nalezeno*/
			
				if (strchr(keysFound, city[strlen(input)]) == NULL){	//pokud je v keysfound pismeno shodne s pismenem na pozici o 1 dal, jak delka inputu, zapisu do keysfound novy charakter
					char key = city[strlen(input)];			// napr. input = BR, checkuju brEclav, brNo, brUntal, pokud jsou v keysfound, nezapisuji, pokud nejsou, zapisu
					strncat(keysFound, &key, 1);
				}
			}
		}

		/*input*/

	}

	/*output*/

	int change = 1;
	while(change != 0){
		change = 0;
		if (hit != 0 && hit != 1){
			for (unsigned int i = 0; i < strlen(keysFound) - 1; i++){
				if (keysFound[i] > keysFound [i + 1]){
					change++;					//bubble sort keys found
					char tempChar = keysFound[i];
					keysFound[i] = keysFound[i + 1];
					keysFound[i + 1] = tempChar;
				}
			}
		}
	}

	if (hit == 0){
		printf("Not found\n");
	} else if (hit == 1) {
		printf("Found: %s\n", outString);	//output printy dle poctu hitu, pokud hit neni, mesto v seznamu vubec neni, 1 hit == nasel jsem final output, vice hitu == potrebuji dalsi upresneni
	} else {
		printf("Enable: %s\n", keysFound);
	}

	/*output*/
	return 0;

}
