#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h> //para usar o bool
#include <allegro5/allegro.h> //biblioteca padrao allegro
#include <allegro5/allegro_primitives.h> //para formas primitivas >> desenhar o retangulo do botao
#include <allegro5/allegro_image.h> //para manipulacao de imagens
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

// ================================ STRUCT BOTAO ===================================================================================================================================================
typedef struct Botao { //struct comum para todos os botoes que vao haver no jogo
    int x, y, largura, altura;
    ALLEGRO_COLOR cor; //para manipulacao das cores dos botoes
    ALLEGRO_BITMAP *cor_normal;
    ALLEGRO_BITMAP *cor_alterada;
    bool clicado;
} Botao;


// ======================================= FUNCOES =================================================================================================================================================
int main(int argc, char **argv);
int looping(ALLEGRO_DISPLAY *janela,  ALLEGRO_EVENT_QUEUE *event_queue, ALLEGRO_TIMER *timer); //para o looping de carregamento inicial
void MENU(ALLEGRO_DISPLAY *janela, ALLEGRO_EVENT_QUEUE *event_queue, ALLEGRO_TIMER *timer); //para abrir o menu
void HISTORIA(ALLEGRO_DISPLAY *janela, ALLEGRO_EVENT_QUEUE *event_queue, ALLEGRO_TIMER *timer); //para abrir a janela da historia do jogo
void SOBRE(ALLEGRO_DISPLAY *janela, ALLEGRO_EVENT_QUEUE *event_queue, ALLEGRO_TIMER *timer); //abrir a janela sobre, contendo informacoes
void JOGO(ALLEGRO_DISPLAY *janela, ALLEGRO_EVENT_QUEUE *event_queue, ALLEGRO_TIMER *timer); //para iniciar o jogo
void game_over(ALLEGRO_DISPLAY *janela, ALLEGRO_EVENT_QUEUE *event_queue, ALLEGRO_TIMER *timer);
bool clicado(Botao *botao, int mouse_x, int mouse_y); //para verificar se o botao foi clicado. Caso sim, retorna TRUE
void desenha_Botao(Botao *botao); //para desenhar o botao na tela
void mudar_corB(Botao *botao, ALLEGRO_BITMAP *novo_fundo, bool clicado);

//============================================================= MAIN ================================================================================================================================
int main(int argc, char **argv) { //funcao main (principal), contendo todas as partes comuns e principais do jogo, evitando redundancia de codigo
    if (!al_init()) { //verificando se o allegro iniciou corretamente
        fprintf(stderr, "Falha ao inicializar Allegro.\n");
        return -1;
    }

    al_init_primitives_addon(); //inicia o addon das formas primitivas
    al_init_image_addon(); // Inicializa addon de imagens
    al_init_font_addon();  // Inicializa addon de fontes
    al_init_ttf_addon();   // Inicializa addon de TrueType Fonts
    al_install_keyboard(); // Instala o teclado
    al_install_mouse();    // Instala o mouse

    ALLEGRO_DISPLAY *janela = al_create_display(1100, 600); //criando a janela com o nome "janela" e definindo seu tamanho
    if (!janela) { //se nao der certo criar a janela, aparece essa mensagem no terminal
        fprintf(stderr, "Falha ao criar a janela.\n");
        return -1;
    }
    int largura_janela = al_get_display_width(janela); 
    int altura_janela = al_get_display_height(janela);

    ALLEGRO_TIMER *timer = al_create_timer(18.0 / 60); //criando o temporizador chamado "timer" e definindo seus ciclos de tempo
    if (!timer) {
        fprintf(stderr, "Falha ao criar o timer.\n");
        al_destroy_display(janela);
        return -1;
    }

    ALLEGRO_EVENT_QUEUE *event_queue = al_create_event_queue(); //cria eventos (como  intercoes com o mouse, teclado etc)
    if (!event_queue) {
        fprintf(stderr, "Falha ao criar a fila de eventos.\n");
        al_destroy_timer(timer);
        al_destroy_display(janela);
        return -1;
    }

    //registrando os eventos da janela, do temporizador, do teclado e do mouse
    al_register_event_source(event_queue, al_get_display_event_source(janela));
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_mouse_event_source());

    int telaPrincipal = looping(janela, event_queue, timer); // a tela que inicializa o jogo eh o looping de carregamento
    if (telaPrincipal != 0) {
        fprintf(stderr, "Erro ao executar o looping.\n");
    }

