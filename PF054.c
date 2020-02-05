/* Interface of MasterMind project                        */
/* 2018/19 - 2Semester                                    */
/* Author: Pedro Ventura, 93155 & Rita Palma, 93172       */

#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <time.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <stdbool.h>
#include "oraculo.h"
#include "Proj_inter.h"
#include "ordena.h"

/* struct que guarda as variavéis de inicialização */
typedef struct _init_dat{
    char *nome;
    int jogos;
    int cores;
    int dimchave;
    char rep_cor;
    int repcor;
    int tent_aleatorias;
    int tent;
} init_dat;

void jogo1(init_dat*,int,char [4][21]);
void jogo2(int,init_dat*,FILE *,FILE *,registo_jogo*);
void tentativas_function(init_dat*,int,registo_jogo*,registo_jogo*);
void pretos_brancos_function(char [],char [],int *,int *);
void pretos_brancos_final(char[],int *,int *);
void ler_pretos_brancos(int *,int *,int );
int gera_combinacoes_rep(init_dat *,char [],int *j);
int gera_combinacoes_sem_rep(init_dat *,char [],int *j);
int valida_combinacao(char [],char **,int **,int);
int permutations(char [],int,char **,int **,int);
int valida_permutacoes(char [],char **,int **,int);
int comparar(const void* , const void* );
void modo_interativo(char [4][21],init_dat*,int*);
void readfile(FILE *,init_dat *);
registo_jogo *cria_jogo(init_dat*,registo_jogo*);
tentativas_pb *cria_tentativa(int ,tentativas_pb *,char **);
void print_lista_ficheiro(FILE *,registo_jogo *);
void organizar (int argc, char *argv[]);


int main(int argc, char *argv[])
{
  int jogadores=0,modo=0,i=0,hist=0;
  char matriz_nomes[4][21]={{0}};
  FILE *fp=NULL,*fp_hist=NULL;
  init_dat *init=NULL;
  registo_jogo *reg_jogo=NULL;

  srand ((unsigned)time(NULL)); /* de modo a ser possivel gerar uma chave aleatória */
  printf("BEM-VINDO AO JOGO MASTERMIND \n");
  init=(init_dat*)calloc(1,sizeof(init_dat)); /*Alocação do ponteiro para a estrutura init_dat */
  if (init==NULL ){  /* Confirmação se a memória foi bem alocada */
     printf("Falha na alocação de memória");
     exit (EXIT_FAILURE);
  }

  if (argc ==1) { /* Se apenas for introduzido um argumento irá correr o modo interativo */
    modo_interativo(matriz_nomes,init,&jogadores);
    jogo1(init,jogadores,matriz_nomes);
    free(init);
  }
  else { /* de modo à ordem no terminal dos argumentos ser indiferente */
      for (i=0; i< argc;i++){ /* corre todos os argumentos executados na linha de comando */
        if (strcmp(argv[i],"-i")==0){ /* apenas se encontrar este argumento abre o ficheiro indicado no argumento seguinte */
          fp = fopen(argv[i+1], "r");
          if (fp==NULL){ /* Confirmação se o ficheiro foi aberto */
            perror("Ficheiro não aberto!\n");
            exit(EXIT_FAILURE);
          }
        }
        if (strcmp(argv[i],"-h")==0){ /* guarda a posição do argumento onde se encontra o nome do ficheiro histórico */
          hist=i+1;
        }
      }
    if (fp==NULL){ /* se o ficheiro de init não foi aberto, então irá correr só a ordenação do histórico */
      organizar(argc,argv);
      printf("A organização do histórico foi concluida\n");
    }
    else{ /* ocorrerá os jogos e a ordenação do histórico, incluido os jogos em questão */
      fp_hist=fopen(argv[hist],"a"); /* escreve no final do ficheiro o registo dos jogos que irão ser executados */
      if (fp_hist == NULL){ /* Confirmação se o ficheiro foi aberto */
          perror("Ficheiro não aberto!\n");
          exit(EXIT_FAILURE);
      }
      reg_jogo=(registo_jogo*)calloc(1,sizeof(registo_jogo)); /* Alocação do ponteiro para a estrutura registo_jogo */
      if (reg_jogo==NULL){ /* Confirmação se a memória foi bem alocada */
         printf("Falha na alocação de memória");
         exit (EXIT_FAILURE);
      }
      jogo2(modo,init,fp,fp_hist,reg_jogo);
      free(init->nome); /* Free da memória que aloca o nome do jogador */
      fclose(fp_hist); /* Fechar o ficheiro que irá ser escrito */
      organizar(argc,argv);
    }
    free(init); /* Free do ponteiro que aponta para a struct init_dat */
  }
  terminate_oracle();
return 0;
}

