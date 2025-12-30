#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

typedef struct baralho{ 
    int id;
	int num;
    char especial[10];
	char cor[9];
	struct baralho* prox;
}Bara;

// O trabalho foi feito em um Sistema Linux, por isso essas duas funções a seguir foram implementadas
void sleep_ms(int milliseconds) {
#ifdef _WIN32
    Sleep(milliseconds);
#else
    usleep(milliseconds * 1000); // Converte milissegundos para microssegundos
#endif
}

void limpar_tela() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}


// Função criada para facilitar a navegação até o final da lista //
Bara* nav(Bara* c) { 
    Bara* aux = c;

    if (c == NULL) {
        return NULL;
    }

    while(aux->prox) {
        aux = aux->prox;
    }

    return aux;
}

// Função criar, e alocar memoria para cartas de 0 a 5, para cada cor. (Vermelho, azul, verde) //
Bara* aux_Cria(Bara* c, char cor[], int num, int id) {
	Bara* novo = NULL;
	Bara* aux = NULL;
	
 
	if (num < 0) { 

		return c;
	} else if (c == NULL){ 

        novo = (Bara*)malloc(sizeof(Bara));
        novo->id = id;
	    novo->num = num;
        strcpy(novo->especial, "0"); // "0" é utilizado para indentificação de uma carta não-especial, para facilitar a impressão
	    strcpy(novo->cor, cor);
        novo->prox = NULL;

		return aux_Cria(novo, cor, num - 1, ++id);
	} else {

        novo = (Bara*)malloc(sizeof(Bara));
        novo->id = id;
	    novo->num = num;
	    strcpy(novo->cor, cor);
        novo->prox = NULL;
        strcpy(novo->especial, "0");

		aux = c;
		aux = nav(c); // Navega até o final da lista e insere a nova carta
		aux->prox = novo;

		return aux_Cria(c, cor, num - 1, ++id);
	}
}

// A Função cria duas cartas +2 e duas cartas de Reverso
Bara* cria_especiais(Bara* c, char cor1[], char cor2[], char cor3[], int n, int id) {
    Bara* aux = c;
    Bara* novo = NULL;
    char especial2[3] = {"+2"}; // Valor especial +2
    char especialRev[8] = {"Reverso"}; // Valor especial Reverso
    id++;
    

    if (n == 0) { // Encerra a recursividade
        return c;
    }

    
    if (n > 3) {
        aux = nav(c);

        novo = (Bara*)malloc(sizeof(Bara));
        strcpy(novo->especial, especial2);
        novo->num = -1; // "-1" Valor numerico utilizado para uma carta especial, para facilitar a impressão
        novo->id = id;
        novo->prox = NULL;
        strcpy(novo->cor, cor1);

        aux->prox = novo;

        cria_especiais(c, cor2, cor3, cor1, n - 1, id); // Chama a função de novo decrementando n e alterando as cores
    } else {

        aux = nav(c);

        novo = (Bara*)malloc(sizeof(Bara));
        strcpy(novo->especial, especialRev);
        novo->num = -1;
        novo->id = id;
        novo->prox = NULL;
        strcpy(novo->cor, cor1);

        aux->prox = novo;

        cria_especiais(c, cor2, cor3, cor1, n - 1, id);
    } 


    return c;
}

// Função criadora do Baralho //
Bara* criaBaralho(Bara* c, int n) {
    Bara* aux = NULL;
	char vermelho[9] = {"vermelho"};
	char azul[9] = {"azul"};
	char verde[9] = {"verde"};
	int num = n;
    int id = 1;
    int contador = 6; // Parâmetro para executar a função cria_especiais seis vezes 
	
	c = aux_Cria(c, vermelho, num, id); // Cria as 6 cartas de um cor
    aux = nav(c); // Retorna um ponteiro para a última carta do baralho
	c = aux_Cria(c, azul, num, ++aux->id);
    aux = nav(c);
	c = aux_Cria(c, verde, num, ++aux->id);	
    aux = nav(c);
    c = cria_especiais(c, azul, vermelho, verde, contador, aux->id); // Cria as cartas +2 e Reverso

	return c;
}

void imprime(Bara* c) { 
	Bara* aux = NULL;
	aux = c;
    char carta[20];
	
	while (aux) {
        if (strcmp(aux->especial, "+2") == 0 || strcmp(aux->especial, "Reverso") == 0){
            printf("[ ");
            strcpy(carta, aux->especial);
            strcat(carta, " ");
            strcat(carta, aux->cor);
            strcat(carta, " ]");
            puts(carta);
            aux = aux->prox;
        } else {
            printf("[ ");
            printf("%d  ", aux->num);
            strcpy(carta, aux->cor);
            strcat(carta, " ]");
            puts(carta);
            aux = aux->prox;
        }
	}
}

