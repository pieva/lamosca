/*
 *	GENERA.C
 *	LaMoSca (LAboratorio di MOtori per SCAcchi)
 *	versione 0.10
 *	2001 Pietro Valocchi
 */


#include <stdio.h>
#include <string.h>
#include "Common.h"
#include "Extern.h"

/* memoria mosse eseguite */
Casa History[MAXLIVELLI+1];


/* posizione iniziale della scacchiera */
int ColoriIniziali[NUMEROCASE] = {
	NERO,   NERO,   NERO,   NERO,   NERO,   NERO,   NERO,   NERO,
	NERO,   NERO,   NERO,   NERO,   NERO,   NERO,   NERO,   NERO,
	VUOTO,  VUOTO,  VUOTO,  VUOTO,  VUOTO,  VUOTO,  VUOTO,  VUOTO,
	VUOTO,  VUOTO,  VUOTO,  VUOTO,  VUOTO,  VUOTO,  VUOTO,  VUOTO,
	VUOTO,  VUOTO,  VUOTO,  VUOTO,  VUOTO,  VUOTO,  VUOTO,  VUOTO,
	VUOTO,  VUOTO,  VUOTO,  VUOTO,  VUOTO,  VUOTO,  VUOTO,  VUOTO,
	BIANCO, BIANCO, BIANCO, BIANCO, BIANCO, BIANCO, BIANCO, BIANCO,
	BIANCO, BIANCO, BIANCO, BIANCO, BIANCO, BIANCO, BIANCO, BIANCO
};

int PezziIniziali[NUMEROCASE] = {
	TORRE,  CAVALLO, ALFIERE, REGINA, RE,     ALFIERE, CAVALLO, TORRE,
	PEDONE, PEDONE,  PEDONE,  PEDONE, PEDONE, PEDONE,  PEDONE,  PEDONE,
	VUOTO,  VUOTO,   VUOTO,   VUOTO,  VUOTO,  VUOTO,   VUOTO,   VUOTO,
	VUOTO,  VUOTO,   VUOTO,   VUOTO,  VUOTO,  VUOTO,   VUOTO,   VUOTO,
	VUOTO,  VUOTO,   VUOTO,   VUOTO,  VUOTO,  VUOTO,   VUOTO,   VUOTO,
	VUOTO,  VUOTO,   VUOTO,   VUOTO,  VUOTO,  VUOTO,   VUOTO,   VUOTO,
	PEDONE, PEDONE,  PEDONE,  PEDONE, PEDONE, PEDONE,  PEDONE,  PEDONE,
	TORRE,  CAVALLO, ALFIERE, REGINA, RE,     ALFIERE, CAVALLO, TORRE
};


/* Indica se un pezzo puo' muovere di piu' case alla volta
   0 PEDONE, 1 CAVALLO, 2 ALFIERE, 3 TORRE, 4 REGINA, 5 RE 
   Il pedone e' trattato separatamente */
BOOL Trascina[NUMEROPEZZI] = {
	FALSE, FALSE, TRUE, TRUE, TRUE, FALSE
};

/* Numero delle direzioni in cui un pezzo puo' muovere */
int NumeroDirezioni[NUMEROPEZZI] = {
	0, 8, 4, 4, 8, 8
};

/* Direzioni dei vari pezzi in una matrice 10*12 */
int OffsetPezzi[NUMEROPEZZI][CASEPERRIGA] = {
	{   0,   0,   0,  0, 0,  0,  0,  0 },
	{ -21, -19, -12, -8, 8, 12, 19, 21 },
	{ -11,  -9,   9, 11, 0,  0,  0,  0 },
	{ -10,  -1,   1, 10, 0,  0,  0,  0 },
	{ -11, -10,  -9, -1, 1,  9, 10, 11 },
	{ -11, -10,  -9, -1, 1,  9, 10, 11 }
};

/* Da12A8[] trasforma le coordinate della rappresentazione 12*12 
   in quelle della rappresentazione 8*8 */
int Da12A8[120] = {
	 -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	 -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	 -1,  0,  1,  2,  3,  4,  5,  6,  7, -1,
	 -1,  8,  9, 10, 11, 12, 13, 14, 15, -1,
	 -1, 16, 17, 18, 19, 20, 21, 22, 23, -1,
	 -1, 24, 25, 26, 27, 28, 29, 30, 31, -1,
	 -1, 32, 33, 34, 35, 36, 37, 38, 39, -1,
	 -1, 40, 41, 42, 43, 44, 45, 46, 47, -1,
	 -1, 48, 49, 50, 51, 52, 53, 54, 55, -1,
	 -1, 56, 57, 58, 59, 60, 61, 62, 63, -1,
	 -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	 -1, -1, -1, -1, -1, -1, -1, -1, -1, -1
};

/* Da8A12[] trasforma le coordinate della rappresentazione 8*8 
   in quelle della rappresentazione 12*12 */
