/*
 *	LaMoSca.C
 *	LAboratorio di MOtori per SCAcchi
 *	versione 0.10
 *	2001 Pietro Valocchi
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include "Common.h"


int   ChiMuove;				/* colore che deve muovere */
int   ColoreProgramma;		/* colore del computer */
BOOL  CercaQuiescenza;		/* flag ricerca quiescenza */
BOOL  ForceMode = FALSE;	/* programma sospeso */
FILE *LogFile = NULL;		/* id log file */
int   LivelloMassimo;		/* livello massimo della ricerca */


/* rappresentazione della scacchiera: vettore con due interi per ogni casa */
/* Colore = BIANCO, NERO o VUOTO */
/* Pezzo = PEDONE, CAVALLO, ALFIERE, TORRE, DONNA, RE o VUOTO */
Casa B[NUMEROCASE]; 


/* indice del livello di analisi */
int Livello;

/* stack delle mosse generate */
Mossa Stack[MAXMOSSE];

/* La lista delle mosse generate per la profondità n inizia con InizioMosse[n] 
   e finisce con InizioMosse[n + 1]. */
int InizioMosse[MAXLIVELLI + 2];

/* lettere dei pezzi bianchi per la VisualizzaScacchiera() 
   in accordo con i define in Common.h */
char SimboliPezziBianco[NUMEROPEZZI] = "PNBRQK";
/* lettere dei pezzi neri */
char SimboliPezziNero[NUMEROPEZZI] = "pnbrqk";

/* vettore per l'algoritmo Hystory Heuristic */
int HistoryHeuristic[NUMEROCASE][NUMEROCASE];


/* VisualizzaScacchiera() stampa la scacchiera */
void VisualizzaScacchiera() {
	int i;
	
	printf("\n8 ");
	for (i = 0; i < NUMEROCASE; i++) {
		switch (B[i].Colore) {
			case BIANCO:
				printf(" %c", SimboliPezziBianco[B[i].Pezzo]);
				break;
			case NERO:
				printf(" %c", SimboliPezziNero[B[i].Pezzo]);
				break;
			case VUOTO:
				printf(" .");
				break;
		}
		if ((i + 1) % CASEPERRIGA == 0 && i != (NUMEROCASE - 1))
			printf("\n%d ", 7 - RIGA(i));
	}
	printf("\n\n   a b c d e f g h\n\n");
}


/* Edit() riporta sulla scacchiera una posizione in formato .fen */
void Edit() {
	char sEd[512];
	int  Casa, i, ch;
	int  ColoreAttivo = BIANCO;

	sEd[0] = 0;
	/* il punto e' la fine dell'edit */
	while (sEd[0] != '.') {
		i = scanf("%s", sEd);
		switch (sEd[0]) {
			case '#' :
			/* il cancelleto pulisce */
				for (i = 0; i < NUMEROCASE; i++) {
					B[i].Pezzo = VUOTO;
					B[i].Colore = VUOTO;
				};
				break;
			case 'x' :
				/* togli pezzo dalla poszione */
				Casa = (sEd[1] - 'a') + CASEPERRIGA * (CASEPERRIGA - (sEd[2] - '0'));
				B[Casa].Pezzo = VUOTO;
       			B[Casa].Colore = VUOTO;
				break;
			case 'K' :
			case 'Q' :
			case 'R' :
			case 'B' :
			case 'N' :
			case 'P' :
				/* posiziona un pezzo */
				ch    = sEd[0];
				Casa  = (sEd[1] - 'a') + CASEPERRIGA * (CASEPERRIGA - (sEd[2] - '0'));
				B[Casa].Colore = ColoreAttivo;
				B[Casa].Pezzo = strchr(SimboliPezziBianco, ch) - SimboliPezziBianco;
				break;
			case 'c' :
				/* cambia colore attivo */
				ColoreAttivo ^= 1;
				break;
		}
	}
}

/* main() è un loop che alternativamente genera un mossa per il programma
   ed esegue un comando o una mossa dell'avversario */