//encerrando eventos, timer e a janela
    al_destroy_event_queue(event_queue); 
    al_destroy_timer(timer);
    al_destroy_display(janela);

    return 0;
}
//================================================ BOOL CLICADO =====================================================================================================================================
bool clicado(Botao *botao, int mouse_x, int mouse_y) { //verificar se o botao foi clicado. Bool pode ser true ou false.
    return mouse_x >= botao->x && mouse_x <= botao->x + botao->largura &&
           mouse_y >= botao->y && mouse_y <= botao->y + botao->altura;
}

void desenha_Botao(Botao *botao) { //desenhar um retangulo na tela com as devidas variaveis de proporcao
    al_draw_filled_rectangle(botao->x, botao->y, botao->x + botao->largura, botao->y + botao->altura, botao->cor); 
}
//============================================================================ MUDAR A COR DOS BOTOES ================================================================================================
void mudar_corB(Botao *botao, ALLEGRO_BITMAP *novo_fundo, bool clicado) {
    botao->clicado = clicado;

    // Mudar a imagem de fundo para a nova imagem passada como parâmetro
    al_draw_scaled_bitmap(novo_fundo, 0, 0, al_get_bitmap_width(novo_fundo), al_get_bitmap_height(novo_fundo),
                          0, 0, al_get_display_width(al_get_current_display()), al_get_display_height(al_get_current_display()), 0);

    al_flip_display();
    al_rest(0.1); // Esperar 0.1 segundos antes de prosseguir
}

//===================================================== LOOPING ======================================================================================================================================
int looping(ALLEGRO_DISPLAY *janela, ALLEGRO_EVENT_QUEUE *event_queue, ALLEGRO_TIMER *timer) {
    al_set_window_title(janela, "LUNO - Carregando..."); // Define o título da janela

    // Carrega os bitmaps de fundo
    ALLEGRO_BITMAP *fundos[] = {
        //al_load_bitmap("/home/Carol/LUNO/src/acessorios/loop1.png"),
        al_load_bitmap("/home/Carol/LUNO/src/acessorios/loop2.png"),
        al_load_bitmap("/home/Carol/LUNO/src/acessorios/loop3.png"),
        al_load_bitmap("/home/Carol/LUNO/src/acessorios/loop4.png"),
        al_load_bitmap("/home/Carol/LUNO/src/acessorios/loop7.png"),
        al_load_bitmap("/home/Carol/LUNO/src/acessorios/loop8.png"),
        al_load_bitmap("/home/Carol/LUNO/src/acessorios/loop9.png")
    };

    int num_fundos = 6;

    int largura_janela = al_get_display_width(janela); 
    int altura_janela = al_get_display_height(janela);

    bool rodando = true; // Define que o loop está rodando
    bool vendo_tela = true; // Define que estamos vendo a tela atualmente

    int indice_fundo = 0; // Índice inicial do fundo

    al_clear_to_color(al_map_rgb(0, 0, 0));
    al_draw_bitmap(fundos[0], 0, 0, 0);
    al_flip_display();

    al_start_timer(timer); // Inicia o temporizador

    while (rodando) {
        ALLEGRO_EVENT evento; // Cria um evento
        al_wait_for_event(event_queue, &evento); // Espera pelo próximo evento
        if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE) { // Se o evento for fechar a janela, sai do loop
            rodando = false;
        } else if (evento.type == ALLEGRO_EVENT_TIMER) {
            vendo_tela = true;
            indice_fundo = (indice_fundo + 1) % num_fundos;
        }if(indice_fundo == num_fundos-1){
            MENU(janela, event_queue, timer); 
            rodando = false;
        }
        if (vendo_tela && al_is_event_queue_empty(event_queue)) {
            vendo_tela = false;
            // Limpa a tela para a cor branca
            al_clear_to_color(al_map_rgb(255, 255, 255));
            // Desenha o fundo atual na tela
            ALLEGRO_BITMAP *fundo_atual = fundos[indice_fundo];
            al_draw_scaled_bitmap(fundo_atual, 0, 0, al_get_bitmap_width(fundo_atual), al_get_bitmap_height(fundo_atual),
                                  0, 0, largura_janela, altura_janela, 0);
            // Atualiza a tela
            al_flip_display();
        }
    }
    // Destrói os bitmaps de fundo carregados
    for (int i = 0; i < num_fundos; ++i) {
        al_destroy_bitmap(fundos[i]);
    }

    return 0;
}

