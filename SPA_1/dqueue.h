#ifndef DQUEUE_H
#define DQUEUE_H

struct dqueue {
	struct dqueue* next;
	struct dqueue* prev;
	int element;
};

#endif