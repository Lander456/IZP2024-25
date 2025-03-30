/*
Tadeas Topinka
xtopint00
259793
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define LEFT_BORDER 1	//define hodnoty leve steny
#define RIGHT_BORDER 2	//define hodnoty prave steny
#define TOP_BOTTOM_BORDER 4	//vrchni, ci spodni hranici definuje stejna hodnota, zavisi na lichosti/ sudosti souctu indexu policka
#define RIGHT 1		//define right, abych si nemusel pamatovat
#define LEFT 2		//deifne left, abych si nemusel pamatovat
#define NOT_FOUND 404	//well known num for the unknown
#define DEFAULT 42	//default value pro policka, kdyby nahodou bylo zadano pole s mensimi rozmery, nez jake byly zadany

//ko9jkiuggutf
typedef struct {
	int rows;
	int cols;
	unsigned char *cells;
} Map;

int LoadMap(char input[], Map *map);
bool isBorder(Map *map, int r, int c, int border);
int startBorder(Map *map, int r, int c, int leftright);
int testMap(Map *map);

int main(int argc, char *argv[]){

	if (argc < 2){
		fprintf(stderr, "ERROR, no arg passed\n");	//pokud je nedostatek argumentu, ERROR
		return 1;
	}

/* --HELP START */

	if (strncmp(argv[1], "--help", strlen(argv[1])) == 0){
		if (argc != 2){
			fprintf(stderr, "ERROR, invalid argcount for --help\n");
			return EXIT_FAILURE;
		}
		printf("To test whether your maze is valid, input ./maze --test maze.txt\n");
		printf("To find a path through your maze using the right hand rule, input: ./maze --rpath R C maze.txt, where R represents the entry row and C represents the entry column.\n");
		printf("To find a path through your maze using the left hand rule, input: ./maze --lpath R C maze.txt, where R represents the entry row and C represents the entry column.\n");
		printf("To find the shortest path through your maze, input: ./maze --shortest R C maze.txt, where R represents the entry row and C represents the entry column.\n");
		return 0;

/* --HELP END */
/* --TEST START */

	} else if(strncmp(argv[1], "--test", strlen(argv[1])) == 0){
		if (argc != 3){
			fprintf(stderr, "ERROR, invalid argnum for --test\n");
			return EXIT_FAILURE;
		}

		Map map;
		LoadMap(argv[2], &map);
		if (testMap(&map) != 0){
			printf("Invalid\n");
			free(map.cells);
			return 0;
		} else {
			printf("Valid\n");
			free(map.cells);
			return 0;
		}

/* --TEST END */

/* --RPATH START */

	} else if(strncmp(argv[1], "--rpath", strlen(argv[1])) == 0){
		if (argc != 5){
			fprintf(stderr, "ERROR invalid argcount for --rpath\n");
			return EXIT_FAILURE;
		}
		Map map;
		int errCheck = LoadMap(argv[4], &map);			//uchovani return hodnoty LoadMap v errCheck, pro zkontrolovani uspesneho nacteni
		if (errCheck != 0){
			return EXIT_FAILURE;				//ERROR print ve fci, zde pouze vracim EXIT_FAILURE
		}

		if (testMap(&map) != 0){
			fprintf(stderr, "ERROR, problems with map, recommend using --test for validity");	//kontrola pripadneho problemu mezi zidli a klavesnici
			free(map.cells);
			return EXIT_FAILURE;
		}
		int row = atoi(argv[2]);
		int col = atoi(argv[3]);
		int border = startBorder(&map, row, col, RIGHT);
		if (border == NOT_FOUND){
			return EXIT_FAILURE;				//error print ve fci, tady pouze returnuju EXIT_FAILURE pro interrupt programu
		}
		while (col <= map.cols && row <= map.rows && col > 0 && row > 0){
			int oldCol = col;	//pamatovani starych koordinatu, abych rozlisil, kdy jsem se pohnul
			int oldRow = row;
			printf("%i,%i\n", row, col);
			while (col == oldCol && row == oldRow){		//dokud jsem se nepohnul z policka, budu kontrolovat ruzne steny
				if ((row+col)%2 == 1){
					switch (border){		//switch case pro vsechny steny
						case TOP_BOTTOM_BORDER:
							if (!isBorder(&map, row-1, col-1, border)){	//pokud neni stena tam kam se kukam, prolezu do dalsiho pole, jinak sa kukam na dalsi steny dle rpath
								border = LEFT_BORDER;			//col++ == pohyb vpravo, col-- == pohyb vlevo, row++ == pohyb dolu, row-- == pohyb nahoru
								row++;					//pohyb provadim nasledovne, vejdu, podivam se na stenu, ktera je napravo, pokud je prazdna, jdu dal, pokud neni, jedu dal po
													//stenach dokud nenajdu prazdnou stenu
								break;
							} else {
								border = RIGHT_BORDER;
								break;
							}
						case RIGHT_BORDER:
							if (!isBorder(&map, row-1, col-1, border)){
								border = RIGHT_BORDER;
								col++;
								break;
							} else {
								border = LEFT_BORDER;
								break;
							}
						case LEFT_BORDER:
							if (!isBorder(&map, row-1, col-1, border)){
								border = TOP_BOTTOM_BORDER;
								col--;
								break;
							} else {
								border = TOP_BOTTOM_BORDER;
								break;
							}
					}
				} else {
					switch (border){
						case TOP_BOTTOM_BORDER:
							if (!isBorder(&map, row-1, col-1, border)){
								border = RIGHT_BORDER;
								row--;
								break;
							} else {
								border = LEFT_BORDER;
								break;
							}
						case LEFT_BORDER:
							if (!isBorder(&map, row-1, col-1, border)){
								border = LEFT_BORDER;
								col--;
								break;
							} else {
								border = RIGHT_BORDER;
								break;
							}
						case RIGHT_BORDER:
							if (!isBorder(&map, row-1, col-1, border)){
								border = TOP_BOTTOM_BORDER;
								col++;
								break;
							} else {
								border = TOP_BOTTOM_BORDER;
								break;
							}
					}
				}
			}
		}

	free(map.cells);
	return 0;

/* --RPATH END */
/* --LPATH START */

	} else if(strncmp(argv[1], "--lpath", strlen(argv[1])) == 0){
		if (argc != 5){
			fprintf(stderr, "ERROR invalid argcount for --lpath\n");
			return EXIT_FAILURE;
		}

		Map map;
		int errCheck = LoadMap(argv[4], &map);					//promenna pro uchovani hodnoty navracene z LoadMap, abych mohl returnovat error z mainu
		if (errCheck != 0){
			return EXIT_FAILURE;						//error fprintf je ve fci, tim padem jen vracim EXIT_FAILURE
		}

		if (testMap(&map) != 0){
			fprintf(stderr, "ERROR, problems with map, recommend checking --test for validity\n");		//kontrola pripadneho problemu mezi zidli a klavesnici
			free(map.cells);
			return EXIT_FAILURE;
		}
		int row = atoi(argv[2]);
		int col = atoi(argv[3]);
		int border = startBorder(&map, row, col, LEFT);
		if (border == NOT_FOUND){
			return EXIT_FAILURE;
		}

		while (col <= map.cols && row <= map.rows && col > 0 && row > 0){
			int oldCol = col;
			int oldRow = row;
			printf("%i,%i\n", row, col);
			while (col == oldCol && row == oldRow){
				if ((row+col)%2 == 1){
					switch (border){
						case TOP_BOTTOM_BORDER:
							if (!isBorder(&map, row-1, col-1, border)){	//obdoba fgovani u rpath, akorat beru steny po levici
							border = RIGHT_BORDER;
							row++;
							break;
							} else {
							border = LEFT_BORDER;
							break;
							}
						case RIGHT_BORDER:
							if (!isBorder(&map, row-1, col-1, border)){
							border = TOP_BOTTOM_BORDER;
							col++;
							break;
							} else {
							border = TOP_BOTTOM_BORDER;
							break;
							}
						case LEFT_BORDER:
							if(!isBorder(&map, row-1, col-1, border)){
							border = LEFT_BORDER;
							col--;
							break;
							} else {
							border = RIGHT_BORDER;
							break;
							}
					}
				} else {
					switch (border){
						case TOP_BOTTOM_BORDER:
							if (!isBorder(&map, row-1, col-1, border)){
							border = LEFT_BORDER;
							row--;
							break;
							} else {
							border = RIGHT_BORDER;
							break;
							}
						case RIGHT_BORDER:
							if (!isBorder(&map, row-1, col-1, border)){
							border = RIGHT_BORDER;
							col++;
							break;
							} else {
							border = LEFT_BORDER;
							break;
							}
						case LEFT_BORDER:
							if (!isBorder(&map, row-1, col-1, border)){
							border = TOP_BOTTOM_BORDER;
							col--;
							break;
							} else {
							border = TOP_BOTTOM_BORDER;
							break;
							}
					}
				}
			}
		}
	free(map.cells);
	return 0;
	
/* --LPATH END */
/* --SHORTEST START */

	} else if(strncmp(argv[1], "--shortest", strlen(argv[1])) == 0){
		printf("Under construction\n");
		return 0;
	}

