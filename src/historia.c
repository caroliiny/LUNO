#include <stdio.h>
#include <stdlib.h>
#include <ctype.h> //para tornar o nome maiusculo

void limparTela() { //funcao de limpar a tela, so chamar ela na parte que quisermos limpar
    //system("cls");  // Para Windows
     system("clear"); // Para Linux
}

void enter() { //a funcao de pegar o enter pra limpar a tela
    printf("\nPressione 'Enter' para continuar...\n");
    getchar();  // Espera pela tecla Enter
    limparTela();
}

int main() {
    char jogador[20];
    printf("******************\n");
    printf("*                *\n");
    printf("*      LUNO      *\n");
    printf("*                *\n");
    printf("******************\n");
    
    printf("Digite seu nome: ");
    gets(jogador); 
    limparTela(); //aqui ja limpa automaticamente pq pegou a funcao

    for(int i=0;i!='\0';i++){ //nao ta funcionando a parte de tornar o nome maiusculo
        jogador[i]=toupper(jogador[i]);
    }
    
    //comecou a historia e vai limpando a tela conforme quisermos 
    printf("Seja bem-vindo(a), %s!\n", jogador);
    printf("Hoje você será encarregado(a) de uma missão.\n");
    enter(); // aqui pega o enter
    
    printf("Este é Luno, ele é aluno de Ciência da Computação na UERN.\n");
    printf("  O  \n");
    printf(" /|) \n");
    printf(" _|_\n");
    printf("\nUltimamente, suas notas têm ido de mal a pior. :( \n");
    printf("E como um bom professor, a sua função será ajudá-lo a se formar! :D \n");
    enter();
    
    printf("Portanto, você deverá desviar dos obstáculos na corrida para o diploma.\n");
    printf("Luno irá correr sozinho, você só precisará auxiliá-lo a ultrapassar as provas e atividades.\n");
    printf("Para desviar, basta pressionar a tecla de espaço do computador.\n");
    printf("\nCaso Luno esbarre em algum obstáculo, você perde o jogo.\n");
    enter();
    
    //chegou aqui, encerra, porem podemos adicionar a funcao abrir_jogo(); que ele vai mas tem que ver como adiciona corretamente
    return 0;
}
