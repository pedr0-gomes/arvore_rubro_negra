#ifndef RBT_H
#define RBT_H

/*
TAD - Árvore Rubro-Negra (Direitista,iterativa com ponteiro pai)
Referência: Aula 21 - Estrutura de Informação
*/

#define RED 1
#define BLACK 0

struct NO
{
    int val;
    struct NO *esq;
    struct NO *dir;
    struct NO *pai;
    int cor;
};

typedef struct NO *ArvRB;

/* Sentinela NIL - representa toda folha ausente */
extern struct NO NIL_NODE;
extern ArvRB NIL;

/* Criar e Destruir */
ArvRB *criar_ArvRB(void);
void destroi_ArvRB(ArvRB *raiz);

/* Consulta */
int busca_ArvBR(ArvRB *raiz,int valor);
int contar_nos_ArvRB(ArvRB raiz);

/* Rotações */
ArvRB rotacao_esq(ArvRB *raiz,ArvRB x);
ArvRB rotacao_dir(ArvRB *raiz,ArvRB x);

/* Validação */
int valida_ArvRB(ArvRB raiz);


#endif