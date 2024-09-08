#ifndef AVL_H
#define AVL_H

#include <stdio.h>

typedef struct no
{
    int chave;
    struct no *esq, *dir;
    int fb;
} no;

typedef no *avl;

enum filhos
{
    NENHUM,
    ESQUERDO,
    DIREITO,
    AMBOS
};

avl inserir(avl raiz, int valor, int *cresceu);
avl remover(avl raiz, int valor, int *diminuiu);

avl rotacionar(avl pivot);
avl rotacaoSimplesEsquerda(avl pivot);
avl rotacaoSimplesDireita(avl pivot);
avl rotacaoDuplaEsquerda(avl pivot);
avl rotacaoDuplaDireita(avl pivot);

avl maiorElemento(avl raiz);

int altura(avl raiz);
int max(int a, int b);

enum filhos filhos(avl raiz);

void run();

void preorder(avl raiz);
#endif