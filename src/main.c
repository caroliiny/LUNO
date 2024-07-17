#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h> //para usar o bool
#include <allegro5/allegro.h> //biblioteca padrao allegro
#include <allegro5/allegro_primitives.h> //para formas primitivas >> desenhar o retangulo do botao
#include <allegro5/allegro_image.h> //para manipulacao de imagens
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_audio.h>


// ================================= STRUCT BOTAO ===================================================================================================================================================
typedef struct Botao { //struct comum para todos os botoes que vao haver no jogo
    int x, y, largura, altura;
    ALLEGRO_COLOR cor; //para manipulacao das cores dos botoes
    ALLEGRO_BITMAP *cor_normal;
    ALLEGRO_BITMAP *cor_alterada;
    bool clicado;
} Botao;


// ======================================== FUNCOES =================================================================================================================================================
int main(int argc, char **argv);
int looping(ALLEGRO_DISPLAY *janela,  ALLEGRO_EVENT_QUEUE *event_queue, ALLEGRO_TIMER *timer); //para o looping de carregamento inicial
void MENU(ALLEGRO_DISPLAY *janela, ALLEGRO_EVENT_QUEUE *event_queue, ALLEGRO_TIMER *timer); //para abrir o menu
void HISTORIA(ALLEGRO_DISPLAY *janela, ALLEGRO_EVENT_QUEUE *event_queue, ALLEGRO_TIMER *timer); //para abrir a janela da historia do jogo
void SOBRE(ALLEGRO_DISPLAY *janela, ALLEGRO_EVENT_QUEUE *event_queue, ALLEGRO_TIMER *timer); //abrir a janela sobre, contendo informacoes
void JOGO(ALLEGRO_DISPLAY *janela, ALLEGRO_EVENT_QUEUE *event_queue, ALLEGRO_TIMER *timer); //para iniciar o jogo
void game_over(ALLEGRO_DISPLAY *janela, ALLEGRO_EVENT_QUEUE *event_queue, ALLEGRO_TIMER *timer);
void VITORIA(ALLEGRO_DISPLAY *janela, ALLEGRO_EVENT_QUEUE *event_queue, ALLEGRO_TIMER *timer);
bool clicado(Botao *botao, int mouse_x, int mouse_y); //para verificar se o botao foi clicado. Caso sim, retorna TRUE
void desenha_Botao(Botao *botao); //para desenhar o botao na tela
void mudar_corB(Botao *botao, ALLEGRO_BITMAP *novo_fundo, bool clicado);

ALLEGRO_SAMPLE *trilha_sonora = NULL;
ALLEGRO_SAMPLE *clique = NULL;
ALLEGRO_SAMPLE *sompulo = NULL;
ALLEGRO_SAMPLE *perdeu = NULL;
ALLEGRO_SAMPLE *ganhou = NULL;

ALLEGRO_SAMPLE_INSTANCE *inst_trilha_sonora = NULL;
ALLEGRO_SAMPLE_INSTANCE *inst_clique = NULL;
ALLEGRO_SAMPLE_INSTANCE *inst_pulo = NULL;
ALLEGRO_SAMPLE_INSTANCE *inst_perdeu = NULL;
ALLEGRO_SAMPLE_INSTANCE *inst_ganhou = NULL;

//============================================================= MAIN ================================================================================================================================
int main(int argc, char **argv) { //funcao main (principal), contendo todas as partes comuns e principais do jogo, evitando redundancia de codigo
    if (!al_init()) { //verificando se o allegro iniciou corretamente
        fprintf(stderr, "Falha ao inicializar Allegro.\n");
    }

//--------------- INICIALIZAÇÂO DE ADDONS E INSTALAÇÂO ---------------------------------

    al_init_primitives_addon(); //inicia o addon das formas primitivas
    al_init_image_addon(); // Inicializa addon de imagens
    al_init_font_addon();  // Inicializa addon de fontes
    al_init_ttf_addon();   // Inicializa addon de TrueType Fonts
    al_install_keyboard(); // Instala o teclado
    al_install_mouse();    // Instala o mouse
    al_init_acodec_addon();
    al_install_audio();
    al_reserve_samples(10);

    ALLEGRO_DISPLAY *janela = al_create_display(1500, 800); //criando a janela com o nome "janela" e definindo seu tamanho

    int largura_janela = al_get_display_width(janela); 
    int altura_janela = al_get_display_height(janela);

    bool tocar = false;


    ALLEGRO_TIMER *timer = al_create_timer(1.0 / 60); //criando o temporizador chamado "timer" e definindo seus ciclos de tempo

    ALLEGRO_EVENT_QUEUE *event_queue = al_create_event_queue(); //cria eventos (como  intercoes com o mouse, teclado etc)
   
//------------------------- TRILHA SONORA -------------------------------------------------
    trilha_sonora = al_load_sample("/home/Carol/LUNO/acessorios/sons/musica.wav");
    perdeu = al_load_sample("/home/Carol/LUNO/acessorios/sons/som-gamer-over.wav");
    ganhou = al_load_sample("/home/Carol/LUNO/acessorios/sons/som-vitoria.wav");
    clique = al_load_sample("/home/Carol/LUNO/acessorios/sons/click.wav");

    inst_trilha_sonora = al_create_sample_instance(trilha_sonora);
    inst_perdeu = al_create_sample_instance(perdeu);
    inst_ganhou = al_create_sample_instance(ganhou);

    al_attach_sample_instance_to_mixer(inst_trilha_sonora, al_get_default_mixer());
    al_attach_sample_instance_to_mixer(inst_perdeu, al_get_default_mixer());
    al_attach_sample_instance_to_mixer(inst_ganhou, al_get_default_mixer());

    al_set_sample_instance_playmode(inst_trilha_sonora, ALLEGRO_PLAYMODE_LOOP);//tocar em loop
    al_set_sample_instance_playmode(inst_perdeu, ALLEGRO_PLAYMODE_ONCE);//tocar em loop
    al_set_sample_instance_playmode(inst_ganhou, ALLEGRO_PLAYMODE_ONCE);//tocar em loo

    al_set_sample_instance_gain(inst_trilha_sonora, 0.4); //0.4 eh o volume da trilha sonora
    al_set_sample_instance_gain(inst_perdeu, 0.4); //0.4 eh o volume da trilha sonora
    al_set_sample_instance_gain(inst_ganhou, 0.4); //0.4 eh o volume da trilha sonora

    // --------------- REGISTRO DE EVENTOS ----------------------------------------------------
    al_register_event_source(event_queue, al_get_display_event_source(janela));
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_mouse_event_source());

    int telaPrincipal = looping(janela, event_queue, timer); // a tela que inicializa o jogo eh o looping de carregamento
    if (telaPrincipal != 0) {
        fprintf(stderr, "Erro ao executar o looping.\n");
    }

