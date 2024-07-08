#include <stdio.h>
#include <allegro5/allegro.h> //padrao allegro
#include <allegro5/allegro_primitives.h> //formas primitivas, como o retangulo do botao
#include <allegro5/allegro_image.h> //para imagens

typedef struct Botao { //cria um struct generico para botao, assim nao precisa haver redundancia de codigo
    int x, y, largura, altura;
    ALLEGRO_COLOR cor;
} Botao;

void desenha_Botao(Botao *botao) { //funcao para desenhar o botao na tela

    al_draw_filled_rectangle(botao->x, botao->y, botao->x + botao->largura, botao->y + botao->altura, botao->cor);
}

bool clicado(Botao *botao, int mouse_x, int mouse_y) { // pra verificar se o botao foi clicado
    return mouse_x >= botao->x && mouse_x <= botao->x + botao->largura &&
           mouse_y >= botao->y && mouse_y <= botao->y + botao->altura;
}


int main(int argc, char **argv) {

  


    if (!al_init()) {
        fprintf(stderr, "Falha ao inicializar Allegro.\n");
        return -1;
    }

    al_init_primitives_addon(); //iniciar formas primitivas
    al_init_image_addon(); //iniciar imagens
    al_install_keyboard(); //para instalar o teclado
    al_install_mouse(); //para instalar o mouse


    ALLEGRO_DISPLAY* janela = al_create_display(1100, 600); //tamanho da janela
    al_set_window_title(janela, "LUNO - Menu"); //titulo da janela

    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 60); // temporizador
    ALLEGRO_BITMAP* fundo = al_load_bitmap("/home/Carol/LUNO/acessorios/fundo.png"); //mudar caminho de acordo com a maquina
    ALLEGRO_EVENT_QUEUE* event_queue = al_create_event_queue(); //eventos


    if (!event_queue) {
        fprintf(stderr, "Falha ao criar a fila de eventos.\n");
        al_destroy_timer(timer);
        al_destroy_display(janela);
        al_destroy_bitmap(fundo);
        return -1;
    }

    //registrando eventos
    al_register_event_source(event_queue, al_get_display_event_source(janela)); //janela
    al_register_event_source(event_queue, al_get_timer_event_source(timer));//temporizador
    al_register_event_source(event_queue, al_get_keyboard_event_source()); //teclado
    al_register_event_source(event_queue, al_get_mouse_event_source());//mouse



    int largura_janela = al_get_display_width(janela);
    int altura_janela = al_get_display_height(janela);


    //todas as informacoes dos botoes JOGAR e SOBRE
     Botao Botao_jogar = {
        largura_janela / 2 - 150,
        altura_janela / 2 - 28,
        260,
        50,
        al_map_rgba(0, 0, 0, 0), //cor invisivel
    };

    Botao Botao_sobre = {
        largura_janela / 2 - 150,
        altura_janela / 2 + 28,
        260,
        50,
        al_map_rgba(0, 0, 0, 0),
    };


    //bool pode ter valores true ou false
    bool rodando = true; 
    bool desenha = true;

    al_start_timer(timer); //iniciar temporizador

    while (rodando) { // enquanto estiver rodando
        ALLEGRO_EVENT evento; //definindo o evento
        al_wait_for_event(event_queue, &evento); //espera pelo evento

        if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE) { //se fechar a janela, para de rodar
            rodando = false;

        } else if (evento.type == ALLEGRO_EVENT_KEY_DOWN) {
            if (evento.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
                rodando = false;
            }

        } else if (evento.type == ALLEGRO_EVENT_TIMER) {
            desenha = true;

        } else if (evento.type == ALLEGRO_EVENT_MOUSE_AXES) {

            if (clicado(&Botao_jogar, evento.mouse.x, evento.mouse.y)) {
            //abrir_jogo();
            } else if (clicado(&Botao_sobre, evento.mouse.x, evento.mouse.y)) {
                //abrir_sobre
            }

            desenha = true;
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

    //encerra os eventos
    al_destroy_event_queue(event_queue);
    al_destroy_timer(timer);
    al_destroy_display(janela);
    al_destroy_bitmap(fundo);

    return 0;
}
