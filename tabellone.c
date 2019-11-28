///file sorgente contenente sottoprogrammi per la gestione del tabellone
#include "funzioni.h"
void azzeramento(Aula *a)
{
    int i; //dichiarazione indice
    for(i=0;i<TABELLONE;i++) //ciclo for 0-39, caselle
    {
        if(!a[i].tipo && a[i].proprietario != BANCO) //se l'aula appartiene a qualche giocatore
        {
            a[i].f_scrivania=false; //si perdono le scrivanie
            a[i].f_tenda=false; //si perdono le tende
            a[i].proprietario=BANCO; //il nuovo proprietario è il banco
        }
    }
}
Sfiga *caricaSfighe(int *t)
{
    Sfiga *l=NULL; //dichiarazione e inizializzazione puntatore
    int a[N_SFIGHE],i; //dichiarazione di un vettore contenente il numero di sfighe di ogni tipo, variabile indice
    FILE *fp=NULL; //puntatore di tipo file inizializzato a null
    fp=fopen("ListaSfighe.txt", "r"); //apertuta del file di testo in modalità lettura
    if(fp==NULL) //controllo apertura file (se null, si esce dalla procedura)
        return NULL; //viene restituito il valore null al chiamante
    for(i=0; i<N_SFIGHE; i++) //ciclo for per memorizzare il numero di sfighe per ogni tipologia all'interno dell'array
    {
        fscanf(fp, "%d ", &a[i]); //lettura da file e scrittura su variabile
        *t+=a[i]; //incremento della dimensione del vettore di sfighe
    }
    l=(Sfiga*)malloc(sizeof(Sfiga)*(*t)); //allocazione dinamica del vettore di sfighe
    if(l==NULL) //controllo sull'allocazione
        return l; //se l'allocazione non è andata a buon fine si restituisce il valore NULL al chiamante
    for(i=0; i<a[TIPO1]; i++) //9, salvataggio delle 9 sfighe del primo tipo
    {                           ///NB i campi settati a zero sono quelli non previsti dalla tipologia della sfiga
        l[i].nuovaposizione=0; //non previsto nella sfiga di tipo 1
        l[i].p_naule=0; //non previsto nel tipo 1
        l[i].p_scrivanie=0; //non previsto
        l[i].p_tende=0; //non previsto
        l[i].tipologia=TIPO1; //inizializzazione tipologia
        fscanf(fp,"%d %[^\n]s", &l[i].importo, l[i].nome_s); //lettura da file del nome della sfiga
        fscanf(fp,"\n"); //carattere per andare a capo
    }
    for(i=a[TIPO1]; i<(a[TIPO1]+a[TIPO2]); i++) // 9 18, salvataggio delle sfighe del second tipo
    {
        l[i].nuovaposizione=0; //campo non previsto
        l[i].importo=0; //campo non previsto
        l[i].tipologia=TIPO2; //inizializzazione tipologia
        fscanf(fp,"%d %d %d %[^\n]s",&l[i].p_naule,&l[i].p_scrivanie,&l[i].p_tende,l[i].nome_s); //lettura da file della stringa della sfiga
        fscanf(fp,"\n"); //a capo
    }
    for(i=(a[TIPO1]+a[TIPO2]); i<(a[TIPO1]+a[TIPO2]+a[TIPO3]); i++) //18 29, salvataggio delle sfighe dell terzo tipo
    {
        l[i].nuovaposizione=0; //campo non previsto
        l[i].p_naule=0; //campo non previsto
        l[i].p_scrivanie=0; //campo non previsto
        l[i].p_tende=0; //campo non previsto
        l[i].importo=0; //campo non previsto
        l[i].tipologia=TIPO3; //assegnazione tipologia
        fscanf(fp,"%[^\n]s", l[i].nome_s); //lettura della stringa
        fscanf(fp,"\n"); //a capo
    }
    for(i=(a[TIPO1]+a[TIPO2]+a[TIPO3]); i<*t; i++) //29, salvataggio delle sfighe del quarto tipo
    {
        l[i].p_naule=0; //campo non previsto
        l[i].p_scrivanie=0; //campo non previsto
        l[i].p_tende=0; //campo non previsto
        l[i].importo=0; //campo non previsto
        l[i].tipologia=TIPO4; //assegnamento tipologia
        fscanf(fp,"%d %[^\n]s", &l[i].nuovaposizione, l[i].nome_s); //lettura stringa da file
        fscanf(fp,"\n"); //a capo
    }
    fclose(fp); //chiusura del file
    ordineCasuale(l,*t); //invocazione funzione per il mescolamento delle sfighe
                        //vengono passati i parametri t (totale sfighe) e il vettore di sfighe
    return l; //restituzione del puntatore al primo elemento del vettore delle sfighe
}
void ordineCasuale(Sfiga *l, int t)
{
    int i,a; //dichiarazione indici
    Sfiga c; //dichiarazione struttura di appoggio (1 cella del vettore sfighe)
    for(i=0; i<t; i++) //ciclo for 0-38
    {
        c=l[i]; //la struttura di appoggio assume il valore dell'iesima cella del vettore
        a=rand()%t, //viene generato un indice casuale compreso tra 0 e 38
        l[i]=l[a]; //l'aesimo elemento viene copiato nella cella con indice i
        l[a]=c; //l'elemento con indice a prende l'elemento con indice i contenuto nella variabile c
    }
}
void modificaBagno(int *p, int *t)
{
    *p=BAGNO; //modifica della posizione del giocatore
    *t=TURNI_B; //modifica dei turni mancanti al rientro in gioco
}
void gestioneBagno(Giocatore *g, Partita *p)
{
    printf("\n\tGiocatore %d --> %s, mancano %d turni...\n\n", p->g_attivo, g[p->g_attivo].nome_g, g[p->g_attivo].p_bagno);
    //vengono mostrati all'utente i turni rimanenti per la pulizia del bagno
    g[p->g_attivo].p_bagno--;   //diminuzione turni di pulizia del bagno
}
void assegnamento_casuale(Aula a[], int gioc)
{
    int t=0, appoggio=rand()%POSSIBILITA; //variabili di appoggio per l'assegnazione casuale delle aule
    int i=0, j=0, c=0; //dichiarazione e inizializzazione indici
    for(j=0; j<N_AULE; j++) //ciclo for da 0-23 (24/40 è il numero delle aule che devono essere assegnate)
    {
        c=0; //"azzeramento" variabile
        do{
            c=rand()%TABELLONE; //alla variabile c viene assegnato un valore casuale compreso tra 0 e 40
        }while(!a[c].costo|| a[c].proprietario!=BANCO);  //continua a ciclare sino a che il numero c non sia indice di un'aula che non abbia ancora un proprietario
        a[c].proprietario=i; //il proprietario dell'aula prende l'indice del giocatore in questione
        t%=POSSIBILITA;///la variabile t prende il valore del modulo tra se stessa e la costante possibilità (3)
                        ///t può assumere i valori 0,1,2
        if(t == appoggio) ///se il valore di t è uguale a quello generato all'inizio della procedura, all'aula verrà assegnata una scrivania gratis!
            a[c].f_scrivania=true;
        t++; //incremento della variabile di appoggio t
        i++; //incremento del giocatore
        i=i%gioc; //modifica del giocatore: i modulo numero giocatori
    }
}
void altro(Aula *a, Giocatore *g, Partita *p) //angoli del tabellone
{
    if(g[p->g_attivo].posizione == BATCAV) //se il giocatore si trova in Batcaverna
    {
        printf("\n\t ---> Non puoi entrare in Batcaverna! Vai a pulire i bagni <---\n\n"); //viene spedito a pulire i bagni!
        modificaBagno(&g[p->g_attivo].posizione, &g[p->g_attivo].p_bagno); //il campo che rappresenta i turni di pulizia viene modificato col valore 3 e la posizione col valore 10; vengono passati l'indirizzo della posizione del giocatore e il campo che rappresenta i turni di pulizia
        gestioneBagno(g,p); //vengono mostrati i turni mancanti
    }
    else //in caso contrario viene mostrato un semplice messaggio di output (casi bagni/corridoio)
        printf("\n\tNon succede nulla di particolare...\n\n");
}
void sfiga(Aula *a, Giocatore *g, Partita *p, Sfiga *l, int *mazzo, int *ultimo, Lista **x, int t, int m)
{
    Lista *tmp=x[p->g_attivo]; //dichirazione di tipo lista
    int aule,tende=0,scrivanie=0, tassa; //dichiarazioni delle variabili indice e appoggio, di cui tre inzializzate (in cui verranno registrati gli importi della tassa per il numero di oggetti in questione posseduti)
    aule=g[p->g_attivo].n_aule; //inizializzazione numero aule del giocatore
    if(l->tipologia==TIPO3) //se la tipologia è la 3
        printf("\n BATTUTE DI MARTINA:\n"); //si stampa un messaggio di output
    printf("\n ---> %s <---\n\n", l->nome_s);//stampa della stringa della sfiga
    switch(l->tipologia) //in base alla tipologia
    {
    case 0:
        printf("\tTASSA SEMPLICE: Devi pagare %d euro -->",l->importo); //stampa dell'importo
        modificaBudget(&g[p->g_attivo].soldi,-l->importo); //le sfighe del primo tipo modificano il budget (si tratta di una tassa); viene richiamata la funzione per la modifica del budget
        break;
    case 1: //le sfighe del secondo tipo modificano il budget con una tassa calcolata in base ai parametri previsti nella sfiga: numero aule / tende /scrivanie possedute
        while(tmp!=NULL) //se il proprietario dell'iesima casella è il giocatore attivo
        {
            if(a[tmp->info].f_scrivania) //se sono presenti scrivanie, la variabile di appoggio che ne rappresenta il numero viene incrementata
                scrivanie++;
            else if(a[tmp->info].f_tenda)//se sono presenti tende, la variabile di appoggio che ne rappresenta il numero viene incrementata
                tende++;
            tmp=tmp->next; //scorrimento della lista
        }
        if(l->p_naule && aule) //se il numero delle aule e la tassa da pagare per aula sono diversi da zero, si stampa il messaggio con il numero di aule e la tassa per ciascuna
            printf("\n\tAule possedute: %d -> tassa per aula: %d euro\n", aule,l->p_naule);
        if(l->p_scrivanie && a[g[p->g_attivo].posizione].f_scrivania)//se il numero delle scrivanie e la tassa da pagare per aula sono diversi da zero, si stampa il messaggio con il numero di scrivanie e la tassa per ciascuna
            printf("\n\tScrivanie possedute: %d -> tassa per aula: %d euro\n", scrivanie,l->p_scrivanie);
        if(l->p_tende && a[g[p->g_attivo].posizione].f_scrivania)//se il numero delle tende e la tassa da pagare per aula sono diversi da zero, si stampa il messaggio con il numero di tende e la tassa per ciascuna
            printf("\n\tTende possedute: %d -> tassa per aula: %d euro\n", tende,l->p_tende);
        tassa=(l->p_naule*aule)+(l->p_scrivanie*scrivanie)+(l->p_tende*tende); //calcolo della tassa in base ai parametri previsti dalla sfiga e il numero di possedimenti
        if(tassa) //se la tassa è maggiore di zero, il suo importo viene stampato
        {
            printf("\n\tTASSA IMMOBILI: Devi pagare %d euro -->", tassa); //stampa dell'importo della tassa
            modificaBudget(&g[p->g_attivo].soldi,-tassa); //il budget viene modificato
        }
        else //se la tassa è uguale a zero, significa che il giocatore non possiede nulla
            printf("\n\tTASSA IMMOBILI: Sei stato fortunato... non devi pagare nulla!\n");
        break;
    ///NB il caso 2 è stato saltato poiché si tratta esclusivamente della stampa della frase (già effettuata all'inizio)
    case 3: //le sfighe del terzo tipo prevedono lo spostamento in una nuova aula
        printf("\n\tGOTO: Giocatore %d, verrai spostato nella casella %d\n\n", p->g_attivo, l->nuovaposizione); //stampa della nuova posizione
        g[p->g_attivo].posizione=l->nuovaposizione; //modifica della posizione
        stampaDettaglioCasella(a,l->nuovaposizione,p); //invocazione della funzione per la stampa dei dettagli della casella
        controlloCasella(a,g,p,l,mazzo,ultimo,x,t,m); //invocazione della funzione per il controllo della casella
    }
}
void tassa(Aula *a, Giocatore *g, Partita *p) //gestione tasse
{
    //vengono mostrati dei messaggi di output che comunicano all'utente l'importo della tassa da pagare
    printf("\n\tPaga la tassa!\n");
    printf("\n\tTassa: %d -->", a[g[p->g_attivo].posizione].p_passaggio);
    modificaBudget(&g[p->g_attivo].soldi,-a[g[p->g_attivo].posizione].p_passaggio); //il budget del giocatore corrente viene decrementato (in base all'importo della tassa)
}
void aula(Aula *a, Giocatore *g, Partita *p, int *ultimo, int m, Lista **x) //gestione aule
{
    char r; //variabile di tipo carattere (risposta dell'utente)
    if(a[g[p->g_attivo].posizione].proprietario != BANCO && a[g[p->g_attivo].posizione].proprietario != p->g_attivo ) //se l'aula è posseduta da un giocatore diverso da quello attivo e diverso dal banco
    {
        if(g[a[g[p->g_attivo].posizione].proprietario].p_bagno && a[g[p->g_attivo].posizione].proprietario!=NA) //se il proprietario sta pulendo i bagni e l'aula appartiene ad un giocatore ancora in gioco
            printf("\n Giocatore %d --> %s, non riceverai nessun bonus.\n", a[g[p->g_attivo].posizione].proprietario, g[a[g[p->g_attivo].posizione].proprietario].nome_g); //viene mostrato un messaggio di output
        //viene mostrato il messaggio riguardo il pagamento del pedaggio e l'importo in base alle costruzioni presenti
        printf("\n\tPaga il pedaggio: euro ");
        //se nell'aula è presente una scrivania
        if(a[g[p->g_attivo].posizione].f_scrivania)
        {
            //viene mostrato l'importo della tassa
            printf("%d -->", a[g[p->g_attivo].posizione].pp_scrivania);
            //il budget del giocatore viene decrementato
            modificaBudget(&g[p->g_attivo].soldi,-(a[g[p->g_attivo].posizione].pp_scrivania*MOLTIPLICATORE_PEDAGGIO));
            //il budget del giocatore che possiede l'aula viene incrementato se non sta pulendo i bagni e se il giocatore è ancora in gioco
            if(!g[a[g[p->g_attivo].posizione].proprietario].p_bagno && a[g[p->g_attivo].posizione].proprietario!=NA)
            {
                printf("\t+%d euro, giocatore %d --> %s! -->", a[g[p->g_attivo].posizione].pp_scrivania, a[g[p->g_attivo].posizione].proprietario, g[a[g[p->g_attivo].posizione].proprietario].nome_g); //stampa del messaggio per l'incremento del budget
                modificaBudget(&g[a[g[p->g_attivo].posizione].proprietario].soldi,a[g[p->g_attivo].posizione].pp_scrivania); //invocazione della funzione per l'incremento del budget
            }
        }
        //se nell'aula è presente una tenda
        else if(a[g[p->g_attivo].posizione].f_tenda)
        {
            //viene mostrato l'importo della tassa
            printf("%d -->", a[g[p->g_attivo].posizione].pp_tenda);
            //il budget del giocatore attivo viene decrementato
            modificaBudget(&g[p->g_attivo].soldi,-(a[g[p->g_attivo].posizione].pp_tenda*MOLTIPLICATORE_PEDAGGIO));
            //il budget del giocatore che possiede l'aula viene incrementato se non sta pulendi i bagni
            if(!g[a[g[p->g_attivo].posizione].proprietario].p_bagno)
            {
                printf("\t+%d euro, giocatore %d --> %s! -->", a[g[p->g_attivo].posizione].pp_tenda, a[g[p->g_attivo].posizione].proprietario, g[a[g[p->g_attivo].posizione].proprietario].nome_g); //stampa di un messaggio di output per l'incremento del budget
                modificaBudget(&g[a[g[p->g_attivo].posizione].proprietario].soldi,a[g[p->g_attivo].posizione].pp_tenda); //invocazione della funzione e incremento del budget
            }
        }
        //se nell'aula non sono presenti né tenda né scrivania
        else
        {
            //viene mostrata la tassa
            printf("%d -->", a[g[p->g_attivo].posizione].p_passaggio);
            //viene decrementato il budget del giocatore attivo
            modificaBudget(&g[p->g_attivo].soldi,-(a[g[p->g_attivo].posizione].p_passaggio*MOLTIPLICATORE_PEDAGGIO));
            //viene incrementato il budget del giocatore che possiede l'aula se non sta pulendo i bagni
            if(!g[a[g[p->g_attivo].posizione].proprietario].p_bagno && a[g[p->g_attivo].posizione].proprietario!=NA)
            {
                printf("\t+%d euro, giocatore %d --> %s! -->", a[g[p->g_attivo].posizione].p_passaggio, a[g[p->g_attivo].posizione].proprietario, g[a[g[p->g_attivo].posizione].proprietario].nome_g); //stampa di un messaggio di output per l'incremento del budget
                modificaBudget(&g[a[g[p->g_attivo].posizione].proprietario].soldi,a[g[p->g_attivo].posizione].p_passaggio); //invocazione della funzione per l'incremento del budget
            }
        }
    }
    //se una delle due condizioni sopra non è verificata, si controlla il budget
    else
    {
        if(g[p->g_attivo].soldi < 0) //se il budget è negativo, si esce senza acquistare nulla
            printf("\n\tBudget negativo, non puoi fare acquisti.\n");
        else //altrimenti, se il budget è positivo e il proprietario è il banco
            if(a[g[p->g_attivo].posizione].proprietario == BANCO)
            {
                do{
                    //viene chiesto all'utente se acquistare o meno l'aula
                    printf("\n\tAcquistare l'aula? 's'/'S' per rispondere positivamente, 'n'/'N' per rispondere negativamente.\n\n Scelta: ");
                    scanf("%c", &r);
                    if(r!= 'n' && r!= 's' && r!= 'N' && r!='S' && strlen(&r)!=CARATTERE)
                    {
                        //se la digitazione è diversa dai casi previsti e la lunghezza della stringa inserita è diversa da 1
                        errore(); //invocazione della funzione per la stampa di un messaggio d'errore
                        getchar();
                    }
                }while(r!= 'n' && r!= 's' && r!= 'N' && r!='S' && strlen(&r)!=CARATTERE);
                //continua a ciclare sino a che non venga inserita una scelta valida
                if(r== 'S' || r=='s') //in caso di scelta positiva
                {
                    a[g[p->g_attivo].posizione].proprietario=p->g_attivo; //al campo proprietario dell'aula viene assegnato l'indice del giocatore
                    printf("\nL'aula e' tua! --> "); //viene mostrato il messaggio di output dell'avvenuto acquisto
                    modificaBudget(&g[p->g_attivo].soldi, - a[g[p->g_attivo].posizione].costo); //il budget del giocatore viene decrementato dopo l'acquisto
                    g[p->g_attivo].n_aule++; //il numero delle aule possedute viene incrementato
                    pausa(); //invocazione della funzione di pausa
                    *ultimo=g[p->g_attivo].posizione; ///la variabile ultimo rappresenta l'ultimo terreno acquistato
                    x=creazioneLista(x,p,a,g,*ultimo,m); //aggiornamento della lista
                    visualizzaBudget(g[p->g_attivo],p->g_attivo); //viene invocata la procedura budget alla quale viene passato la cella dell'array
                    //giocatori contenente le informazioni relative al giocatore corrente e il suo indice
                }
                else
                    getchar();
            }
            //se il proprietario è il giocatore attivo
            else
                if(a[g[p->g_attivo].posizione].proprietario == p->g_attivo)
                    costruzioni(&a[g[p->g_attivo].posizione],&g[p->g_attivo].soldi);
                    //la funzione gestisce la costruzione di tende e scrivania nell'aula corrente
                    //alla funzione vengono passati l'aula corrente e il budget
    }
}
void tenda(Aula *a, int *g)
{
    char r; //variabile di tipo carattere per la scelta dell'utente
    do{
        //viene chiesto all'utente se costruire una tenda
        printf("\n\tSi desidera costruire una Tenda? 's'/'S', 'n'/'N'\n\n Scelta: ");
        scanf("%c", &r);
        //se la scelta non è prevista, si mostra un messaggio di errore
        if(r!= 'n' && r!= 's' && r!= 'N' && r!='S' && strlen(&r)!=CARATTERE)
        {
            errore(); //invocazione della funzione per la stampa di un messaggio d'errore
            getchar();
        }
    }while(r!= 'n' && r!= 's' && r!= 'N' && r!='S' && strlen(&r)!=CARATTERE);  //continua a ciclare sino a che non viene inserito un carattere tra quelli previsti
    if(r=='s' || r=='S') //se la scelta è positiva
    {
        //viene mostrato il messaggio che conferma l'esito
        printf("\n\tTenda costruita --> ");
        a->f_scrivania=false; //il flag della scrivania viene settato "false"
        a->f_tenda=true; //il flag della tenda viene settato "true"
        modificaBudget(g,-TENDA); //il budget del giocatore viene decrementato
    }
}
void scrivania(Aula *a, int *g)
{
    char r; //variabie di tipo carattere per la risposta
    do{
        //viene chiesto all'utente se costruire o meno una scrivania
        printf("\n\tSi desidera costruire una scrivania? 's'/'S', 'n'/'N'\n\n Scelta: ");
        scanf("%c", &r);
        //se la scelta non è tra quelle previste, viene mostrato un messaggio di errore
        if(r!= 'n' && r!= 's' && r!= 'N' && r!='S' && strlen(&r)!=CARATTERE)
        {
            errore(); //invocazione per la funzione di stampa di un messaggio d'errore
            getchar();
        }
    }while(r!= 'n' && r!= 's' && r!= 'N' && r!='S' && strlen(&r)!=CARATTERE);  //continua a ciclare sino a che non viene inserita una delle scelte previste
    if(r=='s'|| r=='S') //se la risposta è positiva
    {
        //viene mostrato il messaggio che mostra l'esito della costruzione
        printf("\n\tScrivania costruita -->");
        //il flag della scrivania viene settato "true"
        a->f_scrivania=true;
        //il budget viene decrementato
        modificaBudget(g,-SCRIVANIA);
    }
}
void controlloCasella(Aula *a, Giocatore *g, Partita *p, Sfiga *l, int *mazzo, int *ultimo, Lista **x, int t, int m)
{
    switch(a[g[p->g_attivo].posizione].tipo) //in base alla tipologia dell'aula nella quale si trova il giocatore
    {
        case AULA:
            aula(a,g,p,ultimo,m,x); //se la tipologia è zero, viene invocata la procedura aula alla quale vengono passate le strutture aula, giocatore e partita e il puntaore all'ultima aula acquistata, il vettore di liste e la modalità di gioco
            break;
        case TASSA:
            tassa(a,g,p);//se la tipologia è uno, viene invocata la procedura tassa alla quale vengono passate le strutture aula, giocatore e partita
            break;
        case SFIGA:
            printf("\n\t%da Sfiga estratta dal mazzo\n",*mazzo+1); // stampa il numero della sfiga
            sfiga(a,g,p,&l[*mazzo],mazzo,ultimo,x,t,m);//se la tipologia è due, viene invocata la procedura sfiga
            (*mazzo)++; //incremento del valore presente nella cella puntata da mazzo
            *mazzo%=t; //modulo tra mazzo e il totale delle sfighe (0-38)
            break;
        case ALTRO:
            altro(a,g,p);//se la tipologia è tre, viene invocata la procedura altro alla quale vengono passate le strutture aula, giocatore e partita
            break;
    }
}
void costruzioni(Aula *a, int *g)
{
    if(!a->f_scrivania && !a->f_tenda) //nel caso in cui non fossero presenti né una scrivania né una tenda
    {
        scrivania(a,g); //viene invocata la procedura scrivania alla quale vengono passate l'aula e il budget giocatore
        getchar();
    }
    else if(!a->f_tenda) //nel caso in cui fosse presente una scrivania ma non una tenda
    {
        printf("\n ---> Possiedi una scrivania <---\n"); //viene mostrato un messaggio di output all'utente
        tenda(a,g); //viene invocata la procedura tenda alla quale viene passata l'aula e il budget giocatore
        getchar();
    }
    else
        printf("\n ---> Possiedi una tenda <---\n"); //viene mostrato un messaggio di output all'utente
}
void stampaDettaglioCasella(Aula a[], int k, Partita *p)
{
    printf("\n.____________________________________________.\n");
    //stampa del nome della casella
    printf("\n\tNome: %s\n", a[k].nome_a);
    printf(".____________________________________________.\n");
    //viene stampato un messaggio di output in base alla tipologia di aula
    printf("\n\tTipologia: ");
    switch(a[k].tipo)
    {
        case AULA: //tipologia zero, stampa aula
            printf("Aula\n");
            printf(".____________________________________________.\n");
            //stampa i dettagli dell'aula
            if(a[k].proprietario!=p->g_attivo)
            {
                if(!a[k].f_scrivania && !a[k].f_tenda) //se non sono presenti né tende, né scrivanie, si stampa l'importo del pedaggio semplice
                printf("\n\tPedaggio: %d euro\n", a[k].p_passaggio);
                if(a[k].f_scrivania) //se il flag scrivania è true
                {
                    printf("\n\tE' presente una scrivania\n"); //si stampa il messaggio di output che comunica la presenza di una scrivania
                    printf("\n\tPedaggio con scrivania: %d\n",a[k].pp_scrivania); //si stampa l'importo della tassa con scrivania
                }
                else //in caso contrario si comunica che non è presente una scrivania
                    printf("\n\tNon e' presente nessuna scrivania\n");
                if(a[k].f_tenda) //se il flag tenda è true
                {
                    printf("\n\tE' presente una tenda\n"); //si comunica all'utente che è presente una tenda
                    printf("\n\tPedaggio con tenda: %d\n",a[k].pp_tenda); //si stampa l'importo del pedaggio con la tenda
                }
                else //in caso contrario si comunica che non è presente nessuna tenda
                    printf("\n\tNon e' presente nessuna tenda\n");
                if(a[k].proprietario==BANCO)
                    printf("\n\tCosto: %d euro\n", a[k].costo); //viene mostrato in output il costo
            }
            break;
        case TASSA: //tipologia uno, stampa tassa
            printf("Tassa\n");
            printf(".____________________________________________.\n");
            break;
        case SFIGA: //tipologia due, stampa sfiga
            printf("Sfiga\n");
            printf(".____________________________________________.\n");
            break;
        case ALTRO: //tipologia tre, stampa altro
            printf("Altro\n");
            printf(".____________________________________________.\n");
    }
    if(a[k].proprietario == NA) //se il campo proprietario è -2, l'aula appartiene al banco (non è acquistabile dai giocatori)
        printf("\n\tNon acquistabile, banco\n");
    else
    {
        printf("\n\tProprietario: "); //viene mostrato il messaggio di output che mostra il proprietario
        if (a[k].proprietario == BANCO)  //se il campo proprietario è -2, l'aula appartiene al banco (viene mostrato un messaggio di output col nome del proprietario)
            printf(" Banco\n"); //altrimenti il nome del giocatore
        else
            printf(" giocatore %d\n", a[k].proprietario);
    }
    printf(".____________________________________________.\n");
}
void via(int *g, int n)
{
    printf("\n\tHai superato il via, +20 euro, giocatore %d! -->", n); //quando il giocatore supera il via, il suo budget viene incrementato di 20 euro
    modificaBudget(g,VIA); //incremento del budget di 20 euro
}
void uscita(Aula *a, int m, Partita *p, Giocatore *g, Lista **x)
{
    Lista *tmp=x[p->g_attivo]; //dichiarazione di un puntatore d'appoggio di tipo lista a cui assegnare la testa della lista dei terreni del giocatore
    printf("\n\tGiocatore %d --> %s, sei fuori. Bancarotta.\n", p->g_attivo, g[p->g_attivo].nome_g); //messaggio di output che comunica all'utente l'uscita dal gioco
    while(tmp!=NULL) //si scorre la lista
    {
        if(m==MOD1) //se la modalità è quella classica, i terreni persi non possono essere acquistati (si paga il pedaggio al banco)
            a[tmp->info].proprietario=NA; //il proprietario della casella viene identificato con una costante che indica che l'aula non è più acquistabile
        else //per la modalità due i terreni passano al banco e possono essere acquistati (perdono tende e scrivania)
        {
            a[tmp->info].proprietario=BANCO; //il nuovo proprietario è il banco e gli altri giocatori possono acquistare l'aula
            if(a[tmp->info].f_scrivania) //se esiste una scrivania
                a[tmp->info].f_scrivania=false; //la scrivania viene tolta
            if(a[tmp->info].f_tenda) //se esiste una tenda
                a[tmp->info].f_tenda=false; //la tenda viene tolta
        }
        tmp=tmp->next; //scorrimento della lista
    }
    if(p->g_attivo != p->n_giocatori) //si scambiano gli indici solo se il giocatore attivo è diverso da n giocatori-1
    {
        tmp=x[p->n_giocatori]; //assegnamento del riferimeto alla testa della lista dell'ultimo giocatore a un puntatore d'appoggio per la modifica degli indici
        while(tmp!=NULL) //scorrimento
        {
            a[tmp->info].proprietario=p->g_attivo; //cambio dell'indice
            tmp=tmp->next; //scorrimento della lista
        }
    }
}
void stampaTabellone(Aula *a, Partita *p)
{
    printf(" Stampa del tabellone\n\n"); //intestazione stampa tabellone
    pausa (); //invocazione della funzione di pausa
    int i=0; //dichiarazione e inizializzazione variabile indice
    for(i=0;i<TABELLONE;i++) //ciclo for 0-39
        stampaDettaglioCasella(a,i,p); //invocazione della funzione per la stampa del dettaglio di ciascuna casella
}
void stampaIniziale()
{ //procedura per la stampa iniziale: logo del gioco e informazioni
 printf("\n\n\tM-----------------------------------------------------------------------------------------------------C\n");
 printf("\t| MMM     MMM   MMMM   MM  MM   MMMM   MMMM    MMMM   MM     MM MM  CCCCCCCCC  C    C  CCCCC   CCCCCC |\n");
 printf("\t| MMMM   MMMM  M    M  MMM MM  M    M  M  MM  M    M  MM      MMM   CC         C    C  CC  CC  C      |\n");
 printf("\t| MMMMM MMMMM  M    M  M MMMM  M    M  MMMM   M    M  MM       M    CC         C    C  CCCCC   CCCCC  |\n");
 printf("\t| MM  MMM  MM  M    M  M  MMM  M    M  M      M    M  MM       M    CC         C    C  CC  CC  C      |\n");
 printf("\t| MM   M   MM   MMMM   M   MM   MMMM   M       MMMM   MMMMMM   M    CCCCCCCCC   CCCC   CCCCC   CCCCCC |\n");
 printf("\tM-----------------------------------------------------------------------------------------------------C\n\n");
 printf("\n\t\tBenvenuti in MonopolyCube! La vostra visita al PDS sta per cominciare...\n");
 pausa(); //invocazione della funzione di pausa
 printf("\n\tE' possibile iniziare una nuova partita oppure caricarne una gia' cominciata.\n");
 pausa(); //invocazione della funzione di pausa
 printf("\n\tAvete la possibilita' di scegliere tra due modalita' di gioco:\n\n");
 printf("\n\t 0) Assegnamento casuale delle aule;\n\t\tle aule del palazzo verranno assegnate casualmente ai giocatori all'inizio del gioco;\n\t\t\tNB: In questa modalita' verranno distribuite scrivanie gratuite ogni tre aule...\n");
 printf("\n\t 1) Acquisto delle aule al passaggio;\n\t\tle aule verranno acquistate per scelta dei giocatori appena si troveranno all'interno di una di esse;\n");
 pausa(); //invocazione della funzione di pausa
 printf("\n\tIl tabellone e' formato da quaranta caselle. Le caselle sono di quattro tipi:\n\n");
 printf("\n\t1) Aula;\n\t\til proprietario di un'aula potra' acquistare scrivanie (50 euro) o tende (100 euro)\n\t\t(solo nel caso in cui sia gia' presente una scrivania!),\n\t\tgli altri giocatori pagheranno il pedaggio al proprietario in base alle costruzioni.\n");
 printf("\n\t2) Tassa;\n\t\te' necessario pagare le tasse universitarie...\n");
 printf("\n\t3) Sfiga;\n");
 printf("\t\t31) Tassa semplice;\n\t\t32) Tassa immobili;\n\t\t\tsi calcola in base ai possedimenti del giocatore\n\t\t33) Battute di Martina;\n\t\t\tBrr!\n\t\t34) GoTo;\n\t\t\ti giocatori verranno spostati in altre caselle\n");
 printf("\n\t4) Altre caselle... rappresentano gli angoli del tabellone.\n\t\t\t\tCorridoio, Bagno, Via, BatCaverna <--- procedere in modo cauto...!\n");
 pausa(); //invocazione della funzione di pausa
 printf("\n\t\t...Buona fortuna!\n\n\n");
 printf(" ... P.S.: Ricordatevi che i bagni sono da pulire!\n");
 pausa(); //invocazione della funzione di pausa
}
