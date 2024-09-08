#include <stdio.h>
#include "avl.h"
#include <stdlib.h>

/** Insere um elemento na árvore
 * @param raiz ponteiro para raiz absoluta da árvore
 * @param valor valor inteiro a ser inserido na árvore
 * @param *cresceu ponteiro para inteiro que informa se a raiz relativa cresceu após a inserção
 * @return A árvore após a inserção
 */
avl inserir(avl raiz, int valor, int *cresceu)
{

    if (raiz == NULL) // Inserir elemento
    {
        avl novo = malloc(sizeof(no));

        novo->chave = valor;
        novo->esq = NULL;
        novo->dir = NULL;
        novo->fb = 0;

        *cresceu = 1;

        return novo;
    }

    if (valor > raiz->chave) // Busque à direita
    {
        raiz->dir = inserir(raiz->dir, valor, cresceu);

        if (*cresceu) // Verifique fatores de balanço
        {
            switch (raiz->fb)
            {
            case -1:
                raiz->fb = 0;
                *cresceu = 0;
                break;
            case 0:
                raiz->fb = 1;
                *cresceu = 1;
                break;
            case 1:
                raiz->fb = 2;
                raiz = rotacionar(raiz);
                *cresceu = 0;
                break;
            }
        }
    }
    else // Busque à esquerda
    {
        raiz->esq = inserir(raiz->esq, valor, cresceu);

        if (*cresceu) // Verifique fatores de balanço
        {
            switch (raiz->fb)
            {
            case -1:
                raiz->fb = -2;
                raiz = rotacionar(raiz);
                *cresceu = 0;
                break;
            case 0:
                raiz->fb = -1;
                *cresceu = 1;
                break;
            case 1:
                raiz->fb = 0;
                *cresceu = 0;
                break;
            }
        }
    }

    return raiz;
}

/** Remove um elemento da árvore
 * @param raiz ponteiro para a raiz relativa da árvore
 * @param valor valor ser removido da árvore
 * @param diminuiu ponteiro que informa se a árvore relativa diminuiu após a remoção
 * @return A árvore após a remoção
 */
avl remover(avl raiz, int valor, int *diminuiu)
{

    if (raiz == NULL) // Elemento não encontrado
    {
        *diminuiu = 0;
        return NULL;
    }

    if (raiz->chave == valor) // Elemento encontrado
    {
        avl esq, dir, maior;

        switch (filhos(raiz)) // Verifica filhos e caso de remoção
        {
        case NENHUM:

            free(raiz);
            *diminuiu = 1;

            return NULL;
            break;

        case ESQUERDO:

            esq = raiz->esq;
            free(raiz);
            *diminuiu = 1;

            return esq;
            break;

        case DIREITO:

            dir = raiz->dir;
            free(raiz);
            *diminuiu = 1;

            return dir;
            break;

        case AMBOS:

            maior = maiorElemento(raiz->esq);
            raiz->chave = maior->chave;
            raiz->esq = remover(raiz->esq, maior->chave, diminuiu);

            return raiz;
            break;
        }
    }

    if (valor > raiz->chave) // Busque à direita
    {
        raiz->dir = remover(raiz->dir, valor, diminuiu);

        if (*diminuiu) // Verifique fatores de balanço
        {
            switch (raiz->fb)
            {
            case -1:
                raiz->fb = -2;
                return rotacionar(raiz);
                break;
            case 0:
                raiz->fb = -1;
                *diminuiu = 0;
                break;
            case 1:
                raiz->fb = 0;
                *diminuiu = 1;
                break;
            }
        }
    }
    else
    {
        raiz->esq = remover(raiz->esq, valor, diminuiu);

        if (*diminuiu) // Verifique fatores de balanço
        {
            switch (raiz->fb)
            {
            case -1:
                raiz->fb = 0;
                *diminuiu = 1;
                break;
            case 0:
                raiz->fb = 1;
                *diminuiu = 0;
                break;
            case 1:
                raiz->fb = 2;
                rotacionar(raiz);
                break;
            }
        }
    }

    return raiz;
}