/*-----------------------------------------------------------------------------------*/
/* Function: jogo1-------------------------------------------------------------------*/
/* Description: Modo de jogo para o modo interativo----------------------------------*/
/* Return: --------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------*/

void jogo1(init_dat*init,int jogadores,char matriz_nomes[4][21])
{
  int i=0,j=0,modo=1;

  activate_oracle(init->dimchave,init->cores,init->repcor);
  for (i=0; i<jogadores; i++) { /* Correr todos os jogadores */
    printf("\n____________________Próximo Jogador____________________\n");
    printf ("É a vez do jogador %s\n",matriz_nomes[i]);
    for(j=0; j<(init->jogos); j++) { /* Correr todos os jogos */
      printf("____________________Próximo Jogo____________________\n");
      printf("Jogo nº%d\n",j+1);
      printf("Pense numa chave. Quando estiver pronto clique ENTER\n");
      getchar(); /* de modo ao utilizador presisonar enter */
      tentativas_function(init,modo,NULL,NULL);
    }
  }
}

/*-----------------------------------------------------------------------------------*/
/* Function: tentativas_function-----------------------------------------------------*/
/* Description: Algoritmo de jogo para adivinhar a chave secreta---------------------*/
/* Return: --------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------*/

void tentativas_function(init_dat *init,int modo,registo_jogo *reg_jogo, registo_jogo *listahead)
{
  char chavesecreta[init->dimchave],chave_permutacoes[init->dimchave],a='A',win=0, **tentativas;
  int i=0,pretos=0,brancos=0,var,j=(init->dimchave)-1,end=0,w=0,**resultados;
  tentativas_pb *newtentativa=NULL, *tent_pb=NULL;

  tent_pb=(tentativas_pb*)calloc(1,sizeof(tentativas_pb)); /* Alocação do ponteiro para a estrutura tentativas_pb */
  tentativas=(char**)malloc((init->tent)*sizeof(char*)); /* Alocação da matriz que guarda as várias tentativas, aleatórias e válidas */
  resultados=(int**)malloc((init->tent)*sizeof(int*)); /* Alocação da matriz que guarda o número de pinos pretos e brancos da comparação da tentativa com a chave secreta */
  if (tentativas == NULL || resultados == NULL || tent_pb ==NULL ){ /* Confirmação se a memória foi bem alocada */
     printf("Falha na alocação de memória");
     exit (EXIT_FAILURE);
  }

  for(i=0;i<(init->tent);i++){ /* Correr o número de vezes do total de tentativas */
    if(i<(init->tent_aleatorias)) { /* gerar chaves aleatórias consoante o número de tentativas aleatórias */
      gerarchave(init->dimchave,init->cores,init->repcor,chavesecreta);
      strcpy(chave_permutacoes,chavesecreta);
    }
    else{
      if(i==(init->tent_aleatorias)){ /* gerar a primeira tentativa de cada opção(com ou sem repetição de cores) */
        if((init->repcor)==1){
          for(w=0;w<(init->dimchave);w++)
              chavesecreta[w]='A';
        }
        else{
          for(w=0;w<(init->dimchave);w++){
              chavesecreta[w]=a;
              a++;
          }
        }
      }
      chavesecreta[init->dimchave]='\0';
      end=0;
      var=valida_combinacao(chavesecreta,tentativas,resultados,i);
      while(end==0){ /* Só sai do ciclo quando a combinação e a permutação forem válidas */
        while(var==0){
          if((init->repcor)==1)
                var=gera_combinacoes_rep(init,chavesecreta,&j);
          else
                var=gera_combinacoes_sem_rep(init,chavesecreta,&j);
          var=valida_combinacao(chavesecreta,tentativas,resultados,i);
        }
        strcpy(chave_permutacoes,chavesecreta);
        end=permutations(chave_permutacoes,init->dimchave,tentativas,resultados,i);
        var=0;
      }
    }
    printf("\nTentativa %d: %s\n",i+1,chave_permutacoes);
    if(modo==2){
        pretos_brancos_final(chave_permutacoes,&pretos,&brancos);
        printf("P%dB%d\n",pretos,brancos);
    }
    else
      ler_pretos_brancos(&pretos,&brancos,init->dimchave);

    tentativas[i]=(char*)malloc(((init->dimchave)+1)*sizeof(char)); /* Alocação das linhas da matriz das tentativas */
    resultados[i]=(int*)malloc((2)*sizeof(int));  /* Alocação das linhas da matriz das tentativas */
    if (tentativas[i] == NULL || resultados[i] == NULL){ /* Confirmação se a memória foi bem alocada */
      printf("Falha na alocação de memória");
      exit (EXIT_FAILURE);
    }

    resultados[i][0]=pretos, resultados[i][1]=brancos;
    strcpy(tentativas[i],chave_permutacoes);
    if(modo==2){
      tent_pb->pretos=pretos, tent_pb->brancos=brancos;
      newtentativa=cria_tentativa(i,tent_pb,tentativas);
      listahead=insert_end_tent(listahead,newtentativa);
      newtentativa->tentativa=(char*)calloc((strlen(tentativas[i])+1),sizeof(char)); /* Alocação de cada tentativa na nova estrutura da tentativa */
      strcpy(newtentativa->tentativa,tentativas[i]);
    }
    if(pretos==(init->dimchave)){
      win=1;
      if(modo==2){
        reg_jogo->numjogadas=i+1;
      }
      printf("Parabéns, acertou!\n");
      break;
    }
  }
  if(modo==2){
    reg_jogo->chavesecreta = (char *) malloc((strlen(tentativas[i-1])+1)*sizeof(char)); /* Alocação da chave secreta no registo_jogo a cada jogo */
    if (((reg_jogo->chavesecreta)== NULL)&&(modo==2)){
      printf("Falha na alocação de memória");
      exit (EXIT_FAILURE);
    }
  }
  if(win!=1)
  {
    printf("Não conseguiu acertar!\n");
    if(modo==2){
      reg_jogo->numjogadas=i;
      reg_jogo->chavesecreta="----"; /* de modo a que no ficheiro historico.dat dê a entender que o utilizador nao acertou a chave secreta */
    }
  }
  else if(modo==2)
    strcpy(reg_jogo->chavesecreta,tentativas[i]);

  if (tentativas != NULL ||resultados!=NULL){
    for (w=0;w<=i;w++){
      if (tentativas[w]!=NULL ||resultados[w] !=NULL){
        free(tentativas[w]); /* Free das linhas da matriz */
        free(resultados[w]);
      }
    }
   free(tentativas); /* Free da matriz */
   free(resultados);
  }
  free(tent_pb); /* Free do apontador para a struct tentativas_pb */
}

