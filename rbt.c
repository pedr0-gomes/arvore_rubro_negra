#include <limits.h>
#include <stdlib.h>
#include "rbt.h"

/* Sentinela NIL */
struct NO NIL_NODE = {0, &NIL_NODE, &NIL_NODE, &NIL_NODE, BLACK};
ArvRB NIL = &NIL_NODE;

/* Criar e Destruir */
ArvRB *criar_ArvRB(void)
{
    ArvRB *raiz = malloc(sizeof(ArvRB));
    if (raiz == NULL) return NULL;
    *raiz = NIL;
    return raiz;
}

static void destroi_rec(ArvRB no)
{
    if (no == NIL) return;
    destroi_rec(no->esq);
    destroi_rec(no->dir);
    free(no);
}

void destroi_ArvRB(ArvRB *raiz)
{
    if (raiz == NULL) return;
    destroi_rec(*raiz);
    free(raiz);
}

/* Consulta */
int busca_ArvBR(ArvRB *raiz,int valor)
{
    if (raiz == NULL) return 0;
    ArvRB p;
    p = *raiz;
    while (p != NIL)
    {
        if (p->val == valor) return 1;
        else if (p->val > valor) p = p->esq;
        else                     p = p->dir;
    }
    return 0;
}

static int conta_rec(ArvRB no)
{
    if (no == NIL) return 0;
    int c1 = conta_rec(no->esq);
    int c2 = conta_rec(no->dir);
    return 1 + c1 + c2;
}

int contar_nos_ArvRB(ArvRB raiz)
{
    return conta_rec(raiz);
}

/* Rotações */
ArvRB rotacao_esq(ArvRB *raiz,ArvRB x)
{
    ArvRB y = x->dir;
    x->dir = y->esq;
    if (y->esq != NIL)  y->esq->pai = x;

    y->pai = x->pai;
    if (x->pai == NIL)          *raiz = y;
    else if (x->pai->dir == x)  x->pai->dir = y;
    else                        x->pai->esq = y;    

    y->esq = x;
    x->pai = y;
    y->dir->cor = BLACK;
    return y;
}
ArvRB rotacao_dir(ArvRB *raiz,ArvRB x)
{
    ArvRB y = x->esq;
    x->esq = y->dir;
    if (y->dir != NIL) y->dir->pai = x;

    y->pai = x->pai;
    if (x->pai == NIL)           *raiz = y;
    else if (x->pai->dir == x)   x->pai->dir = y;            
    else                         x->pai->esq = y;

    y->dir = x;
    x->pai = y;
    y->cor = x->cor;
    x->cor = RED;

    return y;
}

/* Validação */
static int valida_rec(ArvRB no,
                      ArvRB pai_esperado,
                      long long min_exclusivo,
                      long long max_exclusivo,
                      int black_count,
                      int *black_ref,
                      int profundidade,
                      int max_profundidade)
{
    if (no == NULL) return 0;
    if (profundidade > max_profundidade) return 0;

    if (no == NIL)
    {
        int black_path = black_count + 1;
        if (*black_ref == -1)
        {
            *black_ref = black_path;
            return 1;
        }
        return black_path == *black_ref;
    }

    if (no->cor != RED && no->cor != BLACK) return 0;
    if (no->pai != pai_esperado) return 0;
    if (no->esq == NULL || no->dir == NULL) return 0;
    if ((long long)no->val <= min_exclusivo || (long long)no->val >= max_exclusivo) return 0;

    if (no->esq == no || no->dir == no) return 0;

    if (no->cor == RED)
    {
        if (no->esq->cor == RED || no->dir->cor == RED) return 0;
    }

    if (no->esq != NIL && no->esq->pai != no) return 0;
    if (no->dir != NIL && no->dir->pai != no) return 0;

    if (no->cor == BLACK) black_count++;

    if (!valida_rec(no->esq,
                    no,
                    min_exclusivo,
                    no->val,
                    black_count,
                    black_ref,
                    profundidade + 1,
                    max_profundidade))
    {
        return 0;
    }

    return valida_rec(no->dir,
                      no,
                      no->val,
                      max_exclusivo,
                      black_count,
                      black_ref,
                      profundidade + 1,
                      max_profundidade);
}

int valida_ArvRB(ArvRB raiz)
{
    int black_ref = -1;
    const int max_profundidade = 100000;

    if (NIL == NULL) return 0;
    if (NIL != &NIL_NODE) return 0;
    if (NIL->cor != BLACK) return 0;
    if (NIL->esq != NIL || NIL->dir != NIL || NIL->pai != NIL) return 0;

    if (raiz == NULL) return 0;
    if (raiz == NIL) return 1;

    if (raiz->pai != NIL) return 0;
    if (raiz->cor != BLACK) return 0;

    return valida_rec(raiz,
                      NIL,
                      LLONG_MIN,
                      LLONG_MAX,
                      0,
                      &black_ref,
                      0,
                      max_profundidade);
}
