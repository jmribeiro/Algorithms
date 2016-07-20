/* Headers */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/**************************/
/*Estruturas e Declaracoes*/
/**************************/
#define PKG 5

typedef struct sCliente{
    char *nome;
    char pack;
    unsigned long num;
}*Cliente;

typedef struct node{
    Cliente cliente;
    struct node *next;
}*Node;

typedef struct queue{/* Lista */
	Node head, tail;
}*Queue;

/* *************************/
/* *******PROTOTIPOS********/
/* *************************/

/********************/
/* Funcoes de Queue */
/********************/

Q Qinit();
void initQueues();

void Qput(Cliente c, Q q);
Cliente Qget();

Cliente idSeekAndDestroy(Q q, unsigned long id);
Cliente nomeSeekAndDestroy(Q queue, char *nome);

/*******************/
/* Funcoes de Menu */
/*******************/

void executa_a();
void executa_u();
void executa_l();
void executa_p();
void executa_r();

/**********************/
/* Funcoes Auxiliares */
/**********************/

char *mystrdup(char* str);

/*********************/
/* Variaveis Globais */
/*********************/

Q queues[PKG];

/* *******************/
/* *******MAIN********/
/* *******************/

int main(){

    /*variaveis*/
    char comando;

    /*codigo*/
	initQueues();

        while (1) {/* Loop para input infinito*/

            comando = getchar();/*Leitura do comando*/

            switch (comando)
            {
                case 'a':/*ADICIONAR*/
                    executa_a();
                    break;
                case 'u':/*UPDATE*/
                    executa_u();
                    break;
                case 'l':/*LISTAR*/
                    executa_l();
                    break;
                case 'p':/*PROXIMO*/
                    executa_p();
                    break;
                case 'r':/*REMOVER*/
                    executa_r();
                    break;
                case 'x':

                    return EXIT_SUCCESS; /* Termina o programa */
            default:
                printf("ERRO: Comando [%c] desconhecido\n",comando);
        }
        getchar(); /* le o '\n' introduzido pelo utilizador */
    }
    return EXIT_FAILURE;

    /* criar funcao destroy que faz free dos mallocs */
}

/* *******************/
/* *****FUNCOES*******/
/* *******************/

/********************/
/* Funcoes de Queue */
/********************/

Q Qinit(){/* Esta funcao cria e devolve uma lista de espera (Queue) vazia */

    Q q;
	q = (Q)malloc(sizeof(struct queue));

	q->head = NULL;
	q->tail = NULL;

	return q;
}

void initQueues(){/* Esta funcao inicializa a variavel global queues, o nosso array de queues correspondentes aos pacotes */

    int i;/* variavel auxiliar para o ciclo */

    for(i=0;i<PKG;i++){/* ciclo para correr as 5 listas(pacotes A a E) */
		queues[i] = Qinit();
	}

}

void Qput(Cliente c, Q q){/* Funcao de insercao de um novo elemento numa estrutura FIFO */
/*
Esta funcao recebe um ponteiro para um cliente novo e um ponteiro para lista ao qual este sera inserido,
Como se trata de uma estrutura FIFO este sera inserido no final(APAGAR)
*/
    Node n = (Node)malloc(sizeof(struct node));/* novo node */

    /* atribuicao de valores aos atributos da estrutura node */
    n->cliente = c;
    n->next = NULL;

    if(q->head==NULL){/* caso especial com a lista vazia (tanto a head como a tail sao o elemento inserido) */
        q->head = n;
        q->tail = n;
        return;/* nao corre o seguinte codigo */
    }
    /* caso ja hajam elementos na lista */
    q->tail->next = n;
    q->tail = n;

}

Cliente Qget(Q queue){/* Funcao de remocao de um elemento (1o) numa estrutura FIFO */
/*
Esta funcao recebe como argumento uma lista FIFO e devolve, removendo, o ponteiro para o primeiro elemento
*/
    Node nAux;
    Cliente cAux;

    if (queue->head==NULL) return NULL;/* no caso de uma lista vazia ira ser retornado um ponteiro para NULL */


    nAux = queue->head;/* guardamos a cabeca (node a ser removido) no node auxiliar */
    cAux = queue->head->cliente;/* Guardamos o ponteiro para o cliente desse node na variavel cliente auxiliar */

    /* o segundo elemento passara a ser a nova cabeca da lista */
    queue->head = queue->head->next;

    free(nAux);/* Elimina o no auxiliar, sem apagar o cliente */

    return cAux;

}

/*******************/
/* Funcoes de menu */
/*******************/

void executa_a() {/* Esta funcao adiciona um cliente novo a nossa main queue */

	char pack;
    char buffNome[100];
	unsigned long id;
    Cliente c=(Cliente)malloc(sizeof(struct sCliente));

	scanf("%s %lu %c", buffNome, &id,  &pack);

    c->nome = mystrdup(buffNome);
	c->pack = pack;
	c->num = id;

	Qput(c, queues[pack-'A']);
}

