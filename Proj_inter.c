#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <time.h>
#include <string.h>
#include "Proj_inter.h"
#include "oraculo.h"

#define MAX_JOGADORES 4
#define MAX_JOGOS 5
#define MIN_CORES 6
#define MAX_CORES 12
#define MIN_DIMCHAVE 4
#define MAX_DIMCHAVE 8
#define MIN_TENT 10
#define MAX_TENT 20
#define MIN_TEMPJOG 60
#define MAX_TEMPJOG 300

int inicializar1()
{
  int escolha=-1;
	char input[10000]={0}; //input-serve para verificar a veracidade do que é introduzido pelo utilizador

	while(escolha==-1){// Adicionada a variavel escolha como termo de teste para as diversas condições
        printf ("Introduza quantos jogadores são, entre 1 a %d: ", MAX_JOGADORES);
        fgets(input, 10000, stdin);
        if((sscanf(input, "%d", &escolha)!= 1) || (escolha<1 || escolha> MAX_JOGADORES) || (strlen(input)!=2)){ // caso introduzam letras OU nao estar dentro dos parametros OU não ser apenas um caracter
            printf ("Inválido!\n");
            escolha=-1;
            continue;
        }
  }
  return escolha;
}

void inicializar2(int *jogadores, char matriz_nomes[4][21])
{
    char nomes[1000];
    int i;
    //PEDIR OS NOMES DOS JOGADORES
    for (i=0;i<*jogadores;i++){
      do{
          printf ("\nIntroduza o nome do jogador %d (max 20 caracteres): ",i+1);
          fgets (nomes,10000,stdin);
          sscanf(nomes,"%s",matriz_nomes[i]); //matriz_nomes- guarda o nome de cada jogador em cada linha da matriz
          if ((strlen(nomes)>21) || (strlen(nomes)<=1))
              printf ("Inválido! \n");
      }while(strlen(nomes)>21 || strlen(nomes)<=1); // Devido ao nome so poder ter no max 20 caracteres, e /0 contar como um caracter é utilizado o valor 21
  }
}

int inicializar3()
{
    int escolha=-1; // mais uma vez usada como variavel teste para testar o que é inserido
    char input[100]={0}; //input-serve para verificar a veracidade do que é introduzido pelo utilizador

     while (escolha==-1){ // enquanto o que for introduzido for declarado como inválido então o programa retorna a pedir
         printf ("\nQuantos jogos quer realizar, entre 1 a %d ? ", MAX_JOGOS);
        fgets (input, 10000,stdin);
        if((sscanf(input, "%d", &escolha)!= 1) || ((escolha <1) || (escolha >MAX_JOGOS)) || (strlen(input)!=2)){ // caso introduzam letras OU nao estar dentro dos parametros OU não ser apenas um caracter
            printf ("Inválido!\n");
            escolha=-1;
            continue;
        }
    }
    return escolha; // a variavel jogos toma o valor da escolha introduzida pelo utlizador
}

void inicializar4(int *cores,int *dimchave,int *repcor)
{
    int a=0, escolha=-1; // a- verificar se é possivel formar a chave de cores consoante a dimensao da chave e de cores, sem repetição
    char input[1000]={0},letra; //input-serve para verificar a veracidade do que é introduzido pelo utilizador
    // REPETIÇÃO DE CORES
    while (a==0){
        a=0; // para nao funcionar em loop
        while (escolha==-1){ // caso o que for introduzido nao for válido o programa retoma a pedir
          printf("\nA chave pode ter repetições de cores? \nSe sim prima 'S', caso contrário prima 'N': ");
          fgets (input, 1000,stdin);
          sscanf (input,"%c",&letra);
          if((!(letra =='S' || letra == 'N' || letra=='s' || letra=='n')) || strlen(input)!= 2){ // caso introduzam uma letra diferente de S ou N, aceitam se minusculas e maisculas, sendo apenas aceite um caracter
              printf ("Inválido!\n");
              escolha=-1;
              continue;
          }
          else
            escolha=0;
        }
          if (letra=='S' || letra=='s')
            *repcor=1;                    //atribuida um valor à variavel repcor caso haja ou nao repetição de cores
          if (letra=='N' || letra=='n')
            *repcor=0;                    // repcor=0 se nao existir repetição de cor
        a=1; // para ser possivel fechar o ciclo de pedir a repetição
        // QUANTIDADE DE CORES
        escolha=-1;
        while (escolha==-1){ // caso o que for introduzido nao for válido o programa retoma a pedir
            printf ("\nIntroduza o numero de cores que quer utilizar, entre %d a %d: ", MIN_CORES, MAX_CORES);
            fgets (input,10000,stdin);
            if((sscanf(input, "%d", &escolha)!= 1) || escolha <MIN_CORES || escolha >MAX_CORES || (strlen(input)!=2 && escolha<10) || (strlen(input)!=3 && escolha>=10)){ // caso introduzam letras OU nao estar dentro do paramentro OU não ser um caracter ou 2 caracteres pq /0 conta como um caracter nao ser aceite o caso 6a por exemplo
                printf ("Inválido!\n");
                escolha=-1;
                continue;
            }
        }
        *cores=escolha; //o valor introduzido pelo utilizador passa para a variavel cores-quantidade de cores utilizadas na chave
        // DIMENSAO DA CHAVE
        escolha=-1;
        while (escolha==-1){ // enquanto nao for válido volta a ser pedido
            printf(" \nIntroduza a dimensão da chave secreta, entre %d e %d: ", MIN_DIMCHAVE, MAX_DIMCHAVE);
            fgets(input,10000,stdin);
            if((sscanf(input, "%d", &escolha)!= 1) || escolha <MIN_DIMCHAVE || escolha >MAX_DIMCHAVE || (strlen(input)!=2)){ // caso introduzam letras OU nao estar dentro dos parametros OU não ser apenas um caracter
                printf ("Inválido!\n");
                escolha=-1;
                continue;
            }
        }
        *dimchave= escolha; // o valor introduzido passa para a variavel dimchave-dimensão da chave
        // CASO HAJA REPETIÇÃO DE CORES E A DIMENSAO DA CHAVE SEJA SUPERIOR À QUANT DE CORES
        if ((*repcor==0) && (*dimchave>*cores)) {
            a=0;
            printf ("Inválido! Como nao há repetição de cores, a dimensao da chave não pode ser superior à quantidade de cores escolhidas! \n");
        }
        else
            a=1; // para fechar o while
    }
}

