#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rbt.h"

/*
Template para branch de REMOCAO.
- Implemente remover_main_branch().
- Definicao de remocao completa, incluindo balanceamento, é esperada.
*/

#define TEST_MIN_VAL -128
#define TEST_MAX_VAL 128
#define TEST_RANGE_SIZE (TEST_MAX_VAL - TEST_MIN_VAL + 1)

typedef struct {
    char tipo; /* 'I' ou 'R' */
    int valor;
} Operacao;

static int total_casos = 0;
static int casos_ok = 0;

/* TODO: cole aqui sua insercao pronta da branch de insercao */
int insercao_ArvRB(ArvRB *raiz,int valor)
{
    if (raiz == NULL)    return 0;

    // criar nó aux vermelho
    ArvRB aux = malloc(sizeof(struct NO));
    if (aux == NULL) return 0;
    aux->val = valor;
    aux->cor = RED;
    aux->esq = NIL;
    aux->dir = NIL;
    aux->pai = NIL;
    
    // árvore vazia
    if (*raiz == NIL)
    {
        *raiz = aux;
        (*raiz)->pai = NIL;
        (*raiz)->cor = BLACK;
        return 1;
    }

    // descida na arvore
    ArvRB q1 = *raiz;
    ArvRB q2 = NULL;
    while (q1 != NIL)
    {
        q2 = q1;
        if (valor < q1->val)        q1 = q1->esq;
        else if (valor > q1->val)   q1 = q1->dir;
        else
        {
            free(aux);
            return 0;
        }    
    }
    if (valor < q2->val)    q2->esq = aux;
    else                    q2->dir = aux;
    aux->pai = q2;

    //subida e rotacoes
    while (1)
    {
        if (q2->esq != NIL && q2->esq->cor == RED)   q2 = rotacao_dir(raiz, q2);
        if (q2->dir != NIL && q2->dir->cor == RED && q2->dir->dir->cor == RED)
        {
            q2 = rotacao_esq(raiz, q2);
        }
        if (q2->pai == NIL)
        {
            if (*raiz != NIL)
            {
                (*raiz)->cor = BLACK;
                (*raiz)->pai = NIL;
            }
            return 1;
        }
        q2 = q2->pai;
    }
}
static int inserir_main_branch(ArvRB *raiz, int valor)
{
    return insercao_ArvRB(raiz, valor);
}

static ArvRB busca_no(ArvRB *raiz, int valor)
{
    if (raiz == NULL) return NIL;
    ArvRB p = *raiz;
    while (p != NIL)
    {
        if (p->val == valor) return p;
        else if (p->val > valor) p = p->esq;
        else                     p = p->dir;
    }
    return NIL;
}

