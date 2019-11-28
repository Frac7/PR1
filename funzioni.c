///file sorgente contenente funzioni varie per la gestione del gioco
#include "funzioni.h"

void ctrldadi(Giocatore *g, Partita *p, int *c)
{
    int x,y; //definizione delle variabili alle quali verranno assegnati i valori dei due dadi da lanciare
    x=lancio_dadi(); //invocazione della funzione per il lancio dei dadi e assegnamento del valore di ritorno alla variabile x
    y=lancio_dadi(); //invocazione della funzione per il lancio dei dadi e assegnamento del valore di ritorno alla variabile y
    printf("\n\t%d %d -> ",x,y); //stampa del messaggio di output che informa l'utente riguardo il lancio di dadi effettuato
    printf("%d\n\n", x+y); //stampa della somma dei due tiri
    if(x==y) //nel caso in cui i dadi siano doppi
        (*c)++; //la variabile contatore viene incrementata
    else
        *c=0; //se i dadi non sono doppi, la variabile assume il valore zero (NB: inizializzata col valore zero nel caso in cui i dadi siano doppi una sola volta o non lo siano affatto)
    g[p->g_attivo].posizione=g[p->g_attivo].posizione+x+y; //incremento posizione
    g[p->g_attivo].posizione%=TABELLONE; //modulo posizione, range 0-39
}
void stampaMenu(char *s)
{
    //stampa il menu delle opzioni all'avvio del programma
    do{
            printf("\n\n.____________________________________________.\n\n");
            printf(" Menu\n");
            printf("\n\t'n' Nuova partita\n"); //l'utente decide di cominciare una partita
            printf("\n\t'c' Carica partita\n"); //l'utente decide di caricare una partita
            printf("\n\t'u' Esci\n"); //l'utente decide di uscire
            printf("\n Scelta: ");
            scanf("%c", s);
            printf("\n.____________________________________________.\n\n");
            if(*s!='n' && *s!='c' && *s!= 'u' && strlen(s)!=CARATTERE) //se la digitazione non è quella prevista o se la dimensione della stringa inserita è diversa da uno
            {
                errore(); //viene invocata una procedura che mostra un messaggio di errore
                getchar();
            }
    }while(*s!='n' && *s!='c' && *s!= 'u' && strlen(s)!=CARATTERE); //cicla sino a che non venga inserita una scelta valida
}
void pausa()
{
    printf("\n.____________________________________________.");
    //si stampa un messaggio di output per separare la schermata precedente da quella successiva
    //per continuare, l'utente deve premere invio
    printf("\n\nPremere invio per continuare . . .");
    getchar();
    printf("\n.____________________________________________.\n\n");
}
int lancio_dadi()
{
    //generazione di un numero pseudo casuale compreso tra 1 e 6, restituito al chiamante
    return DADOMIN+rand()%(DADOMAX-DADOMIN+1);
}
int caricaPartita(Sfiga *l, Aula *a, int t)
{
    int ultimo=BANCO; //dichiarazione della variabile che rappresenta l'ultimo terreno acquistato e inizializzazione con un valore fuori dal range (banco, -1, utile per l'inizializzazione della lista: indica che la partita è stata caricata)
    int mazzo=0; //dichiarazione e inizializzazione dell'indice per la gestione delle sfighe
    Partita *p=NULL; //dichiarazione di un puntatore a struttura di tipo partita che conterrà i dati letti dal file (riguardo la partita)
    Giocatore *g=NULL; //puntatore a cui viene assegnato il valore simbolico NULL; riferimento alla prima cella dell'array riguardo le informazioni
    //dei giocatori (che verrà allocato dinamicamente con dimensione "p->n_giocaotori"
    int m=0; //variabile sulla quale verrà registrata la modalità di gioco presente sul file: 0, assegnamento casuale; 1, acquisto al passaggio
    char nome[STRINGA+1]; //stringa sulla quale verrà registrato il nome del file da utilizzare per la lettura
    FILE *fp=NULL; //puntatore di tipo FILE (null) che verrà utilizzato per aprire il file e leggerlo
    do
    {
        printf("\nDigita il nome del salvataggio: "); //viene richiesto all'utente di Digita il nome del file
        getchar();
        scanf("%[^\n]s", nome); //acquisizione del nome del file
        fp=fopen(nome,"rb"); //apertuta del file (il cui nome è registrato nella stringa "nome")
        if(fp==NULL) //se il puntatore contiene ancora il valore null, viene mostrato un messaggio d'errore
            errore(); //invocazione della funzione per la stampa del messaggio d'errore
    }
    while(fp==NULL);  //continua a ciclare sino a che non venga inserita una stringa valida
    printf("\n\n ---> Partita caricata!\n\n"); //messaggio di output che mostra l'esito del caricamento
    getchar();
    Lista **x=NULL; //dichiarazione vettore di liste (puntatore a puntatore di tipo lista)
    p=(Partita*)malloc(sizeof(Partita)); //allocazione dinamica della memoria per la struttura di tipo Partita
    if(p==NULL) //controllo sull'allocazione (se il controllo fallisce, si restituisce il controllo al chiamante)
    {
        libera(p,g,x); //viene liberata la memoria puntata da p, g, x
        return SALTA; //viene restituito il valore 0 al chiamante e la procedura termina
    }
    fread(p,sizeof(Partita),1,fp); //lettura della prima struttura del file e scrittura della stessa sulla variabile p
    g=(Giocatore*)malloc(sizeof(Giocatore)*p->n_giocatori); //allocazione dinamica dell'array (tipo Giocatore, dimensione "p.n_giocatori")
    if(g==NULL)//controllo sull'allocazione (se fallisce, la funzione termina)
    {
        libera(p,g,x); //viene liberata la memoria puntata da p, g, x
        return SALTA; //viene restituito il valore zero al chiamante
    }
    fread(g,sizeof(Giocatore),p->n_giocatori,fp); //lettura della seconda struttura del file e scrittura su array di tipo Giocatori
    fread(a,sizeof(Aula),TABELLONE,fp); //lettura da file e scrittura delle informazioni inerenti al tabellone
    pausa(); //invocazione della funzione per la pausa
    stampaTabellone(a,p); //invocazione della funzione per la stampa del tabellone; viene passato il puntatore al primo elemento del vettore aule
    pausa(); //invocazione della funzione pausa
    fread(&m,sizeof(int),1,fp); //lettura da file e scrittura della modalità di gioco su variabile intera
    fclose(fp); //chiusura del file
    x=(Lista**)malloc(sizeof(Lista*)*p->n_giocatori); //allocazione dinamica di un vettore di puntatori (vettore di liste, n_giocatori celle)
    if(x==NULL) //controllo sull'allocazione (se fallisce, il programma termina)
    {
        libera(p,g,x); //rilascio della memoria allocata e puntata da p, g, x
        return SALTA; //restituzione del valore zero
    }
    inizializzazione(x,p); //invocazione della funzione per l'inizializzazione della lista e passaggio dei parametri vettore i liste e puntatore a struttura di tipo partita
    x=creazioneLista(x,p,a,g,ultimo,m); //invocazione della funzione per la creazione della lista e assegnamento del valore di ritorno al puntatore al primo elemento del vettore di liste
    if(x==NULL) //se la funzione creazione lista restituisce il valore NULL per l'allocazione dinamica non riuscita
    {
        libera(p,g,x); //la memoria viene deallocata
        return SALTA; //viene restituito il valore 0
    }
    ultimo=TABELLONE; //inizializzazione della variabile che indica l'ultimo terreno acquistato con un valore fuori range
    giocatoriAncoraInGioco(g,p); //invocazione della funzione per la visualizzazione dei giocatori ancora in gioco, passaggio dei parametri vettore di giocatori e puntatore a struttura di tipo partita
    pausa(); //invocazione della funzione di pausa
    stampaDettaglioSalvataggio(p,m); //invocazione della funzione per la stampa della partita a cui vengono passati due parametri
    //un puntatore a struttura di tipo partita e una variabile intera che rappresenta la modalità di gioco
    pausa(); //invocazione della funzione di pausa
    partita(p,g,a,m,&mazzo,l,ultimo,x,t); //invocazione della funzione partita (gestione globale dei turni)
    //alla funzione vengono passati i puntatori di tipo partita, giocatore e aule (questi ultimi due rappresentano il primo elemento di ciascun array)
    //un puntatore a puntatore di tipo lita (vettore di liste), il vettore di booleani (puntatore di tipo bool)
    //un puntatore di tipo sfiga (primo elemeno dell'array di sfighe)
    //l'indirizzo della variabile intera che rappresenta la mod, l'indice dell'array di sfighe, l'ultimo terreno acquista
    //una variabile che rappresenta il totale di sfighe, t
    x=NULL; //inizializzazione puntatore
    g=NULL; //inizializzazione puntatore
    p=NULL; //inizializzazione puntatore
    return CON; //viene restituito il valore 1, continua

}
void stampaDettaglioSalvataggio(Partita *p, int m)
{
    printf("\nModalita' di gioco: %d\n\n",m); //stampa della mod di gioco
    printf("Numero giocatori: %d\n",p->n_giocatori); //stampa del numero di giocatori in partita
    printf("Turni giocati: %d\n", p->n_turni); //stampa dei turni giocati sino al salvataggio
    printf("Giocatore attivo: %d\n", p->g_attivo);  //stampa dell'indice del giocatore attivo
}
int giocatori()
{
    int n; //dichiarazione variabile  per l'inserimento del numero di giocatori
    do
    {
        //viene richiesta la digitazione del numero di giocatori
        printf("\nDigita il numero di giocatori: (MIN 2 - MAX 8) ");
        scanf("%d", &n);
        //se n non  è compreso tra 2 e 8 viene mostrato un errore
        if(n>MAX || n<MIN)
            errore(); //invocazione della funzione per la stampa di un messaggio d'errore
    }
    while(n>MAX || n<MIN);   //continua a ciclare sino a che non viene inserito un valore valido
    return n; //viene restituito il numero dei giocatori al chiamante
}
void valorigioc(Giocatore *g, int n)
{
    int i=0; //dichiarazione e inizializzazione indice
    for(i=0; i<n; i++) //ciclo 0 -> n_giocatori -1
    {
        //viene chiesto l'inserimento del nome
        do{
                printf("\n\tDigita il nome del giocatore %d: ", i);
                getchar();
                scanf("%[^\n]s", g[i].nome_g);
                if(strlen(g[i].nome_g)>STRINGA) //viene mostrato un messaggio di errore se la stringa inserita è maggiore di 23 caratteri
                    errore(); //invocazione della funzione per la stampa del messaggio d'errore
        }while(strlen(g[i].nome_g)>STRINGA);  //continua a ciclare sino a che non venga inserita una stringa che rispetti i paramentri
        //viene assegnato al giocatore iesimo l'iesimo segnaposto
        g[i].simbolo=i;
        //viene "azzerata" la posizione del giocatore
        g[i].posizione=0;
        //viene "azzerato" il numero di aule possedute
        g[i].n_aule=0;
        //vengono "azzerati" i turni mancanti alla fine della pulizia dei bagni
        g[i].p_bagno=0;
    }
}
void mod_gioco(int *m)
{
    do
    {
        printf("\n\n.____________________________________________.\n\n");
        //gestione della modalità di gioco: vengono mostrate le possibilit di scelta
        printf(" Modalita' di gioco\n");
        printf("\n\t0.Assegnamento casuale delle aule\n");
        printf("\n\t1.Acquisto aule al passaggio\n");
        printf("\n Scelta: ");
        scanf("%d", m);
        printf("\n.____________________________________________.\n\n");
        //se la scelta inserita non è prevista, viene mostrato un messaggio d'errore
        if(*m!=MOD1 && *m!=MOD2)
        {
            errore(); //invocazione della funzione per la stampa di un messaggio d'errore
            *m=getchar();
        }
    }
    while(*m!=MOD1 && *m!=MOD2);   //continua a ciclare sino a che non venga inserito un valore valido
}
int nuovaPartita(Aula *a, Sfiga *l, int t)
{
    int ultimo=TABELLONE; //dichiarazione della variabile che rappresenta l'ultimo terreno acquistato e inizializzazione con un valore fuori dal range, 40
    int mazzo=0; //dichiarazione e inizializzazione dell'indice per la gestione delle sfighe
    Partita *p=NULL; //dichiarazione di un puntatore a struttura di tipo partita che conterrà i dati letti dal file (riguardo la partita)
    Giocatore *g=NULL; //puntatore a cui viene assegnato il valore simbolico NULL; riferimento alla prima cella dell'array riguardo le informazioni
    //dei giocatori (che verrà allocato dinamicamente con dimensione "p->n_giocaotori"
    Lista **x=NULL; //dichiarazione puntatore a puntatore e inizializzazione a NULL
    p=(Partita*)malloc(sizeof(Partita)); //allocazione memoria per a struttura partita
    if(p==NULL) //si controlla che l'allocazione di memoria sia andata a buon fine: se il puntatore contiene il valore NULL, l'esecuzione viene interrotta
    {
        libera(p,g,x); //deallocazione della memoria
        return SALTA; //restituzione del valore zero al chiamante
    }
    int m=0; //modalità di gioco
    p->n_giocatori=giocatori(); //viene invocata la funzione e il valore di ritorno viene assegnato al campo n_giocatori della struttura che rappresenta la partita
    x=(Lista**)malloc(sizeof(Lista*)*p->n_giocatori); //allocazione dinamica della memoria
    if(x==NULL) //controllo sull'allocazione dinamica della memoria (nel caso in cui l'allocazione non sia andata a buon fine, si esce dal programma)
    {
        libera(p,g,x); //deallocazione della memoria
        return SALTA; //restituzione del valore zero al chiamante
    }
    inizializzazione(x,p); //invocazione della funzione per l'inizializzazione delle lista e passaggio dei parametri vettore di liste e puntatore a struttura di tipo partita
    g=(Giocatore*)malloc(sizeof(Giocatore)*p->n_giocatori); //allocazione dinamica della memoria: array di n_giocatori contenente le informazioni sugli utenti
    if(g==NULL) //si controlla che l'allocazione di memoria sia andata a buon fine: se il puntatore contiene il valore NULL, l'esecuzione viene interrotta
    {
        libera(p,g,x); //deallocazione della memoria
        return SALTA; //restituzione del controllo al chiamante
    }
    valorigioc(g, p->n_giocatori); //vengono inizializzati alcuni valori dei giocatori che sono uguali in entrambe le mod
    //vengono passati il numero di giocatori e il puntatore a struttura utenti che contiene le info dei giocatori
    mod_gioco(&m); //viene richiesta la modalità di gioco, passando il parametro che rappresenta la modalità
    getchar(); //il carattere presente nel buffer viene scartato
    switch(m) //a seconda della malità di gioco scelta dall'utente, verranno eseguite diverse operazioni
    {
    case 0:
        assegnamento_casuale(a, p->n_giocatori); //assegnamento casuale delle aule agli utenti
        //vengon passati tre parametri: array delle caselle, dimensione tabellone e numero di giocatori
        stampaTabellone(a,p);//stampa del tabellone tramite funzione; viene passato il vettore di caselle
        pausa(); //invocazione della funzione di pausa
        x=creazioneLista(x,p,a,g,ultimo,m); //creazione lista con i terrenti appena assegnati
        if(x==NULL)
        {
            libera(p,g,x); //deallocazione della memoria
            return SALTA; //restituzione del valore 0
        }
    ///NB: non è stata inserita l'istruzione "break" perché tra le due modalità cambia solo la presenza della funzione
    ///assegnamento casuale; le altre differenze sono gestite in funzioni a parte
    case 1:
        dati(g,a,p,x,m); //caricamento budget utenti; il valore restituito dalla funzione viene assegnato alla struttura utenti
        //alla funzione vengono passati il vettore utenti, il numero di giocatori, l'array delle caselle e la dimensione del tabellone
        pausa(); //invocazione della procedura di pausa
        p->n_turni=0; //azzramento turni
        p->g_attivo=0; //azzeramento giocatore attivo
        partita(p,g,a,m,&mazzo,l,ultimo,x,t); //invocazione della funzione partita (gestione globale dei turni)
        //alla funzione vengono passati i puntatori di tipo partita, giocatore e aule (questi ultimi due rappresentano il primo elemento di ciascun array)
        //un puntatore a puntatore di tipo lita (vettore di liste), il vettore di booleani (puntatore di tipo bool)
        //un puntatore di tipo sfiga (primo elemeno dell'array di sfighe)
        //l'indirizzo della variabile intera che rappresenta la mod, l'indice dell'array di sfighe, l'ultimo terreno acquista
        //una variabile che rappresenta il totale di sfighe, t
    }
    //dopo il precedente rilascio della memoria puntata:
    x=NULL; //inizializzazioni puntatori con valore NULL
    p=NULL;//inizializzazioni puntatori con valore NULL
    g=NULL;//inizializzazioni puntatori con valore NULL
    return CON; //restituzione del valore 1, continua
}
int salvataggioPartita(Partita *p, Giocatore *g, Aula *a, int m)
{
    FILE *fp=NULL; //puntatore a file settato a null
    fp=fopen("Salvataggio.sav", "wb"); //apertura del file binario in modalità lettura
    if(fp==NULL) //nel caso in cui l'apertura non fosse andata a buon fine, si esce dal programma
    {
        printf(" Partita non salvata\n"); //messaggio d'errore
        return SALTA; //restituzione del valore zero
    }
    fwrite(p,sizeof(Partita),1,fp); //si scrive il contenuto del file sulla struttura di tipo Partita
    fwrite(g, sizeof(Giocatore),p->n_giocatori,fp); //si scrive il contenuto del file sulla struttura di tipo Giocatore
    fwrite(a,sizeof(Aula),TABELLONE,fp); //si scrive il contenuto del file sulla struttura di tipo Aula
    fwrite(&m,sizeof(int),1,fp); //si scrive il contenuto del file sulla variabile m (modalità di gioco)
    fclose(fp); //chiusura del file
    printf("\nPartita salvata.\n"); //messaggio di output che conferma l'esito del salvataggio
    return CON; //restituzione del valore 1
}
void gestioneScelta(Partita *p, char *s)
{
    do{
            printf("\n\n.____________________________________________.\n\n");
            printf("\tTurno %d, giocatore %d", p->n_turni, p->g_attivo); //viene mostrato il numero del turno e il numero del giocatore attivo
            printf("\n Digita\n\n\t 'invio' per tirare i dadi\n\n\t 'u' per uscire senza salvare\n\n\t 'o' per le opzioni aggiuntive\n\n Scelta: "); //viene richiesto se tirare i dadi o salvare la partita
            scanf("%c", s); //invio per tirare i dadi, u per uscire, o per le opzioni aggiuntive
            printf("\n.____________________________________________.\n\n");
            if(*s != '\n' && *s != 'u' && *s!='o' && strlen(s)!=CARATTERE) //se il carattere inserito non è valido, viene stampato un messaggio d'errore
            {
                errore(); //invocazione della funzione per la stampa del messaggio d'errore
                getchar();
            }
    }while(*s != '\n' && *s != 'u' && *s!='o' && strlen(s)!=CARATTERE);//continua a ciclare sino a che non venga inserito un carattere tra quelli previsti
}
void gioco(Giocatore *g, Partita *p, Aula *a, Sfiga *l, int *mazzo, int *c, int *ultimo, Lista **x, int t, int m)
{
    *ultimo=TABELLONE; //inizializzazione della variabile che indica l'ultima aula acquistata con un valore fuori dal range (inizializzazione ad ogni turno, anche dopo il tiro doppio)
    printf("\n\n Posizione precedente: casella -> %d\n\t%s\n\n", g[p->g_attivo].posizione,a[g[p->g_attivo].posizione].nome_a); //stampa della posizione dell'utente
    int posizionePrecedente=g[p->g_attivo].posizione; //copia della posizione precedente su una variabile di appoggio (serve per il superamento del via)
    printf("\n Sto tirando i dadi...\n\n"); //messaggio di output per informare l'utente riguardo le operazione da effettuare
    pausa(); //invocazione della funzione di pausa
    ctrldadi(g,p,c); //invocazione della funzione "controllo dadi" il cui valore aggiorna la struttura giocatori
    //nella cella riguardante il giocatore attivo; alla funzione viene passata la cella dell'array
    //della struttura giocatore nella posizione riguardante il giocatore attivo
    if(*c != DOPPI3) //se il contatore assume un valore diverso da tre(zero, niente dadi doppi; uno, una volta sola dadi doppi, due, due volte dadi doppi, tre, tre volte dadi doppi)
    {
        visualizzaBudget(g[p->g_attivo],p->g_attivo); //viene invocata la procedura budget alla quale viene passato la cella dell'array
        //giocatori contenente le informazioni relative al giocatore corrente e il suo indice
        if(g[p->g_attivo].posizione) //posizione diversa da 0
            if(p->n_turni<p->n_giocatori || posizionePrecedente > g[p->g_attivo].posizione) //se il numero di turni è minore del numero giiocatori oppure la poszione precedente è maggiore di quella futura (la quale è diversa da zero)
                if(!(*c)) //se il valore del contatore è zero
                    via(&g[p->g_attivo].soldi,p->g_attivo); //si incrementa il budget
        printf("\n\tGiocatore %d --> %s, sei finito nella casella %d\n", p->g_attivo, g[p->g_attivo].nome_g, g[p->g_attivo].posizione); //viene comunicato al giocatore in quale casella si trova
        stampaDettaglioCasella(a,g[p->g_attivo].posizione,p); //vengono stampati i dettagli della casella
        //alla procedura vengono passati la struttura di tipo Aula, la posizione del giocatore e la struttura Giocatore
        controlloCasella(a,g,p,l,mazzo,ultimo,x,t,m); //viene effettuato il controllo della casella tramite la procedura
        //alla quale vengono passate la struttura Aula, la posizione, la struttura Giocatore, il numero di giocatori e la struttura della Partita
        if(*c && !g[p->g_attivo].p_bagno) //se il contatore vale uno (dadi doppi una volta) e il giocatore non sta pulendo i bagni
        {
            if(g[p->g_attivo].p_bagno) //se il giocatore ha effettuato un tiro doppio ma sta pulendo i bagni
                printf(" Non puoi effettuare il lancio.\n"); //viene mostrato un messaggio di output
            else
            {
                pausa(); //invocazione della funzione di pausa
                printf("\n\tRitira!\n"); //viene mostrato il messaggio di output per tirare di nuovo i dadi
                pausa(); //invocazioe della funzione di pausa
                gioco(g,p,a,l,mazzo,c,ultimo,x,t,m); //la funzione richiama se stessa
            }
        }
    }
    else //se il contatore vale tre (due tiri doppi)
    {
        printf("\nAncora dadi doppi... vai a pulire i bagni\n"); //non si esegue la mossa e il giocatore viene spedito a pulire i bagni
        pausa(); //invocazione della funzione di pausa
        modificaBagno(&g[p->g_attivo].posizione,&g[p->g_attivo].p_bagno); //invocazione della funzione modifica bagno per settare i valori posizione =10 e turni di pulizia =3
        //alla procedura vengono passati gli indirizzi delle due variabili
        gestioneBagno(g,p); //invocazione della funzione gestione bagno e passaggio del puntatore a struttura partita e del primo elemento dell'array giocatori
    }
}
void stampaOpzioni(Partita *p, Giocatore *g, Aula *a, Lista **x, int m)
{
    getchar();
    int i; //dichiarazizone di una variabile intera d'appoggio
    char s; //dichiarazione di una variabile carattere per la scelta dell'utente
    do
    {
        i=0; //inizializzazione variabile d'appoggio
        printf("\n\n.____________________________________________.\n\n");
        printf(" Digita\n\n\t's' per salvare\n\n\t'g' per visualizzare i dettagli dei giocatori\n\n\t't' per visualizzare il tabellone\n\n\t'a' per visualizzare le aule di ogni giocatore\n\n Scelta: ");
        scanf("%c", &s); //scelte previste: salvataggio, s - dettagli giocatori, g - visualizzazione tabellone, t - visualizzazione aule per ogni giocatore, a
        printf("\n.____________________________________________.\n\n");
        printf("\n\n");
        switch(s) //in base alla scelta dell'utente
        {
        case 'g':
            giocatoriAncoraInGioco(g,p); //invocazione della funzione per la visualizzazione dei giocatori ancora in gioco e passaggio dei parametri vettore giocatore e puntatore a struttura
            break;
        case 't':
            stampaTabellone(a,p); //invocazione della funzione per la stampa del tabellone e passaggio del vettore di aule e di un puntatore a struttura di tipo partia
            break;
        case 'a':
            stampa(x,p); //invocazione della funzione per la stampa delle aule di ciascun giocatore e passaggio del vettore di liste e di un puntatore a struttura di tipo partita
            break;
        case 's':
            salvataggioPartita(p,g,a,m); //viene invocata la procedura per il salvataggio della partita alla quale vengono passati
            //la struttura partita, la struttura giocatore, la struttura aula e la modalità di gioco
            break;
        default:
            errore(); //invocazione della funzione per la stampa di un messaggio d'errore
            getchar();
            i=CON; //la variabile i assume il valore 1, condizione del ciclo do-while
            break;
        }
    }
    while(i); //cicla sino a che la variabile i sia diversa da zero
    getchar();
    pausa(); //invocazione della funzione di pausa
    printf(" Rientro in gioco...\n\n"); //messaggio di output per informare all'utente la ripresa del gioco
    pausa(); //invocazione della funzione di pausa
}
int gestioneTurno(Partita *p, Giocatore *g, Aula *a, int m, Sfiga *l, int *mazzo, int *ultimo, Lista **x, int t)
{
    char s; //scelta utente
    int c=0; //contatore per i dadi doppi
    if(g[p->g_attivo].p_bagno) //si controlla che il giocatore stia pulendo i bagni
        gestioneBagno(g,p); //in caso positivo viene invocata la procedura gestioneBagno alla quale si passano la struttura giocatore e la struttura partita
    else //in caso contrario, vengono eseguite le operazioni riguardanti il turno del giocatore
    {
        gestioneScelta(p,&s); //viene invocata la funzione per la gestione della scelta dell'utente
        if(s=='o') //se il carattere digitato dall'utente è o, vengono stampate le opzioni aggiuntive
            stampaOpzioni(p,g,a,x,m);  //invoacazione della funzione per la stampa delle opzioni aggiuntive e passaggio del puntatore a struttura partita, vettore giocatori, aule e liste, modalità di gioco
        else if(s=='u') //se il carattere inserito dall'utente è 'u'
        {
            pausa(); //invocazione della funzione di pausa
            //per uscire dal gioco, il numero di giocatori viene alterato e viene restituito il valore 0 al chiamante
            p->n_giocatori=SUPERSTITE; //il numero di giocatori viene portato a uno per uscire dal ciclo (condizione in procedura "partita)
            return SALTA; //si restituisce zero (letterale che indica "salta il turno")
        }
        gioco(g,p,a,l,mazzo,&c,ultimo,x,t,m); //viene invocata la procedura del gioco
        //alla quale vengono passati la struttura giocatore, partita e aula
    }
    pausa(); //invocazione della procedura per la pausa
    return CON; //viene restituito il valore 1, continua
}
void errore()
{
    printf("\n\tDigitazione errata.\n"); //messaggio di errore per inserimenti non previsti
}
int partita(Partita *p, Giocatore *g, Aula *a, int m, int *mazzo, Sfiga *l, int ultimo, Lista **x, int t)
{
    while(p->n_giocatori > SUPERSTITE) //cicla fino a che il numero dei giocatori è maggiore di 1
    {
        if(g[p->g_attivo].soldi<0) //se il budget è negativo
        {
            p->n_giocatori--; //diminuzione del numero di giocatori
            uscita(a,m,p,g,x); //il giocatore dichiara lo stato di bancarotta
            x=riallocazione_l(p,x); //assegnazione del valore di ritorno al puntatore x (nuovo indirizzo del vettore di liste)
            //parametri passati: puntatore a struttura di tipo partita, puntatore al primo elemento del vettore di liste
            if(x==NULL)
            {
                libera(p,g,x); //deallocazione della memoria
                return SALTA; //restituzione del controllo al chiamant
            }
            g=riallocazione_g(g,p);//assegnazione del valore di ritorno al puntatore g (nuovo indirizzo del vettore di giocatori)
            //parametri passati: puntatore a struttura di tipo partita, puntatore al primo elemento del vettore di giocatori
            if(g==NULL)
            {
                libera(p,g,x); //rilascio della memoria
                return SALTA; //restituzione del controllo al chiamante
            }
            if(p->n_giocatori == SUPERSTITE) //se il numero di giocatori è 1
                vincitore(p); //si dichiara il vincitore
            pausa(); //invocazione della funzione di pausa
            giocatoriAncoraInGioco(g,p); //vengono stampati i giocatori ancora in gioco e i terreni posseduti
            pausa(); //invocazione della funzione di pausa
            stampa(x,p); //invocazione funzione di stampa per il vettore di liste
            pausa(); //invocazione della funzione di pausa
        }
        else
            gestioneTurno(p,g,a,m,l,mazzo,&ultimo,x,t);//funzione per gestione del turno; alla funzione vengono passati i puntatori alle tre strutture Partita, Aula, Giocatore e la modalità di gioco
        p->g_attivo++; //incremento del numero giocatore
        p->g_attivo%=p->n_giocatori;  //viene effettuato il modulo del giocatore attivo e numero giocatori (dopo il precedente incremento)
        p->n_turni++; //incremento del numero turni
        int i;
        if(!p->g_attivo)
        {
            for(i=0;i<p->n_giocatori;i++)
                visualizzaBudget(g[i],i);
            pausa();
        }
    }
    libera(p,g,x); //invocazione della funzione che libera la memoria allocata in precedenza, al termine della partita (dealloca la memoria puntata da p, g e x)
    return CON; //restituzione valore 1
}
void vincitore (Partita *p)
{
    pausa(); //invocazione della funzione di pausa
    int i=0; //dichiarazione variabile indice
    i=p->g_attivo; //alla variabile indice viene assegnato l'indice del giocatore attivo
    i++; //incremento dell'indice
    i%=p->n_giocatori; //modulo tra indice e numero di giocatori
    printf("\n\tVince il giocatore %d\n", i); //proclamazione del vincitore
}
void giocatoriAncoraInGioco(Giocatore *g, Partita *p)
{
    //vengono stampati i dettagli dei gioatori ancora in gioco
    int i=0; //dichiarazione variabile indice
    if(p->n_giocatori!=SUPERSTITE) //se il numero di giocatori è diverso da 1
        printf(" Giocatori ancora in gioco:\n\n");//viene stampata l'intestazione
    for(i=0; i<p->n_giocatori; i++) //ciclo for 0 - n_giocatori-1
    {
        printf("Nome: %s\n", g[i].nome_g); //stampa del nome del giocatore
        printf("Budget: %d euro\n", g[i].soldi); //stampa del budget
        printf("Aule possedute: %d\n", g[i].n_aule); //stampa del numero di aule possedute
        printf("Id giocatore: %d\n", i); //stampa del nuovo indice del giocatore
        printf("Segnaposto: "); //stampa del segnaposto in base al campo simbolo (enumerazione)
        switch(g[i].simbolo)
        {
        case MORGAGNI:
            printf("Busto del Morgagni");
            break;
        case CANCELLINO:
            printf("Cancellino");
            break;
        case PRESA:
            printf("Presa elettrica");
            break;
        case PROIETTORE:
            printf("Proiettore");
            break;
        case GESSETTO:
            printf("Gessetto");
            break;
        case PORTA:
            printf("Porta");
            break;
        case TELECOMANDO:
            printf("Telecomando");
            break;
        case TERMOSIFONE:
            printf("Termosifone");
            break;
        }
        printf("\nPosizione: Casella -> %d\n", g[i].posizione); //stampa della posizione
        printf("Turni di pulizia dei bagni: %d\n\n", g[i].p_bagno); //stampa dei turni di pulizia
    }
}
void libera(Partita *p, Giocatore *g, Lista **x)
{
    eliminaLista(x[p->g_attivo]); //eliminazione dell'ultima lista rimanente
    free(x); //viene liberata la cella del vettore
    free(p); //viene rilasciata la memoria occupata dal puntatore alla struttura partita
    free(g); //viene rilasciata la memoria occupata dal vettore di giocatori
}
