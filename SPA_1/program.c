#include <stdio.h>
#include <string.h>

#define DQUEUE_MAXLENGTH 16

typedef struct {
	int elements[DQUEUE_MAXLENGTH];
	int first;
	int last;
} DQueue;

void MakeNull(DQueue* q) {
	q->first = 0;
	q->last = 0;
}

void EnqueueFront(DQueue* q, int element) {
	q->elements[q->first] = element;
	q->first++;

	if (q->first >= DQUEUE_MAXLENGTH)
		q->first = 0;
}

void EnqueueBack(DQueue* q, int element) {
	q->last--;

	if (q->last < 0)
		q->last = DQUEUE_MAXLENGTH - 1;

	q->elements[q->last] = element;
}

int DequeueFront(DQueue* q) {
	q->first--;

	if (q->first < 0)
		q->first = DQUEUE_MAXLENGTH - 1;

	int index = q->first;

	int result = q->elements[index];
	q->elements[index] = -1;
	return result;
}

int DequeueBack(DQueue* q) {
	int index = q->last;
	q->last++;

	if (q->last >= DQUEUE_MAXLENGTH)
		q->last = 0;

	int result = q->elements[index];
	q->elements[index] = -1;
	return result;
}

int Empty(DQueue* q) {
	return q->first == q->last;
}

//-------------------------------------------------------------------------------


void PrintQueue(DQueue* q) {
	DQueue temp_queue;
	MakeNull(&temp_queue);

	while (!Empty(q)) {
		int num = DequeueFront(q);
		printf("%d ", num);

		EnqueueFront(&temp_queue, num);
	}

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
	EnqueueFront(Q, 17);
	EnqueueFront(Q, 18);
	EnqueueFront(Q, 19);
	EnqueueFront(Q, 20);
	DequeueBack(Q);
	DequeueBack(Q);
	DequeueBack(Q);
	DequeueFront(Q);
	EnqueueBack(Q, 3);
	EnqueueBack(Q, 4);
	EnqueueBack(Q, 5);
}

void main() {
	DQueue Q;
	MakeNull(&Q);

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