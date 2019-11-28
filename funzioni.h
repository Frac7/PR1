///header contenente direttive di preprocessing e dichiarazioni di strutture, enumerazioni, funzioni
/*inclusione librerie*/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
/*definizioni di costanti letterali*/
#define DOPPI3 3 //tre volte dadi doppi (pulire i bagni)
#define CARATTERE 1 //dimensione carattere (char) per il controllo sulla lunghezza dei caratteri inseriti
#define TABELLONE 40 //n caselle tabellone
#define MIN 2 //n minimo giocatori
#define MAX 8 //n massimo giocatori
#define STRINGA 23 //dimensione stringa
#define MOLTIPLICATORE_PEDAGGIO 1000 //costante da moltiplicare per i pedaggi per velocizzare il gioco
#define MOD1 0 //modalità acquisto casuale
#define MOD2 1 //modalità acquisto al passaggio
#define DADOMIN 1 //valore minore dado per generazione casuale
#define DADOMAX 6 //valore massimo dado
#define CBUDGET 12000 //costante per generazione budget
#define N_AULE 24 //n totale aule sul tabellone
#define VIA 20 //importo da aggiungere al budget al superamento del via
#define PROVA 1 //costante per stampe di prova
#define TURNI_B 3 //turni di pulizia del bagno
#define BANCO -1 //costante che indica il banco
#define TENDA 100 //costo costruzione tenda
#define SCRIVANIA 50 //costo costruzione scrivania
#define BATCAV 30 //n casella batcaverna
#define BAGNO 10 //n casella bagno
#define N_SFIGHE 4 //numero totale tipologia di sfighe
#define TIPO1 0 //tipo sfiga 1
#define TIPO2 1 //tipo sfiga 2
#define TIPO3 2 //tipo sfiga 3
#define TIPO4 3 //tipo sfiga 4
#define SUPERSTITE 1 //ultimo giocatore, vincitore
#define NA -2 //costante che identifica un'aula non acquistabile
#define POSSIBILITA 3 //costante per l'assegnamento di scrivanie gratuite: 1 aula su 3
#define CON 1 //valore "di ritorno" per continuare il turno
#define SALTA 0 //valore "di ritorno" per saltare il turno
/*definizione di enumerazioni e nuovi tipi*/
//enumerazione che rappresenta il segnaposto di ogni giocatore (direttamente dall'Aula Costa...)
typedef enum {MORGAGNI, PROIETTORE, CANCELLINO, TELECOMANDO, GESSETTO, PORTA, TERMOSIFONE, PRESA} Segnaposto; //enumerazione segnaposti (0-giallo,...7-nero)
//enumerazione che rappresenta la tipologia delle caselle
typedef enum {AULA, TASSA, SFIGA, ALTRO} Casella; //enumerazione tipologia aule: 0, aula; 1, tassa; ...
/*definizione di strutture e tipi*/
typedef struct
{
    char nome_g[STRINGA+1]; //nome giocatori (24 caratteri)
    Segnaposto simbolo; //segnaposto giocatore
    int soldi; //budget
    int p_bagno; //turni di pulizia del bagno mancanti (turni mancati per rientrare in gioco)
    int n_aule; //n aule possedute
    int posizione; //posizione del giocatore
} Giocatore; //struttura e nuovo tipo giocatore
typedef struct
{
    int costo; //costo aula
    int p_passaggio; //pedaggio semplice
    int pp_scrivania; //pedaggio con scrivania
    int pp_tenda; //pedaggio con tenda
    char nome_a[STRINGA+1]; //nome aula (24 caratteri)
    Casella tipo; //tipologia casella
    _Bool f_scrivania; //flag presenza scrivania
    _Bool f_tenda; //flag presenza tenda
    int proprietario; //indice giocatore proprietario
} Aula; //struttura e tipo aula
typedef struct
{
    int n_giocatori; //numero giocatori
    int n_turni; //turni giocati
    int g_attivo; //indice giocatore attivo
} Partita; //struttura e tipo partita
typedef struct lista
{
    int info; //informazione del nodo
    struct lista* next; //puntatore a nodo successivo
} Lista; //struttura e tipo lista
typedef struct
{
    char nome_s[STRINGA*10]; //nome della sfiga
    int tipologia; //tipologia di sfiga(1, 2, 3, 4)
    int importo; //importo da pagare, tassa semplice
    int p_naule; //importo da pagare per il numero di aule possedute, tassa immobili
    int p_scrivanie; //importo da pagare per il numero di scrivanie possedute, tassa immobili
    int p_tende; //importo da pagare per il numero di tende possedute, tassa immobili
    int nuovaposizione; //nuova posizione sul tabellone, goto
} Sfiga;//struttura e tipo sfiga
/*dichirazione delle funzioni*/

