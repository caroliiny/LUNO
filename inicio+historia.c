#include <stdio.h>
#include <stdlib.h>
main(){
	
	char continuar;
	char jogador[20];
	printf("******************\n");
	printf("*                *\n");
	printf("*      LUNO      *\n");
	printf("*                *\n");
	printf("******************\n");
	
	printf("Digite seu nome: ");
	gets(jogador);
	system("cls");
	printf("Seja bem-vindo(a), %s!\n", jogador);
	printf("Hoje voce sera encarregado(a) de uma missao.\n");
	printf("\nPressione 'Enter' para continuar\n");
	scanf("%c", &continuar);
	if(continuar == '\0' ){
		system("cls");
	}else{
		system("cls");
	}
	printf("Este eh Luno, ele eh aluno de Ciencia da Computacao na UERN.\n");
	printf("  O  \n");
	printf(" /|) \n");
	printf(" _|_\n");
	printf("\nUltimamente, suas notas tem ido de mal a pior. :( \n");
	printf("E como um bom professor, a sua funcao sera ajuda-lo a se formar! :D \n");
	
	printf("\nPressione 'Enter' para continuar\n");
	scanf("%c", &continuar);
	if(continuar == '\0' ){
		system("cls");
	}else{
		system("cls");
	}
	
	printf("Portanto, voce devera desviar dos obstaculos na corrida para o diploma.\n");
	printf("Luno ira correr sozinho, voce so precisara auxilia-lo a ultrapassar as provas e atividades.\n");
	printf("Para desviar, basta pressionar a tecla de espaco do computador.\n");
	printf("\nCaso Luno esbarre em algum obstaculo, voce perde o jogo.\n");
	printf("\nPressione 'Enter' para continuar\n");
	scanf("%c", &continuar);
	if(continuar == '\0' ){
		system("cls");
	}else{
		system("cls");
	}
	
}
