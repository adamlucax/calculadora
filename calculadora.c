/*
 * Universidade Federal de Sergipe
 * Curso: Ciência da Computação
 * Disciplina: Estrutura de Dados I
 * Professor: Carlos Alberto Estombelo Montesco
 * Data: 27/04/2016
 * Alunos: ADAM LUCAS PINHEIRO DA SILVA (201500017836)
 *         FELIPE DE CARVALHO PEREIRA (201500018029)
 *         TAMAR LUIZA CONSERVA DOS SANTOS (201500018430)
 *         YANN GABRIEL FREIRE DE CARVALHO (201500018495)
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <math.h>

typedef struct reg noh;
typedef noh *arvore;

struct reg
{
    char conteudo [255];
    noh *esq;
    noh *dir;
};

bool isoperador (char a)
{
    return (a == '+' || a == '-' || a == '/' || a == '*');
}

bool isNull(noh *a)
{
    return a==NULL;
}

char *substr (char *str, int inicio, int fim)
//pré-condição: str ! NULL && inicio >= 0 && fim >=0
//pós-condição: Retorna uma sub-string limitada pelas posições dado por inicio e fim. Se fim < inicio então retorna string vazia
{
  if (fim >= inicio)
  {
      int tamanho = fim-inicio;
      char *aux = malloc(sizeof(char)*(tamanho));
      tamanho++;
      memset (aux,0,sizeof(aux));
      memcpy(aux,&str[inicio],tamanho+1);
      aux[tamanho]= '\0';
    return aux;
  } else return "\0";
}

void varreduraERD (noh *arv)
//Imprime a arvore binaria a partir do modo de varredura esquerda-raiz-direita (ERD)
{
    if (arv != NULL)
    {
        varreduraERD (arv->esq);
        printf ("%s ", arv->conteudo);
        varreduraERD (arv->dir);
    }
}

void varreduraEDR (noh *arv)
//Imprime a arvore binaria a partir do modo de varredura esquerda-direita-raiz (EDR)
{
    if (arv != NULL)
    {

        varreduraEDR (arv->esq);
        varreduraEDR (arv->dir);
        printf ("%s ", arv->conteudo);
    }
}

void varreduraRED (noh *arv)
//Imprime a arvore binaria a partir do modo de varredura raiz-esquerda-direita (RED)
{
    if (arv != NULL)
    {
        printf ("%s ", arv->conteudo);
        varreduraRED (arv->esq);
        varreduraRED (arv->dir);
    }
}

int emfilerar (int tam_s, char s[tam_s],char fila[1000][255])
//Repassa os valores e variaveis da euqação contidos na string s para uma estrutura de dados do tipo fila; e
//Retorna um inteiro contendo o numero total de elementos na fila.
{
    int i = 0,tam_p=1;
    for (; i<tam_s; i++)
    {
       if (isdigit(s[i]))
       {
           strcpy(fila[tam_p],substr(s,i,i));
           while (s[++i]=='.' || isdigit(s[i]))
             strcat(fila[tam_p],substr(s,i,i));
          --i;
           tam_p++;
        } else if (isoperador(s[i])) {
           strcpy(fila[tam_p++],substr(s,i,i));
        } else if (s[i] == 's') {
            strcpy(fila[tam_p++],"sqrt");
        }
     }
     return tam_p-1;

}

typedef struct stack {
    char conteudoPilha;
    struct stack* proximo;
} pilha;


pilha* criapilha(void) {
    pilha* myPilha = malloc(sizeof(pilha));
    myPilha->proximo = NULL;
    return myPilha;
}

void empilha(pilha* myPilha, char conteudo) {
    pilha* nova = malloc(sizeof(pilha));
    nova->proximo = myPilha->proximo;
    nova->conteudoPilha = conteudo;
    myPilha->proximo = nova;
}

char desempilha(pilha* myPilha) {
    pilha* aux = myPilha->proximo;
    myPilha->proximo = aux->proximo;

    char ch = aux->conteudoPilha;

    free(aux);

    return ch;
}

int contaPilha(pilha* myPilha) {
    if(myPilha->proximo == NULL)
        return 0;
    else
        return 1 + contaPilha(myPilha->proximo);
}

char *toPosfixa(char *infixa)
{
  int n, i = 0, j = 0;
  char *posfixa, *num;
  pilha* pilhaaux;

  n = strlen(infixa);
  posfixa = malloc((n*2+1) * sizeof(char));

  num = malloc((n*2+1) * sizeof(char));

  pilhaaux = criapilha();
  empilha(pilhaaux, infixa[0]);

  while (infixa[i] != '\0')
  {
    char ch;
    i++;

    if (infixa[i] == '(') {
      empilha(pilhaaux, infixa[i]);
    } else if (infixa[i] == ')') {
      ch = desempilha(pilhaaux);
      while (ch != '(')
      {
        posfixa[j++] = ch;
        posfixa[j++] = ' ';
        ch = desempilha(pilhaaux);
      }
    } else if (infixa[i] == '+' || infixa[i] == '-') {
      ch = desempilha(pilhaaux);
      while (ch != '(')
      {
        posfixa[j++] = ch;
        posfixa[j++] = ' ';
        ch = desempilha(pilhaaux);
      }

      empilha(pilhaaux, ch);
      empilha(pilhaaux, infixa[i]);
    } else if (infixa[i] == '*' || infixa[i] == '/') {
      ch = desempilha(pilhaaux);
      while (ch != '(' && ch != '+' && ch != '-')
      {
        posfixa[j++] = ch;
        posfixa[j++] = ' ';
        ch = desempilha(pilhaaux);
      }

      empilha(pilhaaux, ch);
      empilha(pilhaaux, infixa[i]);
    } else if(isdigit(infixa[i])) {
        posfixa[j++] = infixa[i];
        while(isdigit(infixa[i+1])){
            i++;
            posfixa[j++] = infixa[i];
        }
        posfixa[j++] = ' ';
    } else if (strlen(&infixa[i]) >= 4 && strncmp(&infixa[i], "sqrt", 4) == 0) {
        empilha(pilhaaux, 's');
    }
  }

  posfixa[j] = '\0';

  return posfixa;
}


double calc (double num1,char *op, double num2)
//Retorna um calculo efetuado atraves dos valores num1 e num2, usando como operador o definido pelo char *op
//A função também imprime o valor da equação efetuada
//Obs.: Quando a operação é Raiz quadrada só é calculado num2 (nesse caso o valor de num1 e ignorado)
{
           switch (op[0]){
            case '+':  if (num2<0.0)
                             printf("\t-> %f + (%f) = %f\n",num1,num2,num1+num2);
                       else  printf("\t-> %f + %f = %f\n",num1,num2,num1+num2);
                       return num1+num2;
            break;
            case '-':  if (num2<0.0)
                              printf("\t-> %f - (%f) = %f\n",num1,num2,num1-num2);
                       else  printf("\t-> %f - %f = %f\n",num1,num2,num1-num2);
                       return num1-num2;
            break;
            case '*':  if (num2<0.0)
                              printf("\t-> %f * (%f) = %f\n",num1,num2,num1*num2);
                       else  printf("\t-> %f * %f = %f\n",num1,num2,num1*num2);
                       return num1*num2;
            break;
            case '/':  if (num2<0.0)
                              printf("\t-> %f / (%f) = %f\n",num1,num2,num1/num2);
                       else  printf("\t-> %f / %f = %f\n",num1,num2,num1/num2);
                       return num1/num2;
            break;
            case 's':  printf("\t-> sqrt(%f) = %f\n",num2,sqrt(num2));
                       return sqrt(num2);
            break;
          }
}

double recursaoCalc(noh *arv)
//Percorre os galhos arvore binaria selecionando os valores que serão calculados;e
//Retorna o resultado da equação da arvore.
{
    double total = 0.0;

    if (arv->conteudo[0] == 's'){

        if (isdigit(arv->dir->conteudo[0]))
             total = calc(0.0,"sqrt",atof(arv->dir->conteudo));
        else total = calc(0.0,"sqrt",recursaoCalc(arv->dir));

    }else if (isNull(arv->esq->esq) && isNull(arv->esq->dir) &&
              isNull(arv->dir->esq) && isNull(arv->dir->dir)) {
        total = calc(atof(arv->esq->conteudo),arv->conteudo,atof(arv->dir->conteudo));

    }else if (isNull(arv->esq->esq) && isNull(arv->esq->dir)){
        double aux = (recursaoCalc(arv->dir));
        total = calc(atof(arv->esq->conteudo),arv->conteudo,aux);

    }else if (isNull(arv->dir->esq) && isNull(arv->dir->dir)){
        double aux = (recursaoCalc(arv->esq));
        total = calc(aux,arv->conteudo,atof(arv->dir->conteudo));

    }else{
        printf("\n");
        double aux1 = recursaoCalc(arv->esq);
        printf("\n");
        double aux2 = recursaoCalc(arv->dir);
        printf("\n");
        total = calc(aux1,arv->conteudo,aux2);
    }
    return total;
}

noh *criaNoh (char* conteudo,noh *esquerda, noh *direita )
//Cria uma nó (uma folha)
{
      noh *novoNo = malloc(sizeof(noh));
      strcpy(novoNo->conteudo, conteudo);
      novoNo->esq = esquerda;
      novoNo->dir = direita;
      return novoNo;
}

noh *formaArvore(int *tamanho,char fila[1000][255] )
//Forma uma Arvore binaria usando os elementos dispostos em uma estrutura do tipo fila
{
          if (isoperador(fila[*tamanho][0])){
                int aux = *tamanho;
                (*tamanho)--;
                noh* raiz = criaNoh(fila[aux],formaArvore(tamanho,fila),formaArvore(tamanho,fila));
                return raiz;
          }else if (fila[*tamanho][0] == 's'){
                int aux = *tamanho;
                (*tamanho)--;
                noh* raiz = criaNoh("sqrt",NULL,formaArvore(tamanho,fila));
                return raiz;
          }else if (isdigit(fila[*tamanho][0])){
                noh* raiz = criaNoh(fila[*tamanho],NULL,NULL);
                (*tamanho)--;
                return raiz;
        }
}

void imprimirComRecuoAux(noh *arv, int altura){
	int espaco = altura;
	while(espaco > 0){
		printf("   ");
		espaco--;
	}

    if (arv != NULL)
    {
        printf ("%s\n", arv->conteudo);
        imprimirComRecuoAux (arv->esq, altura + 1);
        imprimirComRecuoAux (arv->dir, altura + 1);

    }
    else
    {
        printf("_\n");
    }

}

void imprimirComRecuo (noh *arv)
{
   printf("\nImpressao da Arvore:\n");
    imprimirComRecuoAux(arv, 0);
}

void exibirEquacoes (noh *arv)
{
    printf ("\nEquacao Infixa (ERD): \n\t");
    varreduraERD(arv);
    printf("\n\nEquacao Prefixa (RED): \n\t");
    varreduraRED(arv);
    printf("\n\nEquacao Posfixa (EDR): \n\t");
    varreduraEDR(arv);
}
void exibirCalculo (noh *arv)
{
    printf("\n\n\nCalculos:\n");
    printf("\nResultado: %.2f\n",recursaoCalc(arv));
}
char *digitar ()
//Serve como comando de entrada e converte os valores para a versão posfica
{
    pilha *pi = criapilha();
    char ch;
    empilha(pi,'(');
    fflush(stdin);
   //__fpurge(stdin); para linux
    scanf("%c", &ch);
    while(ch != '\n'){
       empilha(pi, ch);
       scanf("%c", &ch);
    }
    empilha(pi,')');

    int n = contaPilha(pi);
    char inf[n];
    pilha* ax = pi;
    int i;

    for(i = 0; i < n; i++){
        ax = ax->proximo;
        inf[n-1-i] = ax->conteudoPilha;
    }

    return toPosfixa(inf);
}

noh *criarArvore(char *str)
//Inicializa a criação de Arvore binária
{
    char fila[1000][255];
    int inicio = emfilerar(strlen(str), str, fila);
    return formaArvore(&inicio,fila);

}
int main (void)
{
    printf("Atencao: Para Raiz Quadrada digite sqrt()\nDigite uma expressao: ");
    noh *arvore  = criarArvore(digitar());
    imprimirComRecuo(arvore);
    exibirEquacoes(arvore);
    exibirCalculo(arvore);
    return 0;
}