/*-----------------------------------------------------------------------------------*/
/* Function: gere_combinacoes_sem_rep------------------------------------------------*/
/* Description: Gera todas as combinações, sem repetição, uma a uma,-----------------*/
/*       cosoante a dimensão da chave, o número de cores e a repetição de cores------*/
/* Return: 0 ------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------*/

int gera_combinacoes_sem_rep(init_dat *init,char chavesecreta[],int *j)
{
    int i=0,w=0;
    char b,s;

    b='A'+(init->cores)-1; /* Obter a cor máxima existente no jogo */
    if(chavesecreta[(init->dimchave)-1]!=b){ /* Caso a ultima posição não tenha a cor máxima */
        chavesecreta[(init->dimchave)-1]++;
        return 0;
    }
    else{
        i=0;
        while(i==0){
            if(chavesecreta[*j-1]!=chavesecreta[*j]-1){ /* Gerar a combinação sem repetição */
                chavesecreta[*j-1]++;
                i=1;
                s=chavesecreta[*j-1];
                for(w=*j;w<(init->dimchave);w++){
                    s++;
                    chavesecreta[w]=s;
                }
                *j=(init->dimchave)-1;
                return 0;
            }
            else
                *j=*j-1;
        }
    }
    return 0;
}

/*-----------------------------------------------------------------------------------*/
/* Function: gera_combinacoes_rep----------------------------------------------------*/
/* Description: Gera todas as combinações consoante a gerada anteriomente,-----------*/
/*        com repetição, uma a uma, cosoante a dimensão da chave, -------------------*/
/*        o número de cores e a repetição de cores-----------------------------------*/
/* Return: 0-------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------*/

