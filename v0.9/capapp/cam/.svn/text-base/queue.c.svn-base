#include "queue.h"

void visit(void *buffer)
{
	printf("%s\n", (char *)buffer);
}

int init_queue(queueptr queue)
{
	qlock_init(queue);

	queue->front = queue->rear = NULL;
	
	return 0;
}

void destroy_queue(queueptr queue)
{
	assert(queue != NULL);
	nodeptr node;
	qlock(queue);
	while(queue->front)
	{
		node = queue->front;
		queue->front = node->next;
		free(queue->front);
	}

	qunlock(queue);

	return;
}

int queue_empty(queueptr queue)
{
	if(queue->front == NULL)
		return 0;
	else
		return -1;
}

int queue_length(queueptr queue)
{
	qlock(queue);
	nodeptr p = queue->front;
	int length = 0;
	while(p != NULL)
	{
		length++;
		p = p->next;
	}
	qlock(queue);

	return length;
}

int get_head(queueptr queue, char *data)
{
	assert(queue->front != NULL);
	qlock(queue);
	if(queue_empty(queue))
	{
		qunlock(queue);
		return -1;
	}
	else
	{
		data = queue->front->info;
		qunlock(queue);
		return 0;
	}
}

void queue_traverse(queueptr queue, void(*fp)(void *))
{
	assert(queue->front != NULL);
	nodeptr p = queue->front->next;
	while(p)
	{
		(*fp)(p->info);
		p = p->next;
	}
}


void clear_queue(queueptr queue)
{
	assert(queue->front != NULL);
	qlock(queue);
	nodeptr p = queue->front->next;
	while(p)
	{
		queue->front->next = p->next;
		free(p);
		p = queue->front->next;
	}
	qunlock(queue);
}

int en_queue(queueptr queue, char *data, int len)
{
	if(data == NULL || len < 0)
		return -1;
	
	nodeptr temp = (nodeptr)malloc(sizeof(node) + len);
	if(!temp)
		return -1;
	memset(temp, 0, sizeof(node));

	qlock(queue);
	memcpy(temp->info, data, len);
	temp->len = len;
	temp->next = NULL;
	if(NULL == queue->front)
	{
		queue->front = temp;
	}
	else
	{
		queue->rear->next = temp;
	}
	queue->rear = temp;
	
	qunlock(queue);

	return 0;
}

int de_queue(queueptr queue, char *data)
{
	qlock(queue);
	if(queue->front == NULL)
	{
		qunlock(queue);
		return -1;
	}

	nodeptr temp = queue->front;
	memcpy(data, temp->info, temp->len);
	queue->front = temp->next;
	if(queue->front == NULL)
		queue->rear = queue->front;
	free(temp);
	qunlock(queue);

	return 0;
}



