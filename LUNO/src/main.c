//arquivo utilizado para navegar entre as janelas do jogo

#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro_primitives.h>
#include <allegro_image.h>
#include "menu.h" //adicionando aquela funcao de abrir o menu
#include "historia.h" // adicionando a funcao abrir historia
//#include "jogo.h"
//#include "arquivo_janela_nome.h" >> tem que criar o arquivo dentro de include

int main() {

    al_init_primitives_addon();
    al_init_image_addon();
    al_install_keyboard();
    al_install_mouse();

    int estado_atual = 0; // 0: Menu, 1: Janela de pedir o nome, 2: Parte da historia, 3: Jogo

    while (true) {
        if (estado_atual == 0) {

            abrir_menu();
        
        } else if (estado_atual == 1) {

            abrir_historia();
        
        } //} else if (estado_atual == 3) {
            //bota a funcao de abrir a janela de nome
       
        //} // else if (estado_atual == 3) {
            //abrir_jogo();

        //}
    }

    return 0;
}

