/*
 *	CERCA.C
 *	LaMoSca (LAboratorio di MOtori per SCAcchi)
 *	versione 0.10
 *	2001 Pietro Valocchi
 */


#include <stdio.h>
#include "Common.h"
#include "Extern.h"


/* OrdinaMosse() esegue l'ordinamento delle mosse sulla base dei valori 
   generati tramite MVV/LVA e History Heuristic*/
void OrdinaMosse(int Da) {
	int i, j, Max;
	Mossa Tmp;

	/* cerca il massimo */
	Max = Stack[Da].Valore;
	j = Da;
	for (i = (Da+1); i < InizioMosse[Livello]; i++) {
		if (Stack[i].Valore > Max) {
			Max = Stack[i].Valore;
			j = i;
		}
	}

	/* scambia */
	if (Max != Stack[Da].Valore) {
		Tmp.A = Stack[Da].A;
		Tmp.Da = Stack[Da].Da;
		Tmp.Valore = Stack[Da].Valore;
		Stack[Da].A = Stack[j].A;
		Stack[Da].Da = Stack[j].Da;
		Stack[Da].Valore = Stack[j].Valore;
		Stack[j].A = Tmp.A;
		Stack[j].Da = Tmp.Da;
		Stack[j].Valore = Tmp.Valore;

		if (LogFile) {
			fprintf(LogFile, "<S %c%d%c%d=%d-%c%d%c%d=%d> ", 
			CC(Tmp.Da), RD(Tmp.Da), CC(Tmp.A), RD(Tmp.A),
			Tmp.Valore,
			CC(Stack[Da].Da), RD(Stack[Da].Da), CC(Stack[Da].A), RD(Stack[Da].A),
			Stack[Da].Valore);
		}
	}
}


/* AlphaBeta() esegue la ricerca della mosa con valutazione migliore */
BOOL AlphaBeta(Mossa *M, int *ValorePosizione, int Alpha, int Beta) {

	int i;
	int ValoreCorrente = - INFINITO;
	BOOL Trovata = FALSE;

	/* attiva la generazione di sole catture per la quiescenza */
	if (Livello >= LivelloMassimo+1)
		CercaQuiescenza = TRUE;

	/* genera mosse */
	GeneraPseudo();

	/* verifica che il re non sia in una casa destinazione */
	for (i = InizioMosse[Livello-1]; i < InizioMosse[Livello]; i++) {
		if (B[Stack[i].A].Pezzo == RE) {
			/* la mossa del livello precedente ha lasciato il re in scacco */
			return FALSE;
		}
	}

	/* valuta la posizione se siamo al livello massimo o non ci sono altre mosse */
	if ((Livello == (MAXLIVELLI + 1)) || (InizioMosse[Livello-1] == InizioMosse[Livello])) {
		*ValorePosizione = Valuta();
		return TRUE;
	} else {

		/* prova tutte le mosse fino a trovare la migliore */
		if (LogFile) fprintf(LogFile, "\nL=%d, A=%d, B=%d: ", Livello, Alpha, Beta);
		*ValorePosizione = Alpha;

		for (i = InizioMosse[Livello-1]; ((i < InizioMosse[Livello]) && (ValoreCorrente < Beta)); i++) {
			OrdinaMosse(i);
			/* esegui la mossa */
			Muovi(&Stack[i]);
			if (AlphaBeta(M, &ValoreCorrente, -Beta, -Alpha)) {
				Trovata = TRUE;
				ValoreCorrente = - ValoreCorrente;
				if (ValoreCorrente > Alpha) {
					if (ValoreCorrente >= Beta) {
						/* cut */
						HistoryHeuristic[(int)Stack[i].Da][(int)Stack[i].A] += Livello;
						*ValorePosizione = Beta;
					} else {
						Alpha = ValoreCorrente;
						*ValorePosizione = Alpha;
						/* salva la mossa scelta se siamo alla root */
						if (Livello == 2) {
							M->A  = Stack[i].A;
							M->Da = Stack[i].Da;
						}
					}
				}
			}
			/* ripristina la mossa */
			TornaIndietro(&Stack[i]);

			/* torna alla generazione normale delle mosse */
			if (Livello <= LivelloMassimo)
				CercaQuiescenza = FALSE;

			if (LogFile) {
				fprintf(LogFile, "L%d%c%d%c%d=%d ", Livello, 
				CC(Stack[i].Da), RD(Stack[i].Da), CC(Stack[i].A), RD(Stack[i].A),
				ValoreCorrente);
				if (ValoreCorrente >= Beta) {
					fprintf(LogFile, "//Cut");
				}
			}
		}


		if (LogFile) {
			fprintf(LogFile, "\nRisultato L%d: T=%d", Livello, Trovata);
			if (Trovata == TRUE) {
				if (Livello == 1) {
					fprintf(LogFile, ", M=%c%d%c%d", CC(M->Da), RD(M->Da), CC(M->A), RD(M->A));
				}
			}
			fprintf(LogFile, ", A=%d, B=%d\n", Alpha, Beta);
		}
	}

	return Trovata;
}


