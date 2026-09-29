# LaMoSca - LAboratorio di MOtori per SCAcchi

**LaMoSca** è un motore scacchistico didattico originariamente sviluppato nel 2001-2002 da **Pietro Valocchi**, ispirato a progetti storici come *TSCP* (Tom Kerrigan's Simple Chess Program).

L'obiettivo originario del progetto era mostrare in modo incrementale e trasparente (da v0.01 a v0.10) come costruire un motore per scacchi funzionante: rappresentazione della scacchiera, generazione mosse pseudolegali, protocollo di comunicazione (WinBoard/XBoard), valutazione statica (PST) e algoritmi di ricerca dell'albero delle mosse (Minimax, Negamax, Alpha-Beta, ordinamento mosse e ricerca di quiescenza).

---

## 🎯 Il Rilancio del Progetto (2026+)

A distanza di oltre vent'anni, il progetto viene ripreso con l'obiettivo di:
1. **Riconnetterlo al mondo moderno**: testarlo con GUI moderne (come **Arena Chess**) e dotarlo del protocollo **UCI** (*Universal Chess Interface*), lo standard mondiale per i motori scacchistici.
2. **Completare le regole al 100%**: implementare l'arrocco completo generato dal motore, la presa *en passant*, le promozioni complete (D, T, A, C) e le regole di patta ufficiali FIDE, certificando l'assenza di bug tramite **Perft**.
3. **Migliorare la forza di gioco e la competitività**: introdurre una *Transposition Table* con Zobrist Hashing, gestione dinamica del tempo (*Time Management*), potature moderne (*Null-Move*, *LMR*) e una valutazione aggiornata (dalle tabelle PeSTO fino a una rete neurale quantizzata **NNUE**).

---

## 📁 Struttura della Cartella

```text
lamosca/
├── README.md               # Questo file
├── PROJECT_STATUS.md       # Analisi dettagliata, stato dell'arte e roadmap
├── src/                    # Codice sorgente C originale (v0.10)
│   ├── Common.h            # Costanti, strutture dati, prototipi
│   ├── Extern.h            # Riferimenti variabili esterne
│   ├── LaMoSca.c           # Main loop, I/O console / xboard, scacchiera
│   ├── Genera.c            # Generazione mosse pseudolegali, esecuzione mossa
│   ├── Cerca.c             # Ricerca Alpha-Beta, ordinamento mosse, quiescenza
│   └── Valuta.c            # Valutazione materiale e Piece-Square Tables (PST)
├── bin/                    # Eseguibile compilato a 32-bit (v0.10 originale del 2002)
│   └── LaMoSca.exe         # Testato e funzionante su Windows 10/11 x64
├── *.html, *.gif           # Pagine e risorse grafiche del sito web originale
└── LaMoSca*.zip            # Archivi storici delle versioni didattiche 01-10
```

---

## 🚀 Guida Rapida: Vederlo subito in azione con Arena Chess

La versione originale `LaMoSca.exe` (in [bin/LaMoSca.exe](file:///bin/LaMoSca.exe)) è un eseguibile Win32 a 32-bit che gira perfettamente anche su Windows a 64 bit:

1. Scarica e avvia **Arena Chess GUI** (versione 3.5.1 o superiore).
2. Nel menu principale vai su **Engines** -> **Install New Engine...**
3. Seleziona il file `LaMoSca.exe` presente nella cartella `bin/`.
4. Alla richiesta del protocollo, seleziona **WinBoard** (o **WinBoard v1/v2**).
5. Avvia una nuova partita contro il motore: LaMoSca è pronto a giocare!

---

## 🗺️ Roadmap di Sviluppo

Per i dettagli completi sull'architettura, sui linguaggi consigliati (C vs C++ vs Rust) e sulla roadmap passo-passo, consulta il documento **[PROJECT_STATUS.md](file:///PROJECT_STATUS.md)**.