void executa_u(){/* Esta funcao altera o pacote de um cliente, actualizando a queue principal consoante o seu novo pacote */

    int i;
    char pack;
    unsigned long num;
    Cliente c;

    scanf("%lu %c", &num, &pack);

    for(i=0; i<PKG; i++){/* ciclo para procura do elemento da lista principal com o numero pretendido */
        c = idSeekAndDestroy(queues[i], num);
          if(c!=NULL){
            c->pack = pack;
            Qput(c, queues[pack-'A']);
            return;
        }
    }
}

void executa_l(){ /* Esta funcao lista os dados do cliente na frente da fila */

    int i, aux=0;

    for(i=0; i<PKG; i++){/* ciclo que procura o primeiro elemento da lista principal */
        if(queues[i]->head!=NULL){
            printf("%s %lu %c\n", queues[i]->head->cliente->nome, queues[i]->head->cliente->num, queues[i]->head->cliente->pack);
            aux=1;
            break;
        }
    }

    /* se nada foi imprimido (lista vazia) */
    if(aux==0){
        printf("NA\n");
    }
}

void executa_p(){/* Esta funcao "atende" o proximo cliente, ou seja, imprime os seus dados e remove-o da lista principal */

    int i, aux=0;
    Cliente c=(Cliente)malloc(sizeof(struct sCliente));

    for(i=0; i<PKG; i++){/* ciclo que procura o primeiro elemento da lista principal (parecido com o da funcao executa_l())*/
        if(queues[i]->head!=NULL){
            printf("%s %lu %c\n", queues[i]->head->cliente->nome, queues[i]->head->cliente->num, queues[i]->head->cliente->pack);
            c = Qget(queues[i]);/* remocao do primeiro elemento */
            aux=1;
            break;
        }
    }

    /* se nenhum cliente foi atendido, temos uma lista vazia */
    if(aux==0){
        printf("NA\n");
    }

    free(c);/* Elimina o cliente da memoria (ja foi atendido) */

}

void executa_r(){/* Esta funcao recebe o nome de um cliente e elimina-o tanto da lista principal como da memoria */

    int i;
    char buffNome[100];
    Cliente c;

    scanf("%s", buffNome);

    for(i=0; i<PKG; i++){/* ciclo de procura na lista principal o cliente com o nome lido */
        c = nomeSeekAndDestroy(queues[i], buffNome);
        if(c!=NULL){/* nome seek and destroy nao funciona */
            free(c->nome); /* Elimina o cliente da memoria */
            free(c);
            return;
        }

    }
    printf("NA\n");
}

/**********************/
/* Funcoes auxiliares */
/**********************/

/*
As funcoes idSeekAndDestroy e nomeSeekAndDestroy sao identicas excepto no aspecto que toca ao parametro de procura.

Estas funcoes recebem como argumentos uma lista e um parametro de procura,
e como o nome indica procuram o cliente com este mesmo parametro, e caso ele exista removem-no da lista devolvendo-o
*/

Cliente idSeekAndDestroy(Q q, unsigned long id){

    Cliente Caux;
    Node nPrev;
    Node nCurr;

    if(q==NULL || q->head==NULL){
        return NULL;
    }

    for(nPrev=NULL, nCurr=q->head; nCurr!=NULL;nPrev=nCurr, nCurr=nCurr->next){

        Caux = nCurr->cliente;

        if(Caux->num==id){

            if(nCurr==q->head){

                q->head=q->head->next;

                if(q->head==NULL){

                    q->tail=NULL;
                }

                free(nCurr);
                return Caux;
            }

            nPrev->next=nCurr->next;
            free(nCurr);
            if(nPrev->next==NULL){
                q->tail=nPrev;
            }
            return Caux;

        }

    }
    return NULL;

}

Cliente nomeSeekAndDestroy(Q queue, char *nome){

    Cliente Caux;
    Node nPrev;
    Node nCurr;

    if(queue->head==NULL){/* LISTA VAZIA*/
        return NULL;
    }

    if(strcmp(queue->head->cliente->nome, nome)==0){/* Ve o primeiro elemento */
        return Qget(queue);
    }

    if (queue->head->next==NULL){
        return NULL;
    }

    nPrev = queue->head;
    nCurr = queue->head->next;

    while(nCurr!=NULL){

        if(strcmp(nCurr->cliente->nome,nome)==0){

            nPrev->next = nCurr->next;

            if(nCurr->next == NULL){
                queue->tail = nPrev;
            }

            Caux = nCurr->cliente;
            free(nCurr);
            if(nPrev->next==NULL){
                queue->tail = nPrev;
            }
            return Caux;
        }
        nPrev = nCurr;
        nCurr = nCurr->next;
    }

    return NULL;
}

char *mystrdup(char* str){

    char *new = (char*)malloc(sizeof(char)*(strlen(str)+1));
    strcpy(new, str);
    return new;

}
