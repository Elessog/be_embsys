/* Implémentation du module Liste Chaînée Polymorphe */

#include <stdio.h>
#include <stdlib.h>
#include "src/cryptprog/list.h"

/* la structure node */
struct node {
  void        *contenu;
  struct node *suivant;
};


T_LIST nil() {
  return NULL;
}

void supprimer_noeud(void *val, T_LIST *l, int (*equal)()) {
  T_LIST cour, prec;
  cour = *l;
  prec = *l;
  while (!est_vide(cour) && !(*equal)(val, cour -> contenu)) {
	prec = cour;
	cour = cour -> suivant;
  }
  if (!est_vide(cour) && cour == *l) {
	// on modifie la tete
	*l = cour -> suivant;
	free(cour);
  }
  else if (!est_vide(cour)) {
	prec -> suivant = cour ->suivant;
	free(cour);
  }

}

int ajout_tete(void *val, T_LIST *l) {

  T_LIST res = (T_LIST) malloc(sizeof(struct node));

  if(res == NULL) {
    return -1;
  }
  else {
    res -> contenu = val;
    res -> suivant = *l;
    *l = res;
    return -1;
  }

}

int est_vide(T_LIST l) {
  return (l==NULL);
}

void* acces_tete(T_LIST l) {
  if(est_vide(l)) {
    return NULL;
  }
  else {
    return l -> contenu;
  }

}

T_LIST acces_queue(T_LIST l) {

  if(est_vide(l)) {
    return NULL;
  }
  else {
    return l -> suivant;
  }

}

void *membre(void *val, T_LIST l, int (*equal)()) {

  while((l != NULL) && !(*equal)(val, l -> contenu) ){
    l = l -> suivant;
  }

  if(l == NULL) {
    printf("Pas trouve\n");
    return NULL;
  }
  else {
    return l -> contenu;
  }

}


void appliquer_fonction(void (*p)(), T_LIST l) {

  if( est_vide(l) ) {
    return;
  }
  else {
    (*p)(l -> contenu);
    appliquer_fonction(p, l -> suivant);
  }
}

