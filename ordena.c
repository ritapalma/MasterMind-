#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include "ordena.h"
#include "oraculo.h"
#include "Proj_inter.h"

#define LINE_SIZE 200
#define STR_SIZE 200
#define NOME_FICHEIRO "historico.dat"

/*-----------------------------------------------------------------------------------*/
/* Function: create_regist_jogo------------------------------------------------------*/
/* Description: Cria o registo de jogo, com as variáveis lidas ----------------------*/
/*--------no ficheiro de histórico, a cada jogo lido---------------------------------*/
/* Return: --------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------*/

registo_jogo *create_regist_jogo(int num,char *nome, char *jogador_ID, int cores, int dimchave, char rep_cor, char *chavesecreta, int numjogadas, float tempjogado)
{
  registo_jogo *newjogo=NULL;
  newjogo=(registo_jogo*)calloc(1,sizeof(registo_jogo)); /*Alocação do ponteiro para a estrutura registo_jogo, que aloca espaço na memória a cada jogo*/
  newjogo->chavesecreta=(char*)malloc(dimchave*sizeof(int));
  newjogo->jogador_ID=(char*)malloc((strlen(jogador_ID)+1)*sizeof(char));
  newjogo->nome=(char*)malloc((strlen(nome)+1)*sizeof(char));
  if (newjogo==NULL || newjogo->chavesecreta==NULL || newjogo->jogador_ID==NULL|| newjogo->nome==NULL ){ /* Confirmação se a memória foi bem alocada */
    printf("Não foi possível alocar o bloco de memória\n");
    exit(EXIT_FAILURE);
  }
  /* Passagem das variavéis para o nó da lista */
  strcpy(newjogo->chavesecreta,chavesecreta);
  strcpy(newjogo->jogador_ID,jogador_ID);
  strcpy(newjogo->nome,nome);
  newjogo->num=num;
  newjogo->cores=cores;
  newjogo->dimchave=dimchave;
  newjogo->rep_cor=rep_cor;
  newjogo->numjogadas=numjogadas;
  newjogo->tempjogado=tempjogado;
  newjogo->nextjogo=NULL;
  newjogo->nexttentativa=NULL;

  return newjogo;
}

/*-----------------------------------------------------------------------------------*/
/* Function: create_registo_tentativas-----------------------------------------------*/
/* Description: Cria o registo de tentativas, com as variáveis lidas ----------------*/
/*--------no ficheiro de histórico, a cada tentativa lida, em cada jogo--------------*/
/* Return: --------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------*/

tentativas_pb *create_registo_tentativas(int tent_atual, char tentativa[], int pretos, int brancos)
{
  tentativas_pb *newtentativa=NULL;

  newtentativa=(tentativas_pb*)calloc(1,sizeof(tentativas_pb)); /* Alocação do ponteiro para a estrutura tentativas_pb, que aloca espaço na memória a cada tentativa*/
  newtentativa->tentativa=(char*)malloc((strlen(tentativa)+1)*sizeof(char));
  if ((newtentativa==NULL)||(newtentativa->tentativa==NULL)){ /* Confirmação se a memória foi bem alocada */
    printf("Não foi possível alocar o bloco de memória\n");
    exit(EXIT_FAILURE);
  }
  /* Passagem das variavéis para o nó da lista */
  strcpy(newtentativa->tentativa,tentativa);
  newtentativa->tent_atual=tent_atual;
  newtentativa->pretos=pretos;
  newtentativa->brancos=brancos;
  newtentativa->nexttentativa=NULL;

  return newtentativa;
}

/*---------------------------------------------------------------------------------------*/
/* Function: insert_end------------------------------------------------------------------*/
/* Description: insere cada jogo na lista de jogos, consoante o jogo em questão----------*/
/* Return: ------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------*/

registo_jogo* insert_end(registo_jogo *head, registo_jogo *ptr)
{
  registo_jogo *aux1,*aux2;

  if (head==NULL){  /* Primeiro caso quando ainda não foi inserido nenhum nó na lista dos registos de jogos */
    head=ptr;
    head->nextjogo=NULL;
    head->nexttentativa=NULL;
  }
  else{   /* Insere o nó na lista na posição pretendida, consoante o que já foi adicionado anteriormente */
    aux1=head;
    aux2=head->nextjogo;
    while (aux2!=NULL){
      aux1=aux2;
      aux2=aux2->nextjogo;
    }
    aux1->nextjogo=ptr;
  }
  return head;
}

