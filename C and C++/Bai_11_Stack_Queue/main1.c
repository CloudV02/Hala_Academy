#include <stdio.h>
#include <stdlib.h>

typedef struct Queue
{
    int *items; // chua dia chi cua cai Queue
    int size; // size queue
    int front,rear;
}Queue;


void initialize(Queue * queue, int size){
    queue->items = (int*) malloc(sizeof(int)* size);
    queue ->size = size;
    queue ->front = -1;
    queue ->rear = -1;
}

int is_empty(Queue queue){
    return queue.front == -1;
}

int is_full(Queue queue){
    return ((queue.rear+1) % queue.size)==queue.front; 
}

void enqueue(Queue *queue, int value){
    if(!is_full(*queue)){
        if (is_empty(*queue))
        {
            queue->front = queue ->rear = 0; 
        }
        else{
            queue ->rear = (queue ->rear +1) % queue->size; /*  */
        }
        queue->items[queue->rear] = value;
    }
    else{
        printf("Queue overflow\n");
    }
}
int dequeue(Queue *queue){
    if (!is_empty(*queue))
    {
        int dequeue_value = queue->items[queue->front]; /* 0 1 2 3 */
        if(queue->front == queue->rear){
            queue->front = queue->rear = -1;
        }
        else{
            queue->front = (queue->front + 1) % queue->size;
        }
        return dequeue_value;
    }
    else{
        printf("Queue underflow\n");
        return -1;
    }
}
int front(Queue queue) {
    if (!is_empty(queue)) {
        return queue.items[queue.front];
    } else {
        printf("Queue is empty\n");
        return -1;
    }
}



int main()
{
    Queue queue;
    initialize(&queue, 3);

    enqueue(&queue, 10);
    enqueue(&queue, 20);
    enqueue(&queue, 30);

    printf("Front element: %d\n", front(queue));

    printf("Dequeue element: %d\n", dequeue(&queue));
    printf("Dequeue element: %d\n", dequeue(&queue));

    printf("Front element: %d\n", front(queue));

    enqueue(&queue, 40);
    enqueue(&queue, 50);
    printf("Dequeue element: %d\n", dequeue(&queue));
    printf("Front element: %d\n", front(queue));

    return 0;
}