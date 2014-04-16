/*Grupo: Stéphane Ammar ra149071
 * 	 Andréia Yukie Uratsuka 096952 
 * Animacao referente ao Projeto 02 da materia MC504 com a professora Islene
 * referente ao problema : The Dining Savages
 * Data 22/04/2014
*/


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "savage.h"

#define MAXCOL 50
#define MAXLIN 25

void drawField(char campo[MAXLIN][MAXCOL]) 
// Esta funcão monta a matriz onde ficara as informacoes a serem impressas na tela
// outras funcoes irao açterar esta matriz para criar o efeito de movimento na tela.
{
	int i, j;
	for (i = 0; i < MAXLIN; i++) 
	{
		for (j = 0; j < MAXCOL; j++) 
		{
			if (i == 0 || i == MAXLIN - 1) 
				campo[i][j] = '-';
			else if (j == 0 || j == MAXCOL - 1) 
				campo[i][j] = '|';
			else 
				campo[i][j] = ' ';
		}
	}
	campo[0][0] = '+';
	campo[0][MAXCOL - 1] = '+';
	campo[MAXLIN - 1][0] = '+';
	campo[MAXLIN - 1][MAXCOL - 1] = '+';
}

void drawPot(char campo[MAXLIN][MAXCOL], int meals) 
{
// Esta função desenha o pote de comida na matriz 
	int lin, col;
	lin = MAXLIN / 2 + 5;
	col = MAXCOL / 2 - 2;
	campo[lin + 1][col] = '|';
	campo[lin + 1][col + 4] = '|';
	campo[lin + 1][col - 1] = '<';
	campo[lin + 1][col + 5] = '>';
	campo[lin + 2][col] = '|';
	campo[lin + 2][col + 4] = '|';
	campo[lin + 3][col + 1] = '-';
	campo[lin + 3][col + 2] = '-';
	campo[lin + 3][col + 3] = '-';
	campo[lin + 2][col + 2] = (char)(meals + '0');
} 

void printField(char campo[MAXLIN][MAXCOL])
/* Esta função desenha a matriz no console*/
{
	int i, j;
	i = j = 0;
	//coordenadas em que a quantidade de comida será desenhada
	int lin, col; 
	lin = MAXLIN / 2 + 7;
	col = MAXCOL / 2;

	//garanto que a tela estará limpa 
	system("clear");

	printf ("-----------The Dining Savages Problem-------------\n")   ;            	
	for (i = 0; i < MAXLIN; i++)
	{
		for (j = 0; j < MAXCOL; j++)
			printf("%c", campo[i][j]);
		printf("\n");
	}
	printf("\n");
}

void drawCooker(char campo[MAXLIN][MAXCOL], int lin, int col)
//funcao que desenha o cozinheiro na matriz
{
	campo[lin][col + 3] = '0';
	campo[lin][col + 4] = '0';	
	campo[lin + 1][col + 4] = '^';
}

void clearSteps(char campo[MAXLIN][MAXCOL], int lin, int col)
//funcao limpa o selvagem ou cozinheiro para que nao deixe "pegadas"
{
	campo[lin][col] = ' ';
	campo[lin][col + 1] = ' ';	
	campo[lin + 1][col + 1] = ' ';
}

void drawSavage(char campo[MAXLIN][MAXCOL], int lin, int col, int status)
//funcao que desenha o os selvagens na matriz
{
    campo[lin][col] = '0';   
    campo[lin - 1][col] = ',';
    campo[lin][col + 1] = '0';   
    //campo[lin + 1][col + 1] = (status == EATING) ? '#' : '@';   
	if(status==EATING){
		campo[lin + 1][col + 1] = '@';
		} else{
			if(status == HUNGRY || status == WALKING)
					campo[lin + 1][col + 1] = '#';
			else {
				if(status == WAITINGNEIGHBOR)
						campo[lin + 1][col + 1] = '!';
				else//o cara esta fazendo nada
						campo[lin + 1][col + 1] = '%';}
		}
} 


int main()
{
	int numSavages, i, numMeals;
	struct Savage** savages;
	struct Savage* currentSavage;
	char campo[MAXLIN][MAXCOL];
	do
	{
		printf("Digite o número de selvagens (2 a 10): ");
		scanf("%d", &numSavages);
	}
	while (numSavages < 2 || numSavages > 10);
	do
	{
		printf("Digite o número de refeições (2 a 9): ");
		scanf("%d", &numMeals);
	}
	while (numMeals < 2 || numMeals > 9);
	savages = (struct Savage**)malloc(numSavages * sizeof(struct Savage*));
	srand(time(NULL));
	start(15, 24, numMeals, 15, 32, numSavages);
	for (i = 0; i < numSavages; i++)
		savages[i] = newSavage(4, 5 + 4 * i,i);
	while (1)
	{
		drawField(campo);
		drawPot(campo, pot()->meals);
		drawCooker(campo, cooker()->x, cooker()->y);
		for (i = 0; i < numSavages; i++)
		{
			currentSavage = savages[i];
			drawSavage(campo, currentSavage->x, currentSavage->y, currentSavage->status);
		}

		//legenda da animacao
		printField(campo);
		printf  (" ,             ,            ,\n");
		printf  (" 00            00           00           00\n");
		printf  ("  #             @            %%              !");printf("	 	  ^\n");
		printf("\n Selvagem     | Selvagem  | Selvagem     | Selvagem   |  Cozinheiro\n");
		printf(" com fome     |   comendo | fazendo nada | esperando vizinho   |\n");
		printf ("Precione ctrl+c para encerrar o programa.\n");
		usleep(100000);
	}
	return 0;
}


