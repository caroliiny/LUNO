/*Criamos o jogo em somente um arquivo pois nao sabiamos como manipular diversos arquivos e transformar em um so. Infelizmente possui algumas redundancias que, esperamos,
com o tempo, aprenderemos a resolver. A estrutura desse arquivo compoe:
Bibliotecas, struct para botoes, definicao de funcoes que serao utilizadas, funcao main, ORGANIZACAO: abrir_menu(), abrir_historia(), iniciar_jogo(), abrir_sobre();*/

#include <stdio.h>
#include <stdbool.h> //para usar o bool
#include <allegro5/allegro.h> //biblioteca padrao allegro
#include <allegro5/allegro_primitives.h> //para formas primitivas >> desenhar o retangulo do botao
#include <allegro5/allegro_image.h> //para manipulacao de imagens
//#include <allegro5/allegro_audio.h>
//#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

#define tamNome 20 // tamanho do nome que vai ser usado na tela historia

typedef struct Botao { //struct comum para todos os botoes que vao haver no jogo
    int x, y, largura, altura;
    ALLEGRO_COLOR cor; //para manipulacao das cores dos botoes
} Botao;

// defininido todas as funcoes do jogo
int main(int argc, char **argv);
int abrir_menu(ALLEGRO_DISPLAY *janela, ALLEGRO_EVENT_QUEUE *event_queue, ALLEGRO_TIMER *timer); //para abrir o menu
int abrir_historia(ALLEGRO_DISPLAY *janela, ALLEGRO_EVENT_QUEUE *event_queue, ALLEGRO_TIMER *timer); //para abrir a janela da historia do jogo
int abrir_sobre(ALLEGRO_DISPLAY *janela, ALLEGRO_EVENT_QUEUE *event_queue, ALLEGRO_TIMER *timer); //abrir a janela sobre, contendo informacoes
int iniciar_jogo(ALLEGRO_DISPLAY *janela, ALLEGRO_EVENT_QUEUE *event_queue, ALLEGRO_TIMER *timer); //para iniciar o jogo
bool clicado(Botao *botao, int mouse_x, int mouse_y); //para verificar se o botao foi clicado. Caso sim, retorna TRUE
void desenha_Botao(Botao *botao); //para desenhar o botao na tela

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

    ALLEGRO_TIMER *timer = al_create_timer(1.0 / 60); //criando o temporizador chamado "timer" e definindo seus ciclos de tempo
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

    int telaPrincipal = abrir_menu(janela, event_queue, timer); // a tela que inicializa o jogo eh o menu
    if (telaPrincipal != 0) {
        fprintf(stderr, "Erro ao executar o menu.\n");
    }

//encerrando eventos, timer e a janela
    al_destroy_event_queue(event_queue); 
    al_destroy_timer(timer);
    al_destroy_display(janela);

    return 0;
}

bool clicado(Botao *botao, int mouse_x, int mouse_y) { //verificar se o botao foi clicado. Bool pode ser true ou false.
    return mouse_x >= botao->x && mouse_x <= botao->x + botao->largura &&
           mouse_y >= botao->y && mouse_y <= botao->y + botao->altura;
}

void desenha_Botao(Botao *botao) { //desenhar um retangulo na tela com as devidas variaveis de proporcao
    al_draw_filled_rectangle(botao->x, botao->y, botao->x + botao->largura, botao->y + botao->altura, botao->cor); 
}