//-------- DESTRUINDO EVENTOS ------------------------------

    al_destroy_event_queue(event_queue); 
    al_destroy_timer(timer); 
    al_destroy_display(janela);
    al_destroy_sample(trilha_sonora);
    al_destroy_sample(clique);
    al_destroy_sample(perdeu);
    al_destroy_sample(ganhou);
    al_destroy_sample(clique);

    al_destroy_sample_instance(inst_trilha_sonora);
    al_destroy_sample_instance(inst_clique);
    al_destroy_sample_instance(inst_perdeu);
    al_destroy_sample_instance(inst_ganhou);
    al_destroy_sample_instance(inst_pulo);

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

    ALLEGRO_SAMPLE *somClique = al_load_sample("/home/Carol/LUNO/acessorios/sons/click.wav");
;
    
    // Carrega os bitmaps de fundo
    ALLEGRO_BITMAP *fundos[] = {
        al_load_bitmap("/home/Carol/LUNO/acessorios/fundos/loop1.png"),
        al_load_bitmap("/home/Carol/LUNO/acessorios/fundos/loop1.png"),
        al_load_bitmap("/home/Carol/LUNO/acessorios/fundos/loop2.png"),
        al_load_bitmap("/home/Carol/LUNO/acessorios/fundos/loop2.png"),
        al_load_bitmap("/home/Carol/LUNO/acessorios/fundos/loop3.png"),
        al_load_bitmap("/home/Carol/LUNO/acessorios/fundos/loop3.png"),
        al_load_bitmap("/home/Carol/LUNO/acessorios/fundos/loop4.png"),
        al_load_bitmap("/home/Carol/LUNO/acessorios/fundos/loop4.png"),
        al_load_bitmap("/home/Carol/LUNO/acessorios/fundos/loop5.png"),
        al_load_bitmap("/home/Carol/LUNO/acessorios/fundos/loop5.png"),
        al_load_bitmap("/home/Carol/LUNO/acessorios/fundos/loop6.png"),
        al_load_bitmap("/home/Carol/LUNO/acessorios/fundos/loop6.png"),
        al_load_bitmap("/home/Carol/LUNO/acessorios/fundos/loop7.png"),
        al_load_bitmap("/home/Carol/LUNO/acessorios/fundos/loop8.png"),
        al_load_bitmap("/home/Carol/LUNO/acessorios/funddo/loop9.png")
    };

    int num_fundos = 15;

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
    al_destroy_sample(somClique);

    return 0;
}