int gera_combinacoes_rep(init_dat *init,char chavesecreta[],int *j)
{
  int w;
  char a,b;

  b='A'+(init->cores)-1;  /* Obter a cor máxima existente no jogo */
  if (chavesecreta[(init->dimchave)-1]!=b){
    chavesecreta[(init->dimchave)-1]++;
    return 0;
  }
  while(chavesecreta[*j]==b){ /* Gerar a combinação com repetição de cores */
      chavesecreta[*j-1]++;
      a=chavesecreta[*j-1];
      for(w=*j;w<(init->dimchave);w++)
          chavesecreta[w]=a;
      *j=*j-1;
      if(chavesecreta[*j]!=b)
          *j=(init->dimchave)-1;
      return 0;
  }
  return 0;
}

/*-----------------------------------------------------------------------------------*/
/* Function: permutations------------------------------------------------------------*/
/* Description: Gera uma permutação consoante a combinação introduzida, uma a uma----*/
/* Return: 1 ------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------*/

int permutations (char chave_permutacoes[],int dimchave,char **tentativas,int **resultados,int i)
{
    int pos_atual=0, end=0,atual=0,w=0;
    char aux;
    end=valida_permutacoes(chave_permutacoes,tentativas,resultados,i);

    while (end == 0){
        for (pos_atual = dimchave-2; pos_atual >= 0; --pos_atual){ /* caso da posição atual ter um caracter inferior à posição seguinte */
            if (chave_permutacoes[pos_atual] < chave_permutacoes[pos_atual+1])
                break;
        }
        if (pos_atual == -1)
            end=2;
        else { /* obtem a posição da letra mais "elevada" */
            atual=pos_atual+1;
            for(w=pos_atual+2;w<=dimchave-1;w++)
            {
              if(chave_permutacoes[w]>chave_permutacoes[pos_atual] && chave_permutacoes[w]<chave_permutacoes[atual])
                atual=w;
            }
            /* Troca dois caracteres na chave */
            aux=chave_permutacoes[pos_atual];
            chave_permutacoes[pos_atual]=chave_permutacoes[atual];
            chave_permutacoes[atual]=aux;
            qsort(chave_permutacoes+pos_atual+1,dimchave-pos_atual-1,sizeof(chave_permutacoes[0]), comparar); /* de modio às permutações serem ordenadas */
            end=valida_permutacoes(chave_permutacoes,tentativas,resultados,i);
        }
    }
    if(end==2)
        return 0;
    return 1;
}

/*-----------------------------------------------------------------------------------*/
/* Function: valida_combinacao-------------------------------------------------------*/
/* Description: A combinação só é válida se comparada com as tentativas anteriores,--*/
/*   a quantidade de pinos da comparação com as tentativas devolve o mesmo numéro da -*/
/*   comparação das tentativas com a chave secreta ----------------------------------*/
/* Return: 1 se for válida, 0 se for inválida----------------------------------------*/
/*-----------------------------------------------------------------------------------*/

int valida_combinacao(char sub_tent[],char **tentativas,int **resultados,int tent_atual)
{
  int j=0,pretos=0,brancos=0;

  for(j=0;j<tent_atual;j++){
    pretos_brancos_function(sub_tent,tentativas[j],&pretos,&brancos);
    if((pretos+brancos)!=(resultados[j][0]+resultados[j][1])){
      if(pretos==-1 && brancos==-1)
          exit(EXIT_FAILURE);
      return 0;
    }
  }
  return 1;
}

