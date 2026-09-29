/*
 *	EXTERN.H
 *	LaMoSca (LAboratorio di MOtori per SCAcchi)
 *	versione 0.10
 *	2001 Pietro Valocchi
 */


/* la scacchiera */

extern Casa B[NUMEROCASE];


/* variabili per le mosse */
extern int  ChiMuove;
extern int  ColoreProgramma;
extern BOOL ForceMode;
extern BOOL CercaQuiescenza;
extern Mossa Stack[MAXMOSSE];
extern int InizioMosse[MAXLIVELLI+2];
extern int Livello;
extern char SimboliPezziNero[NUMEROPEZZI];
extern FILE *LogFile;
extern int HistoryHeuristic[NUMEROCASE][NUMEROCASE];
extern int LivelloMassimo;