// ============================================================================================ MENU ==================================================================================================
void MENU(ALLEGRO_DISPLAY *janela, ALLEGRO_EVENT_QUEUE *event_queue, ALLEGRO_TIMER *timer) { //iniciando o menu
    al_set_window_title(janela, "LUNO - Menu"); //titulo da janela


    ALLEGRO_SAMPLE *somClique = al_load_sample("/home/Carol/LUNO/acessorios/sons/click.wav");

    ALLEGRO_SAMPLE_INSTANCE *instancia_sample = al_create_sample_instance(somClique);

    // Ajuste o ganho (volume) para 1.5 vezes o volume original
    al_set_sample_instance_gain(instancia_sample, 3.5);

    // Anexe a instância do sample ao mixer padrão
    al_attach_sample_instance_to_mixer(instancia_sample, al_get_default_mixer());

    // Tocar o sample
    al_play_sample_instance(instancia_sample);

    if (!somClique) {
        fprintf(stderr, "Falha ao carregar som: /home/Carol/LUNO/acessorios/sons/click.wav\n");
        return;
    }
        

    ALLEGRO_BITMAP *fundo = al_load_bitmap("/home/Carol/LUNO/acessorios/fundos/fundoMenu.png"); //fundo da janela
    
    //a largura e altura da foto de fundo sao proporcionais ao tamanho da janela
    int largura_janela = al_get_display_width(janela); 
    int altura_janela = al_get_display_height(janela);

    ALLEGRO_BITMAP *cor_normal = fundo;
    ALLEGRO_BITMAP *botaoJogar = al_load_bitmap("/home/Carol/LUNO/acessorios/botoes/botaoJogar.png");
    ALLEGRO_BITMAP *botaoSobre = al_load_bitmap("/home/Carol/LUNO/acessorios/botoes/botaoSobre.png");
    ALLEGRO_BITMAP *botaoSair = al_load_bitmap("/home/Carol/LUNO/acessorios/botoes/botaoSair.png");

    Botao jogar = { //criando o botao jogar do menu
        largura_janela / 2 - 190,//x
        altura_janela / 2 - 31,//y
        330,//posicao x
        60,//posicao y
        al_map_rgba(0,0, 0, 0), // cor invisível
        cor_normal,
        botaoJogar,
        false, //nao foi clicado ainda
    };

    Botao sobre = { //criando o botao sobre do menu
        largura_janela / 2 - 190,
        altura_janela / 2 + 65,
        330,
        60,
        al_map_rgba(0, 0,0, 0),
        cor_normal,
        botaoSobre,
        false,
    };

    Botao sair = { //botao sair
        largura_janela / 2 - 190,
        altura_janela / 2 + 170,
        330,
        60,
        al_map_rgba(0,0, 0,0),
        cor_normal,
        botaoSair,
        false,
    };

    bool rodando = true; //dizendo que rodando eh verdadeiro
    bool vendo_tela = true; //vendo_tela eh verdadeiro

    al_start_timer(timer); //iniciar temporizador

    while (rodando) { //enquando estiver rodando (rodando foi definido como true)
        ALLEGRO_EVENT evento; //criou um evento
        al_play_sample_instance(inst_trilha_sonora);
        al_wait_for_event(event_queue, &evento); //esperar pelo evento

        if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE) { //se o evento for "fechar a janela", para de rodar
            rodando = false;
        } else if (evento.type == ALLEGRO_EVENT_TIMER) {
            vendo_tela = true;
        } else if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) { //interacoes com o mouse
            if (clicado(&jogar, evento.mouse.x, evento.mouse.y)) { //se o botao clicado for o jogar do menu
                al_play_sample(somClique, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                mudar_corB(&jogar, botaoJogar, true);
                HISTORIA(janela, event_queue, timer); //abre a janela de historia
                rodando = false; //para de rodar a janela do menu
                printf("Entrou na Historia"); //mensagem de confirmacao no terminal
            } else if (clicado(&sobre, evento.mouse.x, evento.mouse.y)) { //se o clicado for o botao sobre
                al_play_sample(somClique, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                mudar_corB(&sobre, botaoSobre, true);
                printf("Sobre CLICADO\n");
                SOBRE(janela, event_queue, timer); //abre a tela do sobre
                rodando = false; //para de rodar o menu
            }else if (clicado(&sair, evento.mouse.x, evento.mouse.y)) { //se o clicado for o botao sair
                al_play_sample(somClique, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
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
    al_destroy_sample(somClique);
    al_uninstall_audio();

}

//=================================================================== HISTORIA =========================================================================================================================
void HISTORIA(ALLEGRO_DISPLAY *janela, ALLEGRO_EVENT_QUEUE *event_queue, ALLEGRO_TIMER *timer) {
    al_set_window_title(janela, "LUNO - Historia");


    ALLEGRO_SAMPLE *Digitando = al_load_sample("/home/Carol/LUNO/acessorios/sons/digitando.wav");

    
    if (!Digitando) {
        fprintf(stderr, "Falha ao carregar som: /home/Carol/LUNO/acessorios/sons/digitando.wav\n");
        return;
    }

    int largura_janela = al_get_display_width(janela);
    int altura_janela = al_get_display_height(janela);

    ALLEGRO_BITMAP *fundo1 = al_load_bitmap("/home/Carol/LUNO/acessorios/fundos/nome.png");
    ALLEGRO_BITMAP *fundo2 = al_load_bitmap("/home/Carol/LUNO/acessorios/fundos/nome2.png");
    ALLEGRO_BITMAP *fundo3 = al_load_bitmap("/home/Carol/LUNO/acessorios/fundos/hist1.png");
    ALLEGRO_BITMAP *fundo4 = al_load_bitmap("/home/Carol/LUNO/acessorios/fundos/hist2.png");
    ALLEGRO_BITMAP *fundo5 = al_load_bitmap("/home/Carol/LUNO/acessorios/fundos/hist3.png");
    ALLEGRO_BITMAP *fundo6 = al_load_bitmap("/home/Carol/LUNO/acessorios/fundos/hist4.png");
    ALLEGRO_BITMAP *fundo7 = al_load_bitmap("/home/Carol/LUNO/acessorios/fundos/hist5.png");
    ALLEGRO_BITMAP *fundo8 = al_load_bitmap("/home/Carol/LUNO/acessorios/fundos/hist6.png");
    ALLEGRO_BITMAP *fundo9 = al_load_bitmap("/home/Carol/LUNO/acessorios/fundos/iniciarjogo.png");
    
    
    ALLEGRO_BITMAP *fundo = fundo1;

    ALLEGRO_EVENT_QUEUE *fila_eventos = al_create_event_queue();

    al_register_event_source(fila_eventos, al_get_display_event_source(janela));
    al_register_event_source(fila_eventos, al_get_timer_event_source(timer));
    al_register_event_source(fila_eventos, al_get_keyboard_event_source());

    ALLEGRO_FONT *fonte = al_load_ttf_font("/home/Carol/LUNO/acessorios/DejaVuSans-Bold.ttf", 40, 0); //definindo a fonte
    
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
                    fundo = fundo9; //se estiver na ultima tela da historia, o usuario eh direcionado ao jogo
                }else if(fundo == fundo9){
                    rodando = false;
                    al_stop_sample_instance(inst_trilha_sonora);
                    JOGO(janela, event_queue, timer);

                }
                
            }
        } else if (evento.type == ALLEGRO_EVENT_KEY_CHAR) { //se a tecla digitada for um caractere
            if (!al_play_sample(Digitando, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL)) {
                fprintf(stderr, "Falha ao tocar som de tecla.\n");
            }

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
                al_draw_text(fonte, al_map_rgb(206, 50, 250), 950, 170, 0, nome); //desenhar a fonte na tela com a cor e coordenadas que queremos
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
    al_destroy_bitmap(fundo9);
    al_destroy_sample(Digitando);
    al_uninstall_audio();

}

//======================================== STRUCT OBSTACULOS ==========================================================================================================================================
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
    bool jogo_ganho; // Flag indicando se o jogo foi ganho 
    bool game_over; // Flag indicando se o jogo foi perdido
} Luno;

bool colisao(float ax, float ay, float aw, float ah, float bx, float by, float bw, float bh) {
    // Verifica se há colisão entre dois retângulos
    return (ax + aw > bx && ax < bx + bw && ay + ah > by && ay < by + bh);
}

//======================================== JOGO ============================================================
void JOGO(ALLEGRO_DISPLAY *janela, ALLEGRO_EVENT_QUEUE *event_queue, ALLEGRO_TIMER *timer) {

    al_set_window_title(janela,"LUNO - Jogando...");
    // Inicialização básica do Allegro
    if (!al_init()) {
        fprintf(stderr, "Falha ao inicializar Allegro.\n");
    }

    // Inicialização do addon de imagem do Allegro
    if (!al_init_image_addon()) {
        fprintf(stderr, "Falha ao inicializar addon de imagem do Allegro.\n");
        al_destroy_display(janela);
    }

    // Instalação do teclado
    if (!al_install_keyboard()) {
        fprintf(stderr, "Falha ao inicializar o teclado.\n");
        al_destroy_display(janela);
        al_destroy_event_queue(event_queue);
    }


    ALLEGRO_SAMPLE *somPulo = al_load_sample("/home/Carol/LUNO/acessorios/sons/som-pulo.wav");     

    // Carrega imagens do fundo e do personagem Luno
    ALLEGRO_BITMAP* fundo_jogo = al_load_bitmap("/home/Carol/LUNO/acessorios/fundos/fundolunogame.jpg");
    ALLEGRO_BITMAP* luno1 = al_load_bitmap("/home/Carol/LUNO/acessorios/personagem/luno1.png");
    ALLEGRO_BITMAP* luno2 = al_load_bitmap("/home/Carol/LUNO/acessorios/personagem/luno2.png");
    ALLEGRO_BITMAP* luno3 = al_load_bitmap("/home/Carol/LUNO/acessorios//personagem/luno3.png");

    // Carrega imagens dos obstáculos
    ALLEGRO_BITMAP* obstaculo1 = al_load_bitmap("/home/Carol/LUNO/acessorios/obstaculos/obstaculo1.png");
    ALLEGRO_BITMAP* obstaculo2 = al_load_bitmap("/home/Carol/LUNO/acessorios/obstaculos/obstaculo2.png");
    ALLEGRO_BITMAP* obstaculo3 = al_load_bitmap("/home/Carol/LUNO/acessorios/obstaculos/obstaculo3.png");
    ALLEGRO_BITMAP* obstaculo4 = al_load_bitmap("/home/Carol/LUNO/acessorios/obstaculos/obstaculo4.png");
    ALLEGRO_BITMAP* obstaculo5 = al_load_bitmap("/home/Carol/LUNO/acessorios/obstaculos/obstaculo5.png");
    ALLEGRO_BITMAP* obstaculo6 = al_load_bitmap("/home/Carol/LUNO/acessorios/obstaculos/obstaculo6.png");
    ALLEGRO_BITMAP* obstaculo7 = al_load_bitmap("/home/Carol/LUNO/acessorios/obstaculos/obstaculo7.png");
    ALLEGRO_BITMAP* obstaculo8 = al_load_bitmap("/home/Carol/LUNO/acessorios/obstaculos/obstaculo8.png");
    ALLEGRO_BITMAP* obstaculo9 = al_load_bitmap("/home/Carol/LUNO/acessorios/obstaculos/obstaculo9.png");
    ALLEGRO_BITMAP* obstaculo10 = al_load_bitmap("/home/Carol/LUNO/acessorios/obstaculos/obstaculo10.png");
    ALLEGRO_BITMAP* obstaculo11 = al_load_bitmap("/home/Carol/LUNO/acessorios/obstaculos/obstaculo11.png");
    ALLEGRO_BITMAP* obstaculo12 = al_load_bitmap("/home/Carol/LUNO/acessorios/obstaculos/obstaculo12.png");
    ALLEGRO_BITMAP* obstaculo13 = al_load_bitmap("/home/Carol/LUNO/acessorios/obstaculos/obstaculo13.png");
    ALLEGRO_BITMAP* obstaculo14 = al_load_bitmap("/home/Carol/LUNO/acessorios/obstaculos/obstaculo14.png");
    ALLEGRO_BITMAP* obstaculo15 = al_load_bitmap("/home/Carol/LUNO/acessorios/obstaculos/obstaculo15.png");
    ALLEGRO_BITMAP* obstaculo16 = al_load_bitmap("/home/Carol/LUNO/acessorios/obstaculos/obstaculo16.png");

    // Carrega imagem do game over
    ALLEGRO_BITMAP* game_over_luno = al_load_bitmap("/home/Carol/LUNO/acessorios/fundos/gameoverluno.png");
    ALLEGRO_BITMAP* obstaculo_vitoria = al_load_bitmap("/home/Carol/LUNO/acessorios/obstaculos/obstaculovitoria.png");
    ALLEGRO_BITMAP* imagem_vitoria = al_load_bitmap("/home/Carol/LUNO/acessorios/fundos/vitoria.jpg"); 

      // Verifica se todas as imagens foram carregadas corretamente
    if (!fundo_jogo || !luno1 || !luno2 || !luno3 || !obstaculo1 || !obstaculo2 || !obstaculo3 || !obstaculo4 || !obstaculo5 || !obstaculo6 || !obstaculo7 || !obstaculo8 || !obstaculo9 || !obstaculo10 || !obstaculo11 || !obstaculo12 || !obstaculo13 || !obstaculo14 || !obstaculo15 || !obstaculo16 || !game_over_luno || !obstaculo_vitoria || !imagem_vitoria) {
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
        al_destroy_bitmap(obstaculo_vitoria);
        al_destroy_bitmap(game_over_luno);
        al_destroy_bitmap(imagem_vitoria); 
    }

    ALLEGRO_EVENT_QUEUE *fila_eventos = al_create_event_queue();

    int largura_janela = al_get_display_width(janela); 
    int altura_janela = al_get_display_height(janela);

    // Variáveis para armazenar o tamanho do obstáculo de vitória
    float largura_obstaculo_vitoria_original = al_get_bitmap_width(obstaculo_vitoria);
    float altura_obstaculo_vitoria_original = al_get_bitmap_height(obstaculo_vitoria);
    // Novas dimensões escaladas
    float largura_obstaculo_vitoria = largura_obstaculo_vitoria_original * 3.0;
    float altura_obstaculo_vitoria = altura_obstaculo_vitoria_original * 3.0;

    // Variáveis para controle do jogo
    float fator_escala = (float)al_get_display_height(janela) / al_get_bitmap_height(fundo_jogo);
    int largura_escalada = al_get_bitmap_width(fundo_jogo) * fator_escala;
    float posicao_x = 0; // Posição inicial da imagem de fundo no eixo x
    bool mover_fundo = true; // Variável para controlar se o fundo deve se mover
    // Inicializa a velocidade do jogo
    float velocidade_jogo = 9.5;

    // Variáveis para o personagem Luno
     // Variáveis para o personagem Luno
    Luno luno;
    luno.x = 30;
    luno.y_original = al_get_display_height(janela) * 0.49; // Posição inicial mais alta na tela
    luno.y = luno.y_original;
    luno.largura = al_get_bitmap_width(luno1) * 0.28;
    luno.altura = al_get_bitmap_height(luno1) * 0.28;
    luno.bitmap[0] = luno1;
    luno.bitmap[1] = luno2;
    luno.bitmap[2] = luno3;
    luno.frame_atual = 0;
    luno.velocidade_y = 0.0;
    luno.pulando = false;
    luno.jogo_encerrado = false; // Inicialmente o jogo não acabou
    luno.jogo_ganho = false;
    luno.game_over = false; 


    // Inicialização dos obstáculos
    Obstaculo obstaculos[16]; // Aumente o tamanho do array para 32 para suportar duplicação
    ALLEGRO_BITMAP* obstaculo_bitmaps[16] = { obstaculo1, obstaculo2, obstaculo3, obstaculo4, obstaculo5, obstaculo6, obstaculo7, obstaculo8, obstaculo9, obstaculo10, obstaculo11, obstaculo12, obstaculo13, obstaculo14, obstaculo15, obstaculo16 };

    // Semente aleatória baseada no tempo atual
    srand(time(NULL));

    for (int i = 0; i < 16; ++i) {
        int indice_bitmap = rand() % 16; // Escolhe aleatoriamente um bitmap de obstáculo
        obstaculos[i].x = al_get_display_width(janela) + i * 1500 + rand() % 300; // Espaçamento inicial maior
        obstaculos[i].y = al_get_display_height(janela) - al_get_bitmap_height(obstaculo1) - 30;
        obstaculos[i].largura = al_get_bitmap_width(obstaculo1) * 2.0; // Aumenta a largura
        obstaculos[i].altura = al_get_bitmap_height(obstaculo1) * 2.0; // Aumenta a altura
        obstaculos[i].bitmap = obstaculo_bitmaps[indice_bitmap]; // Atribui um bitmap aleatório a cada obstáculo
        obstaculos[i].ativo = true; // Marca o obstáculo como ativo

        if (indice_bitmap == 16) {
            obstaculos[i].largura = al_get_bitmap_width(obstaculo_bitmaps[indice_bitmap]) * 1.5; // Aumenta a largura em 80%
            obstaculos[i].altura = al_get_bitmap_height(obstaculo_bitmaps[indice_bitmap]) * 1.5; // Aumenta a altura em 80%
        }
        // Ajuste de tamanho específico para obstáculos de 10 a 16
        if (indice_bitmap >= 9 && indice_bitmap < 16) {
            obstaculos[i].largura = al_get_bitmap_width(obstaculo_bitmaps[indice_bitmap]) * 1.6; // Aumenta a largura em 80%
            obstaculos[i].altura = al_get_bitmap_height(obstaculo_bitmaps[indice_bitmap]) * 1.6; // Aumenta a altura em 80%
        }
        
    }

    // Configuração do timer e registro de evento
    al_register_event_source(fila_eventos, al_get_display_event_source(janela));
    al_register_event_source(fila_eventos, al_get_timer_event_source(timer));
    al_register_event_source(fila_eventos, al_get_keyboard_event_source()); // Registro do teclado

    al_start_timer(timer);

    bool tecla_espaço_pressionada = false; // Flag para verificar se a tecla espaço foi pressionada

    int score = 0;  // Variável para armazenar o score
    bool desenhar_score = true; 
    bool rodando = true;

    al_init_font_addon();
    al_init_ttf_addon();
    ALLEGRO_FONT* fonte = al_load_ttf_font("/home/carol/LUNO/acessorios/DejaVuSans-Bold.ttf", 32, 0);

    while (!luno.jogo_encerrado) {

        al_play_sample_instance(inst_trilha_sonora);
        ALLEGRO_EVENT evento;
        al_wait_for_event(fila_eventos, &evento);

        if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            break;
        }

        else if (evento.type == ALLEGRO_EVENT_KEY_DOWN) {
            // Evento de tecla pressionada
            if (evento.keyboard.keycode == ALLEGRO_KEY_SPACE && !luno.pulando) {
                // Se a tecla espaço foi pressionada e o Luno não está pulando
                al_play_sample(somPulo, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                luno.pulando = true;
                luno.velocidade_y = -20.5; // Define a velocidade inicial do pulo
                tecla_espaço_pressionada = true;
            }
                
        }
        else if (evento.type == ALLEGRO_EVENT_KEY_UP) {
            // Evento de tecla solta
            if (evento.keyboard.keycode == ALLEGRO_KEY_SPACE) {
                tecla_espaço_pressionada = false;
            }
        }

        // Atualização da animação do personagem Luno
        if (evento.type == ALLEGRO_EVENT_TIMER) {
            if (!luno.pulando && !tecla_espaço_pressionada) {
                // Atualiza o frame da animação a cada 10 eventos de timer
                const int intervalo_frame = 6;
                static int contagem_frame = 0;
                contagem_frame++;
                if (contagem_frame >= intervalo_frame) {
                    contagem_frame = 0;
                    luno.frame_atual = (luno.frame_atual + 1) % 3;
                }
            }

            // Aplicação da gravidade ao pulo
            luno.y += luno.velocidade_y;
            luno.velocidade_y += 0.6; // Aceleração da gravidade

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

        // Verifica se todos os obstáculos estão desativados
        bool todos_desativados = true;
        for (int i = 0; i < 16; ++i) {
            if (obstaculos[i].ativo) {
                todos_desativados = false;
                break;
            }
        }

        // Se todos os obstáculos principais estiverem desativados, ativa o obstáculo de vitória
        if (todos_desativados) {
            // Calcula as coordenadas x e y do obstáculo de vitória
           // Define as coordenadas iniciais do obstáculo de vitória
            float posicao_obstaculo_vitoria_x = al_get_display_width(janela) - al_get_bitmap_width(obstaculo_vitoria) -180;
            float posicao_obstaculo_vitoria_y = al_get_display_height(janela) - al_get_bitmap_height(obstaculo_vitoria) - 240;
                     

            // Desenha o obstáculo de vitória com as dimensões escaladas
            al_draw_scaled_bitmap(obstaculo_vitoria,
                0, 0, largura_obstaculo_vitoria_original, altura_obstaculo_vitoria_original,
                posicao_obstaculo_vitoria_x, posicao_obstaculo_vitoria_y, largura_obstaculo_vitoria, altura_obstaculo_vitoria,
                0);

            // Verifica colisão com o obstáculo de vitória
            bool colidiu_com_vitoria = colisao(luno.x, luno.y, luno.largura, luno.altura, posicao_obstaculo_vitoria_x, posicao_obstaculo_vitoria_y, largura_obstaculo_vitoria, altura_obstaculo_vitoria);
            bool luno_na_vitoria = (int)luno.x == (int)posicao_obstaculo_vitoria_x; // Verifica se Luno está na mesma coordenada x que o obstáculo de vitória

            // Verifica colisão com o obstáculo de vitória
            if (colidiu_com_vitoria || luno_na_vitoria) {
                rodando = false;
                luno.jogo_ganho = true;
                VITORIA(janela, event_queue, timer);
                printf("Luno ganhou!\n");
            }
            

            // Após desenhar o obstáculo de vitória, pare o movimento do fundo
            mover_fundo = false;
        }

        // Atualização da posição do fundo
        if (mover_fundo) {
            posicao_x -= velocidade_jogo;
            if (posicao_x <= -largura_escalada) {
                posicao_x = 0;
            }
        }
        else {
            // Se o fundo parar de se mover, comece a mover o personagem Luno para a direita
            luno.x += 7.5; 
        }

        // Atualização e desenho dos obstáculos
        for (int i = 0; i < 16; ++i) {
            if (obstaculos[i].ativo) {
                obstaculos[i].x -= velocidade_jogo; // Movimento dos obstáculos para a esquerda

                obstaculos[i].y = al_get_display_height(janela) - obstaculos[i].altura - 110;

                // Desenho dos obstáculos
                if (obstaculos[i].bitmap) {
                    al_draw_scaled_bitmap(obstaculos[i].bitmap,
                        0, 0, al_get_bitmap_width(obstaculos[i].bitmap), al_get_bitmap_height(obstaculos[i].bitmap),
                        obstaculos[i].x, obstaculos[i].y, obstaculos[i].largura, obstaculos[i].altura,
                        0);

                    // Verifica colisão entre Luno e obstáculo
                    if (colisao(luno.x, luno.y, luno.largura, luno.altura, obstaculos[i].x, obstaculos[i].y, obstaculos[i].largura, obstaculos[i].altura)) {
                        rodando = false;
                        luno.game_over = true; 
                        al_stop_sample_instance(inst_trilha_sonora);
                        game_over(janela, event_queue, timer);
                        printf("Luno perdeu!\n");
                        // Aqui você pode implementar a lógica para o que acontece quando Luno colide com um obstáculo
                        // Por exemplo, parar o jogo, mostrar uma mensagem, etc.
                    } 
                }


                // Verifica se o obstáculo saiu da tela
                if (obstaculos[i].x + obstaculos[i].largura < 0) {
                    obstaculos[i].ativo = false; // Desativa o obstáculo
                    // Incrementa a velocidade do jogo quando um obstáculo é desativado
                    velocidade_jogo += 0.3; 
    
                }
            }
        }


        // Desenho do personagem Luno na tela com escala reduzida
        if (!luno.jogo_ganho) {
            if (luno.pulando) {
                // Se o Luno está pulando, desenha apenas o bitmap de pulo (luno1.png)
                al_draw_scaled_bitmap(luno1,
                    0, 0, al_get_bitmap_width(luno1), al_get_bitmap_height(luno1),
                    luno.x, luno.y, luno.largura, luno.altura,
                    0);
            }
            else{
                // Caso contrário, continua com a lógica de animação baseada nos frames
                al_draw_scaled_bitmap(luno.bitmap[luno.frame_atual],
                    0, 0, al_get_bitmap_width(luno.bitmap[luno.frame_atual]), al_get_bitmap_height(luno.bitmap[luno.frame_atual]),
                    luno.x, luno.y, luno.largura, luno.altura,
                    0);
            }
        }else {
           VITORIA(janela, event_queue, timer);
        }
        al_flip_display(); // Atualiza a tela 

        
    }
   
        al_flip_display(); // Atualiza a tela  

    

    // Liberar recursos alocados dinamicamente
    al_destroy_bitmap(fundo_jogo);
    al_destroy_bitmap(luno1);
    al_destroy_bitmap(luno2);
    al_destroy_bitmap(luno3);
    for (int i = 0; i < 16; i++) {
        al_destroy_bitmap(obstaculo_bitmaps[i]);
    }
    al_destroy_bitmap(game_over_luno);
    al_destroy_bitmap(obstaculo_vitoria);
    al_destroy_bitmap(imagem_vitoria); 
    al_destroy_display(janela);
    al_destroy_event_queue(fila_eventos);
    al_destroy_sample(somPulo);
    al_uninstall_audio();
}


//======================================================================================== GAME OVER ====================================================================================================
void game_over(ALLEGRO_DISPLAY *janela, ALLEGRO_EVENT_QUEUE *event_queue, ALLEGRO_TIMER *timer){

    al_set_window_title(janela, "LUNO - GAME OVER");

    ALLEGRO_SAMPLE *somClique = al_load_sample("/home/Carol/LUNO/acessorios/sons/click.wav");

    // ---------------------------- Musica GAME OVER ----------------------------
   
    inst_perdeu = al_create_sample_instance(perdeu);
    al_attach_sample_instance_to_mixer(inst_perdeu, al_get_default_mixer());
    al_set_sample_instance_playmode(inst_perdeu, ALLEGRO_PLAYMODE_ONCE);//tocar em loop
    al_set_sample_instance_gain(inst_perdeu, 0.4); //0.4 eh o volume da musica
    
    if (!somClique) {
        fprintf(stderr, "Falha ao carregar som: /home/Carol/LUNO/acessorios/sons/click.wav\n");
        return;
    }

    ALLEGRO_BITMAP *fundo = al_load_bitmap("/home/Carol/LUNO/acessorios/fundos/gameoverluno.png");
    ALLEGRO_BITMAP *cor_normal = fundo;
    ALLEGRO_BITMAP *botaoJogue = al_load_bitmap("/home/Carol/LUNO/acessorios/botoes/botaoJogue.png");
    ALLEGRO_BITMAP *botaoIrmenu = al_load_bitmap("/home/Carol/LUNO/acessorios/botoes/botaoIrmenu.png");

    int largura_janela = al_get_display_width(janela);
    int altura_janela = al_get_display_height(janela);

     Botao Jogue = {
        largura_janela / 2 - 190,//x
        altura_janela / 2 - 31,//y
        330,//posicao x
        60,//posicao y 
        al_map_rgba(0,0, 0, 0), // cor invisível
        cor_normal,
        botaoJogue,
        false, //nao foi clicado ainda
    };

     Botao ir_menu = {
        largura_janela /2 - 190,//x
        altura_janela / 2 + 65,//y
        330,//posicao x
        60,//posicao y 
        al_map_rgba(0, 0,0, 0), // cor invisível
        cor_normal,
        botaoIrmenu,
        false, //nao foi clicado ainda
    };

    bool rodando = true; //dizendo que rodando eh verdadeiro
    bool vendo_tela = true; //vendo_tela eh verdadeiro


    al_start_timer(timer);

    while (rodando) { //enquando estiver rodando (rodando foi definido como true)
        al_play_sample_instance(inst_perdeu);
        ALLEGRO_EVENT evento; //criou um evento
        al_wait_for_event(event_queue, &evento); //esperar pelo evento

        if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE) { //se o evento for "fechar a janela", para de rodar
            rodando = false;
        } else if (evento.type == ALLEGRO_EVENT_TIMER) {
            vendo_tela = true;
        } else if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) { //interacoes com o mouse
            if (clicado(&Jogue, evento.mouse.x, evento.mouse.y)) { //se o botao clicado for o jogar do menu
                al_play_sample(somClique, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                mudar_corB(&Jogue, botaoJogue, true);
                rodando = false;
                JOGO(janela, event_queue, timer); 
                //para de rodar a janela do menu
                printf("Jogar novamente"); //mensagem de confirmacao no terminal
            } else if (clicado(&ir_menu, evento.mouse.x, evento.mouse.y)) { //se o clicado for o botao sobre
                al_play_sample(somClique, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                mudar_corB(&ir_menu, botaoIrmenu, true);
                rodando = false; //para de rodar
                printf("Voltar pra menu\n");
                MENU(janela, event_queue, timer); //abre a tela do MENU
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
    al_destroy_sample(somClique);
    al_destroy_sample(perdeu);
    al_destroy_sample_instance(inst_perdeu);
    al_uninstall_audio();
}


// ==================================================== VITORIA ==========================================================================================
void VITORIA(ALLEGRO_DISPLAY *janela, ALLEGRO_EVENT_QUEUE *event_queue, ALLEGRO_TIMER *timer){
    al_set_window_title(janela, "LUNO - VITORIA!!!!!!!");


    ALLEGRO_SAMPLE *somVitoria = al_load_sample("/home/Carol/LUNO/acessorios/sons/som-vitoria.wav");
    ALLEGRO_SAMPLE *somClique = al_load_sample("/home/Carol/LUNO/acessorios/sons/click.wav");

    if (!somVitoria) {
        fprintf(stderr, "Falha ao carregar som: /home/Carol/LUNO/acessorios/sons/som-vitoria.wav\n");
        return;
    }

    ALLEGRO_BITMAP *fundo = al_load_bitmap("/home/Carol/LUNO/acessorios/fundos/vitoria.jpg");
    ALLEGRO_BITMAP *cor_normal = fundo;
    ALLEGRO_BITMAP *cor_alterada = al_load_bitmap("/home/Carol/LUNO/acessorios/botoes/botaoVoltar.png");

    int largura_janela = al_get_display_width(janela);
    int altura_janela = al_get_display_height(janela);

     Botao voltar = {
        largura_janela / 2 - 170, //aqui eh o X
        altura_janela / 2 - 150, //aqui eh o Y
        330, //tamanho do x do botao
        60, //tamanho do y do botao
        al_map_rgba(0, 0,0,0), // cor invisível
        cor_normal,
        cor_alterada,
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
            if (clicado(&voltar, evento.mouse.x, evento.mouse.y)) { //se o botao clicado for o jogar do menu
                mudar_corB(&voltar, cor_alterada, true);
                rodando = false;
                MENU(janela, event_queue, timer); 
                //para de rodar a janela do menu
                printf("Voltou ao menu"); //mensagem de confirmacao no terminal
            } 
            vendo_tela = true; //enquanto estiver rodando, podemos ver a tela
        }

        if (vendo_tela && al_is_event_queue_empty(event_queue)) {
            vendo_tela = false;
            al_clear_to_color(al_map_rgb(255, 255, 255));
            al_draw_scaled_bitmap(fundo, 0, 0, al_get_bitmap_width(fundo), al_get_bitmap_height(fundo), 0, 0,
                                  largura_janela, altura_janela, 0); //desenha a imagem de fudo com o tamanho proporcional ao da janela

            //definindo quais botoes serao desenhados na tela
            desenha_Botao(&voltar); 

            al_flip_display();
        }
    }

    al_destroy_bitmap(fundo);
    al_destroy_bitmap(cor_alterada);
  

}

//=============================================================== SOBRE =================================================================================================================================
void SOBRE(ALLEGRO_DISPLAY *janela, ALLEGRO_EVENT_QUEUE *event_queue, ALLEGRO_TIMER *timer) { //codigo da janela SOBRE
    al_set_window_title(janela, "LUNO - Sobre");

    ALLEGRO_SAMPLE *somClique = al_load_sample("/home/Carol/LUNO/acessorios/sons/click.wav");

    if (!somClique) {
        fprintf(stderr, "Falha ao carregar som: /home/Carol/LUNO/acessorios/sons/click.wav\n");
        return;
    }

    ALLEGRO_BITMAP *fundo = al_load_bitmap("/home/Carol/LUNO/acessorios/fundos/fundoSobre.png"); //fundo

    int largura_janela = al_get_display_width(janela);
    int altura_janela = al_get_display_height(janela);

    ALLEGRO_BITMAP *cor_normal = fundo;
    ALLEGRO_BITMAP *botaoSair_sobre= al_load_bitmap("/home/Carol/LUNO/acessorios/botoes/botaoSair_sobre.png");

    Botao sair_sobre = {
        largura_janela / 2 + 570, //aqui eh o X
        altura_janela / 2 - 350, //aqui eh o Y
        100, //tamanho do x do botao
        90, //tamanho do y do botao
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
                al_play_sample(somClique, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                mudar_corB(&sair_sobre, botaoSair_sobre, true);
                printf("Voltou para o menu\n");
                rodando = false; //para de rodar
                MENU(janela, event_queue, timer); //retorna ao menu
                
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
