/*
 *	COMMON.H
 *	LaMoSca (LAboratorio di MOtori per SCAcchi)
 *	versione 0.10
 *	2001 Pietro Valocchi
 */

/* dimensionamenti */

#define LIVELLIANALISI	4
#define MAXLIVELLI		10
#define NUMEROCASE		64
#define CASEPERRIGA		8
#define NUMEROPEZZI		6
#define MAXMOSSE		1024
#define INFINITO		0x7FFFFFFF


/* contenuto case */

#define BIANCO			0
#define NERO			1

#define PEDONE			0
#define CAVALLO			1
#define ALFIERE			2
#define TORRE			3
#define REGINA			4
#define RE				5

#define VUOTO			6

/* case */
#define A1				56
#define B1				57
#define C1				58
#define D1				59
#define E1				60
#define F1				61
#define G1				62
#define H1				63
#define A8				0
#define B8				1
#define C8				2
#define D8				3
#define E8				4
#define F8				5
#define G8				6
#define H8				7


/* elemento dello stack delle mosse per una determinata posizione */

typedef struct {
	int Da;
	int A;
	int Valore;
} Mossa;


/* descrizione di una casa della scacchiera */

typedef struct {
	int Colore;
	int Pezzo;
} Casa;


/* macro */

#define RIGA(x)			(x >> 3)
#define COLONNA(x)		(x & 7)
#define CC(x)			(COLONNA(x) + 'a')
#define RD(x)			(8 - RIGA(x))

/* generali */

#define BOOL			int
#define TRUE			1
#define FALSE			0

/* prototipi di funzioni */
void Inizializza();
BOOL MossaAvversario(char *);
BOOL MossaProgramma(char *);
void GeneraPseudo();
void Muovi(Mossa *);
void TornaIndietro(Mossa *);
BOOL AlphaBeta(Mossa *, int *, int, int);
int  Valuta();
