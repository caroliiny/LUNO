// Codigo da primeira janela que aparece quando abrimos o jogo - MENU

#include <stdio.h> // biblioteca padrao c
#include <allegro5/allegro.h> //bibliotecas necessarias allegro
#include <allegro5/allegro_primitives.h> // formas primitivas
#include <allegro5/allegro_image.h> // para imagens
#include <allegro5/allegro_font.h> // para fontes
#include <allegro5/allegro_ttf.h>


int main(int argc, char **argv) { //funcao principal

    if (!al_init()) { // para iniciar o allegro
        fprintf(stderr, "Falha ao inicializar Allegro.\n"); //teste de erro (opcional, mas eh melhor para ver se houver algum erro)
        return -1;
    }

    //inicializacao
    al_init_primitives_addon(); // formas primitivas
    al_init_image_addon(); // imagem
    al_init_font_addon(); //  fonte
    al_init_ttf_addon(); 
    

    if (!al_install_keyboard()) { // para instalar o teclado
        fprintf(stderr, "Falha ao instalar o teclado.\n"); // se nao funcionar imprime isso na tela
        return -1;
    }

    ALLEGRO_DISPLAY* janela = al_create_display(1000, 900); // para criar a janela, tamanho escolhido para a janela

    al_set_window_title(janela, "LUNO - Menu"); // titulo da janela, nesse caso: LUNO - Menu

    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 60);
    ALLEGRO_FONT* fonte = al_load_ttf_font("/home/Carol/jogo/acessorios/fonts-hosny-thabit/Thabit.ttf", 24, 0);
    ALLEGRO_BITMAP* fundo = al_load_bitmap("/home/Carol/jogo/acessorios/fundo.jpg");
    ALLEGRO_EVENT_QUEUE* event_queue = al_create_event_queue();



    if (!janela) {
        fprintf(stderr, "Falha ao criar a janela.\n"); // se nao funcionar, imprime na tela
        return -1;
    }

    
    // fonte escolhida. Tem que ajustar o caminho de acordo com a maquina (nao sei resolver esse problema)
    
    if (!fonte) {
        fprintf(stderr, "Falha ao carregar a fonte.\n");
        al_destroy_display(janela);
        return -1;
    }

    //para colocar a imagem. Ajustar de acordo com a maquina
    
    
    if (!timer) {
        fprintf(stderr, "Falha ao criar o temporizador.\n");
        al_destroy_display(janela);
        return -1;
    }

    
    if (!event_queue) {
        fprintf(stderr, "Falha ao criar a fila de eventos.\n");
        al_destroy_timer(timer);
        al_destroy_display(janela);
        return -1;
    }

    al_register_event_source(event_queue, al_get_display_event_source(janela));
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_keyboard_event_source());

    al_start_timer(timer);

    bool running = true;

    while (running) {

        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);

        if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            running = false;

        } else if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
            if (ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
                running = false;
            }

        } else if (ev.type == ALLEGRO_EVENT_TIMER) {
            // Limpar a tela 
            al_clear_to_color(al_map_rgb(44, 255, 88));

        }

        // variaveis para escala da imagem
        int largura_janela = al_get_display_width(janela); //para a largura da janela
        int altura_janela = al_get_display_height(janela); // para a altura da janela

        //funcao que ajusta a imagem ao tamanho da janela
        al_draw_scaled_bitmap(fundo, 0, 0, al_get_bitmap_width(fundo), al_get_bitmap_height(fundo), 0, 0, largura_janela, altura_janela, 0);

        al_flip_display();
    }

    al_destroy_event_queue(event_queue);
    al_destroy_timer(timer);
    al_destroy_display(janela);
    al_destroy_font(fonte);
    al_destroy_bitmap(fundo);


    return 0;
}