/* --SHORTEST END */

}

int LoadMap(char input[], Map *map){
	FILE *pMazeFile = fopen(input, "r");
	if (pMazeFile == NULL){			//check, jestli byl soubor vubec otevren, pokud nebyl, ptr musi byt NULL
		fprintf(stderr, "ERROR, failed to open file\n");
		return EXIT_FAILURE;
	}
	fscanf(pMazeFile, "%d %d", &map->rows, &map->cols);					//nacteni velikosti mapy z 1. 2 souradnic
	map->cells = malloc(sizeof(unsigned char)*map->rows*map->cols);				//malloc 1D arraye cells, ktere se budu tvarit, ze je 2D
	if (map->cells == NULL){
		fprintf(stderr, "ERROR, failed to allocate memory\n");
		return EXIT_FAILURE;
	}
	for (long unsigned int i = 0; i < sizeof(unsigned char)*map->rows*map->cols; i++){	//kazdy index vypln default hodnotou, pro ozkouseni vyplneni celeho zadaneho pole
		map->cells[i] = DEFAULT;
	}
	for (int i = 0; i < map->rows; i++){
		for (int j = 0; j < map->cols; j++){
			fscanf(pMazeFile, "%1hhu", &map->cells[i*map->cols+j]);			//nacitani hodnot z .txt do cells, nacitam do 1D pole, takze se k prvkum dostavam pomoci rovnicky
		}
	}
	if (fclose(pMazeFile) != 0){
		fprintf(stderr, "Failed to close file, wat nau?\n");				//check, zda-li je soubor uspesne uzavren
		return EXIT_FAILURE;
	}
	return 0;
}

