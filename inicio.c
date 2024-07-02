#include <stdio.h>
#include <stdlib.h>
main(){
	
	int continuar;
	char jogador[20];
	printf("******************\n");
	printf("*                *\n");
	printf("*  Game do Dino  *\n");
	printf("*                *\n");
	printf("******************\n");
	
	printf("Digite seu nome: ");
	gets(jogador);
	system("cls");
	printf("Seja bem-vindo(a), %s!\n", jogador);
	printf("Hoje voce sera encarregado de uma missao\n");
	printf("\nAperte 1 para continuar\n");
	scanf("%d", &continuar);
	if(continuar == 1 ){
		system("cls");
	}else{
		system("cls");
	}
	printf("Este eh Joaozinho, ele eh aluno de Ciencia da Computacao na UERN\n");
	printf("  O  \n");
	printf(" /|) \n");
	printf(" _|_\n");
	printf("\nUltimamente, suas notas tem ido de mal a pior :( \n");
	printf("Por isso, a sua funcao sera ajuda-lo a se formar :D \n");
	
	printf("\nAperte 1 para continuar\n");
	scanf("%d", &continuar);
	if(continuar == 1 ){
		system("cls");
	}else{
		system("cls");
	}
	
	printf("Portanto, voce devera desviar dos obstaculos na corrida para o diploma.\n");
	printf("Joaozinho ira correr sozinho, voce so precisara auxilia-lo a ultrapassar os professores malignos.\n");
	printf("Para desviar, basta pressionar a tecla de espaco do computador.\n");
	printf("\nCaso Joaozinho esbarre em algum obstaculo, voce perde o jogo.\n");
	printf("\nPressione 1 para iniciar o jogo\n");
	scanf("%d", &continuar);
	if(continuar == 1 ){
		system("cls");
	}else{
		system("cls");
	}
	
	
	
}