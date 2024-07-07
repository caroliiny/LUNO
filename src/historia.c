//FUNCIONANDO OK
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <stdbool.h>

#define tamNome 20

int main(int argc, char **argv) {
    if (!al_init()) {
        fprintf(stderr, "Falha ao inicializar Allegro.\n");
        return -1;
    }

    al_init_image_addon(); // Inicializa addon de imagens
    al_init_font_addon();  // Inicializa addon de fontes
    al_init_ttf_addon();   // Inicializa addon de TrueType Fonts
    al_install_keyboard(); // Instala o teclado
    al_install_mouse();    // Instala o mouse

    ALLEGRO_DISPLAY* janela = al_create_display(1100, 600);
    al_set_window_title(janela, "LUNO - Inserir nome");

    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 60);
    ALLEGRO_BITMAP* fundo1 = al_load_bitmap("/home/Carol/LUNO/acessorios/nome.png");
    ALLEGRO_BITMAP* fundo2 = al_load_bitmap("/home/Carol/LUNO/acessorios/nome2.png");
    ALLEGRO_BITMAP* fundo3 = al_load_bitmap("/home/Carol/LUNO/acessorios/hist1.png"); 
    ALLEGRO_BITMAP* fundo4 = al_load_bitmap("/home/Carol/LUNO/acessorios/hist2.png"); 
    ALLEGRO_BITMAP* fundo5 = al_load_bitmap("/home/Carol/LUNO/acessorios/hist3.png"); 
    ALLEGRO_BITMAP* fundo6 = al_load_bitmap("/home/Carol/LUNO/acessorios/hist4.png"); 
    ALLEGRO_BITMAP* fundo7 = al_load_bitmap("/home/Carol/LUNO/acessorios/hist5.png"); 
    ALLEGRO_BITMAP* fundo8 = al_load_bitmap("/home/Carol/LUNO/acessorios/hist6.png"); 
    ALLEGRO_BITMAP* fundo = fundo1; // a imagem quando inicia o programa
    ALLEGRO_EVENT_QUEUE* fila_eventos = al_create_event_queue();

    if (!fila_eventos) {
        fprintf(stderr, "Falha ao criar a fila de eventos.\n");
        al_destroy_timer(timer);
        al_destroy_display(janela);
        al_destroy_bitmap(fundo);
        return -1;
    }

    al_register_event_source(fila_eventos, al_get_display_event_source(janela));
    al_register_event_source(fila_eventos, al_get_timer_event_source(timer));
    al_register_event_source(fila_eventos, al_get_keyboard_event_source());
    al_register_event_source(fila_eventos, al_get_mouse_event_source());

    ALLEGRO_FONT* fonte = al_load_ttf_font("/home/Carol/LUNO/acessorios/DejaVuSans-Bold.ttf", 30, 0);

    if (!fonte) {
        fprintf(stderr, "Falha ao carregar fonte.\n");
        al_destroy_event_queue(fila_eventos);
        al_destroy_timer(timer);
        al_destroy_display(janela);
        al_destroy_bitmap(fundo);
        return -1;
    }

    char nome[tamNome] = ""; // Inicializa a string do nome
    int posicao_texto = 0;   // Posição atual do texto digitado

    int largura_janela = al_get_display_width(janela);
    int altura_janela = al_get_display_height(janela);

    bool rodando = true;
    bool redesenhar = true;

    al_start_timer(timer);

    while (rodando) {
        ALLEGRO_EVENT evento;
        al_wait_for_event(fila_eventos, &evento);

        if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            rodando = false;
        } else if (evento.type == ALLEGRO_EVENT_KEY_DOWN) {
            if (evento.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
                rodando = false;
            } else if (evento.keyboard.keycode == ALLEGRO_KEY_ENTER) {
                // Troca a imagem ao pressionar Enter
                if (fundo == fundo1) {
                    fundo = fundo2;
                } else if (fundo == fundo2) {
                    fundo = fundo3;
                     // Limpar o nome digitado
                    nome[0] = '\0';
                    posicao_texto = 0;
                } else if (fundo == fundo3) {
                    fundo = fundo4;
                } else if (fundo == fundo4) {
                    fundo = fundo5;
                } else if (fundo == fundo5) {
                    fundo = fundo6;
                } else if (fundo == fundo6) {
                    fundo = fundo7;
                } else if (fundo == fundo7) {
                    fundo = fundo8;
                } //else {
                    //abrir_jogo();
                //}

            }
        } else if (evento.type == ALLEGRO_EVENT_KEY_CHAR) {
            // Verifica se foi digitado um caractere ASCII
            int codigo_tecla = evento.keyboard.unichar;
            if (codigo_tecla >= 32 && codigo_tecla <= 126) {
                if (posicao_texto < tamNome - 1) {
                    nome[posicao_texto++] = (char)codigo_tecla;
                    nome[posicao_texto] = '\0';
                }
            } else if (evento.keyboard.keycode == ALLEGRO_KEY_BACKSPACE) {
                
                if (posicao_texto > 0) {
                    posicao_texto--;
                    nome[posicao_texto] = '\0';
                }
            }
        } else if (evento.type == ALLEGRO_EVENT_TIMER) {
            redesenhar = true;
        }

        if (redesenhar && al_is_event_queue_empty(fila_eventos)) {
            redesenhar = false;
            al_clear_to_color(al_map_rgb(255, 255, 255));
            al_draw_scaled_bitmap(fundo, 0, 0, al_get_bitmap_width(fundo), al_get_bitmap_height(fundo),
                                  0, 0, largura_janela, altura_janela, 0);


            if (fonte) {
                al_draw_text(fonte, al_map_rgb(206, 50, 250), 700, 130, 0, nome); //desenha a fonte com a cor e as coordenadas pedidas
            }

            al_flip_display();
        }
    }

    //encerrando os eventos ao sair do loop

    al_destroy_font(fonte);
    al_destroy_event_queue(fila_eventos);
    al_destroy_timer(timer);
    al_destroy_display(janela);
    al_destroy_bitmap(fundo1);
    al_destroy_bitmap(fundo2);
    al_destroy_bitmap(fundo3);
    al_destroy_bitmap(fundo4);
    al_destroy_bitmap(fundo5);
    al_destroy_bitmap(fundo6);
    al_destroy_bitmap(fundo7);
    al_destroy_bitmap(fundo8);

    return 0;
}

