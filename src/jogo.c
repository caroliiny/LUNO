//aqui eh onde  vamos adicionar o codigo do jogo
//#include "jogo.h" >> quando criarmos o arquivo la dentro do include
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>

//---------------------------------INICIALIZAÇÕES---------------------------------------------

typedef struct {
    float x, y; // Posição do obstáculo
    float width, height; // Dimensões do obstáculo
    ALLEGRO_BITMAP* bitmap; // Bitmap do obstáculo
} Obstaculo;

typedef struct {
    float x, y; // Posição do personagem Luno
    float original_y; // Posição original do Luno quando não está pulando
    float width, height; // Dimensões do personagem Luno
    ALLEGRO_BITMAP* bitmap[3]; // Bitmaps dos frames de animação do personagem Luno
    int current_frame; // Índice do frame atual
    float y_velocity; // Velocidade vertical do pulo
    bool pulando; // Flag indicando se o Luno está pulando
    bool game_over; // Flag indicando se o jogo acabou
} Luno;

bool colisao(float ax, float ay, float aw, float ah, float bx, float by, float bw, float bh) {
    // Verifica se há colisão entre dois retângulos
    return (ax + aw > bx && ax < bx + bw && ay + ah > by && ay < by + bh);
}

int main() {
    // Inicialização básica do Allegro
    if (!al_init()) {
        fprintf(stderr, "Falha ao inicializar Allegro.\n");
        return -1;
    }

    // Criação da janela principal
    ALLEGRO_DISPLAY* display = al_create_display(1100, 600);
    if (!display) {
        fprintf(stderr, "Falha ao criar janela.\n");
        return -1;
    }

    // Inicialização do addon de imagem do Allegro
    if (!al_init_image_addon()) {
        fprintf(stderr, "Falha ao inicializar addon de imagem do Allegro.\n");
        al_destroy_display(display);
        return -1;
    }

    // Criação da fila de eventos do Allegro
    ALLEGRO_EVENT_QUEUE* event_queue = al_create_event_queue();
    if (!event_queue) {
        fprintf(stderr, "Falha ao criar fila de eventos do Allegro.\n");
        al_destroy_display(display);
        return -1;
    }

    // Instalação do teclado
    if (!al_install_keyboard()) {
        fprintf(stderr, "Falha ao inicializar o teclado.\n");
        al_destroy_display(display);
        al_destroy_event_queue(event_queue);
        return -1;
    }

    // Carrega imagens do fundo e do personagem Luno
    ALLEGRO_BITMAP* background = al_load_bitmap("fundolunogame.jpg");
    ALLEGRO_BITMAP* luno1 = al_load_bitmap("luno1.png");
    ALLEGRO_BITMAP* luno2 = al_load_bitmap("luno2.png");
    ALLEGRO_BITMAP* luno3 = al_load_bitmap("luno3.png");

    // Carrega imagens dos obstáculos
    ALLEGRO_BITMAP* obstaculo1 = al_load_bitmap("obstaculo1.png");
    ALLEGRO_BITMAP* obstaculo2 = al_load_bitmap("obstaculo2.png");
    ALLEGRO_BITMAP* obstaculo3 = al_load_bitmap("obstaculo3.png");
    ALLEGRO_BITMAP* obstaculo4 = al_load_bitmap("obstaculo4.png");
    ALLEGRO_BITMAP* obstaculo5 = al_load_bitmap("obstaculo5.png");
    ALLEGRO_BITMAP* obstaculo6 = al_load_bitmap("obstaculo6.png");

    // Carrega imagem do game over
    ALLEGRO_BITMAP* game_over_luno = al_load_bitmap("gameoverluno.jpg");

    // Verifica se todas as imagens foram carregadas corretamente
    if (!background || !luno1 || !luno2 || !luno3 || !obstaculo1 || !obstaculo2 || !obstaculo3 || !obstaculo4 || !obstaculo5 || !obstaculo6 || !game_over_luno) {
        fprintf(stderr, "Falha ao carregar as imagens!\n");
        al_destroy_display(display);
        al_destroy_bitmap(background);
        al_destroy_bitmap(luno1);
        al_destroy_bitmap(luno2);
        al_destroy_bitmap(luno3);
        al_destroy_bitmap(obstaculo1);
        al_destroy_bitmap(obstaculo2);
        al_destroy_bitmap(obstaculo3);
        al_destroy_bitmap(obstaculo4);
        al_destroy_bitmap(obstaculo5);
        al_destroy_bitmap(obstaculo6);
        al_destroy_bitmap(game_over_luno);
        return -1;
    }

    // Variáveis para controle do jogo
    float scale_factor = (float)al_get_display_height(display) / al_get_bitmap_height(background);
    int scaled_width = al_get_bitmap_width(background) * scale_factor;
    float x = 0; // Posição inicial da imagem de fundo no eixo x

    // Variáveis para o personagem Luno
    Luno luno;
    luno.x = 30;
    luno.original_y = al_get_display_height(display) * 0.38; // Posição inicial mais alta na tela
    luno.y = luno.original_y;
    luno.width = al_get_bitmap_width(luno1) * 0.25;
    luno.height = al_get_bitmap_height(luno1) * 0.25;
    luno.bitmap[0] = luno1;
    luno.bitmap[1] = luno2;
    luno.bitmap[2] = luno3;
    luno.current_frame = 0;
    luno.y_velocity = 0.0;
    luno.pulando = false;
    luno.game_over = false; // Inicialmente o jogo não acabou

    // Inicialização dos obstáculos
    Obstaculo obstaculos[6];
    for (int i = 0; i < 6; ++i) {
        obstaculos[i].x = al_get_display_width(display) + i * 600 + rand() % 200; // Espaçamento inicial maior
        obstaculos[i].y = al_get_display_height(display) - al_get_bitmap_height(obstaculo1) - 30;
        obstaculos[i].width = al_get_bitmap_width(obstaculo1) * 2.0; // Aumenta a largura
        obstaculos[i].height = al_get_bitmap_height(obstaculo1) * 2.0; // Aumenta a altura
        obstaculos[i].bitmap = NULL;
    }

    // Configuração do timer e registro de eventos
    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 60.0);
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_keyboard_event_source()); // Registro do teclado
    al_start_timer(timer);

    bool space_pressed = false; // Flag para verificar se a tecla espaço foi pressionada

    while (!luno.game_over) {
        ALLEGRO_EVENT event;
        al_wait_for_event(event_queue, &event);

        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            break;
        }
        else if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
            // Evento de tecla pressionada
            if (event.keyboard.keycode == ALLEGRO_KEY_SPACE && !luno.pulando) {
                // Se a tecla espaço foi pressionada e o Luno não está pulando
                luno.y_velocity = -18.0; // Define a velocidade inicial do pulo
                luno.pulando = true;
                space_pressed = true;
            }
        }
        else if (event.type == ALLEGRO_EVENT_KEY_UP) {
            // Evento de tecla solta
            if (event.keyboard.keycode == ALLEGRO_KEY_SPACE) {
                space_pressed = false;
            }
        }

        // Movimento horizontal da imagem de fundo
        x -= 8.0;
        if (x <= -scaled_width) {
            x += scaled_width;
        }

        // Atualização da animação do personagem Luno
        if (event.type == ALLEGRO_EVENT_TIMER) {
            if (!luno.pulando && !space_pressed) {
                // Atualiza o frame da animação a cada 10 eventos de timer
                const int frame_interval = 10;
                static int frame_count = 0;
                frame_count++;
                if (frame_count >= frame_interval) {
                    frame_count = 0;
                    luno.current_frame = (luno.current_frame + 1) % 3;
                }
            }

            // Aplicação da gravidade ao pulo
            luno.y += luno.y_velocity;
            luno.y_velocity += 0.55; // Aceleração da gravidade

            // Verifica se o Luno chegou ao chão
            if (luno.y >= luno.original_y) {
                luno.y = luno.original_y; // Garante que o Luno não passe do chão
                luno.y_velocity = 0.0; // Reseta a velocidade vertical
                luno.pulando = false; // Finaliza o pulo
            }
        }

        // Desenho da imagem de fundo duas vezes para efeito contínuo
        al_draw_scaled_bitmap(background,
            0, 0, al_get_bitmap_width(background), al_get_bitmap_height(background),
            x, 0, scaled_width, al_get_display_height(display),
            0);

        al_draw_scaled_bitmap(background,
            0, 0, al_get_bitmap_width(background), al_get_bitmap_height(background),
            x + scaled_width, 0, scaled_width, al_get_display_height(display),
            0);

        // Atualização e desenho dos obstáculos
        for (int i = 0; i < 6; ++i) {
            obstaculos[i].x -= 8.0; // Movimento dos obstáculos para a esquerda

            // Reposicionamento contínuo dos obstáculos
            if (obstaculos[i].x + obstaculos[i].width < 0) {
                obstaculos[i].x = al_get_display_width(display) + rand() % 100;

                // Escolha aleatória do tipo de obstáculo
                switch (rand() % 6 + 1) {
                case 1:
                    obstaculos[i].bitmap = obstaculo1;
                    break;
                case 2:
                    obstaculos[i].bitmap = obstaculo2;
                    break;
                case 3:
                    obstaculos[i].bitmap = obstaculo3;
                    break;
                case 4:
                    obstaculos[i].bitmap = obstaculo4;
                    break;
                case 5:
                    obstaculos[i].bitmap = obstaculo5;
                    break;
                case 6:
                    obstaculos[i].bitmap = obstaculo6;
                    break;
                }
            }
            // Reposiciona o obstáculo na altura que quiser
            obstaculos[i].y = al_get_display_height(display) * 0.64;

            // Desenho dos obstáculos
            if (obstaculos[i].bitmap) {
                al_draw_scaled_bitmap(obstaculos[i].bitmap,
                    0, 0, al_get_bitmap_width(obstaculos[i].bitmap), al_get_bitmap_height(obstaculos[i].bitmap),
                    obstaculos[i].x, obstaculos[i].y, obstaculos[i].width, obstaculos[i].height,
                    0);

                // Verifica colisão entre Luno e obstáculo
                if (colisao(luno.x, luno.y, luno.width, luno.height, obstaculos[i].x, obstaculos[i].y, obstaculos[i].width, obstaculos[i].height)) {
                    luno.game_over = true;
                    printf("Luno perdeu!\n");
                    // Aqui você pode implementar a lógica para o que acontece quando Luno colide com um obstáculo
                    // Por exemplo, parar o jogo, mostrar uma mensagem, etc.
                }
            }
        }

        // Desenho do personagem Luno na tela com escala reduzida
        if (luno.pulando) {
            // Se o Luno está pulando, desenha apenas o bitmap de pulo (luno1.png)
            al_draw_scaled_bitmap(luno1,
                0, 0, al_get_bitmap_width(luno1), al_get_bitmap_height(luno1),
                luno.x, luno.y, luno.width, luno.height,
                0);
        }
        else {
            // Caso contrário, continua com a lógica de animação baseada nos frames
            al_draw_scaled_bitmap(luno.bitmap[luno.current_frame],
                0, 0, al_get_bitmap_width(luno.bitmap[luno.current_frame]), al_get_bitmap_height(luno.bitmap[luno.current_frame]),
                luno.x, luno.y, luno.width, luno.height,
                0);
        }

        al_flip_display();
    }

    // Desenho da tela de game over
    al_draw_scaled_bitmap(game_over_luno,
        0, 0, al_get_bitmap_width(game_over_luno), al_get_bitmap_height(game_over_luno),
        0, 0, al_get_display_width(display), al_get_display_height(display),
        0);

    al_flip_display();
    al_rest(2.0); // Espera 2 segundos antes de encerrar o jogo

    // Liberar recursos alocados dinamicamente
    al_destroy_bitmap(background);
    al_destroy_bitmap(luno1);
    al_destroy_bitmap(luno2);
    al_destroy_bitmap(luno3);
    al_destroy_bitmap(obstaculo1);
    al_destroy_bitmap(obstaculo2);
    al_destroy_bitmap(obstaculo3);
    al_destroy_bitmap(obstaculo4);
    al_destroy_bitmap(obstaculo5);
    al_destroy_bitmap(obstaculo6);
    al_destroy_bitmap(game_over_luno);
    al_destroy_display(display);
    al_destroy_event_queue(event_queue);

    return 0;
}