// ============================= MENU ======================================
int abrir_menu(ALLEGRO_DISPLAY *janela, ALLEGRO_EVENT_QUEUE *event_queue, ALLEGRO_TIMER *timer) { //iniciando o menu
    al_set_window_title(janela, "LUNO - Menu"); //titulo da janela

    ALLEGRO_BITMAP *fundo = al_load_bitmap("/home/Carol/LUNO/acessorios/fundo.png"); //fundo da janela
    if (!fundo) {
        fprintf(stderr, "Falha ao carregar a imagem de fundo.\n");
        return -1;
    }
    
    //a largura e altura da foto de fundo sao proporcionais ao tamanho da janela
    int largura_janela = al_get_display_width(janela); 
    int altura_janela = al_get_display_height(janela);

    Botao jogar = { //criando o botao jogar do menu
        largura_janela / 2 - 150,//x
        altura_janela / 2 - 31,//y
        260,//posicao x
        60,//posicao y
        al_map_rgba(0, 0, 0, 0), // cor invisível
    };

    Botao sobre = { //criando o botao sobre do menu
        largura_janela / 2 - 150,
        altura_janela / 2 + 39,
        260,
        60,
        al_map_rgba(0, 0, 0,0),
    };

    Botao sair = { //botao sair
        largura_janela / 2 - 150,
        altura_janela / 2 + 114,
        260,
        60,
        al_map_rgba(0, 0, 0,0),
    };


    bool rodando = true; //dizendo que rodando eh verdadeiro
    bool vendo_tela = true; //vendo_tela eh verdadeiro

    al_start_timer(timer); //iniciar temporizador

    while (rodando) { //enquando estiver rodando (rodando foi definido como true)
        ALLEGRO_EVENT evento; //criou um evento
        al_wait_for_event(event_queue, &evento); //esperar pelo evento

        if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE) { //se o evento for "fechar a janela", para de rodar
            rodando = false;
        } else if (evento.type == ALLEGRO_EVENT_KEY_DOWN) {
            if (evento.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
                rodando = false;
            }
        } else if (evento.type == ALLEGRO_EVENT_TIMER) {
            vendo_tela = true;
        } else if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) { //interacoes com o mouse
            if (clicado(&jogar, evento.mouse.x, evento.mouse.y)) { //se o botao clicado for o jogar do menu
                abrir_historia(janela, event_queue, timer); //abre a janela de historia
                rodando = false; //para de rodar a janela do menu
                printf("Entrou na Historia"); //mensagem de confirmacao no terminal
            } else if (clicado(&sobre, evento.mouse.x, evento.mouse.y)) { //se o clicado for o botao sobre
                rodando = false; //para de rodar o menu
                printf("Sobre CLICADO\n");
                abrir_sobre(janela, event_queue, timer); //abre a tela do sobre
            }else if (clicado(&sair, evento.mouse.x, evento.mouse.y)) { //se o clicado for o botao sair
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

    return 0;
}

//================== CODIGO HISTORIA =========================
int abrir_historia(ALLEGRO_DISPLAY *janela, ALLEGRO_EVENT_QUEUE *event_queue, ALLEGRO_TIMER *timer) {
    al_set_window_title(janela, "LUNO - Historia");

    int largura_janela = al_get_display_width(janela);
    int altura_janela = al_get_display_height(janela);

    ALLEGRO_BITMAP *fundo1 = al_load_bitmap("/home/Carol/LUNO/acessorios/nome.png");
    ALLEGRO_BITMAP *fundo2 = al_load_bitmap("/home/Carol/LUNO/acessorios/nome2.png");
    ALLEGRO_BITMAP *fundo3 = al_load_bitmap("/home/Carol/LUNO/acessorios/hist1.png");
    ALLEGRO_BITMAP *fundo4 = al_load_bitmap("/home/Carol/LUNO/acessorios/hist2.png");
    ALLEGRO_BITMAP *fundo5 = al_load_bitmap("/home/Carol/LUNO/acessorios/hist3.png");
    ALLEGRO_BITMAP *fundo6 = al_load_bitmap("/home/Carol/LUNO/acessorios/hist4.png");
    ALLEGRO_BITMAP *fundo7 = al_load_bitmap("/home/Carol/LUNO/acessorios/hist5.png");
    ALLEGRO_BITMAP *fundo8 = al_load_bitmap("/home/Carol/LUNO/acessorios/hist6.png");
    ALLEGRO_BITMAP *fundo = fundo1;

    if (!fundo1 || !fundo2 || !fundo3 || !fundo4 || !fundo5 || !fundo6 || !fundo7 || !fundo8) {
        fprintf(stderr, "Falha ao carregar a imagem de fundo da história.\n");
        return -1;
    }

    ALLEGRO_EVENT_QUEUE *fila_eventos = al_create_event_queue();
    if (!fila_eventos) {
        fprintf(stderr, "Falha ao criar a fila de eventos.\n");
        al_destroy_bitmap(fundo1);
        al_destroy_bitmap(fundo2);
        al_destroy_bitmap(fundo3);
        al_destroy_bitmap(fundo4);
        al_destroy_bitmap(fundo5);
        al_destroy_bitmap(fundo6);
        al_destroy_bitmap(fundo7);
        al_destroy_bitmap(fundo8);
        return -1;
    }

    al_register_event_source(fila_eventos, al_get_display_event_source(janela));
    al_register_event_source(fila_eventos, al_get_timer_event_source(timer));
    al_register_event_source(fila_eventos, al_get_keyboard_event_source());

    ALLEGRO_FONT *fonte = al_load_ttf_font("/home/Carol/LUNO/acessorios/DejaVuSans-Bold.ttf", 30, 0); //definindo a fonte
    if (!fonte) {
        fprintf(stderr, "Falha ao carregar a fonte.\n");
        al_destroy_event_queue(fila_eventos);
        al_destroy_bitmap(fundo1);
        al_destroy_bitmap(fundo2);
        al_destroy_bitmap(fundo3);
        al_destroy_bitmap(fundo4);
        al_destroy_bitmap(fundo5);
        al_destroy_bitmap(fundo6);
        al_destroy_bitmap(fundo7);
        al_destroy_bitmap(fundo8);
        return -1;
    }

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
                    //iniciar_jogo(janela, event_queue, timer); //se estiver na ultima tela da historia, o usuario eh direcionado ao jogo
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

    return 0;
}

int iniciar_jogo(ALLEGRO_DISPLAY *janela, ALLEGRO_EVENT_QUEUE *event_queue, ALLEGRO_TIMER *timer) {
    // Implemente a lógica para o jogo aqui
    // Este é um esqueleto básico para iniciar a estrutura

    bool rodando = true;

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
            // Lógica do jogo aqui
        }

        // Desenho do jogo aqui

        al_flip_display();
    }

    // Liberação de recursos aqui

    return 0;
}

