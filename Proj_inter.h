/* Interface of Inicializations for MasterMind project */
/* 2018/19 - 2Semester                                 */
/* Author: Pedro Ventura, 93155 & Rita Palma, 93172    */

/* Pede ao utilizador a quantidade de jogadores para o */
/* modo interativo                                     */
int inicializar1();

/* Pede ao utilizador o nome de cada um dos jogadores  */
void inicializar2(int * jogadores,char matriz_nomes[4][21]);

/* Pede a quantidade de jogos                          */
int inicializar3();

/* Pede a quantidade de cores, a dimensao da chave , e */
/* a repetição de cor */
void inicializar4(int *cores,int *dimchave, int *repcor);

/* Pede ao utilizador a quantidade de tentativas para  */
/* cada jogo                                           */
int inicializar5();

/* Pede ao utililizador o tempo máximo de jogo         */
int inicializar6();

/* Pede ao utilizador a quantidade de tentativas       */
/* aleatórias                                          */
int inicializar7(int tent);

/* Gera uma chave aleatória de acordo com os parâmetros */
/* indicados na função incializar4 */
void gerarchave(int dimchave ,int cores,int repcor, char chavesecreta[10]);
