/*
    Projecto IAED Parte 1
    77207 - Fernando Liça
    77209 - Joao Ribeiro
    76557 - Pedro Dias
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NPRODUTOS 5
#define NMP 3
#define NCHAR 11


/* directivas de pre-compilador, definicao de tipos/estruturas, variaveis globais, etc. */

typedef struct {

    /*Nomes dos Produtos e Materias Primas*/
    char nomes_P[NPRODUTOS][NCHAR];
    char nomes_MP[NMP][NCHAR];

    /*Custo das materias primas*/
    double custo_mat[NMP];

    /*Composicao percentual dos PRODUTOS por Materia*/
    double perc_mat[NPRODUTOS][NMP];

    /*Coeficientes por PRODUTOS*/
    int k_mo[NPRODUTOS];
    int k_en[NPRODUTOS];
    int k_eq[NPRODUTOS];

    /*Perdas em percentagem por PRODUTO*/
    double perdas[NPRODUTOS];

    /*Vendas(tonelada) por PRODUTO*/
    double vendas_prod[NPRODUTOS];

    /*Orcamentos para custos (5 custos) (0: A - 5:E) (F nao tem orcamento)*/
    double orcamento[5];

    /*Preco de venda por tonelada por PRODUTO*/
    double preco_prod[NPRODUTOS];

} Empresa;

Empresa e;/* DECLARACAO DA EMPRESA ONDE TODA A INFORMACAO SERA GUARDADA*/

/* INICIO PROTOTIPOS */
void executa_d();
void executa_c();
void executa_w();
void executa_t();
void executa_v();
void bubble(double v[]);
void executa_x();

/* INPUTS */
void le_produtos();
void le_materias();
void le_custo_mat();
void le_vendas_anuais();
void le_preco_produtos();
void le_perdas();
void le_k_mo();
void le_k_en();
void le_k_eq();
void le_orcamentos();
void le_composicao_percentual_materias();

/* CUSTOS */
double calcula_custo_A();
double calcula_custo_B(int prod_i);
double calcula_custo_C(int prod_i);
double calcula_custo_D(int prod_i);
double calcula_custo_E();
double calcula_custo_F(int prod_i);
double calcula_margem(int prod_i);

double custo_tot(int prod_i);
double custo_ind(int prod_i);
double custo_mpee(int prod_i);

/* FIM PROTOTIPOS */

int main()
{
    char comando;

    while (1) {
        comando = getchar(); /* le o comando */
        switch (comando)
        {
        case 'd':
            executa_d(); /* Executa comando d */
            break;
        case 'c':
            executa_c();
            /* Executa comando c */
            break;
        case 'w':
            executa_w();
            /* Executa comando w */
            break;
        case 't':
            executa_t();
            /* Executa comando t */
            break;
        case 'v':
            executa_v();
            /* Executa comando v */
            break;
        case 'x':
            /*Executa comando x */
            executa_x();
            return EXIT_SUCCESS; /* Termina o programa */
        default:
            printf("ERRO: Comando [%c] desconhecido\n",comando);
        }
        getchar(); /* le o '\n' introduzido pelo utilizador */
    }

    return EXIT_FAILURE;
}

void executa_d()
{
    char subcomando;

    getchar(); /* le o espaco */
    subcomando = getchar(); /* le o sub-comando */

    switch (subcomando) {

    case 'i':/*le produtos*/
        le_produtos();
        /* Executa comando d i */
        break;

    case 'j':/*le materias*/
        le_materias();
        /* Executa comando d j */
        break;

    case 'y':/*le custo das materias */
        le_custo_mat();
        /* Executa comando d y */
        break;

    case 'v':/*le as vendas anuais de cada produto */
        le_vendas_anuais();
        /* Executa comando d v */
        break;

    case 'z':/*le preco por tonelada dos produtos */
        le_preco_produtos();
        /* Executa comando d z */
        break;

    case 'p':/*le a percentagem de perdas de cada produto*/
        le_perdas();
        /* Executa comando d p */
        break;

    case 'm':/*le o coeficiente de mao de obra por produto */
        le_k_mo();
        /* Executa comando d m */
        break;

    case 'n':/* le o coeficiente de energia eletrica por produto*/
        le_k_en();
        /* Executa comando d n */
        break;

    case 'q':/* le o coeficiente de uso das maquinas por produto*/
        le_k_eq();
        /* Executa comando d q */
        break;

    case 'o':/* le orcamentos para casta custo (de A a E (F nao tem orcamento))*/
        le_orcamentos();
        /* Executa comando d o */
        break;

    case 'c':/*le a composicao de cada produto em percentagem de materias primas */
        le_composicao_percentual_materias();
        /* Executa comando d c */
        break;

    default:
        printf("ERRO: Comando l [%c] desconhecido\n",subcomando);
    }
}


