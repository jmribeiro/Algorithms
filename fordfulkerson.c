/*
    Projecto ASA Parte 2
    João Ribeiro  - 77209
    Fernando Liça - 77207
    LEIC-T , 2o Semestre 2013/2014
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>

#define BRANCO 0
#define CINZENTO 1
#define PRETO 2

/************/
/*ESTRUTURAS*/
/************/
typedef struct edge{
    int num, flow;
    struct edge *next;
}*Edge;

typedef struct vertice{
    int id;
    int cor;
    Edge first, last;
}*Vertice;

typedef Vertice* Grafo;

/*********************/
/* Variaveis Globais */
/*********************/

int V, E, h;

Grafo G;

int *cor;    /* Array auxiliar que guarda as cores usadas na BFS de cada vertice */

int *output, *targets;

int *caminhoAtual; /* Vetor auxiliar usado na funcao FordFulkerson e na funcao BFS */

int *Lista;
int first, last;

/************/
/*PROTOTIPOS*/
/************/
void initArrays();
void initLista();

int bfs(int source, int target);
int FordFulkerson(int source, int target);

int *novoArray(int colunas);
Vertice newVertice(int valor);
Edge newEdge(int valor);
void insereFimAdj(Vertice vA, int b);

int min(int x, int y);

void initLista();


void enQueue(int v); /* Insere no final da Lista */
int deQueue();/* Remove o primeiro elemento da Lista */

/********/
/* Main */
/********/
int main(){

    int i, o, d, j, aux, aux2, numVertices, s, t;

    scanf("%d %d", &V, &E);

    G = (Grafo)malloc((sizeof(struct vertice*))*V);

    for(i=0; i<V; i++){
        G[i] = newVertice(i);
    }
    
    for(i=0; i<E; i++){
        scanf("%d %d", &o, &d); 
        insereFimAdj(G[o], d);
        insereFimAdj(G[d], o);

    }

    initArrays();
    
    scanf("%d", &h);		/* h linhas de pontos criticos */
    
    output = novoArray(h);
    aux=0;

    for(i=0; i<h; i++){
        scanf("%d", &numVertices);
        if(numVertices==2){
            scanf("%d %d", &s, &t);
            output[i]=FordFulkerson(s,t);
        }
        else if(numVertices>2){
            targets = novoArray(numVertices);
            scanf("%d",&s);
            for(j=0; j<numVertices-1;j++){

                scanf("%d", &t);
                targets[j]=t;

            }
            aux2=INT_MAX;
            for(j=0;j<numVertices-1;j++){
                aux = min(aux2, FordFulkerson(s,targets[j]));
            }
            output[i]=aux;
        }

    }

    for(i=0; i<h; i++){
        printf("%d\n", output[i]);
    }

    return 0;
}

/*********/
/*FUNCOES*/
/*********/

void initArrays(){

    int i;

    cor = novoArray(V);
    Lista = novoArray(V+2);
    caminhoAtual = novoArray(V);

    for(i=0; i<V; i++){
        cor[i]=BRANCO;
        Lista[i]=0;
        caminhoAtual[i]=0;
    }
}

/* QUEUE */
void enQueue(int v){ /* Insere no final da Lista */
    Lista[last] = v; /* Coloca o inteiro v na */
    last++;
    cor[v] = CINZENTO;
}

int deQueue(){ /* Remove o primeiro elemento da Lista */
    int v;
    v = Lista[first];
    first++;
    cor[v] = PRETO;
    return v;
}

/* BFS e FORDFULKERSON */
int bfs(int source, int target){

    int u, v;
    Edge edgeAtual;

    for (u=0; u<V; u++) {
        cor[u] = BRANCO;
    }

    first = 0;
    last = 0;
    
    enQueue(source);
    caminhoAtual[source] = -1;
    
    /*asdasd*/
    while (first!=last) {
        u = deQueue();

        for (edgeAtual = G[u]->first; edgeAtual!=NULL; edgeAtual = edgeAtual->next) {
            v = edgeAtual->num;
            if (cor[v]==BRANCO && edgeAtual->flow==0){
                enQueue(v);
                caminhoAtual[v] = u;
            }
        }

    }
    if(cor[target]==PRETO){
        return 1; /* Valores booleanos */
    }
    else{
        return 0;
    }
}

int FordFulkerson(int source, int target){
    
    int u, v, i;
    /*Inicializar flow e matriz auxiliar de arcos */
    int maxFlow = 0;
    Edge aux, aux1, aux2;
    
    for(i=0; i<V; i++){
        for(aux = G[i]->first; aux!=NULL; aux = aux->next){
            aux->flow=0;
        }
    }
    
    while (bfs(source, target)==1) {
        for (u = target; caminhoAtual[u]>=0; u = caminhoAtual[u])  {
            
            v = caminhoAtual[u];

            aux1=G[v]->first;
            while(1){
                if(aux1->num == u){
                    aux1->flow++;
                    break;
                }
                aux1 = aux1->next;
            }

            aux2=G[u]->first;
            while(1){
                if(aux2->num == v){
                    aux2->flow--;
                    break;
                }
                aux2 = aux2->next;
            }
            
        }
        maxFlow ++;
    }

    return maxFlow;
}

/* Construtores auxiliares */

int *novoArray(int colunas){
    int *a = (int*)malloc(colunas * sizeof(int));
    return a;
}

Vertice newVertice(int valor){
    Vertice v = (Vertice)malloc(sizeof(struct vertice));
    v->id = valor;
    v->first = NULL;
    v->last = NULL;
    return v;
} 

Edge newEdge(int valor){
    Edge n = (Edge)malloc(sizeof(struct edge));
    n->num = valor;
    n->flow = 0;
    n->next = NULL;
    return n;
}

void insereFimAdj(Vertice vA, int b){
    
    Edge nB = newEdge(b);
    if(vA->last == NULL){
        vA->first = nB;
        vA->last = nB;
        return;
    }
    vA->last->next = nB;
    vA->last = nB;
    return;
}

/* Extra */
int min (int x, int y) {
    if(x<y) return x;
    return y;
}
