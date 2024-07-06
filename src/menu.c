#include "menu.h" // aquela funcao abrir_menu();
#include <stdio.h>
#include <allegro5/allegro.h> // padrao allegro
#include <allegro5/allegro_primitives.h> // para formas primitivas, como o retangulo do botao
#include <allegro5/allegro_image.h> // para imagens

typedef struct Botao { // criando o struct para os botoes e definindo o que um botao vai ter
    int x, y, largura, altura; // aqui as variaveis basicas como a altura a largura
    ALLEGRO_COLOR cor; // definindo a variavel para a cor
} Botao;

void desenha_Botao(Botao *botao) { //aqui para desenhaar o botao
    al_draw_filled_rectangle(botao->x, botao->y, botao->x + botao->largura, botao->y + botao->altura, botao->cor); //desenhando um retangulo
}

bool clicar(Botao *botao, int mouse_x, int mouse_y) { //para saber se  o botao foi clicado
    return mouse_x >= botao->x && mouse_x <= botao->x + botao->largura &&
           mouse_y >= botao->y && mouse_y <= botao->y + botao->altura;
}

void abrir_menu() { //implementando a funcao abrir_menu() que foi criadad naquele arquivo .h a dizendo o que ela faz >> o codigo todo do menu
    ALLEGRO_DISPLAY* janela = al_create_display(1100, 600);
    al_set_window_title(janela, "LUNO - Menu"); //titulo que fica la em cima da janela

    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 60); //temporizador
    ALLEGRO_BITMAP* fundo = al_load_bitmap("/home/Carol/jogo/acessorios/fundo.png"); //mudar de acordo com a maquina
    ALLEGRO_EVENT_QUEUE* event_queue = al_create_event_queue(); //evento

    if (!event_queue) { //verificando se os eventos foram criados, se nao, imprime essa mensagem na tela
        fprintf(stderr, "Falha ao criar a fila de eventos.\n");

        //caso nao criado, termina todos os eventos
        al_destroy_timer(timer);
        al_destroy_display(janela);
        al_destroy_bitmap(fundo);
        return;
    }

    //registrando os eventos
    al_register_event_source(event_queue, al_get_display_event_source(janela)); //registrando a janela
    al_register_event_source(event_queue, al_get_timer_event_source(timer)); //registrando o temporizador
    al_register_event_source(event_queue, al_get_keyboard_event_source()); //registrando o teclado
    al_register_event_source(event_queue, al_get_mouse_event_source()); // registrando o mouse

    //para definir a largura e altura da janela
    int largura_janela = al_get_display_width(janela);
    int altura_janela = al_get_display_height(janela);

    Botao Botao_jogar = { //tudo o que tem no botao jogar
        largura_janela / 2 - 150,
        altura_janela / 2 - 28,
        260,
        50,
        al_map_rgba(0, 0, 0, 0), //invisivel
    };

    Botao Botao_sobre = { //tudo o que tem no botao sobre
        largura_janela / 2 - 150, 
        altura_janela / 2 + 28,
        260, //coordenadas y
        50, // coordenadas x
        al_map_rgba(0, 0, 0, 0), //cor invisivel
    };

    bool rodando = true; //bool pode ser true ou false, no caso rodando e true
    bool desenha = true; // desenha e pra desenhar as coisas na tela, como o botao

    al_start_timer(timer);

    while (rodando) { 
        ALLEGRO_EVENT evento;
        al_wait_for_event(event_queue, &evento);

        if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            rodando = false;
        } else if (evento.type == ALLEGRO_EVENT_KEY_DOWN) {
            if (evento.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
                rodando = false;
            }
        } else if (evento.type == ALLEGRO_EVENT_TIMER) {
            desenha = true;
        } else if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
            if (clicar(&Botao_jogar, evento.mouse.x, evento.mouse.y)) { //se o clicado for o botao jogar
                printf("Botão jogar clicado!\n");
                // abrir_nome(); por exemplo, se o nome do arquivo foi nome
            } else if (clicar(&Botao_sobre, evento.mouse.x, evento.mouse.y)) { //se o clicado for o botao sobre
                printf("Botão sobre clicado!\n");
                //aqui podemos adicionar uma funcao para mudar a tela para entrar no sobre, mas tem que ser em outro arquivo eu acho
               
            }
        }

        if (desenha && al_is_event_queue_empty(event_queue)) {
            desenha = false;
            al_clear_to_color(al_map_rgb(255, 255, 255));
            al_draw_scaled_bitmap(fundo, 0, 0, al_get_bitmap_width(fundo), al_get_bitmap_height(fundo), 0, 0, largura_janela, altura_janela, 0);

            desenha_Botao(&Botao_jogar);
            desenha_Botao(&Botao_sobre);

            al_flip_display();
        }
    }

    //encerrando os eventos
    al_destroy_event_queue(event_queue);
    al_destroy_timer(timer);
    al_destroy_display(janela);
    al_destroy_bitmap(fundo);
}