int abrir_sobre(ALLEGRO_DISPLAY *janela, ALLEGRO_EVENT_QUEUE *event_queue, ALLEGRO_TIMER *timer) { //codigo da janela SOBRE
    al_set_window_title(janela, "LUNO - Sobre");

    ALLEGRO_BITMAP *fundo = al_load_bitmap("/home/Carol/LUNO/acessorios/fundoSobre.png"); //fundo
    if (!fundo) {
        fprintf(stderr, "Falha ao carregar a imagem de fundo.\n");
        return -1;
    }



    int largura_janela = al_get_display_width(janela);
    int altura_janela = al_get_display_height(janela);

    Botao sair_sobre = {
        largura_janela / 2 + 420, //aqui eh o X
        altura_janela / 2 - 260, //aqui eh o Y
        80, //tamanho do x do botao
        70, //tamanho do y do botao
        al_map_rgba(0,0, 0,0), // cor invisível
    };

    bool rodando = true;
    bool vendo_tela = true;

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
            vendo_tela = true;
        } else if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
            if (clicado(&sair_sobre, evento.mouse.x, evento.mouse.y)) { //se o botao clicado for o de sair do sobre
                rodando = false; //para de rodar
                abrir_menu(janela, event_queue, timer); //retorna ao menu
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

    return 0;
}