///parametri in ingresso: vettore di giocatori, puntatore a struttura di tipo partita, vettore di liste, intero che indice la modalità di gioco
void dati(Giocatore *g, Aula *a, Partita *p, Lista **x, int m);
///procedura per la modifica del budget iniziale

///parametri in ingresso: puntatore a struttura di tipo partita, vettore giocatori, vettore aule, intero per la modalità di gioco, il vettore di sfighe, un puntatore a intero (indice del mazzo), un puntatore a intero (ultimo terreno acquistato), il vettore di liste, un intero per la modalità di gioco
int gestioneTurno(Partita *p, Giocatore *g, Aula *a, int m, Sfiga *l, int *mazzo, int *ultimo, Lista **x, int t);
///funzione per la gestione del turno
///restituisce un intero: 0 salta il turno, 1 altrimenti

///parametri in ingresso: puntatore al primo elemento della lista, intero per l'informazione da aggiungere
Lista *inserisciNodo(Lista *x, int info);
///funzione per l'inserimento di un nodo in testa alla lista
///restituisce un puntatore di tipo lista

///parametri in ingresso: vettore di giocatori, puntatore a struttura di tipo partita, puntatore a intero per contare i lanci doppi
void ctrldadi(Giocatore *g, Partita *p, int *c);
///procedura che controlla il risultato ottenuto dal lancio dei dadi
///gestisce la possibilità di ottenere dadi doppi (e rilanciarli) o di ottenerli doppi due volte (rimanda alla gestione della pulizia del bagno)); alla funzione viene passata la struttura giocatori
///NB: il puntatore a intero è un contatore. serve per gestire i dadi doppi e gestire un eventuale secondo tiro o spostamento verso i bagni...
/// ---> il contatore assume 0 quando non ci sono numeri doppi
/// ---> "  "         "      1 quando ci sono numeri doppi (la prima volta) - torna a zero al secondo tiro se non ci sono altri dadi doppi
/// ---> "  "         "      2 quando si verificano due tiri doppi (in questo caso il giocatore verrà spostato nella casella 10, bagni)
/// ---> nel caso in cui non vi siano dadi doppi e si passa dal via, il budget viene incrementato automaticamente
/// ---> nel caso in cui vi siano dadi doppi e si passa dal via, il budget viene incrementato dopo il secondo tiro

///parametri in ingresso: vettore di aule, intero per la dimensione del vettore giocatori
void assegnamento_casuale(Aula a[], int gioc);
///procedura per l'assegnamento casuale delle aule
/// ---> nella funzione viene gestito l'assegnamento casuale delle aule e delle scrivanie
/// ---> le aule da assegnare sul tabellone sono 24. all'interno di un ciclo for (0-23), tramite un ciclo do while, viene generato un indice
/// ---> il ciclo do-while termina non appena il numero generato rappresenti l'indice di un'aula acquistabile (tipo 0, proprietario -1)
/// ---> dato che in 1 aula su 3 è già presente una scrivania, all'inizio della procedura viene generato un numero pseudocasuale da 0 a 2
/// ---> questo numero rappresenta la condizione per l'assegnamento della scrivania gratuita:
/// ---> all'interno del ciclo for viene eseguito il modulo tra l'indice e 3: se il risultato è uguale al numero precedentemente genrato,
/// ---> all'aula in questione verrà assegnata una scrivania gratuitamente.

