/* Projecto de Analise e Sintese de Algoritmos - Parte 1 */
/* Fernando Lica - 77207 */
/* Joao Ribeiro  - 77209 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/************/
/*ESTRUTURAS*/
/************/

typedef struct node{     /* Estrutura "no auxiliar" utilizado nas listas de adjacencia */
    int n;               /* Identificacao de cada pessoa (um inteiro)*/
    struct node *next;
}*Node;

typedef struct vertice{     /* Estrutura vertice (cabecas do grafo) */
    int id, low, d;
    int CFL;
    Node first, tail;       /* Identificacao de cada pessoa (um inteiro)*/
    struct vertice *down;   /* Apontador para a proxima pessoa*/
}*Vertice;

/*********************/
/* Variaveis Globais */
/*********************/

Vertice stackHead;
int *vStack;

Vertice *GruposInternos;
int visitados=0;
int grupos=0;
int tamanhoMaiorGrupo=0;
int numInternos;
int sccAtual=0;

/************/
/*PROTOTIPOS*/
/************/

/*TARJAN*/
int *initvStack(int N);

void Push(Vertice v,int *vStack);
Vertice Pop(int *vStack);

void SCC_Tarjan(Vertice *amigos, int N, int* vStack);
void Tarjan_Visit(Vertice *amigos, int casa, int *vStack);

/*GRAFO*/
Vertice InsFimLista(Vertice head, int newID);
Node newNode(int n);
Vertice newVertex(int id);
Vertice *novaListaDeAmigos(int N);
Vertice novaListaAdj(Vertice head, Vertice *amigos, int **Matrix, int idAmigos);
void imprimeGrafo(Vertice *amigos, int numVer);
void imprimeVertice(Vertice v);
void debugImprimeAmigos(Vertice *amigos, int lenAmigos);

/* Funcoes auxiliares */
int min(int x, int y);
int haLeak(Vertice v, Vertice *amigos);
/********/
/* Main */
/********/

int main(){

    int N, P, i, a, b;
    /*int numGrupos, tamanhoMaiorGrupo, numGruposInt;*/

    Vertice *friends; /* Guardamos os vertices num vetor*/

    if(scanf("%d %d", &N, &P)==1){};    /* Le a primeira linha do input, N pessoas, P partilhas  */
    friends=novaListaDeAmigos(N);       /* Aloca o vector de vertices */
    stackHead=(Vertice)malloc(sizeof(struct vertice)); /* Aloca a base da pilha (Global)*/
    vStack = initvStack(N);             /* Depois de ter o num de pessoas, cria um vector auxiliar a pilha */

    for(i=0; i<P; i++){                 /* Ciclo para leitura de partilhas */
        if(scanf("%d %d", &a, &b)==1){};
        InsFimLista(friends[a-1], b);   /* Insere a adjacencia no final da lista */
    }

    SCC_Tarjan(friends, N, vStack);     /* Corre o algoritmo de Tarjan */
    printf("%d\n%d\n%d\n", grupos, tamanhoMaiorGrupo, numInternos);

    return 0;
}

/*********/
/*FUNCOES*/
/*********/

/*TARJAN*/
void SCC_Tarjan(Vertice *amigos, int N, int* vStack){

    int i, *sccRegistados;
    Node aux;

	for (i=0;i<N;i++){
        if(amigos[i]->d==-1){   /*Se ainda nao foi descoberto*/
            Tarjan_Visit(amigos, i, vStack); /* Visita-o */
        }
	}


	/* Output 3*/
	sccRegistados=(int*)malloc(sizeof(int)*grupos);
	for(i=0; i<grupos; i++){
        sccRegistados[i]=0;
	}

    for(i=0; i<N; i++){

        for(aux=amigos[i]->first; aux!=NULL; aux=aux->next){
            if(sccRegistados[amigos[i]->CFL]==0){
                if(amigos[i]->CFL != amigos[aux->n]->CFL){
                    sccRegistados[amigos[i]->CFL]=1;
                    numInternos--;
                }
            }
        }
    }
    numInternos=numInternos+grupos;
}