int main(int argc, char **argv) {

	char  Command[256], Line[256];	/* input */
	BOOL  XBoardMode = FALSE;		/* input da WinBoard */

	if (argc > 1)
		if (!strcmp(argv[1],"log")) {
			LogFile=fopen("LaMoScaLogFile.txt","w");
			if (LogFile) fprintf(LogFile, "LaMoSca v0.10 Log File - Livelli=%d\n", LIVELLIANALISI);
			else printf("Impossibile aprire il file di log\n");
		}

	printf("\n");
	printf("LaMoSca (LAboratorio di MOtori per SCAcchi)\n");
	printf("versione 0.10\n");
	printf("\n");
	printf("\"help\" mostra la lista dei comandi principali.\n");
	printf("\n");

	Inizializza();
	InizioMosse[0] = 0;

	while (TRUE) {

		/* calcola eventualmente la mossa programma */
		if ((ChiMuove == ColoreProgramma) && (!ForceMode)) {
			/* pensa ad una mossa e a realizzarla */
			if(MossaProgramma(Line)) {
				if (XBoardMode == TRUE) {
					printf("move %s\n", Line);
					fflush(stdout);
				} else {
					VisualizzaScacchiera();
				}
				continue;
			} else {
				ColoreProgramma = VUOTO;
				printf("(no legal moves)\n");
				continue;
			}
		}

		/* prende l'input da consolle */
		if (XBoardMode == TRUE) {
			if (!fgets(Line, 256, stdin))
				return 0;
			if (Line[0] == '\n')
				continue;
			sscanf(Line, "%s", Command);
		} else {
			printf("LaMoSca> ");
			if (scanf("%s", Command) == EOF)
				return 0;
		}

		/* verifica se l'input puo' essere interpretato come
		   coordinate di una mossa dell'avversario */
		if (ChiMuove != ColoreProgramma) {
			if (MossaAvversario(Command)) {
				if (!XBoardMode) {
					VisualizzaScacchiera();
				}
				continue;
			}
		}

		/* scarta qualche comando non implementato */
		if (!strcmp(Command, "time"))
			continue;
		if (!strcmp(Command, "otim"))
			continue;
		if (!strcmp(Command, "computer"))
			continue;
		if (!strcmp(Command, "name"))
			continue;

		/* interpreta l'input */
		if (!strcmp(Command, "quit")) {
			printf("LaMoSca> Ciao!\n");
			if (LogFile)
				fclose(LogFile);
			return 0;
		}

		if (!strcmp(Command, "help")) {
			printf("new   - nuova partita\n");
			printf("go    - avvia programma\n");
			printf("force - ferma programma\n");
			printf("d     - visualizza la scacchiera\n");
			printf("quit  - esce\n");
			printf("mosse - inserisci le coordinate; es.: e2e4\n");
			continue;
		}

		if (!strcmp(Command, "xboard")) {
			XBoardMode = TRUE;
			signal (SIGINT, SIG_IGN);
			printf ("\n");
			continue;
		}

		if (!strcmp(Command, "new")) {
			Inizializza();
			if (XBoardMode == FALSE)
				VisualizzaScacchiera();
			continue;
		}

		if (!strcmp(Command, "edit")) {
			Edit();
			continue;
		}

		if (!strcmp(Command, "sd")) {
			scanf("%d", &LivelloMassimo);
			continue;
		}

		if (!strcmp(Command, "force")) {
			ForceMode = TRUE;
			continue;
		}

		if (!strcmp(Command, "white")) {
			ChiMuove = BIANCO;
			ColoreProgramma = NERO;
			continue;
		}

		if (!strcmp(Command, "black")) {
			ChiMuove = NERO;
			ColoreProgramma = BIANCO;
			continue;
		}

		if (!strcmp(Command, "d")) {
			VisualizzaScacchiera();
			continue;
		}

		if (!strcmp(Command, "go")) {
			ForceMode = FALSE;
			ColoreProgramma = ChiMuove;
			continue;
		}

		/* Comando non valido */
		if (XBoardMode == TRUE) {
			printf("Error (unknown command): %s\n", Command);
			fflush(stdout);
		} else {
			printf("LaMoSca> Comando non valido\n");
		}
	}
}