int Da8A12[NUMEROCASE] = {
	21, 22, 23, 24, 25, 26, 27, 28,
	31, 32, 33, 34, 35, 36, 37, 38,
	41, 42, 43, 44, 45, 46, 47, 48,
	51, 52, 53, 54, 55, 56, 57, 58,
	61, 62, 63, 64, 65, 66, 67, 68,
	71, 72, 73, 74, 75, 76, 77, 78,
	81, 82, 83, 84, 85, 86, 87, 88,
	91, 92, 93, 94, 95, 96, 97, 98
};


/* SalvaMossa() inserisce una mossa nello stack */
void SalvaMossa(int Da, int A) {

	if (B[A].Colore != VUOTO) {
		Stack[InizioMosse[Livello]].Da = (char)Da;
		Stack[InizioMosse[Livello]].A  = (char)A;
		Stack[InizioMosse[Livello]].Valore = B[A].Pezzo * 10 - B[Da].Pezzo + HistoryHeuristic[Da][A];
		InizioMosse[Livello]++;
	} else if (CercaQuiescenza == FALSE) {
		Stack[InizioMosse[Livello]].Da = (char)Da;
		Stack[InizioMosse[Livello]].A  = (char)A;
		Stack[InizioMosse[Livello]].Valore = HistoryHeuristic[Da][A];
		InizioMosse[Livello]++;
	}
}


/* GeneraPseudo() genera mosse pseudolegali per la posizione corrente */
void GeneraPseudo() {
	int i, CasaDestinazione, Direzione;

	/* azzera il contatore delle mosse */
	InizioMosse[Livello] = InizioMosse[Livello-1];

	/* cerca tutte le case per pezzi del colore che deve muovere */
	for (i = 0; i < NUMEROCASE; i++) {
		if (B[i].Colore == ChiMuove) {
			if (B[i].Pezzo == PEDONE) {
				if (ChiMuove == BIANCO) {
					/* mosse per il pedone bianco */
					if (COLONNA(i) != 0 && B[i - 9].Colore == NERO)
						/* mangia in avanti a sinistra */
						SalvaMossa(i, i - 9);
					if (COLONNA(i) != 7 && B[i - 7].Colore == NERO)
						/* mangia in avanti a destra */
						SalvaMossa(i, i - 7);
					if (B[i - CASEPERRIGA].Colore == VUOTO) {
						/* avanti una casa */
						SalvaMossa(i, i - 8);
						if (i >= 48 && B[i - 16].Colore == VUOTO)
							/* avanti due case */
							SalvaMossa(i, i - 16);
					}
				}
				else {
					/* mosse per il pedone nero */
					if (COLONNA(i) != 0 && B[i + 7].Colore == BIANCO)
						SalvaMossa(i, i + 7);
					if (COLONNA(i) != 7 && B[i + 9].Colore == BIANCO)
						SalvaMossa(i, i + 9);
					if (B[i + CASEPERRIGA].Colore == VUOTO) {
						SalvaMossa(i, i + 8);
						if (i <= 15 && B[i + 16].Colore == VUOTO)
							SalvaMossa(i, i + 16);
					}
				}
			} else {
				/* per i pezzi differenti dal pedone */
				/* genera mosse per ciascuna delle direzioni legali */
				for (Direzione = 0; Direzione < NumeroDirezioni[B[i].Pezzo]; Direzione++) {
					/* "CasaDestinazione" == casa destinazione inizializzata con la corrente */
					CasaDestinazione = i;
					while (TRUE) {
						/* calcola la casa destinazione sommando l'offset
						per il pezzo corrente alla casa corrente.
						Si passa per la matrice 10*12 per verificare che
						la mossa non cada fuori dalla scacchiera */
						CasaDestinazione = Da12A8[Da8A12[CasaDestinazione] + 
							OffsetPezzi[B[i].Pezzo][Direzione]];
						/* mossa fuori dalla scacchiera */
						if (CasaDestinazione == -1)
							break;
						/* mossa su una casa con un pezzo avversario */
						if (B[CasaDestinazione].Colore != VUOTO) {
							if (B[CasaDestinazione].Colore != ChiMuove)
								SalvaMossa(i, CasaDestinazione);
							break;
						}
						/* mossa su una casa vuota */
						SalvaMossa(i, CasaDestinazione);
						/* se il pezzo puo' muovere di piu' case, continua */
						if (!Trascina[B[i].Pezzo])
							break;
					}
				}
			}
		}
	}
}


/* Inizializza() inizializza la scacchiera */
void Inizializza() {
	int i;

	for (i = 0; i < NUMEROCASE; i++) {
		B[i].Colore = ColoriIniziali[i];
		B[i].Pezzo  = PezziIniziali[i];
	}
	ChiMuove = BIANCO;
	ColoreProgramma = VUOTO;
	LivelloMassimo = LIVELLIANALISI;
}


