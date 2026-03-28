#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rbt.h"

/*
Insercao de nos em Árvore Rubro-Negra.
- Implemente a função inserir_main_branch() abaixo.
- Definicao de insercao completa, incluindo balanceamento, é esperada.
*/

#define TEST_MIN_VAL -128
#define TEST_MAX_VAL 128
#define TEST_RANGE_SIZE (TEST_MAX_VAL - TEST_MIN_VAL + 1)

typedef struct {
    int valor;
} CasoInsercao;

static int total_casos = 0;
static int casos_ok = 0;

/* TODO: implemente sua insercao aqui (branch de insercao) */
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
                         int valor,
                         ArvRB *raiz,
                         int esperado,
                         int obtido,
                         const char *motivo)
{
    printf("\nFALHA\n");
    printf("caso   : %s\n", caso);
    printf("passo  : %d\n", passo);
    printf("op     : I %d\n", valor);
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
                         int valor,
                         ArvRB *raiz,
                         const int *presentes,
                         int esperado_nos)
{
    int i;
    int real_nos;

    if (!valida_ArvRB(*raiz))
    {
        aborta_falha(caso, passo, valor, raiz, 0, 0, "valida_ArvRB retornou 0");
    }

    real_nos = contar_nos_ArvRB(*raiz);
    if (real_nos != esperado_nos)
    {
        aborta_falha(caso,
                     passo,
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
                         valor,
                         raiz,
                         esperado,
                         obtido,
                         "busca inconsistente com estado esperado");
        }
    }
}

static void executa_caso(const char *nome, const CasoInsercao *ops, int n_ops, int imprimir_final)
{
    ArvRB *raiz = criar_ArvRB();
    int presentes[TEST_RANGE_SIZE];
    int esperado_nos = 0;
    int i;

    total_casos++;
    memset(presentes, 0, sizeof(presentes));

    if (raiz == NULL)
    {
        aborta_falha(nome, -1, 0, NULL, 0, 0, "falha ao criar arvore");
    }

    check_estado(nome, 0, 0, raiz, presentes, esperado_nos);

    for (i = 0; i < n_ops; i++)
    {
        int id = idx_valor(ops[i].valor);
        int esperado_retorno;
        int retorno;

        if (id < 0)
        {
            aborta_falha(nome, i + 1, ops[i].valor, raiz, 0, 0, "valor fora do dominio");
        }

        esperado_retorno = (presentes[id] == 0) ? 1 : 0;
        retorno = inserir_main_branch(raiz, ops[i].valor);

        if (retorno != esperado_retorno)
        {
            aborta_falha(nome,
                         i + 1,
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

        check_estado(nome, i + 1, ops[i].valor, raiz, presentes, esperado_nos);
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
    static const CasoInsercao c1[] = {{10}};
    static const CasoInsercao c2[] = {{1}, {2}, {3}, {4}, {5}, {6}};
    static const CasoInsercao c3[] = {{6}, {5}, {4}, {3}, {2}, {1}};
    static const CasoInsercao c4[] = {{10}, {5}, {7}, {6}, {8}, {15}, {12}};
    static const CasoInsercao c5[] = {{20}, {20}, {20}};

    printf("=== TEMPLATE INSERCAO ===\n");
    executa_caso("insere-unico", c1, (int)(sizeof(c1) / sizeof(c1[0])), 1);
    executa_caso("insere-crescente", c2, (int)(sizeof(c2) / sizeof(c2[0])), 0);
    executa_caso("insere-decrescente", c3, (int)(sizeof(c3) / sizeof(c3[0])), 0);
    executa_caso("insere-zigzag", c4, (int)(sizeof(c4) / sizeof(c4[0])), 1);
    executa_caso("insere-duplicata", c5, (int)(sizeof(c5) / sizeof(c5[0])), 1);

    printf("\nRESUMO: %d/%d casos passaram.\n", casos_ok, total_casos);
    return 0;
}