bool isBorder(Map *map, int r, int c, int border){
	unsigned char currentCell = map->cells[r*map->cols+c];
	if ((currentCell & border) == border){							//zjistovani, zda-li je dany border vskutku border, ci dira, bitovy soucet zjisti, zda-li na pozici bitu naleziciho dane hranici je bit
		return true;									//pokud ano, tak vysledek bitoveho souctu bude roven danemu borderu
	} else {
		return false;
	}
}

int startBorder (Map *map, int r, int c, int leftright){
	int colsMax = map->cols;								//definovani maxim pro cols a rows, at vim kde muzu, nemuzu hledat
	int rowsMax = map->rows;								//napr, pokud mi uzivatel zada start 99 99 v poli 6x7, tak to asi bohuzel nenajdu nikdy

	/*start --rpath*/
	if (leftright == RIGHT){
		if ((c+r)%2 == 0){
			
			/*start suda policka*/
			if (c == 1 && !isBorder(map, r-1, c-1, LEFT_BORDER)){			//kontrola rozdelena na 2 typy policek, se sudym souctem indexu a s lichym souctem indexu
				return RIGHT_BORDER;						//vzhledem k prioritam vstupu prvne zkontroluji levou, pak pravou stranu az potom vrsek, spodek
			} else if (c == colsMax && !isBorder(map, r-1, c-1, RIGHT_BORDER)){	//border vracim dle toho, zda-li jdu pravidlem prave, ci leve ruky, jaky typ policka to je a kudy jsem vstoupil
				return TOP_BOTTOM_BORDER;
			} else if (r == 1 && !isBorder(map, r-1, c-1, TOP_BOTTOM_BORDER)){
				return LEFT_BORDER;
			}
			/*end suda policka*/

		} else {
			
			/*start licha policka*/
			if (c == 1 && !isBorder(map, r-1, c-1, LEFT_BORDER)){
				return RIGHT_BORDER;
			} else if (c == colsMax && !isBorder(map, r-1, c-1, RIGHT_BORDER)){
				return TOP_BOTTOM_BORDER;
			} else if (r == rowsMax && !isBorder(map, r-1, c-1, TOP_BOTTOM_BORDER)){
				return RIGHT_BORDER;
			}
			/*end licha policka*/

		}
	
	/*end --rpath*/
	/*start --lpath*/

	} else if (leftright == LEFT){
		if ((c+r)%2 == 0){
			
			/*start suda policka*/
			if (c == 1 && !isBorder(map, r-1, c-1, LEFT_BORDER)){
				return TOP_BOTTOM_BORDER;			//v sudem policku, jdu-li dle pravidla leve ruky, vracim spodni steny pri pruchodu levou
			} else if (c == colsMax && !isBorder(map, r-1, c-1, RIGHT_BORDER)){
				return LEFT_BORDER;				//obdoba 1., akorat s jinymi bordery
			} else if (r == 1 && !isBorder(map, r-1, c-1, TOP_BOTTOM_BORDER)){
				return RIGHT_BORDER;
			}
			/*end suda policka*/

		} else {
			
			/*start licha policka*/
			if (c == 1 && !isBorder(map, r-1, c-1, LEFT_BORDER)){
				return RIGHT_BORDER;				//v lichem policku, jdu-li dle pravidla leve ruky, projdu levou stenou, vracim pravou, neb tam je tzv. striska
			} else if (c == colsMax && !isBorder(map, r-1, c-1, RIGHT_BORDER)){
				return TOP_BOTTOM_BORDER;			//obdoba 1., akorat jine bordery
			} else if (r == rowsMax && !isBorder(map, r-1, c-1, TOP_BOTTOM_BORDER)){
				return LEFT_BORDER;
			}
			/*end licha policka*/

		}

	}
	fprintf(stderr, "ERROR, could not identify startBorder, did you input the correct start coordinates?\n");	//pokud nenajdu zadnou hranici, uzivatel mi zadal bud spatne koordinaty (uprostred maze, mimo maze, na policku bez
	return NOT_FOUND;												//vstupu) vracim 404
}