///parametri in ingresso: puntatore a intero per la modalità di gioco
void mod_gioco(int *m);
///procedura per la scelta della modalità di gioco
///0 assegnamento casuale, 1 modalità classica (acquisto aule al passaggio)

///parametri in ingresso: puntatore a carattere per la scelta dopo la stampa del menu
void stampaMenu(char *s);
///procedura per la stampa del menu principale

///parametri in ingresso: vettore giocatori, intero che indica l'indice del giocatore corrente
void valorigioc(Giocatore *g, int n);
///procedura per l'inserimento dei dati dei giocatori

///parametri in ingresso: i-esima cella del vettore giocatore e indice del giocatore attivo
void visualizzaBudget(Giocatore g, int i);
///procedura che stampa il budget degli utenti

///parametri in ingresso: puntatore a intero per il budget e indice del giocatore attivo
void via(int *g, int n);
///procedura per l'incremento del budget al superamento del via

///parametri in ingresso: puntatore a struttura di tipo partita
void vincitore(Partita *p);
///procedura per la proclamazione del vincitore

///parametri in ingresso: vettore di aule, indice dell'aula corrente, puntatore a struttura di tipo partita
void stampaDettaglioCasella(Aula a[], int k, Partita *p);
///procedura per la stampa dei dettagli di una casella

///parametri in ingresso: vettore di aule, vettore di giocatori, puntatore a struttura di tipo partita, vettore di sfighe, pntatore a intero (indice array sfighe), puntatore a intero (indice ultima aula acquistata), vettore di liste, dimensione vettore di sfighe, modalità di gioco
void controlloCasella(Aula *a, Giocatore *g, Partita *p, Sfiga *l, int *mazzo, int *ultimo, Lista **x, int t, int m);
///procedura per il controllo della casella (eventuali operazioni da eseguire in base al tipo)

///parametri in ingresso: vettore di aule, puntatore a intero per il budget del giocatore
void costruzioni(Aula *a, int *g);
///procedura che permette di costruire tende e scrivanie

///parametri in ingresso: vettore di sfighe, vettore di aule, dimensione del vettore di sfighe
int caricaPartita(Sfiga *l, Aula *a, int t);
///funzione per la gestione del caricamento della partita da file
///restituisce zero nel caso in cui l'allocazione dinamica non sia andata a buon fine

void pausa();
///procedura per "pausa"

///parametri in ingresso: vettore di aule, vettore di sfighe, dimensione del vettore di sfighe
int nuovaPartita(Aula *a, Sfiga *l, int t);
///procedure per l'iniizio di una nuova partita
///restituisce zero nel caso in cui la memoria non sia stata allocata dinamicamente

///parametri in ingresso: vettore di aule, vettore di giocatori, puntatore a struttura di tipo partita, puntatore a intero (ultimo terreno acquistato), modalità di gioco, vettore di liste
void aula(Aula *a, Giocatore *g, Partita *p, int *ultimo, int m, Lista **x);
///procedura che mostra le opzioni per l'aula

///parametri in ingresso: vettore di aule, vettore di giocatori, putatore a struttura di tipo partita
void tassa(Aula *a, Giocatore *g, Partita *p);
///procedura che mostra le opzioni per la casella di tipo tassa

///parametri in ingresso: vettore di aule, vettore giocatori, puntatore a struttura di tipo partita
void altro(Aula *a, Giocatore *g, Partita *p);
///procedura che esegue le opzioni per una casella di tipo altro

///parametri in ingresso: vettore di aule, vettore di giocatori, puntatore a struttura di tipo partita, vettore di sfighe, puntatore a intero (indice mazzo), puntatore a intero (ultimo terreno acquistato), vettore di liste, dimensione del vettore di sfighe, modalità di gioco
void sfiga(Aula *a, Giocatore *g, Partita *p, Sfiga *l, int *mazzo, int *ultimo, Lista **x, int t, int m);
///procedura che esegue le diverse operazioni casella di tipo sfiga a seconda del tipo