/*-----------------------------------------------------------------------------------*/
/* Function: valida_permutacoes------------------------------------------------------*/
/* Description: A permutação só é válida se comparada com as tentativas anteriores,--*/
/*   a quantidade de pinos pretos e brancos é o mesmo da comparação com as tentativas-*/
/*   devolve o mesmo numéro da comparação das tentativas com a chave secreta --------*/
/* Return: 1 se for válida, 0 se for inválida----------------------------------------*/
/*-----------------------------------------------------------------------------------*/

int valida_permutacoes(char sub_tent[],char **tentativas,int **resultados,int tent_atual)
{
  int j=0,pretos=0,brancos=0;

  for(j=0;j<tent_atual;j++){
    pretos_brancos_function(sub_tent,tentativas[j],&pretos,&brancos);
    if((pretos!=resultados[j][0])||(brancos!=resultados[j][1]))
      return 0;
  }
  return 1;
}

/*-----------------------------------------------------------------------------------*/
/* Function: jogo2-------------------------------------------------------------------*/
/* Description: Modo de jogo para o modo teste---------------------------------------*/
/* Return: --------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------*/

void jogo2 (int modo,init_dat *init, FILE*fp, FILE *fp_hist,registo_jogo *reg_jogo)
{
  int a=0;
  clock_t start,end; /* de modo a contar o tempo de cada jogo através dos clocks */
  registo_jogo *listahead=NULL, *newjogo=NULL;

  printf("MODO TESTE:\n");
  modo=2;
  listahead=(registo_jogo*)calloc(1,sizeof(registo_jogo)); /* Alocação do ponteiro que aponta para a struct registo_jogo */
  if (listahead==NULL ){  /* Confirmação se a memória foi bem alocada */
    printf("Não foi possível alocar o bloco de memória\n");
    exit(EXIT_FAILURE);
  }

  readfile(fp,init);
  activate_oracle(init->dimchave,init->cores,init->repcor);
  printf("Dimensão da chave %d, número de cores %d, tentativas aleatorias %d, número de jogos %d, número de tentativas %d\n",init->dimchave,init->cores,init->tent_aleatorias,init->jogos,init->tent);
  reg_jogo->jogos=init->jogos,reg_jogo->nome=init->nome,reg_jogo->cores=init->cores,reg_jogo->dimchave=init->dimchave,reg_jogo->rep_cor=init->rep_cor,reg_jogo->tent_aleatorias=init->tent_aleatorias,reg_jogo->tent=init->tent;
  for (a=0;a<(init->jogos);a++){
    printf("____________________Próximo Jogo____________________\n");
    sleep(1); /* de forma que ao longo dos jogos a chave gerada pelo oráculo seja diferente */
    generate_key(1);
    start=clock();
    printf("Jogo n.º %d\n",a+1);
    newjogo=(registo_jogo*)calloc(1,sizeof(registo_jogo)); /* Alocação do ponteiro para a struct registo_jogo */
    if (newjogo==NULL){ /* Confirmação se a memória foi bem alocada */
      printf("Não foi possível alocar o bloco de memória\n");
      exit(EXIT_FAILURE);
    }

    newjogo=cria_jogo(init,newjogo);
    listahead=insert_end(listahead,newjogo);
    tentativas_function(init,modo,reg_jogo,listahead);
    newjogo->numjogadas=reg_jogo->numjogadas;
    newjogo->chavesecreta=(char*)calloc((init->dimchave +1),sizeof(char)); /* Alocação da chave secreta a cada jogo realizado */
    newjogo->nome=(char*)calloc((strlen(init->nome)+1),sizeof(char)); /* Alocação do nome do jogador a cada jogo realizado */
    if (newjogo->chavesecreta==NULL || newjogo->nome ==NULL){ /* Confirmação se a memória foi bem alocada */
      printf("Não foi possível alocar o bloco de memória\n");
      exit(EXIT_FAILURE);
    }
    strcpy(newjogo->chavesecreta,reg_jogo->chavesecreta);
    strcpy(newjogo->nome,init->nome);
    free(reg_jogo->chavesecreta);
    end=clock();
    newjogo->tempjogado=((float)(end-start)/CLOCKS_PER_SEC);
    printf("Tempo Jogado %.3fs\n",newjogo->tempjogado);
  }
  print_lista_ficheiro(fp_hist,listahead);
  free_lista(listahead); /* free do ponteiro alocado nesta função */
  free(reg_jogo); /* free do ponteiro alocado nesta função */
}