// ============================================================================================ MENU ==================================================================================================
void MENU(ALLEGRO_DISPLAY *janela, ALLEGRO_EVENT_QUEUE *event_queue, ALLEGRO_TIMER *timer) { //iniciando o menu
    al_set_window_title(janela, "LUNO - Menu"); //titulo da janela

    ALLEGRO_BITMAP *fundo = al_load_bitmap("/home/Carol/LUNO/src/acessorios/fundoMenu.png"); //fundo da janela
    
    //a largura e altura da foto de fundo sao proporcionais ao tamanho da janela
    int largura_janela = al_get_display_width(janela); 
    int altura_janela = al_get_display_height(janela);

    ALLEGRO_BITMAP *cor_normal = fundo;
    ALLEGRO_BITMAP *botaoJogar = al_load_bitmap("/home/Carol/LUNO/src/acessorios/botaoJogar.png");
    ALLEGRO_BITMAP *botaoSobre = al_load_bitmap("/home/Carol/LUNO/src/acessorios/botaoSobre.png");
    ALLEGRO_BITMAP *botaoSair = al_load_bitmap("/home/Carol/LUNO/src/acessorios/botaoSair.png");

    Botao jogar = { //criando o botao jogar do menu
        largura_janela / 2 - 150,//x
        altura_janela / 2 - 31,//y
        260,//posicao x
        60,//posicao y
        al_map_rgba(0, 0, 0, 0), // cor invisível
        cor_normal,
        botaoJogar,
        false, //nao foi clicado ainda
    };

    Botao sobre = { //criando o botao sobre do menu
        largura_janela / 2 - 150,
        altura_janela / 2 + 39,
        260,
        60,
        al_map_rgba(0, 0, 0,0),
        cor_normal,
        botaoSobre,
        false,
    };

    Botao sair = { //botao sair
        largura_janela / 2 - 150,
        altura_janela / 2 + 114,
        260,
        60,
        al_map_rgba(0, 0, 0,0),
        cor_normal,
        botaoSair,
        false,
    };

    bool rodando = true; //dizendo que rodando eh verdadeiro
    bool vendo_tela = true; //vendo_tela eh verdadeiro

    al_start_timer(timer); //iniciar temporizador

    while (rodando) { //enquando estiver rodando (rodando foi definido como true)
        ALLEGRO_EVENT evento; //criou um evento
        al_wait_for_event(event_queue, &evento); //esperar pelo evento

        if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE) { //se o evento for "fechar a janela", para de rodar
            rodando = false;
        } else if (evento.type == ALLEGRO_EVENT_TIMER) {
            vendo_tela = true;
        } else if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) { //interacoes com o mouse
            if (clicado(&jogar, evento.mouse.x, evento.mouse.y)) { //se o botao clicado for o jogar do menu
                mudar_corB(&jogar, botaoJogar, true);
                HISTORIA(janela, event_queue, timer); //abre a janela de historia
                rodando = false; //para de rodar a janela do menu
                printf("Entrou na Historia"); //mensagem de confirmacao no terminal
            } else if (clicado(&sobre, evento.mouse.x, evento.mouse.y)) { //se o clicado for o botao sobre
                mudar_corB(&sobre, botaoSobre, true);
                printf("Sobre CLICADO\n");
                SOBRE(janela, event_queue, timer); //abre a tela do sobre
                rodando = false; //para de rodar o menu
            }else if (clicado(&sair, evento.mouse.x, evento.mouse.y)) { //se o clicado for o botao sair
                mudar_corB(&sair, botaoSair, true);
                rodando = false; //para de rodar a janela, encerrando e fechando tudo
                printf("Saiu do jogo\n");
            }
            vendo_tela = true; //enquanto estiver rodando, podemos ver a tela
        }
        if (vendo_tela && al_is_event_queue_empty(event_queue)) {
            vendo_tela = false;
            al_clear_to_color(al_map_rgb(255, 255, 255));
            al_draw_scaled_bitmap(fundo, 0, 0, al_get_bitmap_width(fundo), al_get_bitmap_height(fundo), 0, 0,
                                  largura_janela, altura_janela, 0); //desenha a imagem de fudo com o tamanho proporcional ao da janela

            //definindo quais botoes serao desenhados na tela
            desenha_Botao(&jogar); 
            desenha_Botao(&sobre);
            desenha_Botao(&sair);

            al_flip_display();
        }
    }

    al_destroy_bitmap(fundo);
    al_destroy_bitmap(botaoJogar);
    al_destroy_bitmap(botaoSobre);
    al_destroy_bitmap(botaoSair);
}

