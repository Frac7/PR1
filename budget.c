///sorgente contentente funzioni e procedure per la gestione del budget nel gioco
#include "funzioni.h"
void visualizzaBudget(Giocatore g, int i)
{
    printf("\n\tBudget giocatore %d: %d euro\n\n", i,g.soldi); //visualizzazione del budget del giocatore
}
void modificaBudget(int *b, int t)
{
    *b+=t; //modifica del budget
    printf(" Nuovo budget: %d euro <--\n\n", *b); //stama della modifica del budget
}
void dati(Giocatore *g, Aula *a, Partita *p, Lista **x, int m)
{
    Lista *tmp; //puntatore di tipo lista "temporaneo" (appoggio)
    int i, v_aule; //dichiarazione di due variabili intere: un indice e una variabile contatore
    for(i=0; i<p->n_giocatori; i++) //ciclo for da 0 a ngiocatori -1
    {
        v_aule=0; //inizializzazione della variabile valore aule (valore aule del giocatore corrente)
        if(!m) //modalità di gioco 0, assegnamento casuale
        {
            tmp=x[i]; //il puntatore di appoggio assume lo stesso valore della testa della lista iesima
            while(tmp!=NULL) //finché la lista non è vuota
            {
                g[i].n_aule++; //si incrementa del numero di aule del giocatore
                v_aule+=a[tmp->info].costo; //si incrementa il valore delle aule possedute dal giocatore
                tmp=tmp->next; //il puntatore di appoggio assume il valore del nodo successivo
            }
        }
        g[i].soldi=(CBUDGET/p->n_giocatori)-(v_aule);//alla fine del ciclo interno, il budget del giocatore viene modificato in base al valore delle aule possedute
        visualizzaBudget(g[i],i); //viene visualizzato il budget del giocatore corrente tramite la funzione invocata a cui vengono passati la cella iesima del vettore giocatori e l'indice del giocatore
    }
}