/*-------------------------------------------------------------------------------------------------------*/
/* Function: modo_interativo-----------------------------------------------------------------------------*/
/* Description: Chama as funções do projeto intermédio paras as inicializações e guarda-as na estrutura--*/
/* Return: ----------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------*/

void modo_interativo(char matriz_nomes[4][21],init_dat* init,int *jogadores)
{
  int repcor,cores,dimchave;
  printf ("MODO INTERATIVO:\n");
  /* Passagem das variavéis para a struct init_dat */
  *jogadores = inicializar1();
  inicializar2(jogadores,matriz_nomes);
  init->jogos = inicializar3();
  inicializar4(&cores,&dimchave,&repcor);
  init->cores=cores;
  init->dimchave=dimchave;
  init->repcor=repcor;
  init->tent = inicializar5();
  init->tent_aleatorias = inicializar7(init->tent);
}

/*-----------------------------------------------------------------------------------------*/
/* Function: comparar----------------------------------------------------------------------*/
/* Description: Compara se um caracter é superior ao outro, cosoante a tabela de ASCII----*/
/* Return: --------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------*/

int comparar(const void* a, const void* b)
{
    return (*(char*)a - *(char*)b);
}

/*-----------------------------------------------------------------------------------*/
/* Function: pretos_brancos_function-------------------------------------------------*/
/* Description: Compara duas chaves, através do compare_keys do oráculo--------------*/
/*        e devolve a qantidade de pinos pretos e brancos-----------------------------*/
/* Return: --------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------*/

void pretos_brancos_function(char key1[],char key2[],int *pretos,int *brancos)
{
  reply *rep;
  rep=compare_keys(key1,key2);
  *pretos=get_blacks(rep);
  *brancos=get_whites(rep);

}

/*---------------------------------------------------------------------------------------------*/
/* Function: pretos_brancos_final--------------------------------------------------------------*/
/* Description: Compara a chave introduzida com a do generate_key,-----------------------------*/
/*       utilizando o validate_key do oraculo--------------------------------------------------*/
/* Return: ------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------*/

void pretos_brancos_final(char chave_permutacoes[],int *pretos,int *brancos)
{
  reply *rep;
  rep=validate_key(chave_permutacoes);
  *pretos=get_blacks(rep);
  *brancos=get_whites(rep);
}

/*------------------------------------------------------------------------------------------------*/
/* Function: ler_pretos_brancos-------------------------------------------------------------------*/
/* Description: Lê a quantidade de pinos pretos e brancos inseridos pelo utilizador e valida-os----*/
/* Return: ---------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------*/

void ler_pretos_brancos(int *pretos,int *brancos,int dimchave)
{
  int teste=1;
  char input[1000]={0};

  printf("Introduza o número de pinos pretos e de brancos\n");
  do{   /* Validação dos valores introduzidos pelo utilizador */
      printf("Pretos:");
      fgets(input,10000,stdin);
      if((sscanf(input,"%d", pretos)!= 1) || *pretos <0 || *pretos > dimchave || strlen(input) !=2){
          printf ("Inválido!\n");
          teste=0;
          continue;
      }
      printf("Brancos:");
      fgets(input,10000,stdin);
      if((sscanf(input,"%d", brancos)!= 1) || *brancos <0 || *brancos >dimchave || (*pretos+*brancos)> dimchave || strlen(input) !=2){
          printf ("Inválido!\n");
          teste=0;
          continue;
      }
      else
          teste=1;
    }while(teste==0);
}

/*-----------------------------------------------------------------------------------*/
/* Function: readfile----------------------------------------------------------------*/
/* Description: Lê as inicializações do ficheiro init--------------------------------*/
/* Return: --------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------*/

