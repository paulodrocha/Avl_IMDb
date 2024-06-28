#include <stdio.h>
struct avl{
    int Info;
    int Altura;
    struct avl *esq, *dir;
};
typedef struct avl Avl;

int alturaAvl(Avl *a){
    if(a == NULL) return -1;
    else return a->Altura;
}

int maior(int x, int y){
    if(x > y) return x;
    else return y;
}

int FatorBalanceAvl(Avl * a){
    return abs(alturaAvl(a->esq) - alturaAvl(a->dir));
}

Avl* RotacaoLL(Avl *a){
    Avl *no;
    no = a->esq;
    a->esq = no->dir;
    no->dir = a;
    a->Altura = maior(alturaAvl(a->esq),
                            alturaAvl(a->dir))
                            + 1;
    no->Altura = maior(alturaAvl(no->esq),
                       a->Altura) + 1;
    return no;
}

Avl * RotacaoRR(Avl *a){
    Avl *no;
    no = a->dir;
    a->dir = no->esq;
    no->esq = a;
    a->Altura = maior(alturaAvl(a->esq),
                            alturaAvl(a->dir))
                            + 1;
    no->Altura = maior(alturaAvl(no->dir),
                       a->Altura) + 1;
    return no;
}

Avl * RotacaoLR(Avl *a){
    a->esq = RotacaoRR(a->esq);
    a = RotacaoLL(a);
    return a;
}

Avl * RotacaoRL(Avl *a){
    a->dir = RotacaoLL(a->dir);
    a = RotacaoRR(a);
    return a;
}

Avl * InserirAvl(Avl* a, int Valor){
    if(a == NULL){
        Avl* Novo = (Avl*) malloc(sizeof(Avl));
        Novo->Info = Valor;
        Novo->Altura = 0;
        Novo->esq = Novo->dir = NULL;
        return Novo;
    }
    Avl* Atual = a;
    if(Valor < a->Info){
        a->esq = InserirAvl(a->esq, Valor);
        if(FatorBalanceAvl(a) > 1){
            if(Valor < a->esq->Info)
                a = RotacaoLL(a);
            else a = RotacaoLR(a);
        }
    }
    else
    if(Valor > a->Info){
        a->dir = InserirAvl(a->dir, Valor);
        if(FatorBalanceAvl(a) > 1){
            if(Valor > a->dir->Info)
                a = RotacaoRR(a);
            else a = RotacaoRL(a);
        }
    }
    else{
        printf("Valor duplicado!!\n");
        exit(1);
    }
    Atual->Altura = maior(alturaAvl(Atual->esq), alturaAvl(Atual->dir)) + 1;
    return a;
}

Avl * RetirarAvl(Avl* a, int Valor){
    if(a == NULL){
        return NULL;
    }
    Avl* Atual = a;
    if(Valor < a->Info){
        a->esq = RetirarAvl(a->esq, Valor);
        if(FatorBalanceAvl(a) > 1){
            if(alturaAvl(a->dir->dir) < alturaAvl(a->dir->esq))
                a = RotacaoRL(a);
            else a = RotacaoRR(a);
        }
    }
    else
    if(Valor > a->Info){
        a->dir = RetirarAvl(a->dir, Valor);
        if(FatorBalanceAvl(a) > 1){
            if(alturaAvl(a->esq->esq) < alturaAvl(a->esq->dir))
                a = RotacaoLR(a);
            else a = RotacaoLL(a);
        }
    }
    else{
        if(a->esq == NULL && a->dir == NULL){
            free(a);
            a = NULL;
            return a;
        }
        else if(a->esq == NULL){ // Tem apenas a direita
            Avl *t = a;
            a = a->dir;
            free(t);
        }
        else if(a->dir == NULL){ // Tem apenas a esquerda
            Avl *t = a;
            a = a->esq;
            free(t);
        }
        else{ // Tem os dois
            Avl *t = a->esq;
            while(t->dir != NULL){
                t = t->dir;
            }
            a->Info = t->Info;
            t->Info = Valor;
            a->esq = RetirarAvl(a->esq, Valor);
        }
    }
    Atual->Altura = maior(alturaAvl(Atual->esq), alturaAvl(Atual->dir)) + 1;
    return a;
}

void ImprimirAvl(Avl *a){
    if(a == NULL) return;
    printf("%d  Altura %d\n", a->Info, a->Altura);
    ImprimirAvl(a->esq);
    ImprimirAvl(a->dir);
}

void ImprimirAvl2(Avl *a, int Nivel){
    if(a == NULL) return;
    ImprimirAvl2(a->dir, Nivel+1);
    printf("%*d\n", Nivel*5, a->Info);
    ImprimirAvl2(a->esq, Nivel+1);
}

int main(){
    Avl *a = NULL;

    a = InserirAvl(a, 30);
    a = InserirAvl(a, 40);
    a = InserirAvl(a, 50);
    a = InserirAvl(a, 60);
    a = InserirAvl(a, 55);
    a = InserirAvl(a, 35);
    a = InserirAvl(a, 45);
    a = InserirAvl(a, 56);
    a = InserirAvl(a, 65);
    a = InserirAvl(a, 57);
    a = InserirAvl(a, 20);
    a = InserirAvl(a, 10);

    ImprimirAvl(a);
    printf("\n\n\n");
    ImprimirAvl2(a,0);

    printf("\n\n\n");
    a = RetirarAvl(a, 55);
    //a = RetirarAvl(a, 50);

    ImprimirAvl(a);
    printf("\n\n\n");
    ImprimirAvl2(a,0);

    return 0;
}