/** Atualiza os fatores de balanço e chama a função rotação
 * @param pivot ponteiro para o elemento pivot da rotação
 * @return Árvore após rotação e fatores de balanço atualizados
 */
avl rotacionar(avl pivot)
{
    avl u, v;

    if (pivot->fb > 0) // Rotação Esquerda
    {
        u = pivot->dir;

        if (u->fb >= 0) // Rotação Simples
        {
            if (u->fb == 1) // fb(2,1)
            {
                pivot->fb = 0;
                u->fb = 0;
            }
            else // fb(2,0)
            {
                pivot->fb = 1;
                u->fb = -1;
            }

            return rotacaoSimplesEsquerda(pivot);
        }
        else // Rotação Dupla
        {
            v = u->esq;

            switch (v->fb) // fb(2, -1, {-1,0,1})
            {
            case -1:
                pivot->fb = 0;
                u->fb = 1;
                break;
            case 0:
                pivot->fb = 0;
                u->fb = 0;
                v->fb = 0;
                break;
            case 1:
                pivot->fb = -1;
                u->fb = 1;
                break;
            }

            return rotacaoDuplaEsquerda(pivot);
        }
    }
    else // Rotação Direita
    {

        u = pivot->esq;

        if (u->fb <= 0) // Rotação Simples
        {
            if (u->fb == -1) // fb(-2, -1)
            {
                pivot->fb = 0;
                u->fb = 0;
            }
            else // fb(-2, 0)
            {
                pivot->fb = -1;
                u->fb = 1;
            }

            return rotacaoSimplesDireita(pivot);
        }
        else // Rotação Dupla
        {
            v = u->dir;

            switch (v->fb) // fb(-2, 0, {-1, 0, 1})
            {
            case -1:
                pivot->fb = 1;
                u->fb = 0;
                break;
            case 0:
                pivot->fb = 0;
                u->fb = 0;
                break;
            case 1:
                pivot->fb = 0;
                u->fb = -1;
                break;
            }
            return rotacaoDuplaDireita(pivot);
        }
    }
}

/** Atualiza os ponteiros pós rotação simples esquerda
 * @param pivot ponteiro para o elemento pivot da rotação
 * @return Árvore após a rotação simples esquerda
 */
avl rotacaoSimplesEsquerda(avl pivot)
{
    /* Representação

       p             u
      / \           / \
     t1  u    ->   p  t3
        / \       / \
       t2 t3     t1 t2

     · Note que 't1' e 't3' são redundantes para a atualização.
    */
    avl u, t2;

    // Atribuição
    u = pivot->dir;
    t2 = u->esq;

    // Reposicionamento
    u->esq = pivot;
    pivot->dir = t2;

    return u;
}

/** Atualiza os ponteiros pós rotação simples direita
 * @param pivot ponteiro para o elemento pivot da rotação
 * @return Árvore após a rotação simples direita
 */
avl rotacaoSimplesDireita(avl pivot)
{
    /* Representação

         p             u
        / \           / \
       u  t3    ->   t1  p
      / \               / \
     t1 t2             t2 t3

     · Note que a alteração para 't1' e 't3' é redundante.
    */

    avl u, t2;

    // Atribuição
    u = pivot->esq;
    t2 = u->dir;

    // Reposicionamento
    pivot->esq = t2;
    u->dir = pivot;

    return u;
}

/** Atualiza os ponteiros pós rotação dupla esquerda
 * @param pivot ponteiro para o elemento pivot da rotação
 * @return Árvore após a rotação simples esquerda
 */
avl rotacaoDuplaEsquerda(avl pivot)
{
    /* Representação

       p        ·      p         ·
      / \       ·     / \        ·       v
     t1  u      ·    t1  v       ·     /   \
        / \     ·       / \      ·    p     u
       v  t4    ·      t2  u     ·   / \   / \
      / \       ·         / \    ·  t1 t2 t3 t4
     t2 t3      ·        t3 t4   ·

     · Note que a alteração para 't1' e 't4' é redundante.

    */

    avl u, v, t2, t3;

    // Atribuição
    u = pivot->dir;
    v = u->esq;
    t2 = v->esq;
    t3 = v->dir;

    // Reposicionamento
    v->esq = pivot;
    v->dir = u;
    pivot->dir = t2;
    u->esq = t3;

    return v;
}