int inicializar5()
{
    int escolha=-1;
    char input[100]={0}; //input-serve para verificar a veracidade do que é introduzido pelo utilizador

    while (escolha==-1){ // caso o que for introduzido nao for válido o programa retoma a pedir
        printf("\nIntroduza o número máximo de tentativas, entre %d e %d: ", MIN_TENT, MAX_TENT);
        fgets(input,10000, stdin);
        if((sscanf(input, "%d", &escolha)!= 1) || (escolha <MIN_TENT) || (escolha >MAX_TENT) || (strlen(input)!=3)){ // caso introduzam letras OU nao estar dentro dos parametros OU não ser apenas um caracter
            printf ("Inválido!\n");
            escolha=-1;
            continue;
        }
    }
    return escolha; // o valor introduzido pelo utilizador passa para a variavel tent-tentativas
}

int inicializar6()
{
    int escolha=-1;
    char input[1000]={0}; //input-serve para verificar a veracidade do que é introduzido pelo utilizador

    while (escolha==-1){ // caso o que for introduzido nao for válido o programa retoma a pedir
        printf("\nIntroduza o tempo máximo de jogo, entre %d e %d: ", MIN_TEMPJOG, MAX_TEMPJOG);
        fgets(input,10000, stdin);
        if((sscanf(input, "%d", &escolha)!= 1) || (escolha < MIN_TEMPJOG) || (escolha >MAX_TEMPJOG) || (strlen(input)!=3 && escolha<100 ) ||(strlen(input)!=4 && escolha>=100)){ // caso introduzam letras OU nao estar dentro dos parametros OU não ser apenas 2 ou 3 caracteres pq /0 conta como um caracter e nao ser aceite o caso 60a por exemplo
            printf ("Inválido!\n");
            escolha=-1;
            continue;
        }
    }
    return escolha; // o valor introduzido pelo utilizador passa para a variavel tempjog-tempo de jogo total
}

void gerarchave(int dimchave,int cores,int repcor,char chavesecreta[100])
{
  int  i=0,j=0,quantcores;
  char cortemp='0', numero='0', maiscula='0', b='A';

  quantcores= cores; // para a variavel que foi introduzida na variavel cores ser fixa ao longo do ciclo do for
  // GERAR CHAVE SEM REPETIÇÃO
	if (repcor==0) {
        for(i = 0; i < dimchave; i++){
          cortemp = rand()%cores + 65; // gera uma chave com letras aletatórias até ao valor introduzido pela quantidadede cores e o número de vezes da dimensao da chave
          j = 0;
          while(j < i) {
              if(chavesecreta[j] != cortemp) // se a letra na posição j for diferente à gerada anterorimente entao o j aumenta
                  j++;
              else if (chavesecreta[j] == cortemp){ // se a letra na posição j for a mesma que a gerada anterorimente entao é gerada uma nova letra e o j volta a ser zero de modo a verificar todas as posições
                  cortemp = rand()%cores + 65;
                  j = 0;
              }
          }
          chavesecreta[i] = cortemp;
        }
        chavesecreta[i]='\0';
    }
    // GERAR CHAVE COM REPETIÇÃO
    else if (repcor==1){
        for (cores=0; cores<dimchave; cores++){
            numero=rand()% quantcores ; // gerar uma chave ate ao valor de cores introduzido
            maiscula= numero+b; // de modo a imprimir a chave em letras
            chavesecreta[cores] = maiscula; //arranjar uma chave utilizando numeros até ao escolhido de cores pelo utilizador
        }
        chavesecreta[cores]='\0';
    }
}

int inicializar7(int tent)
{
  int escolha=-1;
	char input[10000]={0}; //input-serve para verificar a veracidade do que é introduzido pelo utilizador

	while(escolha==-1){// Adicionada a variavel escolha como termo de teste para as diversas condições
        printf ("\nIntroduza quantas tentativas aleatórias, entre 0 a %d: ", tent-1);
        fgets(input, 10000, stdin);
        if((sscanf(input, "%d", &escolha)!= 1) || (escolha<0 || escolha> tent) || ((strlen(input)!=2)&&(strlen(input)!=3))){ // caso introduzam letras OU nao estar dentro dos parametros OU não ser apenas um caracter
            printf ("Inválido!\n");
            escolha=-1;
            continue;
        }
  }
  return escolha;
}
