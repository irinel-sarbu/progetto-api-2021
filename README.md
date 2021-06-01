# Progetto API 2021
## Politecnico di Milano
### Graph Ranker
- L'obiettivo del progetto è la gestione di una classifica tra grafi diretti pesati
    - La classifica tiene traccia dei k "migliori" grafi
- Il programma da realizzare riceve in ingresso
    - due paramentri, una sola volta (sulla prima riga del file, separati da uno spazio)
        - d: il numero di nodi dei grafi
        - k: la lunghezza della classifica
    - una sequenza di comandi tra
        - 'AggiungiGrafo [matrice-di-adiacenza]'
        - 'TopK'
