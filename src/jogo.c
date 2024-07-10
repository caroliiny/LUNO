//aqui eh onde  vamos adicionar o codigo do jogo
//#include "jogo.h" >> quando criarmos o arquivo la dentro do include
#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>

int main() {
    al_init();
    al_init_font_addon();
    al_init_image_addon();

    ALLEGRO_DISPLAY* display = al_create_display(1100, 600);
    al_set_window_position(display, 125, 75);

    int display_width = al_get_display_width(display);
    int display_height = al_get_display_height(display);

    ALLEGRO_FONT* font = al_create_builtin_font();
    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 60.0);

    ALLEGRO_EVENT_QUEUE* event_queue = al_create_event_queue();
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_start_timer(timer);

    // Carregar imagens do fundo e do personagem Luno
    ALLEGRO_BITMAP* background = al_load_bitmap("C:\\Users\\aretu\\Downloads\\LunoGame\\fundolunogame1.jpg");
    ALLEGRO_BITMAP* luno1 = al_load_bitmap("C:\\Users\\aretu\\Downloads\\LunoGame\\luno1.png");
    ALLEGRO_BITMAP* luno2 = al_load_bitmap("C:\\Users\\aretu\\Downloads\\LunoGame\\luno2.png");
    ALLEGRO_BITMAP* luno3 = al_load_bitmap("C:\\Users\\aretu\\Downloads\\LunoGame\\luno3.png");

    if (!background || !luno1 || !luno2 || !luno3) {
        fprintf(stderr, "Falha ao carregar as imagens!\n");
        return -1;
    }

    float scale_factor = (float)display_height / al_get_bitmap_height(background); // Fator de escala para ajustar à altura da tela
    int scaled_width = al_get_bitmap_width(background) * scale_factor; // Largura escalada da imagem de fundo

    float x = 0; // Posição inicial da imagem de fundo no eixo x

    // Posicionar o personagem Luno no canto inferior esquerdo da tela
    float luno_x = 30;
    float luno_y = display_height - al_get_bitmap_height(luno1) * 0.33; // Ajuste para reduzir o tamanho do personagem

    int current_frame = 0; // Frame atual da animação do personagem Luno
    ALLEGRO_BITMAP* current_bitmap = luno1; // Bitmap atual do personagem Luno

    while (true) {
        ALLEGRO_EVENT event;
        al_wait_for_event(event_queue, &event);
        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            break;
        }

        al_clear_to_color(al_map_rgb(255, 255, 255));

        // Movimento horizontal da imagem de fundo para a esquerda
        x -= 7.0; // Ajuste a velocidade de movimento conforme desejado

        // Reposicionamento contínuo da imagem de fundo
        if (x <= -scaled_width) {
            x += scaled_width; // Reposiciona a imagem no lado direito para continuar o efeito contínuo
        }

        // Desenhar a imagem de fundo duas vezes em sequência para parecer contínua
        al_draw_scaled_bitmap(background,
            0, 0, al_get_bitmap_width(background), al_get_bitmap_height(background),
            x, 0, scaled_width, display_height,
            0);

        al_draw_scaled_bitmap(background,
            0, 0, al_get_bitmap_width(background), al_get_bitmap_height(background),
            x + scaled_width, 0, scaled_width, display_height,
            0);

        // Atualização da animação do personagem Luno
        current_frame++;
        if (current_frame % 6 == 0) { // Ajuste o número para controlar a velocidade da animação
            if (current_bitmap == luno1) {
                current_bitmap = luno2;
            }
            else if (current_bitmap == luno2) {
                current_bitmap = luno3;
            }
            else {
                current_bitmap = luno1;
            }
        }

        // Desenhar o personagem Luno na tela com escala reduzida
        float luno_scale = 0.25; // Fator de escala para reduzir o tamanho do personagem
        int luno_width = al_get_bitmap_width(current_bitmap) * luno_scale;
        int luno_height = al_get_bitmap_height(current_bitmap) * luno_scale;
        al_draw_scaled_bitmap(current_bitmap,
            0, 0, al_get_bitmap_width(current_bitmap), al_get_bitmap_height(current_bitmap),
            luno_x, luno_y, luno_width, luno_height,
            0);

        al_flip_display();
    }

    al_destroy_bitmap(background);
    al_destroy_bitmap(luno1);
    al_destroy_bitmap(luno2);
    al_destroy_bitmap(luno3);
    al_destroy_font(font);
    al_destroy_display(display);
    al_destroy_event_queue(event_queue);

    al_shutdown_image_addon();

    return 0;
}