void executa_c(){

    int Pi;

    printf("cTOT");
    for (Pi=0;Pi<NPRODUTOS; Pi++)
        printf(" %.2f", custo_tot(Pi)); /* custo total de cada produto */

    printf("\nM");
    for (Pi=0;Pi<NPRODUTOS; Pi++)
        printf(" %.2f", calcula_margem(Pi)); /* margem de cada produto */

    printf("\ncIND");
    for(Pi=0;Pi<NPRODUTOS;Pi++)
        printf(" %.2f",custo_ind(Pi)); /* custo industrial de cada produto */

    printf("\ncMPEE");
    for(Pi=0;Pi<NPRODUTOS;Pi++)
        printf(" %.2f", custo_mpee(Pi)); /* indice materia-prima, embalagem e energia de cada produto */

	printf("\n");
}

void executa_w(){

    int Pi;

    getchar();
    scanf("%d", &Pi);

    /* Imprime os custos unitarios para cada um dos custos especificados (A a F). */
    printf("W %s A:%.2f B:%.2f C:%.2f D:%.2f E:%.2f F:%.2f",e.nomes_P[Pi-1], calcula_custo_A(),calcula_custo_B(Pi),calcula_custo_C(Pi), calcula_custo_D(Pi), calcula_custo_E(), calcula_custo_F(Pi));

	printf("\n");
}

void executa_t() {

    int Pi;

    double custo;
    double venda;
    double lucro;
    double margem;

    custo=0;
    venda=0;

    for(Pi=0;Pi<NPRODUTOS;Pi++) {
        custo += (custo_tot(Pi)* e.vendas_prod[Pi]);         /* custo total da producao das vendas para cada produto*/
        venda += (e.vendas_prod[Pi] * e.preco_prod[Pi]);     /* valor da venda da producao para cada produto*/
    }

    lucro = venda - custo;
    margem = ((venda-custo)/venda)*100;

    printf("T %.2f %.2f %.2f %.2f", custo, venda, lucro, margem);     /* imprime o custo, a venda, o lucro e a margem correspondente a cada produto*/
	printf("\n");
}

void executa_v() {

	int Pi;

	double dscnt;
	char *decisao;
	double venda_d;
	double margem_d;

	getchar();
    scanf("%d %lf",&Pi,&dscnt);

    Pi--;
    venda_d = e.preco_prod[Pi] * (1-(dscnt/100));        /* venda do produto em causa, com o desconto aplicado*/
	margem_d = (((venda_d - custo_tot(Pi))/venda_d)*100); /* margem da venda do produto em causa, com o desconto aplicado*/

	if (custo_tot(Pi)<=venda_d)
        decisao="tot";
	if(custo_ind(Pi)<=venda_d && venda_d < custo_tot(Pi))
		decisao="ind";
	if(custo_mpee(Pi)<=venda_d && venda_d < custo_ind(Pi))
		decisao="mpee";
	if (venda_d<custo_mpee(Pi))
		decisao="0";

	printf("VD %s %.2f %.2f %.2f %s", e.nomes_P[Pi],  calcula_margem(Pi) , dscnt, margem_d, decisao);

	printf("\n");
}

void executa_x() {
    int x;/*Variavel de input*/
    int i;/*Variavel auxiliar para os ciclos de produtos*/
    int j, k;/*Variaveis para os ciclos do bubble sort*/

    double vector_todos_precos[NPRODUTOS];
    char nomes[NPRODUTOS][NCHAR];

    /*Auxiliares ao bubble sort*/
    double aux;
    char auxS[NCHAR];

    getchar(); /*espaco*/
    scanf("%d",&x);

    if(x==1){

        for (i=0;i<NPRODUTOS;i++){/*Preenche o array de precos*/
           vector_todos_precos[i] = custo_tot(i);
           strcpy(nomes[i], e.nomes_P[i]);
        }

        /* BUBBLE SORT*/
        for(j=0; j<NPRODUTOS-1;j++){

            for(k=0;k<NPRODUTOS-j-1;k++){

                if(vector_todos_precos[k]>vector_todos_precos[k+1]) {
                    /*Ordena array de precos */
                    aux=vector_todos_precos[k];
                    vector_todos_precos[k]=vector_todos_precos[k+1];
                    vector_todos_precos[k+1]=aux;

                    /*Ordena array de nomes*/
                    strcpy(auxS, nomes[k]);
                    strcpy(nomes[k], nomes[k+1]);
                    strcpy(nomes[k+1], auxS);


                }
            }

        }
        /* Output da funcao */
        for(i=0;i<NPRODUTOS;i++){
            printf("%s %.2f\n", nomes[i],vector_todos_precos[i]);
        }
    }
    /**/
    if(x==0){

    }
}

/* FUNCOES PARA LER INFORMACAO (INPUT)*/
/* i */
void le_produtos(){

    int i;

    for(i=0; i<NPRODUTOS; i++){
        scanf("%s", e.nomes_P[i]);
        if(i!=0) printf(" ");
        printf("%s", e.nomes_P[i]);
    }

    printf("\n");
}

