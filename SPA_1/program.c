#include <stdio.h>
#include <string.h>

#define DQUEUE_MAXLENGTH 10

typedef struct {
	int elements[DQUEUE_MAXLENGTH];
	int first;
	int last;
} DQueue;

// Izjednaciti first i last da dqueue bude "prazan"
void MakeNull(DQueue* q) {
	q->first = 0;
	q->last = 0;
}

// Broj elemenata u queue-u
// 
//                           Indeksi u polje
//                            |0123456789|                                                                                              o
//  Zadnja strana polja ->    |--321-----|   <- Prednja strana polja (first element, front element), ako promatramo sa desne strane    /|\
//                                                                                                                                     / \
// 
// |0123456789|  <- Indeksi u polje/array
// |---XXXX---|  <- Array; first je 7, last je 3; 7 - 3 = 4
//
// |0123456789|
// |XXXXX-----|  <- Array; first je 5, last je 0; 5 - 0 = 5
//
// |0123456789|
// |XZ---AXXXX|  <- Array; first je 2, last je 5; 2 - (5 - DQUEUE_MAXLENGTH) = 2 - (5 - 10) = 7
//                  Oduzimamo last sa DQUEUE_MAXLENGTH jer elementi na pocetku polja overflowaju na drugu stranu jer smo ograniceni sa velicinom elements arraya,
//                  (circular buffer), ako gledamo polje sa desne strane i "prvi" nam je element do nas
//
// |0123456789|
// |--AXXXXXZ-|  <- Array; first je 9, last je 2; 9 - 2 = 7
//                  Isto kao primjer iznad, samo su svu elementi pomaknuti u lijevo za 3 mjesta, pa ne overflowaju sa desne strane u lijevu stranu
//
int Count(DQueue* q) {
	int last = q->last;
	int first = q->first;

	if (last > first) {
		last = last - DQUEUE_MAXLENGTH;
	}

	return first - last;
}

// DQueue je prazan
int Empty(DQueue* q) {
	return (q->first) == (q->last);
}

// DQueue je pun dok dqueue sadrzi 9 elemenata 
// Moze sadrzavati 10, ali onda ne razlikujemo pun niz od praznog jer first == last
int Full(DQueue* q) {
	return Count(q) == (DQUEUE_MAXLENGTH - 1);
}


// U obicnom queue-u je ovo funkcija Enqueue
void EnqueueFront(DQueue* q, int element) {
	// Postaviti element u prvo slobodno mjesto sa prednje strane (first po defaultu pokazuje na prvo slobodno mjesto)
	q->elements[q->first] = element;

	// Povecati indeks na prvo slobodno mjesto
	q->first++;

	// Wrap around
	// Ako je indeks veci od duzine polja, idemo ispocetka
	// Jer MakeNull ne mora nuzno postaviti i first i last u nulu
	// To moze biti bilo koji arbitrarni indeks u polju
	if (q->first >= DQUEUE_MAXLENGTH) {
		q->first = 0;
	}
}

void EnqueueBack(DQueue* q, int element) {
	// Smanjiti indeks za -1 da pokazuje na prvo slobodno mjesto odozada (last po defaultu pokazuje na prvo zauzeto mjesto)
	q->last--;

	// Wrap around
	if (q->last < 0) {
		q->last = DQUEUE_MAXLENGTH - 1;
	}

	// Postaviti element u prvo slobodno mjesto sa zadnje strane
	q->elements[q->last] = element;
}

int DequeueFront(DQueue* q) {
	// Smanjiti indeks za -1 da pokazuje na prvo zauzeto mjesto sa prednje strane
	q->first--;

	// Wrap around
	if (q->first < 0) {
		q->first = DQUEUE_MAXLENGTH - 1;
	}

	int index = q->first;

	// Dohvati element i makni iz polja
	int result = q->elements[index];
	q->elements[index] = -1; // Ovo je realno nepotrebno

	return result;
}

// U obicnom queue-u je ovo funkcija Dequeue
int DequeueBack(DQueue* q) {
	// Spremiti last u indeks jer vec pokazuje na prvo zauzeto mjesto sa zadnje strane
	int index = q->last;

	// Povecati da pokazuje na drugo zauzeto mjesto sa zadnje strane jer ovo prvo micemo van
	q->last++;

	// Wrap around
	if (q->last >= DQUEUE_MAXLENGTH) {
		q->last = 0;
	}

	// Dohvati element i makni iz polja
	int result = q->elements[index];
	q->elements[index] = -1;

	return result;
}

// Element na pocetku queue-a, element koji je zadnji dodan
int Front(DQueue* q) {
	// First pokazuje na prvo slobodno mjesto, pa ga umanjimo za -1
	int index = q->first - 1;

	// Wrap around
	if (index < 0) {
		index = DQUEUE_MAXLENGTH - 1;
	}

	return q->elements[index];
}

// Element na kraju queue-a, element koji je prvi dodan
int Back(DQueue* q) {
	// Last pokazuje na prvo zauzeto mjesto sa zadnje strane, nije potreban ++ ili --
	int index = q->last;

	return q->elements[index];
}

//-------------------------------------------------------------------------------


void PrintQueue(DQueue* q) {
	// Ovdje koristimo jos jedan queue da dohvatimo sve elemente iz prvog i spremimo u drugi
	// Razlog tome je jer ne mozemo indeksirati bilo koji element u queueu koji nije na pocetku i na kraju,
	// a moramo ispisati sve elemente u queueu

	DQueue temp_queue;
	MakeNull(&temp_queue);

	// Dok ima elemenata u queueu, dohvati sa pocetka, ispisi na ekran i gurni u drugi queue na pocetak
	// Ta operacija okrece redoslijed kao u stacku, znaci iz queuea 3 2 1 -> u queue 1 2 3 (jer uzimamo sa kraja i stavljamo na kraj)
	// 
	// NAPOMENA: Ako ce trazit da se okrene redoslijed ispisa na ekran, onda samo zamijeniti sve *Front funkcije sa *Back funkcijama ovdje ispod
	//                                                                                                vvvvvvvvvvvvvvvvvv

	while (!Empty(q)) {
		int num = DequeueFront(q);
		printf("%d ", num);

		EnqueueFront(&temp_queue, num);
	}

	// Vrati sve elemente iz drugog queuea u prvi na isti nacin
	while (!Empty(&temp_queue)) {
		int num = DequeueFront(&temp_queue);
		EnqueueFront(q, num);
	}

	printf("\n");
}

void main() {
	DQueue Q;
	MakeNull(&Q);

	char cmd1[32];
	char cmd2[32];
	int num;

	while (1) {
		scanf("%s %s", cmd1, cmd2);

		if (!strcmp(cmd1, "UBACI")) {
			scanf("%d", &num);

			if (!strcmp(cmd2, "POCETAK")) {

				EnqueueFront(&Q, num);
				PrintQueue(&Q);

			}
			else if (!strcmp(cmd2, "KRAJ")) {

				EnqueueBack(&Q, num);
				PrintQueue(&Q);

			}
		}
		else if (!strcmp(cmd1, "IZBACI")) {
			if (!strcmp(cmd2, "POCETAK")) {

				DequeueFront(&Q);
				PrintQueue(&Q);

			}
			else if (!strcmp(cmd2, "KRAJ")) {

				DequeueBack(&Q);
				PrintQueue(&Q);

			}
		}
	}
}