/* TODO: implemente aqui sua remocao */
int remover_ArvRB(ArvRB *raiz,int valor)
{
    // buscar nó
    ArvRB z = busca_no(raiz, valor);
    if (z == NIL) return 0;

    // costurar nó (remocao fisica)
    ArvRB y;
    if (z->esq == NIL || z->dir == NIL)
    {
        // caso 1: z possui no maximo um filho, entao ele proprio sai da arvore
        y = z;
    }
    else
    {
        // caso 2: dois filhos
        y = z->dir;
        while (y->esq != NIL)
        {
            y = y->esq;
        }
    }
    ArvRB x;
    if (y->esq != NIL)  x = y->esq;
    else                x = y->dir;
    x->pai = y->pai;
    if (y->pai == NIL)          *raiz = x;
    else if (y == y->pai->esq)  y->pai->esq = x;
    else                        y->pai->dir = x;  
    int cor_removida = y->cor; 
    if (y != z)
    {
        y->pai = z->pai;
        if (z->pai == NIL)          *raiz = y;
        else if (z == z->pai->esq)  z->pai->esq = y;
        else                        z->pai->dir = y;
        y->esq = z->esq;
        if (y->esq != NIL) y->esq->pai = y;
        y->dir = z->dir;
        if (y->dir != NIL) y->dir->pai = y;
        y->cor = z->cor;
        if (x->pai == z) x->pai = y;
    }

    // rebalanceamento
    if (cor_removida == BLACK)
    {
        while (x != *raiz && x->cor == BLACK)
        {
            if (x == x->pai->esq)
            {
                ArvRB w = x->pai->dir;
                if (w->cor == RED)
                {
                    w->cor = BLACK;
                    x->pai->cor = RED;
                    rotacao_esq(raiz, x->pai);
                    w = x->pai->dir;
                }
                if (w->esq->cor == BLACK && w->dir->cor == BLACK)
                {
                    w->cor = RED;
                    x = x->pai;
                }
                else
                {
                    if (w->dir->cor == BLACK)
                    {
                        w->esq->cor = BLACK;
                        w->cor = RED;
                        rotacao_dir(raiz, w);
                        w = x->pai->dir;
                    }
                    w->cor = x->pai->cor;
                    x->pai->cor = BLACK;
                    w->dir->cor = BLACK;
                    rotacao_esq(raiz, x->pai);
                    x = *raiz;
                }
            }
            else
            {
                ArvRB w = x->pai->esq;
                if (w->cor == RED)
                {
                    w->cor = BLACK;
                    x->pai->cor = RED;
                    rotacao_dir(raiz, x->pai);
                    w = x->pai->esq;
                }
                if (w->dir->cor == BLACK && w->esq->cor == BLACK)
                {
                    w->cor = RED;
                    x = x->pai;
                }
                else
                {
                    if (w->esq->cor == BLACK)
                    {
                        w->dir->cor = BLACK;
                        w->cor = RED;
                        rotacao_esq(raiz, w);
                        w = x->pai->esq;
                    }
                    w->cor = x->pai->cor;
                    x->pai->cor = BLACK;
                    w->esq->cor = BLACK;
                    rotacao_dir(raiz, x->pai);
                    x = *raiz;
                }
            }
        }
        x->cor = BLACK;
    }

    // ajuste estrutural para garantir as definições da arvore rn do material
    ArvRB n = x->pai;
    while (n != NIL)
    {
    if (n->esq != NIL && n->esq->cor == RED) n = rotacao_dir(raiz, n);
    if (n->dir != NIL && n->dir->cor == RED && n->dir->dir->cor == RED) n = rotacao_esq(raiz, n);
    n = n->pai;
    }

    // raiz sempre preta
    if (*raiz != NIL)
    {
        (*raiz)->cor = BLACK;
        (*raiz)->pai = NIL;
    }
    NIL->cor = BLACK;
    NIL->esq = NIL;
    NIL->dir = NIL;
    NIL->pai = NIL;

    free(z);
    return 1;
}
static int remover_main_branch(ArvRB *raiz, int valor)
{
    return remover_ArvRB(raiz, valor);
}

static int idx_valor(int valor)
{
    if (valor < TEST_MIN_VAL || valor > TEST_MAX_VAL) return -1;
    return valor - TEST_MIN_VAL;
}

static const char *cor_nome(int cor)
{
    return (cor == RED) ? "R" : "B";
}

static void imprimir_inorder_rec(ArvRB no)
{
    if (no == NIL) return;
    imprimir_inorder_rec(no->esq);
    printf("%d(%s) ", no->val, cor_nome(no->cor));
    imprimir_inorder_rec(no->dir);
}

static void imprimir_inorder(ArvRB raiz)
{
    printf("in-order: ");
    if (raiz == NIL)
    {
        printf("<vazia>\n");
        return;
    }
    imprimir_inorder_rec(raiz);
    printf("\n");
}

static void aborta_falha(const char *caso,
                         int passo,
                         char op,
                         int valor,
                         ArvRB *raiz,
                         int esperado,
                         int obtido,
                         const char *motivo)
{
    printf("\nFALHA\n");
    printf("caso   : %s\n", caso);
    printf("passo  : %d\n", passo);
    printf("op     : %c %d\n", op, valor);
    printf("motivo : %s\n", motivo);
    if (esperado != obtido)
    {
        printf("esperado: %d | obtido: %d\n", esperado, obtido);
    }
    if (raiz != NULL)
    {
        printf("nos    : %d\n", contar_nos_ArvRB(*raiz));
        imprimir_inorder(*raiz);
    }
    exit(1);
}

static void check_estado(const char *caso,
                         int passo,
                         char op,
                         int valor,
                         ArvRB *raiz,
                         const int *presentes,
                         int esperado_nos)
{
    int i;
    int real_nos;

    if (!valida_ArvRB(*raiz))
    {
        aborta_falha(caso, passo, op, valor, raiz, 0, 0, "valida_ArvRB retornou 0");
    }

    real_nos = contar_nos_ArvRB(*raiz);
    if (real_nos != esperado_nos)
    {
        aborta_falha(caso,
                     passo,
                     op,
                     valor,
                     raiz,
                     esperado_nos,
                     real_nos,
                     "contagem de nos diferente do esperado");
    }

    for (i = 0; i < TEST_RANGE_SIZE; i++)
    {
        int v = TEST_MIN_VAL + i;
        int esperado = presentes[i];
        int obtido = busca_ArvBR(raiz, v);
        if (!!esperado != !!obtido)
        {
            aborta_falha(caso,
                         passo,
                         op,
                         valor,
                         raiz,
                         esperado,
                         obtido,
                         "busca inconsistente com estado esperado");
        }
    }
}