/* j */
void le_materias(){

    int i;

    for(i=0; i<NMP; i++){
        scanf("%s", e.nomes_MP[i]);
        if(i!=0) printf(" ");
        printf("%s", e.nomes_MP[i]);
    }

    printf("\n");
}

/* y */
void le_custo_mat(){

    int i;

    for(i=0; i<NMP; i++)
        scanf("%lf", &e.custo_mat[i]);

}

/* v */
void le_vendas_anuais(){

    int i;

    for(i=0; i<NPRODUTOS; i++)
        scanf("%lf", &e.vendas_prod[i]);

}

/* z */
void le_preco_produtos(){

    int i;

    for(i=0; i<NPRODUTOS; i++)
        scanf("%lf", &e.preco_prod[i]);

}

/* p */
void le_perdas(){

    int i;

    for(i=0; i<NPRODUTOS; i++)
        scanf("%lf", &e.perdas[i]);

}

/* n */
void le_k_mo(){

    int i;

    for(i=0; i<NPRODUTOS; i++)
        scanf("%d", &e.k_mo[i]);

}

/* m */
void le_k_en(){

    int i;

    for(i=0; i<NPRODUTOS; i++)
        scanf("%d", &e.k_en[i]);

}

void le_k_eq(){

    int i;

    for(i=0; i<NPRODUTOS; i++)
        scanf("%d", &e.k_eq[i]);

}

void le_orcamentos(){

    int i;

    for(i=0; i<5; i++)
        scanf("%lf", &e.orcamento[i]);

}

void le_composicao_percentual_materias(){

    int i, j;

    for(i=0; i<NPRODUTOS; i++){
        for(j=0; j<NMP; j++)
            scanf("%lf", &e.perc_mat[i][j]);
    }
}

/*FUNCOES PARA CALCULAR CUSTOS (A-F)*/

double calcula_custo_A(){ /* calcula gastos administrativos --- orcamento admin / vendas ( de cada produto ) --- */

    int i;
    double vendas=0;

    for(i=0;i<NPRODUTOS;i++)
        vendas+=e.vendas_prod[i];

    return e.orcamento[0]/vendas;

}

double calcula_custo_B(int prod_i){  /* calcula os vencimentos e encargos com pessoal de producao pra x toneladas */

	double mo=0;
    int i;

    prod_i--;

    for(i=0;i<NPRODUTOS;i++)
        mo+=(e.k_mo[i]*e.vendas_prod[i]);

    return ((e.orcamento[1]*e.k_mo[prod_i])/mo);
}

double calcula_custo_C(int prod_i) {  /*  calcula a energia electrica para a producao de x toneladas  */

    double en=0;
    int i;

    prod_i--;

    for(i=0;i<NPRODUTOS;i++)
        en+=(e.k_en[i]*e.vendas_prod[i]);

    return ((e.orcamento[2]/en)*e.k_en[prod_i]);

}

double calcula_custo_D(int prod_i) {  /* calcula a amortizacao de maquinas e equipamentos de producao */

    double eq=0;
    int i;

    prod_i--;

    for(i=0;i<NPRODUTOS;i++)
        eq+=(e.k_eq[i]*e.vendas_prod[i]);

    return (e.orcamento[3]/eq)*e.k_eq[prod_i];
}

double calcula_custo_E() {  /* custo da embalagem por tonelada para cada produto */

    double vendas=0;
    int i;

    for(i=0;i<NPRODUTOS;i++)
        vendas+=e.vendas_prod[i];

    return e.orcamento[4]/vendas;
}

double calcula_custo_F(int prod_i) {  /* calcula o custo reçativo a quantidade/preco de cada materia para perfazer uma tonelada */

    double ganhos;
    double dist_custo=0;
    int i;

    prod_i--;

    ganhos = (100-e.perdas[prod_i]); /* 100 - a % de perdas de cada produto*/

    for(i=0;i<NMP;i++)
        dist_custo+=(e.perc_mat[prod_i][i]*e.custo_mat[i]);

    return (dist_custo/ganhos)*1000;

}

double calcula_margem(int prod_i){

    return((e.preco_prod[prod_i] - custo_tot(prod_i))/e.preco_prod[prod_i])*100;

}

double custo_tot(int prod_i) {   /* custo total da producao de UM produto ou seja todos os custos - A B C D E F */

    prod_i++;

    return (calcula_custo_A() + calcula_custo_B(prod_i) + calcula_custo_C(prod_i) + calcula_custo_D(prod_i) + calcula_custo_E() + calcula_custo_F(prod_i));
}

double custo_ind(int prod_i) {   /* custos ligados directamente a producao - B C D E F */

    prod_i++;

    return (calcula_custo_B(prod_i) + calcula_custo_C(prod_i) + calcula_custo_D(prod_i) + calcula_custo_E() + calcula_custo_F(prod_i));
}

double custo_mpee(int prod_i) { /* custos imediatos de producao - C E F */

    prod_i++;

    return (calcula_custo_C(prod_i) + calcula_custo_E() + calcula_custo_F(prod_i));

}