/*---------------------------------------------------------------------------------------*/
/* Function: insert_end------------------------------------------------------------------*/
/* Description: insere cada tentativa na lista de tentativas correspondente a cada jogo--*/
/* Return: ------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------*/

registo_jogo* insert_end_tent(registo_jogo *head, tentativas_pb *ptr)
{
  registo_jogo *aux1,*aux2;
  tentativas_pb *auxiliar1,*auxiliar2;

  if(1){    /* Primeiro caso quando ainda não foi inserido nenhum nó na lista das tentativas a cada jogo */
      aux1=head;
      aux2=head->nextjogo;
      while (aux2!=NULL){ /* Insere as tentativas segundo o jogo atual */
        aux1=aux2;
        aux2=aux2->nextjogo;
      }
      if(aux1->nexttentativa!=NULL) { /* Insere o nó na lista na posição pretendida, consoante o que já foi adicionado anteriormente */
        auxiliar1=aux1->nexttentativa;
        auxiliar2=auxiliar1->nexttentativa;
        while (auxiliar2!=NULL) {
            auxiliar1=auxiliar2;
            auxiliar2=auxiliar2->nexttentativa;
        }
        auxiliar1->nexttentativa=ptr;
      }
      else
        aux1->nexttentativa=ptr;
  }
  return head;
}

/*---------------------------------------------------------------------------------------*/
/* Function: load_lista------------------------------------------------------------------*/
/* Description: lê todas as linhas do ficheiro e consoante os argumentos-----------------*/
/*---------de cada linha do histórico cria a lista de listas-----------------------------*/
/* Return: ------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------*/

registo_jogo *load_lista(FILE *fp)
{
    char line[LINE_SIZE], nome[STR_SIZE],jogador_ID[STR_SIZE],chavesecreta[STR_SIZE],rep_cor='0',tentativa[LINE_SIZE];
    int cores,dimchave,numjogadas,num=0,tent_atual=0,pretos,brancos;
    float tempjogado;
    registo_jogo *head, *aux_ptr;
    tentativas_pb *aux_tent;

    head= NULL;
    while( fgets(line, LINE_SIZE, fp) != NULL )  {
        if (sscanf(line,"%d %s %s %d %d %c %s %d %f\n",&num,jogador_ID,nome,&cores,&dimchave,&rep_cor,chavesecreta,&numjogadas,&tempjogado) == 9) {
          aux_ptr= create_regist_jogo(num,nome,jogador_ID,cores,dimchave,rep_cor,chavesecreta,numjogadas,tempjogado); /* cria o nó da lista */
          if (aux_ptr==NULL) {  /* confirmação se a memória foi bem alocada */
              printf("ERROR 1: Memory allocation for list element failed.\n");
              exit(EXIT_FAILURE);
          } else
              head=insert_end(head, aux_ptr);  /* insere o nó na lista  de jogos */
        }
        else {
          sscanf(line,"%d %s P%dB%d\n",&tent_atual,tentativa,&pretos,&brancos);
          aux_tent=create_registo_tentativas(tent_atual,tentativa,pretos, brancos); /* cria o nó da lista */
          if (aux_tent==NULL){  /* confirmação se a memória foi bem alocada */
              printf("ERROR 1: Memory allocation for list element failed.\n");
              exit(EXIT_FAILURE);
          } else
              head=insert_end_tent(head, aux_tent);  /* insere o nó da lista de tentativas, consoante o jogo em questão */
        }
    }
    return head;
}

/*---------------------------------------------------------------------------------------*/
/* Function: write_lista-----------------------------------------------------------------*/
/* Description: Escreve a lista ordenada no ficheiro histórico---------------------------*/
/* Return: ------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------*/

void write_lista(FILE *fp, registo_jogo *head)
{
   registo_jogo *ptr;
   tentativas_pb *ptr_tent;

   if (fp==NULL || head==NULL)
       exit(EXIT_FAILURE);

   for (ptr= head ; ptr!=NULL ; ptr= ptr->nextjogo) { /* print da lista de jogos */
          fprintf(fp, "%d %s %s %d %d %c %s %d %.3f\n",ptr->num,ptr->jogador_ID,ptr->nome,ptr->cores,ptr->dimchave, ptr->rep_cor,ptr->chavesecreta,ptr->numjogadas,ptr->tempjogado );
          for(ptr_tent=ptr->nexttentativa; ptr_tent !=NULL; ptr_tent=ptr_tent->nexttentativa){ /* em cada jogo, print da lista de tentativas correspondente ao jogo*/
            fprintf(fp,"%d %s P%dB%d\n",ptr_tent->tent_atual,ptr_tent->tentativa,ptr_tent->pretos,ptr_tent->brancos);
          }
    }
}