/** Atualiza os ponteiros pós rotação dupla direita
 * @param pivot ponteiro para o elemento pivot da rotação
 * @return Árvore após a rotação simples direita
 */
avl rotacaoDuplaDireita(avl pivot)
{
    /* Representação

         p      ·       p      ·
        / \     ·      / \     ·      v
       u  t4    ·     v  t4    ·    /   \
      / \       ·    / \       ·   u     p
     t1  v      ·   u  t3      ·  / \   / \
        / \     ·  / \         · t1 t2 t3 t4
       t2 t3    · t1 t2        ·

     · Note que a alteração para 't1' e 't4' é redundante.

    */

    avl u, v, t2, t3;

    // Atribuição
    u = pivot->esq;
    v = u->dir;
    t2 = v->esq;
    t3 = v->dir;

    // Reposicionamento
    v->dir = pivot;
    v->esq = u;
    u->dir = t2;
    pivot->esq = t3;

    return v;
}

/** Altura duma raiz relativa
 * @param raiz raiz relativa da árvore
 * @return A altura da raiz relativa
 */
int altura(avl raiz)
{
    if (raiz == NULL)
        return 0;

    return 1 + max(altura(raiz->esq), altura(raiz->dir));
};

/** Máximo entre inteiros
 * @param a,b Inteiro a ser comparado
 * @return O máximo entre dois números inteiros
 */
int max(int a, int b)
{
    return a > b ? a : b;
}

/** Maior elemento duma raiz relativa
 * @param raiz raiz relativa
 * @return O elemento de maior valor a partir duma raiz relativa
 */
avl maiorElemento(avl raiz)
{
    // Árvore vazia
    if (raiz == NULL)
        return NULL;

    if (raiz->dir == NULL)
        return raiz;

    return maiorElemento(raiz->dir);
}

/** Verifica quantos e quais filhos o elemento possui.
 * @param raiz raiz relativa
 * @return Enum relativo ao caso de quantidade de filhos
 */
enum filhos filhos(avl elemento)
{
    if (elemento == NULL)
        return -1;

    if (elemento->esq == NULL && elemento->dir == NULL)
        return NENHUM;
    if (elemento->esq != NULL && elemento->dir == NULL)
        return ESQUERDO;
    if (elemento->esq == NULL && elemento->dir != NULL)
        return DIREITO;
    if (elemento->esq != NULL && elemento->dir != NULL)
        return AMBOS;
}

/** Inicia o loop de execuções do menu
 *
 */
void run()
{
    avl raiz = NULL;
    int cresceu = 0;
    int diminuiu = 0;

    int operacao, input;

    while (1)
    {
        printf("(1) Inserir\n");
        printf("(2) Preorder\n");
        printf("(3) Remover\n");
        printf("(99) Sair\n");

        printf("> \n");
        scanf("%d", &operacao);

        switch (operacao)
        {

        case 1:
            scanf("%d", &input);
            raiz = inserir(raiz, input, &cresceu);
            break;

        case 2:
            preorder(raiz);
            printf("\n");
            break;

        case 3:
            scanf("%d", &input);
            raiz = remover(raiz, input, &diminuiu);
            break;

        case 99:
            return;
            break;

        default:
            printf("\033[1;33m\nOpcao invalida, tente novamente\n\n\033[0m");
        }
    }
}

/** Imprime os elementos da árvore e seus respectivos fatores de balanço
 @param raiz raiz relativa
 */
void preorder(avl raiz)
{
    if (raiz != NULL)
    {
        printf("[%d %d]", raiz->chave, raiz->fb);
        preorder(raiz->esq);
        preorder(raiz->dir);
    }
}
