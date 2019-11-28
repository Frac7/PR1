///file sorgente contenente funzioni e procedure per la gestione delle liste e di array allocati dinamicamente

///riallocazioni: in tutte le riallocazioni (vettore di liste, vettore giocatori)
///la cella del vettore con l'indice del giocatore attivo assume il valore della cella dell'ultimo giocatore
///infine il vettore viene riallocato e viene restituito il nuovo indirizzo
///NB: nella riallocazione del vettore di liste, la prima operazione eseguita è l'eliminazione della lista del giocatore attivo.
#include "funzioni.h"
void inizializzazione(Lista **x, Partita *p)
{
    int i;
    for(i=0; i<p->n_giocatori; i++) //inizializzazione lista (0 - n_giocatori-1)
        x[i]=NULL; //inizializzazione per successiva creazione della lista
}
Giocatore *riallocazione_g(Giocatore *g, Partita *p)
{
    if(p->g_attivo!=p->n_giocatori) //se il giocatore attivo è diverso dal numero di giocatori -1
        g[p->g_attivo]=g[p->n_giocatori]; //si scambia la cella del giocatore attivo con quella dell'ultimo
    g=(Giocatore*)realloc(g,sizeof(Giocatore)*p->n_giocatori); //riallocazione del vettore
    return g; //si restituisce il vettore di giocatori
}
Lista **riallocazione_l(Partita *p, Lista **x)
{
    eliminaLista(x[p->g_attivo]); //viene eliminata la lista del giocatore attivo
    if(p->n_giocatori != p->g_attivo) //se il giocatore attivo è diverso dal numero di giocatori - 1
        x[p->g_attivo]=x[p->n_giocatori]; //la cella del giocatore in uscita assume il valore della cella dell'ultimo giocatore
    x=(Lista**)realloc(x,sizeof(Lista*)*p->n_giocatori); //riallocazione del vettore
    return x; //restituzione del puntatore al primo elemento del vettore di liste al chiamante
}
void stampa(Lista **x, Partita *p)
{
    int i; //dichiarazione indice
    for(i=0; i<p->n_giocatori; i++) //ciclo for 0 - n_giocatori-1
    {
        printf("\nGiocatore %d\n", i); //stampa dell'intestazione della lista
        if(x[i]==NULL) //se la lista è vuota, il giocatore non possiede nessuna aula
            printf("\tNessuna aula posseduta.\n"); //si stampa il messaggio di output per la comunicazione all'utente
        else //altrimenti si stampa la lista
            stampaLista(x[i]); //invocazione della funzione per la stampa della lista e passaggio del rierimento alla testa
    }
}
void eliminaLista(Lista *l)
{
    if(l!=NULL) //se la lita non è vuota
    {
        eliminaLista(l->next); //la funzione richiama se stessa passando l'indirizzo del nodo successivo
        free(l); //la zona di memoria puntata viene liberata
    }
}
void stampaLista(Lista *x)
{
    if(x!=NULL) //se la lista non è vuota
    {
        printf("\tAula -> %d\n", x->info); //si stampa l'informazione del nodo
        stampaLista(x->next); //la funzione richiama se stessa passando il puntatore al nodo successivo
    }
}
Lista *inserisciNodo(Lista *x, int info)
{
    Lista *nuovo=NULL; //puntatore per il nuovo elemento
    nuovo=(Lista*)malloc(sizeof(Lista)); //allocazione dinamica per il nuovo nodo
    if(nuovo==NULL) //controllo sull'allocazione dinamica della memoria (nel caso in cui l'allocazione non sia andata a buon fine, si esce dal programma)
        return nuovo;
    nuovo->info=info; //il campo info prende l'indice dell'aula
    nuovo->next=x; //il campo next prende la testa della lista
    return nuovo; //restituzione del nuovo riferimento alla testa
}
Lista **creazioneLista(Lista **x, Partita *p, Aula *a, Giocatore *g, int ultimo, int m)
{
    int i,j; //dichiarazione indici
    if(!m || ultimo==BANCO) //se la mod è 0 o se è stato caricato un salvataggio (mod indifferente, potrebbero esserci aule acquistate o nessuna aula acquistata con mod 1)
    {
        for(i=0;i<p->n_giocatori;i++) //ciclo for da 0 a n_giocatori-1
            for(j=0;j<TABELLONE;j++) //ciclo per caselle
                if(a[j].proprietario==i) //se l'aula è posseduta dal giocatore iesimo
                {
                    x[i]=inserisciNodo(x[i],j); //viene inserito l'id dell'aula in testa
                    if(x==NULL)
                        return x;
                }
        stampa(x,p); //invocazione della funzione per la stampa di tutte le liste e passaggio del vettore di liste e del puntatore a stuttura di tipo partita
        pausa(); //invocazione della funzione di pausa
    }
    else if(m &&  ultimo!=TABELLONE)//mod 1 oppure variabile che indice l'ultima aula acquistata con valore diverso da 40(fuori range)
    {
        x[p->g_attivo]=inserisciNodo(x[p->g_attivo],ultimo); //la lista viene aggiornata
        if(x[p->g_attivo]==NULL)
            return NULL;
        stampa(x,p); //la lista viene stampata tramite la funzione di stampa
        pausa(); //invocazione della funzione di pausa
    }
    return x; //restituzione del riferimento alla prima lista (prima cella del vettore)
}