int testMap(Map *map){
	
	if (map->rows <= 0 || map->cols <= 0){
		return EXIT_FAILURE;
                }
	for (int i = 0; i < map->rows; i++){
		for (int j = 0; j < map->cols; j++){
			unsigned char currentCell = map->cells[i*map->rows+j];
			if (currentCell > 7 || currentCell == DEFAULT){		//Policko ma vyssi/nizsi hodnotu nez je mozne, max bude 7, min 0, lebo ma hodnotu default == nebylo na nej nic zapsano
				return EXIT_FAILURE;
			}

			if (map->rows <= 0 || map->cols <= 0){			//Invalid map size, mapa nemuze mit rozmery 0x0, ci (-1)x(-1)
				return EXIT_FAILURE;
			}

			if (i < map->rows-1 && j < map->cols-1){
				if ((i+j)%2 == 0){				//suda policka
					if(isBorder(map, i, j, RIGHT_BORDER) != isBorder(map, i, j+1, LEFT_BORDER)){	//kontrolovani sdilene steny 2 poli ,aby 1. nemelo stenu a 2. diru
						return EXIT_FAILURE;
					}
				} else {				//licha policka
					if((isBorder(map, i, j, RIGHT_BORDER) != isBorder(map, i, j+1, LEFT_BORDER))||(isBorder(map, i, j, TOP_BOTTOM_BORDER) != isBorder(map, i+1, j, TOP_BOTTOM_BORDER))){
						return EXIT_FAILURE;
					}
				}
			}
		}
	}
	return 0;
}