//================================================================== HISTORIA =========================================================================================================================
void HISTORIA(ALLEGRO_DISPLAY *janela, ALLEGRO_EVENT_QUEUE *event_queue, ALLEGRO_TIMER *timer) {
    al_set_window_title(janela, "LUNO - Historia");

    int largura_janela = al_get_display_width(janela);
    int altura_janela = al_get_display_height(janela);

    ALLEGRO_BITMAP *fundo1 = al_load_bitmap("/home/Carol/LUNO/src/acessorios/nome.png");
    ALLEGRO_BITMAP *fundo2 = al_load_bitmap("/home/Carol/LUNO/src/acessorios/nome2.png");
    ALLEGRO_BITMAP *fundo3 = al_load_bitmap("/home/Carol/LUNO/src/acessorios/hist1.png");
    ALLEGRO_BITMAP *fundo4 = al_load_bitmap("/home/Carol/LUNO/src/acessorios/hist2.png");
    ALLEGRO_BITMAP *fundo5 = al_load_bitmap("/home/Carol/LUNO/src/acessorios/hist3.png");
    ALLEGRO_BITMAP *fundo6 = al_load_bitmap("/home/Carol/LUNO/src/acessorios/hist4.png");
    ALLEGRO_BITMAP *fundo7 = al_load_bitmap("/home/Carol/LUNO/src/acessorios/hist5.png");
    ALLEGRO_BITMAP *fundo8 = al_load_bitmap("/home/Carol/LUNO/src/acessorios/hist6.png");
    ALLEGRO_BITMAP *fundo = fundo1;

    ALLEGRO_EVENT_QUEUE *fila_eventos = al_create_event_queue();

    al_register_event_source(fila_eventos, al_get_display_event_source(janela));
    al_register_event_source(fila_eventos, al_get_timer_event_source(timer));
    al_register_event_source(fila_eventos, al_get_keyboard_event_source());

    ALLEGRO_FONT *fonte = al_load_ttf_font("/home/Carol/LUNO/src/acessorios/DejaVuSans-Bold.ttf", 30, 0); //definindo a fonte
    
    #define tamNome 20 // tamanho do nome que vai ser usado na tela historia
    char nome[tamNome] = ""; //definindo uma variavel pro nome 
    int posicao_texto = 0; //a posicao em que o texto vai ficar

    bool rodando = true;
    bool redesenhar = true;

    al_start_timer(timer);

    while (rodando) { //loop principal (todas as janelas possuem um)
        
        ALLEGRO_EVENT evento;
        al_wait_for_event(fila_eventos, &evento);

        if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE) { //fechar janela
            rodando = false;
        } else if (evento.type == ALLEGRO_EVENT_KEY_DOWN) {
            if (evento.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
                rodando = false;
            } else if (evento.keyboard.keycode == ALLEGRO_KEY_ENTER) { //troca as imagens de acordo com os cliques na tecla ENTER
                if (fundo == fundo1) {
                    fundo = fundo2;
                } else if (fundo == fundo2) {
                    nome[0] = '\0';
                    posicao_texto = 0;
                    fundo = fundo3;
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
                } else if(fundo == fundo8){
                    rodando = false;
                    JOGO(janela, event_queue, timer); //se estiver na ultima tela da historia, o usuario eh direcionado ao jogo
                }
                
            }
        } else if (evento.type == ALLEGRO_EVENT_KEY_CHAR) { //se a tecla digitada for um caractere
            int codigo_tecla = evento.keyboard.unichar;
            if (codigo_tecla >= 32 && codigo_tecla <= 126) { //verificando se eh um caractere imprimivel de acordo com a ASCII
                if (posicao_texto < tamNome - 1) {
                    nome[posicao_texto++] = (char)codigo_tecla;
                    nome[posicao_texto] = '\0';
                }
            } else if (evento.keyboard.keycode == ALLEGRO_KEY_BACKSPACE) { //opcao de apagar o nome caso o usario erre (tecla BACKSPACE)
                if (posicao_texto > 0) {
                    posicao_texto--; //vai decrementando a posicao, permitindo que seja adicionada novamente
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
                al_draw_text(fonte, al_map_rgb(206, 50, 250), 700, 130, 0, nome); //desenhar a fonte na tela com a cor e coordenadas que queremos
            }

            al_flip_display();
        }
    }
    //encerrando os eventos
    al_destroy_font(fonte);
    al_destroy_event_queue(fila_eventos);
    al_destroy_bitmap(fundo1);
    al_destroy_bitmap(fundo2);
    al_destroy_bitmap(fundo3);
    al_destroy_bitmap(fundo4);
    al_destroy_bitmap(fundo5);
    al_destroy_bitmap(fundo6);
    al_destroy_bitmap(fundo7);
    al_destroy_bitmap(fundo8);
}

