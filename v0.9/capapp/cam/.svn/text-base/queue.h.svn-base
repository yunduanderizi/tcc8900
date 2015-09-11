#ifndef __QUEUE_H__
#define __QUEUE_H__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <pthread.h>

#define qlock_init(queue)	pthread_mutex_init(&queue->queue_lock, NULL)
#define qlock(queue)	pthread_mutex_lock(&queue->queue_lock)
#define qunlock(queue)	pthread_mutex_unlock(&queue->queue_lock)

#if 0
typedef struct _node
{
	unsigned int resolution;
	char *info;
	struct _node *next;
	char data[0];
}node, *nodeptr;
#else
typedef struct _node
{
	int len;
	struct _node *next;
	char info[0];
}node, *nodeptr;
#endif

typedef struct _queue
{
	pthread_mutex_t  queue_lock;
	nodeptr front;
	nodeptr rear;
}queue, *queueptr;

#if 0
int en_queue(queueptr queue, char *data, int re);
int de_queue(queueptr queue, char *data, int *re);
#else
int en_queue(queueptr queue, char *data, int len);
int de_queue(queueptr queue, char *data);
#endif
int queue_empty(queueptr queue);

int init_queue(queueptr queue);

#endif
