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

int Empty(DQueue* q) {
	return q->first == q->last;
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
	if (q->first >= DQUEUE_MAXLENGTH)
		q->first = 0;
}

void EnqueueBack(DQueue* q, int element) {
	// Smanjiti indeks za -1 da pokazuje na prvo slobodno mjesto odozada (last po defaultu pokazuje na prvo zauzeto mjesto)
	q->last--;

	// Wrap around
	if (q->last < 0)
		q->last = DQUEUE_MAXLENGTH - 1;

	// Postaviti element u prvo slobodno mjesto sa zadnje strane
	q->elements[q->last] = element;
}

int DequeueFront(DQueue* q) {
	// Smanjiti indeks za -1 da pokazuje na prvo zauzeto mjesto sa prednje strane
	q->first--;

	// Wrap around
	if (q->first < 0)
		q->first = DQUEUE_MAXLENGTH - 1;

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
	if (q->last >= DQUEUE_MAXLENGTH)
		q->last = 0;

	// Dohvati element i makni iz polja
	int result = q->elements[index];
	q->elements[index] = -1;

	return result;
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

void DoData(DQueue* Q) {
	/*
	UBACI POCETAK 3
	UBACI KRAJ 7
	UBACI POCETAK 17
	IZBACI KRAJ
	IZBACI POCETAK
	UBACI POCETAK 17
	UBACI POCETAK 18
	UBACI POCETAK 19
	UBACI POCETAK 20
	IZBACI KRAJ
	IZBACI KRAJ
	IZBACI KRAJ
	IZBACI POCETAK
	UBACI KRAJ 3
	UBACI KRAJ 4
	UBACI KRAJ 5
	*/

	EnqueueFront(Q, 3);
	EnqueueBack(Q, 7);
	EnqueueFront(Q, 17);
	DequeueBack(Q);
	DequeueFront(Q);
	EnqueueFront(Q, 69);
	EnqueueFront(Q, 18);
	EnqueueFront(Q, 19);
	EnqueueFront(Q, 20);
	DequeueBack(Q);
	DequeueBack(Q);
	DequeueBack(Q);
	DequeueFront(Q);
	EnqueueBack(Q, 420);
	EnqueueBack(Q, 4);
	EnqueueBack(Q, 5);
}

void main() {
	DQueue Q;
	MakeNull(&Q); // MakeNull u ovom slucaju postavlja oba indexa u 4

	/*DoData(&Q);
	while (1) {
	}*/

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

			} else if (!strcmp(cmd2, "KRAJ")) {

				EnqueueBack(&Q, num);
				PrintQueue(&Q);

			}
		} else if (!strcmp(cmd1, "IZBACI")) {
			if (!strcmp(cmd2, "POCETAK")) {

				DequeueFront(&Q);
				PrintQueue(&Q);

			} else if (!strcmp(cmd2, "KRAJ")) {

				DequeueBack(&Q);
				PrintQueue(&Q);

			}
		}
	}
}