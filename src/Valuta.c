/*
 *	VALUTA.C
 *	LaMoSca (LAboratorio di MOtori per SCAcchi)
 *	versione 0.10
 *	2001 Pietro Valocchi
 */


#include <stdio.h>
#include "Common.h"
#include "Extern.h"


/* Valore dei pezzi:
  100 per il pedone, 
  300 per il cavallo
  300 per l'alfiere
  500 per la torre
 1000 per la regina
10000 per il re 
    0 se e' vuoto */
int ValorePezzi[NUMEROPEZZI + 1] = {
	100, 300, 300, 500, 1000, 10000, 0
};

/* Valore della posizione */
/* Tabella per il pedone */
int SpostaAvanti[NUMEROCASE] = {
	  0,  0,  0,  0,  0,  0,  0,  0,
	  5, 10, 15, 20, 20, 15, 10,  5,
	  4,  8, 12, 16, 16, 12,  8,  4,
	  3,  6,  9, 12, 12,  9,  6,  3,
	  2,  4,  6,  8,  8,  6,  4,  2,
	  1,  2,  3,  4,  4,  3,  2,  1,
	  0,  0,  0, -4, -4,  0,  0,  0,
	  0,  0,  0,  0,  0,  0,  0,  0
};

/* Tabella per il cavallo e l'alfiere */
int AlCentro[NUMEROCASE] = {
	0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  5,  5,  5,  5,  0,  0,
	0,  0,  5, 10, 10,  5,  0,  0,
	0,  0,  5, 10, 10,  5,  0,  0,
	0,  0,  5,  5,  5,  5,  0,  0,
	0,  0,  0,  0,  0,  0,  0,  0,
	0, -5, -4,  0,  0, -4, -5,  0
};


/* Il vettore RuotaScacchiera è usato per ottenere il valore della 
   posizione dei pezzi neri */
int RuotaScacchiera[NUMEROCASE] = {
	 56,  57,  58,  59,  60,  61,  62,  63,
	 48,  49,  50,  51,  52,  53,  54,  55,
	 40,  41,  42,  43,  44,  45,  46,  47,
	 32,  33,  34,  35,  36,  37,  38,  39,
	 24,  25,  26,  27,  28,  29,  30,  31,
	 16,  17,  18,  19,  20,  21,  22,  23,
	  8,   9,  10,  11,  12,  13,  14,  15,
	  0,   1,   2,   3,   4,   5,   6,   7
};


/* Valuta() valuta una posizione sulla scacchiera */
int Valuta() {
	int i;
	int PunteggioBianco = 0;
	int PunteggioNero = 0;

	for (i = 0; i < NUMEROCASE; i++) {
		if (B[i].Colore == BIANCO) {
			/* valore del pezzo */
			PunteggioBianco += ValorePezzi[B[i].Pezzo];
			/* posizione del pezzo */
			switch (B[i].Pezzo) {
				case PEDONE:
					PunteggioBianco += SpostaAvanti[i];
					break;
				case CAVALLO:
					PunteggioBianco += AlCentro[i];
					break;
				case ALFIERE:
					PunteggioBianco += AlCentro[i];
					break;
			}
		} else {
			PunteggioNero += ValorePezzi[B[i].Pezzo];
			if (B[i].Colore != VUOTO) {
				switch (B[i].Pezzo) {
					case PEDONE:
						PunteggioNero += SpostaAvanti[RuotaScacchiera[i]];
						break;
					case CAVALLO:
						PunteggioNero += AlCentro[RuotaScacchiera[i]];
						break;
					case ALFIERE:
						PunteggioNero += AlCentro[RuotaScacchiera[i]];
						break;
				}
			}
		}
	}

	if (ChiMuove == BIANCO) {
		return PunteggioBianco - PunteggioNero;
	} else {
		return PunteggioNero - PunteggioBianco;
	}
}