static void executa_cenario(const char *nome, const Operacao *ops, int n_ops, int imprimir_final)
{
    ArvRB *raiz = criar_ArvRB();
    int presentes[TEST_RANGE_SIZE];
    int esperado_nos = 0;
    int i;

    total_casos++;
    memset(presentes, 0, sizeof(presentes));

    if (raiz == NULL)
    {
        aborta_falha(nome, -1, '-', 0, NULL, 0, 0, "falha ao criar arvore");
    }

    check_estado(nome, 0, '-', 0, raiz, presentes, esperado_nos);

    for (i = 0; i < n_ops; i++)
    {
        int id = idx_valor(ops[i].valor);
        int esperado_retorno;
        int retorno;

        if (id < 0)
        {
            aborta_falha(nome, i + 1, ops[i].tipo, ops[i].valor, raiz, 0, 0, "valor fora do dominio");
        }

        if (ops[i].tipo == 'I')
        {
            esperado_retorno = (presentes[id] == 0) ? 1 : 0;
            retorno = inserir_main_branch(raiz, ops[i].valor);
            if (retorno != esperado_retorno)
            {
                aborta_falha(nome,
                             i + 1,
                             ops[i].tipo,
                             ops[i].valor,
                             raiz,
                             esperado_retorno,
                             retorno,
                             "retorno inesperado de inserir_main_branch");
            }
            if (retorno == 1)
            {
                presentes[id] = 1;
                esperado_nos++;
            }
        }
        else if (ops[i].tipo == 'R')
        {
            esperado_retorno = (presentes[id] == 1) ? 1 : 0;
            retorno = remover_main_branch(raiz, ops[i].valor);
            if (retorno != esperado_retorno)
            {
                aborta_falha(nome,
                             i + 1,
                             ops[i].tipo,
                             ops[i].valor,
                             raiz,
                             esperado_retorno,
                             retorno,
                             "retorno inesperado de remover_main_branch");
            }
            if (retorno == 1)
            {
                presentes[id] = 0;
                esperado_nos--;
            }
        }
        else
        {
            aborta_falha(nome,
                         i + 1,
                         ops[i].tipo,
                         ops[i].valor,
                         raiz,
                         0,
                         0,
                         "operacao invalida no cenario");
        }

        check_estado(nome, i + 1, ops[i].tipo, ops[i].valor, raiz, presentes, esperado_nos);
    }

    if (imprimir_final)
    {
        printf("\nCASO: %s\n", nome);
        imprimir_inorder(*raiz);
    }

    destroi_ArvRB(raiz);
    casos_ok++;
}

int main(void)
{
    static const Operacao c1[] = {
        {'I', 10}, {'I', 5}, {'I', 15}, {'I', 3}, {'I', 7}, {'I', 12}, {'I', 20},
        {'R', 3}
    };
    static const Operacao c2[] = {
        {'I', 10}, {'I', 5}, {'I', 15}, {'I', 12},
        {'R', 15}
    };
    static const Operacao c3[] = {
        {'I', 10}, {'I', 5}, {'I', 15}, {'I', 12}, {'I', 20},
        {'R', 15}
    };
    static const Operacao c4[] = {
        {'I', 10}, {'I', 5}, {'I', 15}, {'I', 2}, {'I', 7}, {'I', 12}, {'I', 20},
        {'R', 10}
    };
    static const Operacao c5[] = {
        {'I', 8}, {'I', 4}, {'I', 12},
        {'R', 100}
    };
    static const Operacao c6[] = {
        {'I', 8}, {'I', 4}, {'I', 12}, {'I', 2}, {'I', 6}, {'I', 10}, {'I', 14},
        {'R', 2}, {'R', 4}, {'R', 6}, {'R', 8}, {'R', 10}, {'R', 12}, {'R', 14}
    };

    printf("=== TEMPLATE REMOCAO ===\n");
    executa_cenario("remove-folha", c1, (int)(sizeof(c1) / sizeof(c1[0])), 1);
    executa_cenario("remove-um-filho", c2, (int)(sizeof(c2) / sizeof(c2[0])), 1);
    executa_cenario("remove-dois-filhos", c3, (int)(sizeof(c3) / sizeof(c3[0])), 1);
    executa_cenario("remove-raiz", c4, (int)(sizeof(c4) / sizeof(c4[0])), 1);
    executa_cenario("remove-inexistente", c5, (int)(sizeof(c5) / sizeof(c5[0])), 1);
    executa_cenario("remove-ate-esvaziar", c6, (int)(sizeof(c6) / sizeof(c6[0])), 1);

    printf("\nRESUMO: %d/%d casos passaram.\n", casos_ok, total_casos);
    return 0;
}
