/* Interface of Inicializations for MasterMind project    */
/* 2018/19 - 2Semester                                    */
/* Author: Pedro Ventura, 93155 & Rita Palma, 93172       */

/* struct que guarda num jogo, a cada tentativa o registo da tentativa */
typedef struct _tentativas_pb {
  int tent_atual; /* número da tentativa atual do jogo */
  char *tentativa; /* tentativa válida do jogo */
  int pretos; /* número de pinos pretos dessa tentativa */
  int brancos; /* número de pinos brancos dessa tentativa */
  struct _tentativas_pb *nexttentativa; /* apontador para o próximo nó da lista de tentativas */
} tentativas_pb;

/*struct que guarda em cada jogo o seu registo */
typedef struct _registo_jogo{
  int num; /* número de jogo */
  char *jogador_ID;  /* string do jogador ID */
  char *nome;  /* string do nome do jogador */
  int jogos; /* número de jogos realizados */
  int cores; /* número de cores */
  int dimchave; /* dimensão da chave */
  char rep_cor; /* char da repetição de cores */
  int tent_aleatorias; /* número de tentativas aleatórias */
  int tent; /* número de tentativas totais que se podem realizar */
  char *chavesecreta; /* string da chave secreta a cada jogo */
  int numjogadas; /* número de jogadas realizadas até acertar a chave  secreta*/
  float tempjogado; /* tempo jogado até acertar a chave secreta */
  struct _registo_jogo *nextjogo; /* apontador para o próximo nó da lista de jogos */
  tentativas_pb *nexttentativa; /* apontadir existente em cada jogo para a lista de tentativas, formando uma lista de listas */
}registo_jogo;

/* struct que contêm os critérios de ordenação */
typedef enum _ordenacao {
    TEMPJOGADO,
    NUMJOGADAS
} ordenacao;


/* Lê os argumentos inseridos no modo de execução         */
void usage(char *programa);

/* Através dos argumentos organiza pelo que é pedido     */
void organizar (int argc, char *argv[]);

/* Lê o ficheiro de historico e consoante os argumentos   */
/* de cada linha guarda em estruturas diferentes          */
registo_jogo *load_lista(FILE *fp);

/* Coloca as variavéis da struct registo_jogo na          */
/* estrutura                                              */
registo_jogo *create_regist_jogo(int num, char *nome, char *jogador_ID,int cores, int dimchave, char rep_cor, char *chavesecreta, int numjogadas, float tempjogado);

/* Coloca as variavéis da struct tentativas_pb na         */
/* estrutura                                              */
tentativas_pb *create_registo_tentativas(int tent_atual, char tentativa[], int pretos, int brancos);

/* Insere a estrutura criada em create_registo_jogo       */
/* na lista de jogos                                      */
registo_jogo* insert_end(registo_jogo *head, registo_jogo *ptr);

/* Insere a estrutura criada em crete_registo_tentativas  */
/* na lista de tentativas consoante o jogo em questão     */
registo_jogo* insert_end_tent(registo_jogo *head, tentativas_pb *ptr);

/* Ordena a lista consoante os parâmetros pedidos,        */
/* primeiro organiza-a por categorias e depois ordena,    */
/* segundo o modo de execução no terminal                 */
registo_jogo *sort_lista(registo_jogo *ListHead, ordenacao ordem);

/* Troca dois elemetos da lista                           */
registo_jogo *troca(registo_jogo *left, registo_jogo *right);

/* Reescreve de novo a lista ordenada                      */
void write_lista(FILE *fp, registo_jogo *head);

/* Faz free da lista e de todos os allocs feitos ao longo  */
/* da ordenação */
void free_lista(registo_jogo *ListHead);
