/* Interface du module Liste Chaînée Polymorphe qui définit
 le type abstrait T_LIST dont les éléments sont de type (void *)              */

#ifndef _LIST
#define _LIST
  typedef struct node *T_LIST;


/* fonction pour récupérer une liste vide
    l = nil()                                                                 */
 T_LIST nil();

/* fonction pour supprimer un noeud dans la liste pointee par l .
Utilisation :  supprimer_noeud(p, pointeursurliste, compare),
ou compare est une fonction de prototype :
int compare(void* n1, void* n2)*/
void supprimer_noeud(void *val, T_LIST *l, int (*equal)());

/* fonction pour ajouter un élément à la tête d'une liste l
    ret = ajout_tete( (void *) a, &l)
   avec ret = ALLOC_ERROR si un problème d'allocation se pose,
              NO_ERROR sinon
*/
 int ajout_tete(void *val, T_LIST *l);

/* fonction pour tester si la liste est vide. 1 si elle est vide, 0 sinon
 */
 int est_vide(T_LIST l);

/* fonction pour récupérer l'élément en tête de liste l
   val = (int *) acces_tete(l) récupère le pointeur vers l'élément en
   tête (ici pour l'EXEMPLE un entier)
*/
 void* acces_tete(T_LIST l);

/* fonction pour récupérer le reste de la liste l. Attention, ici
queue = le pointeur sur l'element suivant la tete, et non pas la fin
de la liste!  q = acces_queue(l) */
 T_LIST acces_queue(T_LIST l);

/* fonction pour récupérer le contenu dans la liste l égal au sens
   de la fonction equal à val ;
   si la recherche échoue, le résultat est NULL.
   exemple : pi = (int*) member(qi, l, eq_int)                                */
 void *membre(void *val, T_LIST l, int (*equal)());

/* procédure d'application de la procédure p à tous les contenus de la liste l
   exemple appliquer_fonction(aff_int, l)                                                */
 void appliquer_fonction(void (*p)(), T_LIST l);

#endif