void imprime_Carta(Bara* c) {
    Bara* aux = c;
	aux = nav(c);
    char carta[20];
	
    printf("[ ");
    if (strcmp(aux->especial, "+2") == 0 || strcmp(aux->especial, "Reverso") == 0){
        strcpy(carta, aux->especial);
        strcat(carta, " ");
        strcat(carta, aux->cor);
        strcat(carta, " ]");
        puts(carta);
            
    } else {
        printf("%d  ", aux->num);
        strcpy(carta, aux->cor);
        strcat(carta, " ]");
        puts(carta);
        
    }
	
}

// Função que embaralha o Baralho
Bara* embaralhar(Bara* c, int n, int total_cartas) {
    Bara* aux = c;
    Bara* ant = NULL;
    Bara* carta = NULL; // Ponteiro que guarda a carta retirada do baralho para mudar ela de posição 
    int i;


    int aleatorio1 = (rand() % total_cartas + 1); // Gera um numero aleatório entre 1 e 22
    int aleatorio2 = (rand() % total_cartas + 1); // Gera um numero aleatório entre 1 e 22


    if (n == 0) { // Termina a função
       
        return c;
    }
   
    if (aleatorio1 == aleatorio2) { // Verifica se os números gerados são igual, se sim retorna a função 
        return embaralhar(c, n, total_cartas);
    }

   
    for (i = 1; i < aleatorio1; i++) { // Navega até o numero gerado
        ant = aux;
        aux = aux->prox;
    }

    if (aleatorio1 == 1) { 
        c = aux->prox; // Caso onde o primeiro elemento da lista precisa ser retirado
    } else {
        ant->prox = aux->prox; // Faz a ligação correta entre os elementos para retirar o elemento escolhido
    }

    carta = aux; // Armazena o elemento retirado da lista

    // Reseta os ponteiros auxiliares
    aux = c;
    ant = NULL; 
    
    for (i = 1; i < aleatorio2; i++) { // Navega até a posição onbde a carta será inserida
        ant = aux;
        aux = aux->prox;
    }

    if (ant == NULL) { // Caso onde a posição escolhida é o primeiro elemento
        carta->prox = c;
        c = carta;
    } else { // Insere a carta no meio do baralho
        ant->prox = carta;
        carta->prox = aux;
    }
 

    return embaralhar(c, n - 1, total_cartas); // Repete função até n ter valor 0

}


void libera_memoria(Bara* c) { // Libera da memória as cartas no final do jogo
    Bara* remove = NULL;

    while (c) {
        remove = c;
        c = c->prox;
        free(remove);
    }
}

void distribui_cartas(Bara** c, Bara** j1, Bara** j2, Bara** desc) {
    Bara* aux = NULL;
    Bara* aux1 = NULL;
    Bara* ant = NULL; 
    int i;
    
    for (i = 0; i < 4; i++) {
        aux = *c;

        while (aux->prox) { // Navega até o final do baralho e armazena a penultima posição
            ant = aux;
            aux = aux->prox;
        }

        ant->prox = NULL; // Retira a última carta do baralho

        if (*j1 == NULL) {
            *j1 = aux; // Coloca a carta retirada do baralho na mão do jogador 1, se o jogador não tiver cartas 
        } else {
            aux1 = nav(*j1); // Navega até o final da mão do jogador e coloca a carta do baralho
            aux1->prox = aux;
        }

        aux = *c;

        while (aux->prox) {
            ant = aux;
            aux = aux->prox;
        }

        ant->prox = NULL;

        if (*j2 == NULL) {
            *j2 = aux;
        } else {
            aux1 = nav(*j2);
            aux1->prox = aux;
        }

        aux = *c;

        if (i == 3){ // Na última repetição do laço, coloca uma carta do baralho na mesa

            while (aux->prox) {
                ant = aux;
                aux = aux->prox;
            }

            ant->prox = NULL;

            if (*desc == NULL) {
                *desc = aux;
            } 
        }
       
    }

}

void empilhar(Bara** c, Bara* carta) {
    Bara* novo = NULL;
    Bara* aux = NULL;

    if (carta == NULL) { // Verifica se a função recebeu alguma carta para empilhar, se não retorna
        return;
    } 

    carta->prox = NULL; // Isola a carta recebida

    if (*c == NULL) { // Caso em que o local está sendo empilhado não tem cartas
        *c = carta;
        return;
    }
    
    aux = nav(*c);
    aux->prox = carta; // Coloca a carta na última posição da lista onde será colocada
    
}