//======================================== STRUCT OBSTACULOS ==========================================================================================================================================
typedef struct {
    float x, y; // Posição do obstáculo
    float width, height; // Dimensões do obstáculo
    ALLEGRO_BITMAP* bitmap; // Bitmap do obstáculo
} Obstaculo;

//========================================================================= PERSONAGEM ================================================================================================================
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

//====================================================== BOOL COLISAO =================================================================================================================================
bool colisao(float ax, float ay, float aw, float ah, float bx, float by, float bw, float bh) {
    // Verifica se há colisão entre dois retângulos
    return (ax + aw > bx && ax < bx + bw && ay + ah > by && ay < by + bh);
}

//=============================================== JOGO =================================================================================================================================================
void JOGO(ALLEGRO_DISPLAY *janela, ALLEGRO_EVENT_QUEUE *event_queue, ALLEGRO_TIMER *timer) {
    al_set_window_title(janela, "LUNO - Jogando...");
    // Inicialização básica do Allegro

    // Carrega imagens do fundo e do personagem Luno
    ALLEGRO_BITMAP* background = al_load_bitmap("/home/Carol/LUNO/src/acessorios/fundolunogame.jpg");
    ALLEGRO_BITMAP* luno1 = al_load_bitmap("/home/Carol/LUNO/src/acessorios/luno1.png");
    ALLEGRO_BITMAP* luno2 = al_load_bitmap("/home/Carol/LUNO/src/acessorios/luno2.png");
    ALLEGRO_BITMAP* luno3 = al_load_bitmap("/home/Carol/LUNO/src/acessorios/luno3.png");

    // Carrega imagens dos obstáculos
    ALLEGRO_BITMAP* obstaculo1 = al_load_bitmap("/home/Carol/LUNO/src/acessorios/obstaculo1.png");
    ALLEGRO_BITMAP* obstaculo2 = al_load_bitmap("/home/Carol/LUNO/src/acessorios/obstaculo2.png");
    ALLEGRO_BITMAP* obstaculo3 = al_load_bitmap("/home/Carol/LUNO/src/acessorios/obstaculo3.png");
    ALLEGRO_BITMAP* obstaculo4 = al_load_bitmap("/home/Carol/LUNO/src/acessorios/obstaculo4.png");
    ALLEGRO_BITMAP* obstaculo5 = al_load_bitmap("/home/Carol/LUNO/src/acessorios/obstaculo5.png");
    ALLEGRO_BITMAP* obstaculo6 = al_load_bitmap("/home/Carol/LUNO/src/acessorios/obstaculo6.png");

    // Carrega imagem do game over
    ALLEGRO_BITMAP* game_over_luno = al_load_bitmap("/home/Carol/LUNO/src/acessorios/gameoverluno.jpg");

    // Verifica se todas as imagens foram carregadas corretamente
    if (!background || !luno1 || !luno2 || !luno3 || !obstaculo1 || !obstaculo2 || !obstaculo3 || !obstaculo4 || !obstaculo5 || !obstaculo6 || !game_over_luno) {
        fprintf(stderr, "Falha ao carregar as imagens!\n");
        al_destroy_display(janela);
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
    }

    // Variáveis para controle do jogo
    float scale_factor = (float)al_get_display_height(janela) / al_get_bitmap_height(background);
    int scaled_width = al_get_bitmap_width(background) * scale_factor;
    float x = 0; // Posição inicial da imagem de fundo no eixo x

    // Variáveis para o personagem Luno
    Luno luno;
    luno.x = 30;
    luno.original_y = al_get_display_height(janela) * 0.38; // Posição inicial mais alta na tela
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
        obstaculos[i].x = al_get_display_width(janela) + i * 600 + rand() % 200; // Espaçamento inicial maior
        obstaculos[i].y = al_get_display_height(janela) - al_get_bitmap_height(obstaculo1) - 30;
        obstaculos[i].width = al_get_bitmap_width(obstaculo1) * 2.0; // Aumenta a largura
        obstaculos[i].height = al_get_bitmap_height(obstaculo1) * 2.0; // Aumenta a altura
        obstaculos[i].bitmap = NULL;
    }

    // Configuração do timer e registro de eventos
    al_register_event_source(event_queue, al_get_display_event_source(janela));
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
                luno.y_velocity = +18.0; // Define a velocidade inicial do pulo
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
            x, 0, scaled_width, al_get_display_height(janela),
            0);

        al_draw_scaled_bitmap(background,
            0, 0, al_get_bitmap_width(background), al_get_bitmap_height(background),
            x + scaled_width, 0, scaled_width, al_get_display_height(janela),
            0);

        // Atualização e desenho dos obstáculos
        for (int i = 0; i < 6; ++i) {
            obstaculos[i].x -= 8.0; // Movimento dos obstáculos para a esquerda

            // Reposicionamento contínuo dos obstáculos
            if (obstaculos[i].x + obstaculos[i].width < 0) {
                obstaculos[i].x = al_get_display_width(janela) + rand() % 100;

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
            obstaculos[i].y = al_get_display_height(janela) * 0.64;

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
                    game_over(janela,event_queue,timer);
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

    al_flip_display();
    al_rest(20.0); // Espera 2 segundos antes de encerrar o jogo

    // Liberar recursos alocados dinamicamente
    
}

//======================================================================================== GAME OVER ====================================================================================================
void game_over(ALLEGRO_DISPLAY *janela, ALLEGRO_EVENT_QUEUE *event_queue, ALLEGRO_TIMER *timer){
    al_set_window_title(janela, "LUNO - GAME OVER");

    ALLEGRO_BITMAP *fundo = al_load_bitmap("/home/Carol/LUNO/src/acessorios/gameoverluno.jpg");
    ALLEGRO_BITMAP *cor_normal = fundo;
    ALLEGRO_BITMAP *botaoJogue = al_load_bitmap("/home/Carol/LUNO/src/acessorios/gameoverluno.jpg");
    ALLEGRO_BITMAP *botaoIrmenu = al_load_bitmap("/home/Carol/LUNO/src/acessorios/gameoverluno.jpg");

    int largura_janela = al_get_display_width(janela);
    int altura_janela = al_get_display_height(janela);

     Botao Jogue = {
        largura_janela / 2 - 150,//x
        altura_janela / 2 - 31,//y
        260,//posicao x
        60,//posicao y 
        al_map_rgba(0, 0, 0, 0), // cor invisível
        cor_normal,
        botaoJogue,
        false, //nao foi clicado ainda
    };

     Botao ir_menu = {
        largura_janela /2 - 150,//x
        altura_janela / 2 + 40,//y
        260,//posicao x
        60,//posicao y 
        al_map_rgba(0, 0, 0, 0), // cor invisível
        cor_normal,
        botaoIrmenu,
        false, //nao foi clicado ainda
    };

    bool rodando = true; //dizendo que rodando eh verdadeiro
    bool vendo_tela = true; //vendo_tela eh verdadeiro

    al_start_timer(timer);

    while (rodando) { //enquando estiver rodando (rodando foi definido como true)
        ALLEGRO_EVENT evento; //criou um evento
        al_wait_for_event(event_queue, &evento); //esperar pelo evento

        if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE) { //se o evento for "fechar a janela", para de rodar
            rodando = false;
        } else if (evento.type == ALLEGRO_EVENT_TIMER) {
            vendo_tela = true;
        } else if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) { //interacoes com o mouse
            if (clicado(&Jogue, evento.mouse.x, evento.mouse.y)) { //se o botao clicado for o jogar do menu
                mudar_corB(&Jogue, botaoJogue, true);
                rodando = false;
                JOGO(janela, event_queue, timer); 
                //para de rodar a janela do menu
                printf("Jogar novamente"); //mensagem de confirmacao no terminal
            } else if (clicado(&ir_menu, evento.mouse.x, evento.mouse.y)) { //se o clicado for o botao sobre
                mudar_corB(&ir_menu, botaoIrmenu, true);
                rodando = false; //para de rodar o menu
                printf("Voltar pra menu\n");
                MENU(janela, event_queue, timer); //abre a tela do sobre
            }
            vendo_tela = true; //enquanto estiver rodando, podemos ver a tela
        }
        if (vendo_tela && al_is_event_queue_empty(event_queue)) {
            vendo_tela = false;
            al_clear_to_color(al_map_rgb(255, 255, 255));
            al_draw_scaled_bitmap(fundo, 0, 0, al_get_bitmap_width(fundo), al_get_bitmap_height(fundo), 0, 0,
                                  largura_janela, altura_janela, 0); //desenha a imagem de fudo com o tamanho proporcional ao da janela

            //definindo quais botoes serao desenhados na tela
            desenha_Botao(&Jogue); 
            desenha_Botao(&ir_menu);

            al_flip_display();
        }
    }

    al_destroy_bitmap(fundo);
    al_destroy_bitmap(botaoJogue);
    al_destroy_bitmap(botaoIrmenu);
}

