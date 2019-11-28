///file sorgente contenente il programma principale
#include "funzioni.h"

int main()
{
    stampaIniziale(); //invocazione della procedura per la stampa delle informazioni del gioco
    srand(time(NULL)); //inizializzazione seed
    char scelta='a'; //scelta utente, inizializzazione con un valore diverso da u (uscita)
    Aula a[TABELLONE]=
    {
        {0,     0,      0,      0,      "VIA!",                 3,      0,      0,      -1},
        {60,    3,      6,      12,     "AULA G",               0,      0,      0,      -1},
        {0,     0,      0,      0,      "SFIGA!",               2,      0,      0,      -1},
        {60,    3,      6,      12,     "AULA C",               0,      0,      0,      -1},
        {80,    4,      8,      16,     "AULA F",               0,      0,      0,      -1},
        {0,     100,    0,      0,      "TASSE!",               1,      0,      0,      -1},
        {100,   5,      10,     20,     "AULA STUDIO",          0,      0,      0,      -1},
        {0,     0,      0,      0,      "SFIGA",                2,      0,      0,      -1},
        {100,   5,      10,     20,     "SIMAZ",                0,      0,      0,      -1},
        {120,   6,      12,     24,     "LAB T",                0,      0,      0,      -1},
        {0,     0,      0,      0,      "BAGNI",                3,      0,      0,      -1},
        {140,   7,      14,     28,     "VAX",                  0,      0,      0,      -1},
        {0,     0,      0,      0,      "SFIGA!",               2,      0,      0,      -1},
        {140,   7,      14,     28,     "G.A.R.",               0,      0,      0,      -1},
        {160,   8,      16,     32,     "BIBLIOTECA",           0,      0,      0,      -1},
        {0,     150,    0,      0,      "TASSE!",               1,      0,      0,      -1},
        {180,   9,      18,     36,     "AULA B",               0,      0,      0,      -1},
        {0,     0,      0,      0,      "SFIGA!",               2,      0,      0,      -1},
        {180,   9,      18,     36,     "SEGRETERIA",           0,      0,      0,      -1},
        {200,   10,     20,     40,     "AULA A",               0,      0,      0,      -1},
        {0,     0,      0,      0,      "CORRIDOIO",            3,      0,      0,      -1},
        {220,   11,     22,     44,     "UFFICIO SCATENI",      0,      0,      0,      -1},
        {0,     0,      0,      0,      "SFIGA!",               2,      0,      0,      -1},
        {220,   11,     22,     44,     "BAGNO PROFESSORI",     0,      0,      0,      -1},
        {240,   12,     24,     48,     "AULA D",               0,      0,      0,      -1},
        {0,     250,    0,      0,      "TASSE!",               1,      0,      0,      -1},
        {260,   13,     26,     52,     "LAB 3",                0,      0,      0,      -1},
        {0,     0,      0,      0,      "SFIGA!",               2,      0,      0,      -1},
        {260,   13,     26,     52,     "LAB 4",                0,      0,      0,      -1},
        {280,   14,     28,     56,     "LAB M",                0,      0,      0,      -1},
        {0,     0,      0,      0,      "BATCAVERNA!",          3,      0,      0,      -1},
        {300,   15,     30,     60,     "AULA M FISICA",        0,      0,      0,      -1},
        {0,     0,      0,      0,      "SFIGA!",               2,      0,      0,      -1},
        {300,   15,     30,     60,     "AULA M CHIMICA",       0,      0,      0,      -1},
        {320,   16,     32,     64,     "AULA M MATEMATICA",    0,      0,      0,      -1},
        {0,     350,    0,      0,      "TASSE!",               1,      0,      0,      -1},
        {360,   18,     36,     72,     "BAR ROTONDO",          0,      0,      0,      -1},
        {0,     0,      0,      0,      "SFIGA!",               2,      0,      0,      -1},
        {360,   18,     36,     72,     "AULA ANATOMICA",       0,      0,      0,      -1},
        {400,   20,     40,     80,     "AULA COSTA",           0,      0,      0,      -1}
    }; //array contenente le informazioni sulle caselle del tabellone
    Sfiga *l=NULL; //dichiarazione e inizializzazione puntatore di tipo sfiga
    int t=0; //dichiarazione e inizializzazione di una variabile intera a cui verrà assegnato il totale delle sfighe (dimensione array)
    l=caricaSfighe(&t); //assegnamento al puntatore l del valore restituito dalla funzione invocata alla quale è stato passato l'indirizzo del parametro intero t
    if(l==NULL) //se l'allocazione dinamica della memoria non è andata a buon fine (è stato restituito il valore NULL dalla funzione carica sfighe)
        scelta='u'; //la variabile scelta viene settata in modo da non poter entrare nel ciclo e uscire dal gioco
    while(scelta != 'u')
    {
        stampaMenu(&scelta); //stampa le opzioni all'avvio del gioco; viene passato il parametro scelta
        switch(scelta) //a seconda del valore inserito dall'utente, verrà effettuata una diversa operazione
        {
            case 'u':
                getchar();
                printf("\n\nUscita\n"); //digitando 'u', l'utente decide di uscire; verrà mostrato un output il messaggio "uscita"
                break;
            case 'n':
                nuovaPartita(a,l,t); //viene invocata la procedura alla quale viene passato il vettore che rappresenta il tabellone
                //la procedura contiente altre funzioni e procedure inerenti alla scelta dell'utente (l'utente sceglie di avviare una nuova partita)
                azzeramento(a); //invocazione della procedura per il reset del tabellone a cui viene passato il vettore di aule
                break;
            case 'c':
                caricaPartita(l,a,t); //viene invocata la procedura alla quale viene passato il vettore che rappresenta il tabellone
                //la procedura contiene le istruzioni che riguardano il cariamento di una partita già avviata
                azzeramento(a); //invocazione della procedura per il reset del tabellone a cui viene passato il vettore di aule
                break;
        }
    }
    //cicla sino a che la scelta dell'utente è diversa da 'u'
    free(l); //la memoria puntata da l viene rilasciata
    pausa();//invocazione della funzione di pausa
    return 0;
}