/*---------------------------------------------------------------------------------------*/
/* Function: troca-----------------------------------------------------------------------*/
/* Description: Troca a ordem de 2 elementos na lista de listas, ------------------------*/
/*----------neste caso da lista de jogos-------------------------------------------------*/
/* Return: ------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------*/

registo_jogo *troca(registo_jogo *left, registo_jogo *right)
{
  left->nextjogo = right->nextjogo;
  right->nextjogo = left;
  return right;
}

/*---------------------------------------------------------------------------------------*/
/* Function: sort_lista------------------------------------------------------------------*/
/* Description: Ordena a lista de listas, primeiramente organiza segundo categorias------*/
/*--------e depois ordena segundo o critério da linha de comando ------------------------*/
/* Return: ------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------*/

registo_jogo *sort_lista(registo_jogo *ListHead, ordenacao ordem)
{
    registo_jogo *left, *right,*head, aux_struct;
    bool trocas= true;

    head = &aux_struct;
    head->nextjogo = ListHead;
    if ((ListHead != NULL ) && ((ListHead->nextjogo != NULL ))){
       while(trocas) { /* ordenar o histórico todo, garantindo que todas as trocas necessárias são executadas */
           trocas= false;
           left= head;
           right= head->nextjogo;
           while(right->nextjogo != NULL) { /* quando já não houver mais jogos para ordenar então já nao será executado o ciclo */
               if (right->dimchave > right->nextjogo->dimchave){  /* organizar primeiro pela dimensão da chave */
                    left->nextjogo= troca(right, right->nextjogo);
                    trocas= true; /* houve 1 troca, necessário percorrer a lista toda novamente */
               }
               else if (right->dimchave==right->nextjogo->dimchave && right->cores > right->nextjogo->cores ){ /* organizar pelo número de cores */
                    left->nextjogo= troca(right, right->nextjogo);
                    trocas= true; /* houve 1 troca, necessário percorrer a lista toda novamente */
               }
               else if(right->rep_cor=='S' && right->nextjogo->rep_cor=='N' && right->cores==right->nextjogo->cores && right->dimchave==right->nextjogo->dimchave){ /* organizar pela repetição de cores */
                    left->nextjogo= troca(right, right->nextjogo);
                    trocas= true; /* houve 1 troca, necessário percorrer a lista toda novamente */
               }
               else if (right->rep_cor== right->nextjogo->rep_cor && right->cores==right->nextjogo->cores && right->dimchave==right->nextjogo->dimchave){ /* ordenar pelo modo escolhido no modo de execução */
                 if ((ordem==TEMPJOGADO && right->tempjogado > right->nextjogo->tempjogado) ||
                     (ordem==NUMJOGADAS && right->numjogadas > right->nextjogo->numjogadas) )  {
                     trocas= true; /* houve 1 troca, necessário percorrer a lista toda novamente */
                     left->nextjogo= troca(right, right->nextjogo);
                 }
                 else{  /* caso haja empate ordenar pela ordem contrária, para desempatar */
                     if (ordem==TEMPJOGADO){
                       if(right->rep_cor==right->nextjogo->rep_cor && right->cores==right->nextjogo->cores && right->dimchave==right->nextjogo->dimchave && right->tempjogado== right->nextjogo->tempjogado && right->numjogadas > right->nextjogo->numjogadas){
                           trocas= true; /* houve 1 troca, necessário percorrer a lista toda novamente */
                           left->nextjogo= troca(right, right->nextjogo);
                      }
                     }
                     else{
                        if(right->rep_cor==right->nextjogo->rep_cor && right->cores==right->nextjogo->cores && right->dimchave==right->nextjogo->dimchave && right->numjogadas == right->nextjogo->numjogadas && right->tempjogado > right->nextjogo->tempjogado){
                           trocas= true; /* houve 1 troca, necessário percorrer a lista toda novamente */
                           left->nextjogo= troca(right, right->nextjogo);
                        }
                    }
                 }
               }
               left=right;
               if (right->nextjogo != NULL)
                      right = right->nextjogo; /* de modo a avançar na lista */
           }
      }
   }
   right = head->nextjogo;
   return right;
}