//=============================================================== SOBRE =================================================================================================================================
void SOBRE(ALLEGRO_DISPLAY *janela, ALLEGRO_EVENT_QUEUE *event_queue, ALLEGRO_TIMER *timer) { //codigo da janela SOBRE
    al_set_window_title(janela, "LUNO - Sobre");

    ALLEGRO_BITMAP *fundo = al_load_bitmap("/home/Carol/LUNO/src/acessorios/fundoSobre.png"); //fundo

    int largura_janela = al_get_display_width(janela);
    int altura_janela = al_get_display_height(janela);

    ALLEGRO_BITMAP *cor_normal = fundo;
    ALLEGRO_BITMAP *botaoSair_sobre= al_load_bitmap("/home/Carol/LUNO/src/acessorios/botaoSair_sobre.png");

    Botao sair_sobre = {
        largura_janela / 2 + 420, //aqui eh o X
        altura_janela / 2 - 260, //aqui eh o Y
        80, //tamanho do x do botao
        70, //tamanho do y do botao
        al_map_rgba(0,0, 0,0), // cor invisível
        cor_normal,
        botaoSair_sobre,
        false, //nao foi clicado ainda
    };

    bool rodando = true;
    bool vendo_tela = true;

    al_start_timer(timer);


    while (rodando) {
        ALLEGRO_EVENT evento;
        al_wait_for_event(event_queue, &evento);

        if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            rodando = false;
        } else if (evento.type == ALLEGRO_EVENT_TIMER) {
            vendo_tela = true;
        } else if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
            if (clicado(&sair_sobre, evento.mouse.x, evento.mouse.y)) { //se o botao clicado for o de sair do sobre
                mudar_corB(&sair_sobre, botaoSair_sobre, true);
                rodando = false; //para de rodar
                MENU(janela, event_queue, timer); //retorna ao menu
                printf("Voltou para o menu\n");
            } 
            vendo_tela = true;
        }

        if (vendo_tela && al_is_event_queue_empty(event_queue)) {
            vendo_tela = false;
            al_clear_to_color(al_map_rgb(255, 255, 255));
            al_draw_scaled_bitmap(fundo, 0, 0, al_get_bitmap_width(fundo), al_get_bitmap_height(fundo), 0, 0,
                                  largura_janela, altura_janela, 0);

            desenha_Botao(&sair_sobre);

            al_flip_display();
        }
    }
    al_destroy_bitmap(fundo);
}
