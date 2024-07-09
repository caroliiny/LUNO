#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

typedef struct Botao {
    int x, y, largura, altura;
    ALLEGRO_COLOR cor;
} Botao;

void desenha_Botao(Botao *botao) {
    al_draw_filled_rectangle(botao->x, botao->y, botao->x + botao->largura, botao->y + botao->altura, botao->cor);
}

bool clicado(Botao *botao, int mouse_x, int mouse_y) {
    return mouse_x >= botao->x && mouse_x <= botao->x + botao->largura &&
           mouse_y >= botao->y && mouse_y <= botao->y + botao->altura;
}

int main(int argc, char **argv) {
    if (!al_init()) {
        fprintf(stderr, "Falha ao inicializar Allegro.\n");
        return -1;
    }

    al_init_primitives_addon();
    al_init_image_addon();
    al_install_keyboard();
    al_install_mouse();
    al_install_audio();
    al_init_acodec_addon();
    
    if (!al_reserve_samples(1)) {
        fprintf(stderr, "Falha ao reservar canais de áudio.\n");
        return -1;
    }

    ALLEGRO_DISPLAY *janela = al_create_display(1100, 600);
    if (!janela) {
        fprintf(stderr, "Falha ao criar a janela.\n");
        return -1;
    }
    al_set_window_title(janela, "LUNO - Menu");

    ALLEGRO_TIMER *timer = al_create_timer(1.0 / 60);
    if (!timer) {
        fprintf(stderr, "Falha ao criar o timer.\n");
        al_destroy_display(janela);
        return -1;
    }

    ALLEGRO_BITMAP *fundo = al_load_bitmap("/home/carol/LUNO/acessorios/fundo.png");
    if (!fundo) {
        fprintf(stderr, "Falha ao carregar a imagem de fundo.\n");
        al_destroy_timer(timer);
        al_destroy_display(janela);
        return -1;
    }

    ALLEGRO_EVENT_QUEUE *event_queue = al_create_event_queue();
    if (!event_queue) {
        fprintf(stderr, "Falha ao criar a fila de eventos.\n");
        al_destroy_timer(timer);
        al_destroy_display(janela);
        al_destroy_bitmap(fundo);
        return -1;
    }

    al_register_event_source(event_queue, al_get_display_event_source(janela));
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_mouse_event_source());

    int largura_janela = al_get_display_width(janela);
    int altura_janela = al_get_display_height(janela);

    Botao Botao_jogar = {
        largura_janela / 2 - 150,
        altura_janela / 2 - 28,
        260,
        50,
        al_map_rgba(0, 0, 0, 0), //cor invisível
    };

    Botao Botao_sobre = {
        largura_janela / 2 - 150,
        altura_janela / 2 + 28,
        260,
        50,
        al_map_rgba(0, 0, 0, 0),
    };

    // Carrega a música de fundo
    ALLEGRO_SAMPLE *som = al_load_sample("/home/carol/LUNO/acessorios/musicaintro.mp3");
    if (!som) {
        fprintf(stderr, "Falha ao carregar a música.\n");
        al_destroy_timer(timer);
        al_destroy_display(janela);
        al_destroy_bitmap(fundo);
        al_destroy_event_queue(event_queue);
        return -1;
    }

    // Inicia a reprodução da música em loop
    if (!al_play_sample(som, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, NULL)) {
        fprintf(stderr, "Falha ao reproduzir a música.\n");
        al_destroy_sample(som);
        al_destroy_timer(timer);
        al_destroy_display(janela);
        al_destroy_bitmap(fundo);
        al_destroy_event_queue(event_queue);
        return -1;
    }

    bool rodando = true;
    bool desenha = true;

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
        } else if (evento.type == ALLEGRO_EVENT_MOUSE_AXES) {
            if (clicado(&Botao_jogar, evento.mouse.x, evento.mouse.y)) {
                // Abrir tela de jogo
            } else if (clicado(&Botao_sobre, evento.mouse.x, evento.mouse.y)) {
                // Abrir tela "Sobre"
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

    // Libera a amostra de áudio e demais recursos
    al_destroy_sample(som);
    al_destroy_timer(timer);
    al_destroy_display(janela);
    al_destroy_bitmap(fundo);
    al_destroy_event_queue(event_queue);
    al_uninstall_audio();

    return 0;
}