Bara* desempilhar(Bara** c, Bara* carta) {
    Bara* ant = NULL; 
    Bara* aux = *c;

    if (aux == NULL) { // Verifica se a lista que recebeu é NULA
        return NULL;
    }

    if (carta == NULL) { // Caso onde a carta que está sendo desempilhada é da pilha de compras, retorna a última carta
        
        while (aux->prox) {
            ant = aux;
            aux = aux->prox;
        }

        carta = aux;
        ant->prox = NULL;

        return carta;
    }

    if ((*c)->prox == NULL && (*c)->id == carta->id) { // Se a lista tiver apenas uma elemento, trata adequadamente
        carta = *c;
        *c = NULL;
        return carta;
    }

    while (aux != NULL && aux->id != carta->id) { // Procura pela carta que será retirada da mão de um jogador      
        ant = aux;
        aux = aux->prox;
    }

    if (ant == NULL) { 
        *c = aux->prox;
    } else {
        ant->prox = aux->prox;
    }

    carta = aux;
    carta->prox = NULL;

    return carta;
}

int verificaTamMao (Bara* l) {

    if (l == NULL) {
        return 0;
    } else {
        return 1 + verificaTamMao(l->prox);
    }
}


void aux_imprimeJogo(Bara* desc, int total_cartas) { // Imprime o menu do jogo
    Bara* aux = NULL;
    printf("=============================\n");
    printf("=== SIMULACAO DO JOGO UNO ===\n");
    printf("=============================\n");
    printf("\n");

    printf("Baralho: %d cartas\n", total_cartas);
    printf("Carta na mesa: ");
    aux = nav(desc);
    imprime_Carta(desc);
    printf("\n------------------------------------------\n\n");    

}

Bara* verifica_Mais2adv(Bara* aux, Bara* carta) { // Função usada para verificar se o adversário tem uma carta +2 para rebater a do outro jogador

    if (aux == NULL) {
        return NULL;
    }

    if (strcmp(aux->especial, carta->especial) == 0) {
        return aux;
    } else {
        return verifica_Mais2adv(aux->prox, carta);
    }
    
}