///parametri in ingresso: vettore di aule, puntatore a intero (budget giocatore)
void scrivania(Aula *a, int *g);
///procedura per la costruzione di una scrivania

///parametri in ingresso: vettore di aule, puntatore a interto (budget giocatore)
void tenda(Aula *a, int *g);
///procedura per la costruzione di una tenda

///parametri in ingresso: vettore di giocatori, puntatore a struttura di tipo partita
void gestioneBagno(Giocatore *g, Partita *p);
///procedura per la gestione dei turni mancanti per completare la pulizia dei bagni e tornare al gioco

///parametri in ingresso: puntatore a struttura di tipo partita, puntatore a carattere per la scelta (all'inizio del turno)
void gestioneScelta(Partita *p, char *s);
///procedura che effettua il controllo sulla scelta utente (dadi/salvataggio partita)

///parametri in ingresso: puntatore a intero per la posizione e puntatore a intero per i turni mancanti per la pulizia
void modificaBagno(int *p, int *t);
///procedura per la modifica del campo dei turni mancanti per rientrare in gioco e la posizione dell'utente

void errore();
///procedura per la stampa un messaggio d'errore
///nessun parametro in entrata, nessun parametro in uscita


///parametri in ingresso: puntatore a intero (budget del giocaotre), intero che rappresenta la tassa da pagare
void modificaBudget(int *b, int t);
///procedura per la modifica del budget
///parametri in ingresso: puntatore a intero (budget giocatore), intero (tassa), array giocatori
///NB: ---> la funzione esegue una semplice somma. Le sottrazioni vengono gestite ponendo davanti alla variabile che identifica la tassa
///il segno meno, durante il passaggio dei parametri nella chiamata della funzione. <---

///parametri in ingresso: riferimento alla testa della lista da eliminare
void eliminaLista(Lista *l);
///procedura ricorsiva per l'eliminazione di una lista

///parametri in ingresso: vettore di aule, modalità di gioco, puntatore a struttura di tipo partita, vettore di giocatori, vettore di liste
void uscita(Aula *a, int m, Partita *p, Giocatore *g, Lista **x);
///procedura per l'uscita di un giocatore e riassegnazione dei terreni posseduti

///parametri in ingresso: vettore di giocatori, puntatore a struttura di tipo partita
void giocatoriAncoraInGioco(Giocatore *g, Partita *p);
///procedura per la stampa dei dettagli dei giocatori ancora in gioco

///parametri in ingresso: puntatore a struttura di tipo partita, vettore di giocatori, vettore di aule, modalità di gioco, puntatore a intero(indice vettore sfighe), vettore sfighe, intero per ultimo terreno acquistato, vettore di liste, dimensione vettore di sfighe
int partita(Partita *p, Giocatore *g, Aula *a, int m, int *mazzo, Sfiga *l, int ultimo, Lista **x, int t);
///funzione per la gestione globale del turno
///restituisce zero nel caso in cui l'allocazione dinamica della memoria non sia andata a buon fine

///parametri in ingresso: vettore di giocatori, puntatore a struttura di tipo partita, vettore di aule, vettore di sfighe, puntatore a intero per l'indice del mazzo di sfighe, puntatore a intero per il controllo dei dadi doppi, puntatore a intero per l'ultimo terreno acquistato, vettore di liste, modalità di gioco, dimensione array di sfighe
void gioco(Giocatore *g, Partita *p, Aula *a, Sfiga *l, int *mazzo, int *c, int *ultimo, Lista **x, int t, int m);
///gestione del turno (tiro dei dadi, stampa dettaglio casella ecc)

int giocatori();
///funzione per l'inserimento del numero giocatori
///non riceve alcun parametro, restituisce un intero (il numero di giocatori)

///parametri in ingresso: puntatore a struttura di tipo partita, vettore di giocatori, modalità di gioco
void creazioneBudget_mod1(Partita *p, Giocatore *g, int m);
///procedura per la creazione del budget della mod 1 e stampa dello stesso per mod 1 e 0

int lancio_dadi();
///funzione per la generazione di un numero compreso tra 1 e 6
///restituisce un intero, non riceve alcun parametro