/*---------------------------------------------------------------------------------------*/
/* Function: free_lista------------------------------------------------------------------*/
/* Description: Liberta a memória alocada para a lista de listas-------------------------*/
/* Return: ------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------*/

void free_lista(registo_jogo *ListHead)
{
    registo_jogo *aux_jogo, *jogo;
    tentativas_pb *aux_tentativa, *tent;

    aux_jogo = ListHead;
    while (aux_jogo != NULL){ /* liberta cada jogo da lista */
        aux_tentativa = aux_jogo->nexttentativa;
        while (aux_tentativa != NULL) { /*liberta a lista de tentativas correspondente a cada jogo */
            tent = aux_tentativa;
            aux_tentativa = aux_tentativa->nexttentativa;
            free(tent->tentativa);
            free(tent);
        }
        jogo = aux_jogo;
        aux_jogo = aux_jogo->nextjogo;
        free(jogo->jogador_ID);
        free(jogo->chavesecreta);
        free(jogo->nome);
        free(jogo);
    }
}

/*---------------------------------------------------------------------------------------*/
/* Function: usage-----------------------------------------------------------------------*/
/* Description: caso haja argumentos inválidos, colocar informações no output------------*/
/* Return: ------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------*/

void usage(char *programa)
{
    printf("Utilização: %s [opções] [nome]\n", programa);
    printf("opções válidas:\n");
    printf("[-h ficheiro]\t   usar <ficheiro> de entrada em vez de \"%s\"\n", NOME_FICHEIRO);
    printf("[-o short|fast] ordenar por tempo ou por numero de tentativas\n");
}

/*---------------------------------------------------------------------------------------*/
/* Function: organizar-------------------------------------------------------------------*/
/* Description: Ler os diversos argumentos e chamar as funções necessárias --------------*/
/*--------de modo a organizar a lista de listas------------------------------------------*/
/* Return: ------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------*/

void organizar (int argc, char *argv[])
{
    registo_jogo *lista_head= NULL;
    char *ficheiro= NOME_FICHEIRO;
    FILE *fp=NULL;                       /* para ler ou escrever o ficheiro */
    ordenacao ordem= TEMPJOGADO;          /* modo de ordenação */
    bool ordenar= false, escrever= false;    /* flags auxiliares */
    int opt;    /* para getopt() */

    /* processar as opções da linha de comando */
    while ((opt= getopt(argc, argv, "i:h:t:arpco:h")) != -1) {
        switch (opt) {
            case 'i':
                continue;
            case 'h': /* nome do ficheiro de entrada/saída */
                ficheiro= optarg;
                break;
            case 'o': /* ordenar: verificar o parâmetro com modo de ordenação */
                if (strcmp(optarg, "short")==0) {
                    ordem= NUMJOGADAS;
                    ordenar= true;
                } else if (strcmp(optarg, "fast")==0) {
                    ordem= TEMPJOGADO;
                    ordenar= true;
                } else {
                    printf("opção de ordenação \"%s\" desconhecida. ignorada.\n", optarg);
                    usage(argv[0]);
                }
                break;
            default: /* '?' opções inválidas */
                usage(argv[0]);
                exit(EXIT_FAILURE);
        }
    }
    /* ler o ficheiro para a lista */
    fp= fopen(ficheiro, "r");
    if (fp == NULL)  {  /* confirmação se o ficheiro existe */
        printf("Cannot open input file \"%s\". A new file will be created.\n", ficheiro);
        escrever= true; /* forçar a criação do ficheiro no fim */
    } else {
        lista_head= load_lista(fp);
        fclose(fp); /* após ler a lista o ficheiro é fechado do modo leitura */
    }
    if (ordenar) {
        lista_head= sort_lista(lista_head, ordem);
        escrever= true;  /* é preciso re-escrever o ficheiro */
    }
    escrever=true;
    if (escrever) {
        fp= fopen(ficheiro, "w");
        if (fp == NULL)  { /* confirmação se o ficheiro foi aberto */
            printf("ERROR: Cannot open output file \"%s\"!\n", ficheiro);
            free_lista(lista_head);
            exit(EXIT_FAILURE);
        }
    }

    write_lista(fp, lista_head);
    fclose(fp); /* após escrever no ficheiro é fechado do modo de escrita */
    free_lista (lista_head);
}