/* Muovi() riporta una mossa sulla scacchiera */
void Muovi(Mossa *M) {

	/* salva informazioni sulla casa destinazione 
	per tornare eventualmente indietro */
	History[Livello-1].Colore = B[M->A].Colore;
	History[Livello-1].Pezzo  = B[M->A].Pezzo;

	/* esegui la mossa */
	B[M->A].Colore  = B[M->Da].Colore;
	B[M->A].Pezzo   = B[M->Da].Pezzo;
	B[M->Da].Colore = VUOTO;
	B[M->Da].Pezzo  = VUOTO;

	/* cambia colore */
	ChiMuove ^= 1;

	/* incrementa il contatore dei livelli */
	Livello++;

}


/* TornaIndietro() ripristina una mossa salvata */
void TornaIndietro(Mossa *M) {

	/* decrementa il contatore dei livelli */
	Livello--;

	/* cambia colore */
	ChiMuove ^= 1;

	/* ripristina la vecchia mossa */
	B[M->Da].Colore = B[M->A].Colore;
	B[M->Da].Pezzo  = B[M->A].Pezzo;
	B[M->A].Colore  = History[Livello-1].Colore;
	B[M->A].Pezzo   = History[Livello-1].Pezzo;

}


/* MossaAvversario() esegue una mossa dell'avversario */
BOOL MossaAvversario(char * Input) {
	BOOL Trovato = FALSE;
	int i, Da, A, ch;
	Mossa M;

	/* calcola dai primi due caratteri l'indice della casa di partenza */
	Da = (Input[0] - 'a') + CASEPERRIGA * (CASEPERRIGA - (Input[1] - '0'));
	/* calcola dai caratteri rimanenti l'indice della casa di destinazione */
	A  = (Input[2] - 'a') + CASEPERRIGA * (CASEPERRIGA - (Input[3] - '0'));

	/* genera lo stack delle mosse per l'avversario */
	CercaQuiescenza = FALSE;
	Livello = 1;
	GeneraPseudo();

	/* controlla che la mossa sia legale cercandola nello stack */
	for (i = InizioMosse[Livello-1]; i < InizioMosse[Livello]; i++) {
		if ((Stack[i].Da == Da) && (Stack[i].A == A)) {
			Muovi(&Stack[i]);
			Trovato = TRUE;

			/* verifica se si tratta di una promozione */
			if ((B[A].Pezzo == PEDONE) && ((RIGA(A) == 0) || (RIGA(A) == 7))) {
				ch = Input[4];
				B[Stack[i].A].Pezzo = strchr(SimboliPezziNero, ch) - SimboliPezziNero;
			}

			/* se è la prima mossa dell'avversario, fa partire il programma */
			if ((ForceMode == FALSE) && (ColoreProgramma == VUOTO))
				ColoreProgramma = ChiMuove;
			break;

		}
	}

	if (Trovato == FALSE) {
		if (ChiMuove == BIANCO) {
			/* controlla se si tratta di un arrocco */
			if (!strcmp(Input, "e1g1")) {
				M.Da = H1; M.A = F1;
				Muovi(&M);
				M.Da = E1; M.A = G1;
				Muovi(&M);
				Trovato = TRUE;
				/* considera come una sola mossa */
				ChiMuove ^= 1;
				Livello--;
			}
			if (!strcmp(Input, "e1c1")) {
				M.Da = A1; M.A = D1;
				Muovi(&M);
				M.Da = E1; M.A = C1;
				Muovi(&M);
				Trovato = TRUE;
				ChiMuove ^= 1;
				Livello--;
			}
		} else {
			if (!strcmp(Input, "e8g8")) {
				M.Da = H8; M.A = F8;
				Muovi(&M);
				M.Da = E8; M.A = G8;
				Muovi(&M);
				Trovato = TRUE;
				ChiMuove ^= 1;
				Livello--;
			}
			if (!strcmp(Input, "e8c8")) {
				M.Da = A8; M.A = D8;
				Muovi(&M);
				M.Da = E8; M.A = C8;
				Muovi(&M);
				Trovato = TRUE;
				ChiMuove ^= 1;
				Livello--;
			}
		}
	}

	return Trovato;
}


/* MossaProgramma() genera una mossa per il computer */
BOOL MossaProgramma(char *CoordinateMossa) {

	Mossa M;
	int Alpha = - INFINITO;
	int Beta  = INFINITO;
	int ValorePosizione;
	BOOL Trovata;

	Livello = 1;
	CercaQuiescenza = FALSE;
	memset(HistoryHeuristic, 0, sizeof(HistoryHeuristic));

	Trovata = AlphaBeta(&M, &ValorePosizione, Alpha, Beta);

	if (Trovata) {
		/* esegui la mossa */
		Muovi(&M);

		/* crea la stringa con le coordinate della mossa corrente */
		sprintf(CoordinateMossa, "%c%d%c%d", CC(M.Da), RD(M.Da), CC(M.A), RD(M.A));
	}


	return Trovata;
}


