#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct movie {
  char movie_id[10];
  char movie_name[100];
  int year;
  char certificate[7];
  char run_time[7];
  char genre[100];
  double rating;
  char description[200];
  char diretor[200];
  char director_id[70];
  char star[200];
  char star_id[70];
  double votes;
  double gross;
};
typedef struct movie Movies;
Movies MOVIES[16][60000];


enum Genre {
  ACTION,
  ADVENTURE,
  ANIMATION,
  BIOGRAPHY,
  CRIME,
  FAMILY,
  FANTASY,
  FILM_NOIR,
  HISTORY,
  HORROR,
  MYSTERY,
  ROMANCE,
  SCIFI,
  SPORTS,
  THRILLER,
  WAR
};

int converterChave(char movie_id[], int x) {
    //printf("MOVIE_ID: %s\n", movie_id);
  char minusT[8];
  int i = 0, d = 0;
  for (; i < 8; i++){
      minusT[i] = movie_id[i + 2];
    }
      d = (atoi(minusT) * 100) + x;
      //printf("Chave: %d\n", d);
  return d;
}

struct avl{
    Movies *Movie;
    int chave;
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

Avl * InserirAvl(Avl* a, int chave, Movies *movie){
    if(a == NULL){
        Avl* Novo = (Avl*) malloc(sizeof(Avl));
        Novo->Movie = movie;
        Novo->Altura = 0;
        Novo->esq = Novo->dir = NULL;
        return Novo;
    }
    Avl* Atual = a;
    if(chave < a->chave){
        a->esq = InserirAvl(a->esq, chave, movie);
        if(FatorBalanceAvl(a) > 1){
            if(chave < a->esq->chave)
                a = RotacaoLL(a);
            else a = RotacaoLR(a);
        }
    }
    else
    if(chave > a->chave){
        a->dir = InserirAvl(a->dir, chave, movie);
        if(FatorBalanceAvl(a) > 1){
            if(chave > a->dir->chave)
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

Avl * RetirarAvl(Avl* a, int chave){
    if(a == NULL){
        return NULL;
    }
    Avl* Atual = a;
    if(chave < a->chave){
        a->esq = RetirarAvl(a->esq, chave);
        if(FatorBalanceAvl(a) > 1){
            if(alturaAvl(a->dir->dir) < alturaAvl(a->dir->esq))
                a = RotacaoRL(a);
            else a = RotacaoRR(a);
        }
    }
    else
    if(chave > a->chave){
        a->dir = RetirarAvl(a->dir, chave);
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
            a->chave = t->chave;
            t->chave = chave;
            a->esq = RetirarAvl(a->esq, chave);
        }
    }
    Atual->Altura = maior(alturaAvl(Atual->esq), alturaAvl(Atual->dir)) + 1;
    return a;
}

void ImprimirAvl(Avl *a, int i){
    if(a == NULL) return;
    if(a->chave / 100000000 == i)
    printf("%s\n%s\n%d\n%s\n%s\n%s\n%lf\n%s\n%s\n%s\n%s\n%s\n%lf\n%lf\n\n---------  Chave: %d  --------------\n",
            a->Movie->movie_id, a->Movie->movie_name, a->Movie->year,
            a->Movie->certificate, a->Movie->run_time,
            a->Movie->genre, a->Movie->rating, a->Movie->description,
            a->Movie->diretor, a->Movie->director_id, a->Movie->star,
            a->Movie->star_id, a->Movie->votes, a->Movie->gross, a->chave);
    ImprimirAvl(a->esq, i);
    ImprimirAvl(a->dir, i);
}

void ImprimirAvl2(Avl *a, int Nivel){
    if(a == NULL) return;
    ImprimirAvl2(a->dir, Nivel+1);
    printf("%*d\n", Nivel*5, a->chave);
    ImprimirAvl2(a->esq, Nivel+1);
}

int carregarDados(int x) {
  FILE *fp;
  char linha[10000], *p, texto[10000];
  int campo;
  int cont = 0;
    if(x == ACTION) {
      fp = fopen("IMDbData/action.CSV", "r");
      if (fp == NULL) {
        printf("Nao abriu action.CSV!\n");
        exit(1);
      }
    }

    else if(x == ADVENTURE) {
      fp = fopen("IMDbData/adventure.CSV", "r");
      if (fp == NULL) {
        printf("Nao abriu adventure.CSV!\n");
        exit(1);
      }
    }

else if(x == ANIMATION) {
      fp = fopen("IMDbData/animation.CSV", "r");
      if (fp == NULL) {
        printf("Nao abriu animation.CSV!\n");
        exit(1);
      }
    }

else if(x == BIOGRAPHY) {
      fp = fopen("IMDbData/biography.CSV", "r");
      if (fp == NULL) {
        printf("Nao abriu biography.CSV!\n");
        exit(1);
      }
    }

else if(x == CRIME) {
      fp = fopen("IMDbData/crime.CSV", "r");
      if (fp == NULL) {
        printf("Nao abriu crime.CSV!\n");
        exit(1);
      }
    }

else if(x == FAMILY) {
      fp = fopen("IMDbData/family.CSV", "r");
      if (fp == NULL) {
        printf("Nao abriu family.CSV!\n");
        exit(1);
      }
    }

else if(x == FANTASY) {
      fp = fopen("IMDbData/fantasy.CSV", "r");
      if (fp == NULL) {
        printf("Nao abriu fantasy.CSV!\n");
        exit(1);
      }
    }

else if(x == FILM_NOIR) {
      fp = fopen("IMDbData/film-noir.CSV", "r");
      if (fp == NULL) {
        printf("Nao abriu film-noir.CSV!\n");
        exit(1);
      }
    }

else if(x == HISTORY) {
      fp = fopen("IMDbData/history.CSV", "r");
      if (fp == NULL) {
        printf("Nao abriu history.CSV!\n");
        exit(1);
      }
    }

else if(x == HORROR) {
      fp = fopen("IMDbData/horror.CSV", "r");
      if (fp == NULL) {
        printf("Nao abriu horror.CSV!\n");
        exit(1);
      }
    }


    else if(x == MYSTERY) {
      fp = fopen("IMDbData/mystery.CSV", "r");
      if (fp == NULL) {
        printf("Nao abriu mystery.CSV!\n");
        exit(1);
      }
    }

    else if(x == ROMANCE) {
      fp = fopen("IMDbData/romance.CSV", "r");
      if (fp == NULL) {
        printf("Nao abriu romance.CSV!\n");
        exit(1);
      }
    }

    else if(x == SCIFI) {
      fp = fopen("IMDbData/scifi.CSV", "r");
      if (fp == NULL) {
        printf("Nao abriu scifi.CSV!\n");
        exit(1);
      }
    }

    else if(x == SPORTS) {
      fp = fopen("IMDbData/sports.CSV", "r");
      if (fp == NULL) {
        printf("Nao abriu sports.CSV!\n");
        exit(1);
      }
    }

    else if(x == THRILLER) {
      fp = fopen("IMDbData/thriller.CSV", "r");
      if (fp == NULL) {
        printf("Nao abriu thriller.CSV!\n");
        exit(1);
      }
    }

    else if(x == WAR) {
      fp = fopen("IMDbData/war.CSV", "r");
      if (fp == NULL) {
        printf("Nao abriu war.CSV!\n");
        exit(1);
      }
    }

    if (fscanf(fp, " %[\n]", linha) == EOF)
      return 0;

    while (fscanf(fp, " %[^\n]", linha) != EOF) {

      // printf("%s\n", linha);

      p = strtok(linha, ",");
      campo = 0;

      while (p != NULL) {
          strcpy(texto, p);
            if(texto[0] == '\"' || texto[0] == '\t'){ // Se tem uma aspas
                strcpy(texto, p+1); // Tira a aspas
                p = strtok(NULL, "\""); // Localiza a aspas de finalização
                strcat(texto, ","); // Coloca a vírgula
                if(p != NULL) strcat(texto, p); // Cópia o restante até antes da próxima aspas
            }
      //   printf("%s\n", p);
      //   sleep(1);
        if (campo == 0)
          strcpy(MOVIES[x][cont].movie_id, texto);
        if (campo == 1)
          strcpy(MOVIES[x][cont].movie_name, texto);
        if (campo == 2)
          MOVIES[x][cont].year = atoi(texto);
        if (campo == 3)
          strcpy(MOVIES[x][cont].certificate, texto);
        if (campo == 4)
          strcpy(MOVIES[x][cont].run_time, texto);
        if (campo == 5)
          strcpy(MOVIES[x][cont].genre, texto);
        if (campo == 6)
          MOVIES[x][cont].rating = atof(texto);
        if (campo == 7)
          strcpy(MOVIES[x][cont].description, texto);
        if (campo == 8)
          strcpy(MOVIES[x][cont].diretor, texto);
        if (campo == 9)
          strcpy(MOVIES[x][cont].director_id, texto);
        if (campo == 10)
          strcpy(MOVIES[x][cont].star, texto);
        if (campo == 11)
          strcpy(MOVIES[x][cont].star_id, texto);
        if (campo == 12)
          MOVIES[x][cont].votes = atof(texto);
        if (campo == 13)
          MOVIES[x][cont].gross = atof(texto);
        campo++;
        p = strtok(NULL, ",");
      }

      cont++;
    }
    fclose(fp);
  return cont;
}

int main(){
    Avl *a = NULL;
    int cont[16], d = 0;
    int i = 3;
    cont[i] = carregarDados(i);

        for (int j = 0; j < 5; j++) {
            d = converterChave(MOVIES[i][j].movie_id, i);
            if(d != 0)
                a = InserirAvl(a, d, &MOVIES[i][j]);

                printf("\nmovie_id: %s\nmovie_name: %s\nyear: %d\ncertificate: %s\nrun_time: %s\ngenre: %s\nrating: %lf\ndescription: %s\ndiretor: %s\ndirector_id: %s\nstar: %s\nstar_id: %s\nvotes: %lf\ngross: %lf\n\n",
            MOVIES[i][j].movie_id, MOVIES[i][j].movie_name, MOVIES[i][j].year,
            MOVIES[i][j].certificate, MOVIES[i][j].run_time,
            MOVIES[i][j].genre, MOVIES[i][j].rating, MOVIES[i][j].description,
            MOVIES[i][j].diretor, MOVIES[i][j].director_id, MOVIES[i][j].star,
            MOVIES[i][j].star_id, MOVIES[i][j].votes, MOVIES[i][j].gross);
            sleep(5);
        }

    //ImprimirAvl(a, 0);
    /* printf("\n\n\n");

    printf("\n\n\n");
    a = RetirarAvl(a, 55);
    //a = RetirarAvl(a, 50);

    ImprimirAvl(a);
    printf("\n\n\n");
    ImprimirAvl2(a,0); */

    return 0;
}
