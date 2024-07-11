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
    float largura, altura; // Dimensões do obstáculo
    ALLEGRO_BITMAP* bitmap; // Bitmap do obstáculo
    bool ativo; // Indica se o obstáculo está ativo
} Obstaculo;

typedef struct {
    float x, y; // Posição do personagem Luno
    float y_original; // Posição original do Luno quando não está pulando
    float largura, altura; // Dimensões do personagem Luno
    ALLEGRO_BITMAP* bitmap[3]; // Bitmaps dos frames de animação do personagem Luno
    int frame_atual; // Índice do frame atual
    float velocidade_y; // Velocidade vertical do pulo
    bool pulando; // Flag indicando se o Luno está pulando
    bool jogo_encerrado; // Flag indicando se o jogo acabou
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
    ALLEGRO_DISPLAY* janela = al_create_display(1100, 600);
    if (!janela) {
        fprintf(stderr, "Falha ao criar janela.\n");
        return -1;
    }

    // Inicialização do addon de imagem do Allegro
    if (!al_init_image_addon()) {
        fprintf(stderr, "Falha ao inicializar addon de imagem do Allegro.\n");
        al_destroy_display(janela);
        return -1;
    }

    // Criação da fila de eventos do Allegro
    ALLEGRO_EVENT_QUEUE* fila_eventos = al_create_event_queue();
    if (!fila_eventos) {
        fprintf(stderr, "Falha ao criar fila de eventos do Allegro.\n");
        al_destroy_display(janela);
        return -1;
    }

    // Instalação do teclado
    if (!al_install_keyboard()) {
        fprintf(stderr, "Falha ao inicializar o teclado.\n");
        al_destroy_display(janela);
        al_destroy_event_queue(fila_eventos);
        return -1;
    }

    // Carrega imagens do fundo e do personagem Luno
    ALLEGRO_BITMAP* fundo_jogo = al_load_bitmap("fundolunogame.jpg");
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
    ALLEGRO_BITMAP* obstaculo7 = al_load_bitmap("obstaculo7.png");
    ALLEGRO_BITMAP* obstaculo8 = al_load_bitmap("obstaculo8.png");
    ALLEGRO_BITMAP* obstaculo9 = al_load_bitmap("obstaculo9.png");
    ALLEGRO_BITMAP* obstaculo10 = al_load_bitmap("obstaculo10.png");
    ALLEGRO_BITMAP* obstaculo11 = al_load_bitmap("obstaculo11.png");
    ALLEGRO_BITMAP* obstaculo12 = al_load_bitmap("obstaculo12.png");
    ALLEGRO_BITMAP* obstaculo13 = al_load_bitmap("obstaculo13.png");
    ALLEGRO_BITMAP* obstaculo14 = al_load_bitmap("obstaculo14.png");
    ALLEGRO_BITMAP* obstaculo15 = al_load_bitmap("obstaculo15.png");
    ALLEGRO_BITMAP* obstaculo16 = al_load_bitmap("obstaculo16.png");

    // Carrega imagem do game over
    ALLEGRO_BITMAP* game_over_luno = al_load_bitmap("gameoverluno.jpg");

    // Verifica se todas as imagens foram carregadas corretamente
    if (!fundo_jogo || !luno1 || !luno2 || !luno3 || !obstaculo1 || !obstaculo2 || !obstaculo3 || !obstaculo4 || !obstaculo5 || !obstaculo6 || !obstaculo7 || !obstaculo8 || !obstaculo9 || !obstaculo10 || !obstaculo11 || !obstaculo12 || !obstaculo13 || !obstaculo14 || !obstaculo15 || !obstaculo16 || !game_over_luno) {
        fprintf(stderr, "Falha ao carregar as imagens!\n");
        al_destroy_display(janela);
        al_destroy_bitmap(fundo_jogo);
        al_destroy_bitmap(luno1);
        al_destroy_bitmap(luno2);
        al_destroy_bitmap(luno3);
        al_destroy_bitmap(obstaculo1);
        al_destroy_bitmap(obstaculo2);
        al_destroy_bitmap(obstaculo3);
        al_destroy_bitmap(obstaculo4);
        al_destroy_bitmap(obstaculo5);
        al_destroy_bitmap(obstaculo6);
        al_destroy_bitmap(obstaculo7);
        al_destroy_bitmap(obstaculo8);
        al_destroy_bitmap(obstaculo9);
        al_destroy_bitmap(obstaculo10);
        al_destroy_bitmap(obstaculo11);
        al_destroy_bitmap(obstaculo12);
        al_destroy_bitmap(obstaculo13);
        al_destroy_bitmap(obstaculo14);
        al_destroy_bitmap(obstaculo15);
        al_destroy_bitmap(obstaculo16);
        al_destroy_bitmap(game_over_luno);
        return -1;
    }

    // Variáveis para controle do jogo
    float fator_escala = (float)al_get_display_height(janela) / al_get_bitmap_height(fundo_jogo);
    int largura_escalada = al_get_bitmap_width(fundo_jogo) * fator_escala;
    float posicao_x = 0; // Posição inicial da imagem de fundo no eixo x

    // Variáveis para o personagem Luno
    Luno luno;
    luno.x = 30;
    luno.y_original = al_get_display_height(janela) * 0.38; // Posição inicial mais alta na tela
    luno.y = luno.y_original;
    luno.largura = al_get_bitmap_width(luno1) * 0.25;
    luno.altura = al_get_bitmap_height(luno1) * 0.25;
    luno.bitmap[0] = luno1;
    luno.bitmap[1] = luno2;
    luno.bitmap[2] = luno3;
    luno.frame_atual = 0;
    luno.velocidade_y = 0.0;
    luno.pulando = false;
    luno.jogo_encerrado = false; // Inicialmente o jogo não acabou

    // Inicialização dos obstáculos
    Obstaculo obstaculos[16]; // Aumente o tamanho do array para 32 para suportar duplicação
    ALLEGRO_BITMAP* obstaculo_bitmaps[16] = { obstaculo1, obstaculo2, obstaculo3, obstaculo4, obstaculo5, obstaculo6, obstaculo7, obstaculo8, obstaculo9, obstaculo10, obstaculo11, obstaculo12, obstaculo13, obstaculo14, obstaculo15, obstaculo16 };

    // Semente aleatória baseada no tempo atual
    srand(time(NULL));

    for (int i = 0; i < 16; ++i) {
        int indice_bitmap = rand() % 16; // Escolhe aleatoriamente um bitmap de obstáculo
        obstaculos[i].x = al_get_display_width(janela) + i * 1200 + rand() % 400; // Espaçamento inicial maior
        obstaculos[i].y = al_get_display_height(janela) - al_get_bitmap_height(obstaculo1) - 30;
        obstaculos[i].largura = al_get_bitmap_width(obstaculo1) * 2.2; // Aumenta a largura
        obstaculos[i].altura = al_get_bitmap_height(obstaculo1) * 2.2; // Aumenta a altura
        obstaculos[i].bitmap = obstaculo_bitmaps[indice_bitmap]; // Atribui um bitmap aleatório a cada obstáculo
        obstaculos[i].ativo = true; // Marca o obstáculo como ativo

        // Ajuste de tamanho específico para obstáculos de 10 a 16
        if (indice_bitmap >= 9 && indice_bitmap < 16) {
            obstaculos[i].largura = al_get_bitmap_width(obstaculo_bitmaps[indice_bitmap]) * 1.8; // Aumenta a largura em 80%
            obstaculos[i].altura = al_get_bitmap_height(obstaculo_bitmaps[indice_bitmap]) * 1.8; // Aumenta a altura em 80%
        }
    }

    // Configuração do timer e registro de eventos
    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 60.0);
    al_register_event_source(fila_eventos, al_get_display_event_source(janela));
    al_register_event_source(fila_eventos, al_get_timer_event_source(timer));
    al_register_event_source(fila_eventos, al_get_keyboard_event_source()); // Registro do teclado
    al_start_timer(timer);

    bool tecla_espaço_pressionada = false; // Flag para verificar se a tecla espaço foi pressionada

    while (!luno.jogo_encerrado) {
        ALLEGRO_EVENT evento;
        al_wait_for_event(fila_eventos, &evento);

        if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            break;
        }
        else if (evento.type == ALLEGRO_EVENT_KEY_DOWN) {
            // Evento de tecla pressionada
            if (evento.keyboard.keycode == ALLEGRO_KEY_SPACE && !luno.pulando) {
                // Se a tecla espaço foi pressionada e o Luno não está pulando
                luno.velocidade_y = -17.5; // Define a velocidade inicial do pulo
                luno.pulando = true;
                tecla_espaço_pressionada = true;
            }
        }
        else if (evento.type == ALLEGRO_EVENT_KEY_UP) {
            // Evento de tecla solta
            if (evento.keyboard.keycode == ALLEGRO_KEY_SPACE) {
                tecla_espaço_pressionada = false;
            }
        }

        // Movimento horizontal da imagem de fundo
        posicao_x -= 8.0;
        if (posicao_x <= -largura_escalada) {
 
            posicao_x += largura_escalada;
        }

        // Atualização da animação do personagem Luno
        if (evento.type == ALLEGRO_EVENT_TIMER) {
            if (!luno.pulando && !tecla_espaço_pressionada) {
                // Atualiza o frame da animação a cada 10 eventos de timer
                const int intervalo_frame = 10;
                static int contagem_frame = 0;
                contagem_frame++;
                if (contagem_frame >= intervalo_frame) {
                    contagem_frame = 0;
                    luno.frame_atual = (luno.frame_atual + 1) % 3;
                }
            }

            // Aplicação da gravidade ao pulo
            luno.y += luno.velocidade_y;
            luno.velocidade_y += 0.55; // Aceleração da gravidade

            // Verifica se o Luno chegou ao chão
            if (luno.y >= luno.y_original) {
                luno.y = luno.y_original; // Garante que o Luno não passe do chão
                luno.velocidade_y = 0.0; // Reseta a velocidade vertical
                luno.pulando = false; // Finaliza o pulo
            }
        }

        // Desenho da imagem de fundo duas vezes para efeito contínuo
        al_draw_scaled_bitmap(fundo_jogo,
            0, 0, al_get_bitmap_width(fundo_jogo), al_get_bitmap_height(fundo_jogo),
            posicao_x, 0, largura_escalada, al_get_display_height(janela),
            0);

        al_draw_scaled_bitmap(fundo_jogo,
            0, 0, al_get_bitmap_width(fundo_jogo), al_get_bitmap_height(fundo_jogo),
            posicao_x + largura_escalada, 0, largura_escalada, al_get_display_height(janela),
            0);

        // Atualização e desenho dos obstáculos
        for (int i = 0; i < 16; ++i) {
            if (obstaculos[i].ativo) {
                obstaculos[i].x -= 8.0; // Movimento dos obstáculos para a esquerda

                //y dos obstaculos
                obstaculos[i].y = al_get_display_height(janela) - obstaculos[i].altura - 110; 

                // Desenho dos obstáculos
                if (obstaculos[i].bitmap) {
                    al_draw_scaled_bitmap(obstaculos[i].bitmap,
                        0, 0, al_get_bitmap_width(obstaculos[i].bitmap), al_get_bitmap_height(obstaculos[i].bitmap),
                        obstaculos[i].x, obstaculos[i].y, obstaculos[i].largura, obstaculos[i].altura,
                        0);

                    // Verifica colisão entre Luno e obstáculo
                    if (colisao(luno.x, luno.y, luno.largura, luno.altura, obstaculos[i].x, obstaculos[i].y, obstaculos[i].largura, obstaculos[i].altura)) {
                        luno.jogo_encerrado = true;
                        printf("Luno perdeu!\n");
                        // Aqui você pode implementar a lógica para o que acontece quando Luno colide com um obstáculo
                        // Por exemplo, parar o jogo, mostrar uma mensagem, etc.
                    }
                }

                // Verifica se o obstáculo saiu da tela
                if (obstaculos[i].x + obstaculos[i].largura < 0) {
                    obstaculos[i].ativo = false; // Desativa o obstáculo
                }
            }
        }

        // Desenho do personagem Luno na tela com escala reduzida
        if (luno.pulando) {
            // Se o Luno está pulando, desenha apenas o bitmap de pulo (luno1.png)
            al_draw_scaled_bitmap(luno1,
                0, 0, al_get_bitmap_width(luno1), al_get_bitmap_height(luno1),
                luno.x, luno.y, luno.largura, luno.altura,
                0);
        }
        else {
            // Caso contrário, continua com a lógica de animação baseada nos frames
            al_draw_scaled_bitmap(luno.bitmap[luno.frame_atual],
                0, 0, al_get_bitmap_width(luno.bitmap[luno.frame_atual]), al_get_bitmap_height(luno.bitmap[luno.frame_atual]),
                luno.x, luno.y, luno.largura, luno.altura,
                0);
        }

        al_flip_display();
    }

    // Desenho da tela de game over
    al_draw_scaled_bitmap(game_over_luno,
        0, 0, al_get_bitmap_width(game_over_luno), al_get_bitmap_height(game_over_luno),
        0, 0, al_get_display_width(janela), al_get_display_height(janela),
        0);

    al_flip_display();
    al_rest(2.0); // Espera 2 segundos antes de encerrar o jogo

    // Liberar recursos alocados dinamicamente
    al_destroy_bitmap(fundo_jogo);
    al_destroy_bitmap(luno1);
    al_destroy_bitmap(luno2);
    al_destroy_bitmap(luno3);
    al_destroy_bitmap(obstaculo1);
    al_destroy_bitmap(obstaculo2);
    al_destroy_bitmap(obstaculo3);
    al_destroy_bitmap(obstaculo4);
    al_destroy_bitmap(obstaculo5);
    al_destroy_bitmap(obstaculo6);
    al_destroy_bitmap(obstaculo7);
    al_destroy_bitmap(obstaculo8); 
    al_destroy_bitmap(obstaculo9); 
    al_destroy_bitmap(obstaculo10); 
    al_destroy_bitmap(obstaculo11); 
    al_destroy_bitmap(obstaculo12); 
    al_destroy_bitmap(obstaculo13); 
    al_destroy_bitmap(obstaculo14); 
    al_destroy_bitmap(obstaculo15); 
    al_destroy_bitmap(obstaculo16); 
    al_destroy_bitmap(game_over_luno);
    al_destroy_display(janela);
    al_destroy_event_queue(fila_eventos);

    return 0;
}

