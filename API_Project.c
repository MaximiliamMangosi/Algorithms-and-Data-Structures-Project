#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INFINITY -1
#define DIMENSIONE_TABELLA 50000


struct listaMacchine {
    int autonomia;
    struct listaMacchine* next;
};

struct Node{
    int distanza;
    int n_auto;
    int max;
    char colore;
    int dist;
    struct Node* precendente;
    struct listaMacchine* veicoli;
    struct Node* next;
    struct Node* prev;
};

struct Node* last = NULL;
struct Node* first = NULL;


typedef struct {
    struct Node* front;
    struct Node* back;
    int size;
} Coda;


typedef struct HashNode {
    int key;
    struct Node* value;
    struct HashNode* next;
} HashNode;

HashNode* TabellaStazioni[DIMENSIONE_TABELLA] = {0};



void insertIntoTabellaStazioni(int distanza, struct Node* value) {
    int index = distanza % DIMENSIONE_TABELLA;
    HashNode* newNode = (HashNode*)malloc(sizeof(HashNode));
    newNode->key = distanza;
    newNode->value = value;
    newNode->next = NULL;

    if(TabellaStazioni[index] == NULL) {
        TabellaStazioni[index] = newNode;
    } else {
        HashNode* temp = TabellaStazioni[index];
        while(temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = newNode;
    }
}

struct Node* searchTabellaStazioni(int distanza) {
    int index = distanza % DIMENSIONE_TABELLA;
    HashNode* temp = TabellaStazioni[index];
    while(temp != NULL) {
        if(temp->key == distanza) {
            return temp->value;
        }
        temp = temp->next;
    }
    return NULL;
}

void deleteFromHashTable(int distanza) {
    int index = distanza % DIMENSIONE_TABELLA;
    HashNode* temp = TabellaStazioni[index];
    HashNode* prev = NULL;

    if (temp == NULL) return;

    if (temp->key == distanza) {
        TabellaStazioni[index] = temp->next;
        free(temp);
        return;
    }

    while (temp != NULL && temp->key != distanza) {
        prev = temp;
        temp = temp->next;
    }

    if (temp == NULL) return;

    prev->next = temp->next;
    free(temp);
}




struct Node* insertAtPosition(struct Node* head);
struct Node* deleateNode(struct Node* head);
struct Node* insertAuto(struct Node* head);
struct Node* deleateAuto(struct Node* head);
struct Node* PianificaPercorso(struct Node* head);


Coda* creaCoda();
void enque(Coda* coda, struct Node* stazione);
void enquePrev(Coda* coda, struct Node* stazione);
struct Node* deque(Coda* coda);
struct Node* dequePrev(Coda* coda);



int main() {
    struct Node* head = NULL;  
    
    char comandi[25];
    char StazioneA[] = "aggiungi-stazione";
    char StazioneD[] = "demolisci-stazione";
    char AutoA[] = "aggiungi-auto";
    char AutoD[] = "rottama-auto";
    char PianificaP[] = "pianifica-percorso";


    while (scanf("%s", comandi) == 1) {
        if(strcmp(comandi, StazioneA)==0){
            head = insertAtPosition(head);
        }else if(strcmp(comandi, StazioneD)==0){
            head = deleateNode(head);
        }else if(strcmp(comandi, AutoA)==0){
            head = insertAuto(head);
        }else if(strcmp(comandi, AutoD)==0){
            head = deleateAuto(head);
        }else if(strcmp(comandi, PianificaP)==0){
            head = PianificaPercorso(head);
        }else{
            printf("errore: input sbagliato\n");
        }
    }

    return 0;
}


struct Node* insertAtPosition(struct Node* nodoPrecedente) {
    struct Node* newNode = malloc(sizeof(struct Node));
    int dist, n_automobili, massimo = 0;

    newNode->veicoli = NULL;
    if (scanf("%d %d ", &dist, &n_automobili)) {}


    for (int j = 0; j < n_automobili; j++) {
        struct listaMacchine* newMacchina = malloc(sizeof(struct listaMacchine));
        int autonomia;
        if (scanf("%d", &autonomia)) {}

        newMacchina->autonomia = autonomia;
        newMacchina->next = newNode->veicoli;
        newNode->veicoli = newMacchina;


        if (massimo < autonomia) {
            massimo = autonomia;
        }
    }
    newNode->max = massimo;

    newNode->n_auto = n_automobili;
    newNode->distanza = dist;
    newNode->next = NULL;
    newNode->prev = NULL;
    newNode->colore = 'W';
    newNode->dist = INFINITY;
    newNode->precendente = NULL;


    if (!nodoPrecedente) {
        printf("aggiunta\n");
        first = newNode;
        last = newNode;
        insertIntoTabellaStazioni(dist, newNode);
        return newNode;
    }

    

    struct Node* current = nodoPrecedente;
    struct Node* curr_first = first;
    struct Node* curr_last = last;

    if (dist > nodoPrecedente->distanza) {
        while (current->next) {
            if (current->next->distanza > dist || current->next==NULL){
                break;
            }else if(curr_last->distanza <= dist){
                current = curr_last;
                break;
            }
            current = current->next;
            curr_last = curr_last->prev;
        }
        if (current->distanza != dist) {
            newNode->next = current->next;
            newNode->prev = current;
            if (current->next) {
                current->next->prev = newNode;
            } else {
                last = newNode;
            }
            current->next = newNode;
            printf("aggiunta\n");
            insertIntoTabellaStazioni(dist, newNode);

            return newNode;
        } else {
            printf("non aggiunta\n");
            free(newNode);
            return nodoPrecedente;
        }
    } else {
        while (current->prev) {
            if (current->prev->distanza < dist || current->prev==NULL){
                break;
            }else if(curr_first->distanza >= dist){
                current = curr_first;
                break;
            }
            current = current->prev;
            curr_first = curr_first->next;
        }
        if (current->distanza != dist) {
            newNode->next = current;
            newNode->prev = current->prev;
            if (current->prev) {
                current->prev->next = newNode;
            } else {
                first = newNode;
            }
            current->prev = newNode;
            printf("aggiunta\n");
            insertIntoTabellaStazioni(dist, newNode);
            return newNode;
        } else {
            printf("non aggiunta\n");
            free(newNode);
            return nodoPrecedente;
        }
    }
}


struct Node* deleateNode(struct Node* nodoPrecedente) {
    int dist;
    if (scanf("%d ", &dist)){}
    if (nodoPrecedente == NULL) {
        printf("non demolita\n");
        return NULL;
    }

    struct Node* currentNode = nodoPrecedente;

    
    if (nodoPrecedente->distanza == dist) {
        if(nodoPrecedente->prev){
            nodoPrecedente = nodoPrecedente->prev;
        }else if(nodoPrecedente->next){
            nodoPrecedente = nodoPrecedente->next;
        }else{
            nodoPrecedente = NULL;
            printf("demolita\n");
            deleteFromHashTable(dist);
            return nodoPrecedente;
        }
    }

    
    currentNode = searchTabellaStazioni(dist);
    

    if (currentNode && currentNode->distanza == dist) {
        if (currentNode->prev) {
            currentNode->prev->next = currentNode->next;
        }else{ 
            first = currentNode->next;
        }
        if (currentNode->next) {
            currentNode->next->prev = currentNode->prev;
        }else{ 
            last = currentNode->prev;
        }
        free(currentNode->veicoli);
        free(currentNode);
        printf("demolita\n");
        deleteFromHashTable(dist);
        return nodoPrecedente;
    }
    
    printf("non demolita\n");
    return nodoPrecedente;
}



struct Node* insertAuto(struct Node* head){
    int dist, autonomia;
    if(scanf("%d %d ", &dist, &autonomia)){}
    
    struct Node* curr = head;


    if(curr->distanza == dist){

    }else{
        curr = searchTabellaStazioni(dist);
    }

    if(curr==NULL){
        printf("non aggiunta\n");
        return head;
    }

    curr->n_auto++;

    struct listaMacchine* newMacchina = malloc(sizeof(struct listaMacchine));
    newMacchina->autonomia = autonomia;
    newMacchina->next = curr->veicoli; 
    curr->veicoli = newMacchina;

    printf("aggiunta\n");
    if(curr->max<autonomia){
        curr->max=autonomia;
    }
    return head;
}


struct Node* deleateAuto(struct Node* head){
    int dist, autonomia, max=0;
    if(scanf("%d %d ", &dist, &autonomia)){}
    
    struct Node* curr = head;
    if(curr->distanza == dist){

    }else{
        curr = searchTabellaStazioni(dist);
    }

    if(curr==NULL || curr->veicoli==NULL){
        printf("non rottamata\n");
        return head;
    }

    struct listaMacchine* temp = curr->veicoli;

    if(temp->autonomia == autonomia){
        curr->veicoli = temp->next;
        free(temp);
        struct listaMacchine* pippo = curr->veicoli;
        if(autonomia == curr->max){
            while(pippo != NULL) {
                if(max < pippo->autonomia) {
                    max = pippo->autonomia;
                }
                pippo = pippo->next;
            }
            curr->max = max;
        }
        printf("rottamata\n");
        
        return head;
    }

    if(curr->max==autonomia){
        while (temp->next != NULL && temp->next->autonomia != autonomia) {
            if(max < temp->autonomia){
                max = temp->autonomia;
            }
            temp = temp->next;
        }
    }else{
        while (temp->next != NULL && temp->next->autonomia != autonomia) {
            temp = temp->next;
        }
    }

    if(temp->next==NULL){
        printf("non rottamata\n");
        return head;
    }

    struct listaMacchine* nodoDaEliminare = temp->next;
    temp->next = temp->next->next;

    free(nodoDaEliminare);
    printf("rottamata\n");
    
    if(curr->max == autonomia && temp->next!=NULL){
        while(temp->next!=NULL){
            if(max<temp->autonomia){
                max = temp->autonomia;
            }
            temp=temp->next;
        }
    }
    if(curr->max==autonomia){
        curr->max = max;
    }

    return head;
}




void stampaPercorso(struct Node* percorso) {
    if (!percorso) {
        printf("nessun percorso\n");
        return;
    }

    while (percorso) {
        printf("%d", percorso->distanza);
        percorso = percorso->next;
        if (percorso) 
            printf(" ");
    }
    printf("\n");
}




Coda* creaCoda() {
    Coda* coda = malloc(sizeof(Coda));
    if (!coda) {
        return NULL;
    }
    coda->front = coda->back = NULL;
    coda->size = 0;
    return coda;
}

void enque(Coda* coda, struct Node* stazione) {
    if (!coda->front) {
        coda->front = coda->back = stazione;
    } else {
        coda->back->next = stazione;
        coda->back = stazione;
    }
    coda->size++;
}

void enquePrev(Coda* coda, struct Node* stazione) {
    if (!coda->front) {
        coda->front = coda->back = stazione;
    } else {
        coda->back->prev = stazione;
        coda->back = stazione;
    }
    coda->size++;
}

struct Node* deque(Coda* coda) {
    if (!coda->front){
        return NULL;
    }

    struct Node* temp = coda->front;
    coda->front = coda->front->next;
    if (!coda->front) {
        coda->back = NULL;
    }
    coda->size--;

    return temp;
}

struct Node* dequePrev(Coda* coda) {
    if (!coda->front){
        return NULL;
    }
    struct Node* temp = coda->front;
    coda->front = coda->front->prev;
    if (!coda->front) {
        coda->back = NULL;
    }
    coda->size--;

    return temp;
}


struct Node* ricostruisciPercorso(struct Node* arrivo, struct Node* partenza) {
    struct Node* percorso = NULL;
    struct Node* attuale = arrivo;

    while(attuale != NULL) {
        struct Node* nuovoNodo = malloc(sizeof(struct Node));
        *nuovoNodo = *attuale;
        nuovoNodo->next = percorso;
        percorso = nuovoNodo;
        attuale = attuale->precendente;
    }

    return percorso;
}

struct Node* ricostruisciPercorsoPrev(struct Node* arrivo, struct Node* partenza) {
    struct Node* percorso = NULL;
    struct Node* attuale = arrivo;
    struct Node* prevNodo = NULL;

    while(attuale != NULL) {
        struct Node* nuovoNodo = malloc(sizeof(struct Node));
        *nuovoNodo = *attuale;
        nuovoNodo->next = percorso;
        nuovoNodo->prev = prevNodo;
        if (percorso != NULL) {
            percorso->prev = nuovoNodo;
        }
        percorso = nuovoNodo;
        prevNodo = nuovoNodo;
        attuale = attuale->precendente;   
    }
    if (partenza->distanza != percorso->distanza){
        return NULL;
    }
    return percorso;
}

struct Node* percorsoNext(struct Node* head, int partenzaDistanza, int arrivoDistanza){
        struct Node* u;
        struct Node* partenza = head;
        struct Node* v;
        if(partenza->distanza == partenzaDistanza){

        }else{
            partenza = searchTabellaStazioni(partenzaDistanza);
        }


        if(partenza->next==NULL){
            printf("nessun percorso\n");
            return head;
        }
        if((arrivoDistanza - partenzaDistanza) <= partenza->max){
            printf("%d %d\n", partenzaDistanza, arrivoDistanza);
            return head;
        }

        for(u = partenza; u!=NULL && u->distanza <= arrivoDistanza; u = u->next) {
            if(u->colore!='W'){
                u->colore = 'W';
                u->dist = INFINITY;
                u->precendente = NULL;
            }
        }

        partenza->colore = 'G';
        partenza->dist = 0;
        v = partenza->next;
        Coda* coda = creaCoda();
        enque(coda, partenza);
        struct Node* nodiFinali = NULL;
        int trovato = 0;
        while(coda->size != 0) {
            u = deque(coda);
            while(v && (v->distanza - u->distanza) <= u->max) {
                if(v->colore == 'W') {
                    v->colore = 'G';
                    v->dist = u->dist + 1;
                    v->precendente = u;
                    enque(coda, v);
                    if (v->distanza == arrivoDistanza){
                        nodiFinali = ricostruisciPercorso(v, partenza);
                        trovato = 1;
                        break;
                    }
                }
                v = v->next;
            }
            u->colore = 'B';
            if(trovato==1){
                break;
            }
        }
        if(nodiFinali) {
            stampaPercorso(nodiFinali);
        }else {
            printf("nessun percorso\n");
        }

        
    return head;
}
struct Node* percorsoPrev(struct Node* head, int partenzaDistanza, int arrivoDistanza){
        struct Node* migliorPercorso = NULL;
        struct Node* u;
        struct Node* partenza = head;

        if(partenza->distanza == partenzaDistanza){

        }else{
            partenza = searchTabellaStazioni(partenzaDistanza);
        }


        if(partenza->prev==NULL){
            printf("nessun percorso\n");
            return head;
        }
        if((partenzaDistanza - arrivoDistanza)<=partenza->max){
            printf("%d %d\n", partenzaDistanza, arrivoDistanza);
            return head;
        }

        u = partenza;

        for(u = partenza; u!=NULL && u->distanza >= arrivoDistanza; u = u->prev) {
            if(u->colore!='W'){
                u->colore = 'W';
                u->dist = INFINITY;
                u->precendente = NULL;
            }
        }

        partenza->colore = 'G';
        partenza->dist = 0;

        Coda* coda = creaCoda();
        enquePrev(coda, partenza);
        while(coda->size != 0) {
            u = dequePrev(coda);

            struct Node* v = u->prev;
            while(v){
                if(u->distanza - v->distanza <= u->max){
                    if(v->colore == 'W') {
                        v->colore = 'G';
                        v->dist = u->dist + 1;
                        v->precendente = u;
                        enquePrev(coda, v);
                    }else if(v->colore=='G' && v->dist > u->dist){
                        v->precendente = u;
                    }
                }
                if(v->distanza == arrivoDistanza){
                    break;
                }
                v = v->prev;
            }
            u->colore = 'B';
            if(u->prev->distanza == arrivoDistanza){
                break;
            }
        }
        migliorPercorso = ricostruisciPercorsoPrev(u->prev, partenza);

        if(migliorPercorso) {
            stampaPercorso(migliorPercorso);
        }else {
            printf("nessun percorso\n");
        }
        
    return head;
    
}


struct Node* PianificaPercorso(struct Node* head){
    int partenzaDistanza, arrivoDistanza;
    if(scanf("%d %d ", &partenzaDistanza, &arrivoDistanza)){}
    
    if(partenzaDistanza<arrivoDistanza){
        percorsoNext(head, partenzaDistanza, arrivoDistanza);
    }else if(partenzaDistanza>arrivoDistanza){
        percorsoPrev(head, partenzaDistanza, arrivoDistanza);
    }else{
        printf("nessun percorso\n");
        return head;
    }
    return head;
}
