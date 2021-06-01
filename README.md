# Progetto API 2021 Politecnico di Milano
## Graph Ranker
### Obiettivo
L'obiettivo del progetto è la gestione di una classifica tra grafi diretti pesati
    -La classifica tiene traccia dei k "migliori" grafi
Il programma da realizzare riceve in ingresso
    - due paramentri, una sola volta (sulla prima riga del file, separati da uno spazio)
        - d: il numero di nodi dei grafi
        - k: la lunghezza della classifica
    - una sequenza di comandi tra
        - 'AggiungiGrafo [matrice-di-adiacenza]'
        - 'TopK'
### AggiungiGrafo
Richiede di aggiungere un grafo a quelli considerati per stilare la classifica. È
seguito dalla matrice di adiacenza del grafo stesso, stampata una riga per
ogni rigo, con gli elementi separati da virgole.
I nodi del grafo sono da considerarsi etichettati logicamente con un indice
intero tra 0 e d-1; il nodo in posizione 0 è quello la cui stella uscente è
descritta dalla prima riga della matrice.
I pesi degli archi del grafo elementi sono interi nell'intervallo [0, 2
32 – 1].
- Esempio per d=3
'''
AggiungiGrafo
3,7,42
0,7,2
7,4,3
'''