void readfile(FILE *fp,init_dat*init)
{
  char nome_temp[20];

  fscanf(fp,"%s\n%d\n%d\n%d\n%c\n%d\n%d\n",nome_temp,&init->jogos,&init->cores,&init->dimchave,&init->rep_cor,&init->tent_aleatorias,&init->tent);
  init->nome = (char *) malloc((strlen(nome_temp)+1)*sizeof(char));
  strcpy(init->nome, nome_temp);
  if (init->rep_cor=='S')
    init->repcor=1;
  else
    init->repcor=0;
  fclose(fp);
}

/*-----------------------------------------------------------------------------------*/
/* Function: cria_jogo---------------------------------------------------------------*/
/* Description: Cria o nó do registo de jogo, colocando as variáveis-----------------*/
/*       da estrutura registo_jogo --------------------------------------------------*/
/* Return: --------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------*/

registo_jogo *cria_jogo(init_dat*init,registo_jogo* newjogo)
{
  newjogo->jogos=init->jogos;
  newjogo->cores=init->cores;
  newjogo->dimchave=init->dimchave;
  newjogo->rep_cor=init->rep_cor;
  newjogo->tent_aleatorias=init->tent_aleatorias;
  newjogo->tent=init->tent;
  newjogo->nextjogo=NULL;
  newjogo->nexttentativa=NULL;

  return newjogo;
}

/*-----------------------------------------------------------------------------------*/
/* Function: cria_tentativa----------------------------------------------------------*/
/* Description: Cria o nó do registo de tentativas, colocando as variavéis-----------*/
/*         da estrutura tentativas_pb -----------------------------------------------*/
/* Return: --------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------*/

tentativas_pb *cria_tentativa(int tent_atual,tentativas_pb *tent_pb,char **tentativas)
{
  tentativas_pb *newtentativa=NULL;

  newtentativa=(tentativas_pb*)calloc(1,sizeof(tentativas_pb)); /* Alocação do ponteiro da struct tentativas_pb, alocado em cada tentativa */
  if (newtentativa==NULL){
    printf("Não foi possível alocar o bloco de memória1\n");
    exit(EXIT_FAILURE);
  }
  newtentativa->tent_atual=tent_atual+1;
  newtentativa->pretos=tent_pb->pretos;
  newtentativa->brancos=tent_pb->brancos;
  newtentativa->nexttentativa=NULL;

  return newtentativa;
}

/*-----------------------------------------------------------------------------------*/
/* Function: print_lista_ficheiro----------------------------------------------------*/
/* Description: Faz print da lista para o ficheiro histórico------------------------ */
/* Return: --------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------*/

void print_lista_ficheiro(FILE *fp,registo_jogo *head)
{
  registo_jogo *ptr;
  tentativas_pb *ptr_tent;
  int num=1;
  char *jogador_ID="J001";

  if (fp==NULL || head==NULL)
      exit(EXIT_FAILURE);

  for (ptr= head->nextjogo ; ptr!=NULL ; ptr= ptr->nextjogo, num++) {
    ptr->jogador_ID=(char*)malloc((strlen(jogador_ID)+1)*sizeof(char)); /* Alocação do jogador ID na struct apontada por ptr */
    if (ptr->jogador_ID==NULL){ /* Confirmação se a memória foi bem alocada */
      printf("Não foi possível alocar o bloco de memória\n");
      exit(EXIT_FAILURE);
    }
    strcpy(ptr->jogador_ID,jogador_ID);
    fprintf(fp,"%d %s %s %d %d %c %s %d %.3f\n",num,ptr->jogador_ID,ptr->nome,ptr->cores,ptr->dimchave, ptr->rep_cor,ptr->chavesecreta,ptr->numjogadas,ptr->tempjogado );
    for(ptr_tent=ptr->nexttentativa; ptr_tent !=NULL; ptr_tent=ptr_tent->nexttentativa){
      fprintf(fp,"%d %s P%dB%d\n",ptr_tent->tent_atual,ptr_tent->tentativa,ptr_tent->pretos,ptr_tent->brancos);
    }
  }
}