void simulajogo(Bara* c, Bara* j1, Bara* j2, Bara* desc, int total_cartas, int controlador) {
    Bara* aux = NULL;
    Bara* aux1 = NULL;
    Bara* aux_cartas = NULL; 
    Bara* topo_desc = NULL;
    int id_jogador = 0;
    int id_jogadorAdversario = 0;
    
    Bara** jogador_atual = NULL;
    Bara** jogador_adversario = NULL;
    
    
    while (j1 && j2) { // Condição para o jogo continuar
        topo_desc = nav(desc);

        if (controlador % 2 == 0) { // Define qual jogador está jogando 
            jogador_atual = &j1;
            jogador_adversario = &j2;
            id_jogador = 1;
            id_jogadorAdversario = 2;
        } else {
            jogador_atual = &j2;
            jogador_adversario = &j1;
            id_jogador = 2;
            id_jogadorAdversario = 1;
        }

        aux_imprimeJogo(desc, total_cartas);


        sleep_ms(1500);
        if (desc->prox == NULL) { // Mensagens mostradas no inicio do jogo
            printf("Cartas distribuidas!\n\n");
            sleep_ms(1500);

            printf("Jogador %d comeca!\n", id_jogador);
            sleep_ms(1500);
            limpar_tela();
        }
        limpar_tela();
        aux_imprimeJogo(desc, total_cartas);

        // Indica qual jogador joga
        printf("Vez do jogador %d\n\n", id_jogador);
        
        
        printf("Jogador 1 (%d cartas)\n", verificaTamMao(j1));
        imprime(j1);

        printf("\n");

        printf("Jogador 2 (%d cartas)\n", verificaTamMao(j2));
        imprime(j2);

        printf("Pressione enter para continuar!");
        getchar();
        limpar_tela();

        
        aux = *jogador_atual;

        while (aux) { 

            if (strcmp(aux->cor, topo_desc->cor) == 0 || aux->num == topo_desc->num) { // Encontra e joga uma carta normal


                aux_imprimeJogo(desc, total_cartas);


                aux_cartas = desempilhar(jogador_atual, aux);
                printf("Jogador %d jogou a carta: ", id_jogador);
                imprime_Carta(aux_cartas);
                empilhar(&desc, aux_cartas);

                sleep_ms(1500);
                limpar_tela();


                if (strcmp(aux_cartas->especial, "Reverso") == 0) { // Se a carta for Reverso, faz seu efeito
                    aux_imprimeJogo(desc, total_cartas);
            
                    printf("Sentido alterado!\n");
                    controlador--;
                        
                    sleep_ms(1500);
                }

                if (strcmp(aux_cartas->especial, "+2") == 0) { // Se a carta for +2, faz seu efeito
                        

                    if (verifica_Mais2adv(*jogador_adversario , aux_cartas) != NULL) { // Verifica se o adversário tem uma carta +2, se ele tiver ela é jogada e o jogador que jogou o primeiro +2 compra 4 cartas
                        aux_imprimeJogo(desc, total_cartas);


                        aux_cartas = verifica_Mais2adv(*jogador_adversario , aux_cartas);
                        printf("Jogador %d rebateu o +2!\n\n", id_jogadorAdversario);

                        //int count = verificaTamLista(lista);
                        empilhar(&desc, desempilhar(jogador_adversario, aux_cartas));
                        printf("Jogador %d jogou a carta: ", id_jogadorAdversario);
                        imprime_Carta(aux_cartas);
                        printf("\n");
                        sleep_ms(1500);

                        for (int i = 0; i <4; i++) {
                            aux_cartas = desempilhar(&c, NULL);
                            printf("Jogador %d comprou a carta: ", id_jogador);
                            imprime_Carta(aux_cartas);
                            empilhar(jogador_atual, aux_cartas);
                            printf("\n");
                            sleep_ms(1500);
                        }

                            
                    } else {
                        aux_imprimeJogo(desc, total_cartas); // Se o adversário não tiver um +2, ele compra duas cartas
                        for (int i = 0; i < 2; i++) {
                                
                            aux_cartas = desempilhar(&c, NULL);
                            printf("Jogador %d comprou a carta: ", id_jogadorAdversario);
                            imprime_Carta(aux_cartas);
                            empilhar(jogador_adversario, aux_cartas);
                            printf("\n");
    
                            sleep_ms(1500);
                        }
                        controlador++;
                        
                        limpar_tela();
                    }


                }

                    
                limpar_tela();
                break;
            }

            
            aux = aux->prox;
        }

        if (aux == NULL) { // Se o laço não encontrou nenhuma carta jogável, jogador uma compra uma carta

            if (c == NULL) { // Verifica se ainda existem carta para serem compradas, se não o jogo termina e é dado como empate
                aux_imprimeJogo(desc, total_cartas);

                printf("O monte de compras acabou! Nao ha mais cartas para comprar.\n");
                sleep_ms(3000);
                    
                libera_memoria(j1);
                libera_memoria(j2);
                j1 = NULL;
                j2 = NULL;
            }

            aux_imprimeJogo(desc, total_cartas);

            aux_cartas = desempilhar(&c, NULL);
            printf("Jogador %d comprou a carta: ", id_jogador);
            imprime_Carta(aux_cartas);
                
            sleep_ms(1500);
                
            if (strcmp(topo_desc->cor, aux_cartas->cor) == 0 || topo_desc->num == aux_cartas->num) { // Se a carta comprada pode ser jogada na mesa ele joga

                printf("Jogador %d jogou a carta comprada: ", id_jogador);
                imprime_Carta(aux_cartas);
                empilhar(&desc, aux_cartas);
                sleep_ms(1500);
            } else { // Se não, apenas guarda na mão

                empilhar(jogador_atual, aux_cartas);
            }
        }

        limpar_tela();

        controlador++; // Muda o valor do controlador 
    }



    if (j1 == NULL && j2 == NULL) { // Anuncia o empate ou anuncia o vencedor do jogo
        aux_imprimeJogo(desc, total_cartas);

        printf("Jogo terminado em empate!\n");
        printf("Enter para sair!");
        getchar();

        sleep_ms(1500);
        return;
    } else if (j1 == NULL) {
        aux_imprimeJogo(desc, total_cartas);

        printf("Jogador 1 venceu!\n");
        printf("Enter para sair!");
        getchar();

        sleep_ms(1500);
        return;
    } else {
        aux_imprimeJogo(desc, total_cartas);
        printf("Jogador 2 venceu!\n");
        printf("Enter para sair!");
        getchar();
            
        sleep_ms(1500);
        return;
    }
    
}


int main(){
	Bara* cartas = NULL;
	Bara* jogador1 = NULL;
	Bara* jogador2 = NULL;
	Bara* descarte = NULL;
    Bara* aux = NULL;
	int n = 5;
    int contador_emb = 75;
    int total_cartas;
    srand(time(NULL));

	cartas = criaBaralho(cartas, n);

    aux = nav(cartas);
    total_cartas = aux->id;
    cartas = embaralhar(cartas, contador_emb, total_cartas);

    distribui_cartas(&cartas, &jogador1, &jogador2, &descarte);

    simulajogo(cartas, jogador1, jogador2, descarte, total_cartas, 0);

    
    libera_memoria(cartas);
    libera_memoria(jogador1);
    libera_memoria(jogador2);
    libera_memoria(descarte);
    

	return 0;
}