///parametri in ingresso: puntatore a intero per la dimensione del vettore di sfighe
Sfiga *caricaSfighe(int *t);
///funzione per il caricamento delle sfighe
///retituisce un puntatore di tipo sfiga (array delle sfighe)

///parametri in ingresso: vettore di sfighe, dimensione del vettore
void ordineCasuale(Sfiga *l, int t);
///procedura per il mescolamento delle sfighe
///NB: --> una volta lette da file le sfighe, vengono generati degli indici casuali 0-tot_sfighe
/// ---> l'elemento iesimo viene scambiato con quello contenuto nella cella che corrisponde al numero generato

///parametri in ingresso: vettore di liste, puntatore a struttura di tipo partita, vettore di aule, vettore di giocatori, intero per l'ultimo terreno acquistato, modalità di gioco
Lista **creazioneLista(Lista **x, Partita *p, Aula *a, Giocatore *g, int ultimo, int m);
///funzione per la creaione del vettore di liste e delle liste contenute nell'array
///restituisce un puntatore a puntatore (il primo elemento del vettore di liste)

///parametri in ingresso: vettore di aule
void azzeramento(Aula *a);
///procedura utilizzata per eliminare scrivanie e tende acquistate ed eventuali proprietari diversi dal banco

///parametri in ingresso: testa della lista (iesima cella del vettore di liste)
void stampaLista(Lista *x);
///procedura per la stampa delle i-esima lista contenuta nel vettore di liste

///parametri in ingresso: vettore di giocatori, puntatore a struttura di tipo partita
Giocatore *riallocazione_g(Giocatore *g, Partita *p);
///funzione per la riallocazione del vettore di giocatori, restituisce un puntatore al primo elemento del vettore di giocatori

///parametri in ingresso: puntatore a struttura di tipo partita, vettore di liste
Lista **riallocazione_l(Partita *p, Lista **x);
///funzione per la riallocazione del vettore di liste; restituisce il puntatore al primo elemento del vettore di liste

///parametri in ingresso: puntatore a struttura di tipo partita, vettore di giocatori, vettore di aule, modalità di gioco
int salvataggioPartita(Partita *p, Giocatore *g, Aula *a, int m);
///funzione per il salvataggio della partita
///restituisce zero nel caso in cui l'allocazione della memoria non sia andata a buon fine

///parametri in ingresso: vettore di liste, puntatore a struttura di tipo partita
void stampa(Lista **x, Partita *p);
///procedura per la la stampa dell'array di liste (questa procedura richiama all'interno di un ciclo la procedura stampalista)

///parametri in ingresso: puntatore a struttura di tipo partita, vettore di giocatori, vettore di liste
void libera(Partita *p, Giocatore *g, Lista **x);
///procedura utilizzata per liberare la memoria allocata all'inizio della partita/dopo il caricamento del salvataggio

///parametri in ingresso: puntatore a struttura di tipo partita, modalità di gioco
void stampaDettaglioSalvataggio(Partita *p, int m);
///procedura per la stampa dei dettagli di un salvataggio appena caricato (mod, numero turni, numero giocatori, giocatore attivo)

///parametri in ingresso: vettore di aule, puntatore a struttura di tipo partita
void stampaTabellone(Aula *a, Partita *p);
///procedura per la stampa del tabellone

void stampaIniziale();
///funzione per la stampa del logo del gioco e le informazioni del gioco

///parametri in ingresso: vettore di liste, puntatore a struttura di tipo partita
void inizializzazione(Lista **x,Partita *p);
///il vettore viene inizializzato (ad ogni cella, testa di ciascuna lista, viene assegnato il valore NULL)

///parametri in ingresso: puntatore a struttura di tipo partita, vettore di giocatori, vettore di aule, vettore di listem modalità di gioco
void stampaOpzioni(Partita *p, Giocatore *g, Aula *a, Lista **x, int m);
///procedura per la stampa delle opzioni aggiuntive: visualizzazione tabellone, aule dei giocatori, dettagli giocatori, salvataggio