void Tarjan_Visit(Vertice *amigos, int casa, int *vStack){

    Vertice v = amigos[casa];

    Vertice popped;
    Node aux;

    int tamanhoGrupoAtual;

	v->d = visitados;
	v->low = visitados;
    visitados++;

	Push(v, vStack);    /* Coloca o vertice na pilha */

    for(aux=v->first; aux!=NULL ;aux=aux->next){
        if((amigos[(aux->n)]->d)==-1){
                Tarjan_Visit(amigos, amigos[(aux->n)]->id, vStack);
                if(amigos[(aux->n)]->low < v->low)
                    v->low= amigos[(aux->n)]->low;
        }
        else if(vStack[(aux->n)]==1){
            if(amigos[(aux->n)]->d < v->low)
                v->low= amigos[(aux->n)]->low;
            }
    }
    if(v->d == v->low){
        tamanhoGrupoAtual=0;
        do{                 /* Pop de vertices ate encontrar a raiz do SCC*/

            popped = Pop(vStack);
            popped->CFL=sccAtual;
            tamanhoGrupoAtual++;  /* Incrementa o tamanho do grupo*/

        }while(popped->id != v->id);

        sccAtual++;

        if(tamanhoGrupoAtual >= tamanhoMaiorGrupo){ /* Se este grupo e maior que o maior grupo registado, passa a ser o maior grupo*/
            tamanhoMaiorGrupo=tamanhoGrupoAtual;
        }

        grupos++;

    }
}

void Push(Vertice v, int *vStack){
    v->down = stackHead; /* Mete a cabeça da pilha (stack) como o next do novo  */
    stackHead = v;       /* Aponta a cabeça da pilha para o novo */
    vStack[(v->id)]=1;   /* Coloca a entrada a 1 do vetor auxilar a pilha */
}

Vertice Pop(int* vStack){
    Vertice taken;
    if(stackHead==NULL){    /* Se a pilha estiver vazia retorna NULL */
        return NULL;
    }
    taken = stackHead; /* O vertice devolvido sera o primeiro da pilha */
    stackHead = stackHead->down; /* O novo primeiro da pilha passa a ser o que estava em segundo */
    vStack[(taken->id)]=0; /* Actualiza o vetor auxiliar que diz se um vertice esta ou nao na pilha */
    return taken;

}

/*FUNCOES DE GRAFO*/
Vertice newVertex(int id){
    Vertice v = (Vertice)malloc(sizeof(struct vertice));
    v->id=id;
    v->low=-1;
    v->d=-1;
    v->first=NULL;
    v->tail=NULL;
    return v;
}

Node newNode(int id){
    Node novo = (Node)malloc(sizeof(struct node));
    novo->n = id-1;
    novo->next=NULL;
    return novo;
}

Vertice *novaListaDeAmigos(int N){
    int i;
    Vertice *amigos=(Vertice*)malloc(N*sizeof(struct vertice));

    for(i=0; i<N; i++){
        amigos[i]=newVertex(i);
    }
    return amigos;
}


Vertice InsFimLista(Vertice v, int newID){

    /* Esta funcao cria um novo nodo auxiliar e
    coloca-o no final da lista ligada com
    a cabeca pertencente ao grafo original*/

    Node new = newNode(newID);

    if(v->tail==NULL){/* caso  a lista seja apenas a cabeca */
        v->first=new;
        v->tail=new;
        return v;
    }

    v->tail->next = new;
    v->tail = new;
    return v;
}

int min(int x, int y){
	if(x==-1){ /* No contexto do problema, -1 corresponde a +oo, ou seja, -1 e maior do que qualquer outro inteiro */
        return y;
	}
	if(y==-1){
        return x;
	}

	if(x < y){
       return x;
    }
    return y;
}

int haLeak(Vertice v, Vertice *amigos){ /* Funcao booleana */

    Node aux;
    aux=v->first;
    while(aux!=NULL){
        if(v->CFL != amigos[aux->n]->CFL){
            return 1;
        }
        aux=aux->next;
    }
    return 0;

}

/* Funcao que inicializa o vetor auxiliar a pilha */
int* initvStack(int N){
    int i;
    int *vet = (int*)malloc(sizeof(int)*N);
    for(i=0; i<N; i++){
        vet[i]=0;
    }
    return vet;
}
