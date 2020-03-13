#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

int buffer[8];
int maxProduction;
int maxConsumption;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t notFull = PTHREAD_COND_INITIALIZER;
pthread_cond_t notEmpty = PTHREAD_COND_INITIALIZER;

void* produce(void* value)
{
    int producerId = *(int*) value;
    int production = producerId * maxProduction;
    int i;

    for(i = 0; i < maxProduction; i++){
        pthread_mutex_lock(&mutex);
        while(isFull() == 1){
            pthread_cond_wait(&notFull, &mutex);
        }
        //producing an int
        int freeSpot = getFreeSpot();
        buffer[freeSpot] = production;
        printf("Producer %d Produced  %d\n", producerId, production);
        production++;
        pthread_cond_signal(&notEmpty);
        pthread_mutex_unlock(&mutex);
    }
    //printf("maxProduction %d: maxConsumption %d \n\n", maxProduction, maxConsumption);
    pthread_exit(NULL);
}

void* produceWithDelay(void* value)
{
    int producerId = *(int*) value;
    int production = producerId * maxProduction;
    int i;

    for(i = 0; i < maxProduction; i++){
        pthread_mutex_lock(&mutex);
        while(isFull() == 1){
            pthread_cond_wait(&notFull, &mutex);
        }
        //producing an int
        int freeSpot = getFreeSpot();
        buffer[freeSpot] = production;
        printf("Producer %d Produced  %d\n", producerId, production);
        production++;

        printBuffer();  //test

        usleep(500000);
        pthread_cond_signal(&notEmpty);
        pthread_mutex_unlock(&mutex);

    }
    /*
    printf("Delayed Producer %d Produced  \n", producerId);
    printf("maxProduction %d: maxConsumption %d \n\n", maxProduction, maxConsumption);
    */
    pthread_exit(NULL);
}

void* consume(void* value)
{
    int consumerId = *(int*) value;
    int consumed;
    int i;

    for(i = 0; i < maxConsumption; i++){
        pthread_mutex_lock(&mutex);
        while(isEmpty() == 1){
            printf("Consumer %d blocked  \n", consumerId);
            pthread_cond_wait(&notEmpty, &mutex);

        }
        //consume an integer
        consumed = consumeValue();
        printf("Consumer %d consumed  %d, i is %d\n", consumerId, consumed,i);

        consumed++;

        printBuffer();

        pthread_cond_signal(&notFull);
        pthread_mutex_unlock(&mutex);
    }
    //printf("maxProduction %d: maxConsumption %d \n\n", maxProduction, maxConsumption);
    printf("Consumer %d is DONE\n", consumerId);

    pthread_exit(NULL);
}

void* consumeWithDelay(void* value)
{
    int consumerId = *(int*) value;
        int consumed = consumerId * maxConsumption;
    int i;

    for(i = 0; i < maxConsumption; i++){
        pthread_mutex_lock(&mutex);
        while(isEmpty() == 1){
            pthread_cond_wait(&notEmpty, &mutex);
        }
        //consume an integer
        consumeValue(consumed);
        printf("Consumer %d consumed  %d\n", consumerId, consumed);
        consumed++;
        usleep(500000);

        pthread_cond_signal(&notFull);
        pthread_mutex_unlock(&mutex);
    }

    /*
    printf("Delayed Consumer %d Consumed  \n", consumerId);
    printf("maxProduction %d: maxConsumption %d \n\n", maxProduction, maxConsumption);
    */

    pthread_exit(NULL);
}

void printBuffer()
{
    int i;
    for(i = 0; i < 8; ++i)
    {
        printf("%d[%d] : ",i, buffer[i]);
    }
    printf("\n\n");
}

void fillBuffer()
{
    int i;
    for(i = 0; i < 8; ++i)
    {
        buffer[i] = -1;
    }
}

// Return 0 if its NOT full and 1 if it is FULL
int isFull()
{
    int i;
    for(i = 0; i < 8; ++i)
    {
        if(buffer[i] == -1)
            return 0;
    }
    return 1;
}

//return 0 is its NOT empty and 1 if it is Empty
int isEmpty()
{
    int result = 1;
    int i;
    for(i = 0; i < 8; ++i)
    {
        if(buffer[i] != -1)
            result = 0;
    }
    return result;
}

int getFreeSpot()
{
    int i;
    for(i = 0; i < 8; ++i)
    {
        if(buffer[i] == -1)
            return i;
    }
}

int consumeValue()
{
    int temp, i;
    for(i = 0; i < 8; ++i)
    {
        if(buffer[i] != -1)
            temp = buffer[i];
            buffer[i] = -1;
    }
    return temp;
}

int main(int argc, char **argv)
{
    int producerNum = 2;        //P : change this to argv[1] later
    int consumerNum = 4;        //C : change this to argv[2] later
    int delay = 0;              //D :change this to argv[4] later IF == 0 then producer delay, IF == 1 then consumer delay
    maxProduction = 10;         //I : change this to argv[3] later
    maxConsumption = producerNum * maxProduction / consumerNum;

    pthread_t producers[producerNum];
    pthread_t consumers[consumerNum];

    printf("In main\n");
    fillBuffer();
    printBuffer();

    if(delay == 0)
    {
            printf("Running with Producer Delay  \n\n");

        int i;
        for (i = 0; i < producerNum; i++){
            pthread_create(&producers[i], NULL, produceWithDelay, &i);
        }
        for (i = 0; i < consumerNum; i++){
            pthread_create(&consumers[i], NULL, consume, &i);
        }

        for (i = 0; i < producerNum; i++){
            pthread_join(producers[i], NULL);
        }
        for (i = 0; i < consumerNum; i++){
            pthread_join(consumers[i], NULL);
        }
    }
    else{
            printf("Running with Consumer Delay  \n\n");

        int i;
        for (i = 0; i < producerNum; i++){
            pthread_create(&producers[i], NULL, produce, &i);
        }
        for (i = 0; i < consumerNum; i++){
            pthread_create(&consumers[i], NULL, consumeWithDelay, &i);
        }

        for (i = 0; i < producerNum; i++){
            pthread_join(producers[i], NULL);
        }
        for (i = 0; i < consumerNum; i++){
            pthread_join(consumers[i], NULL);
        }
    }
    return 0;
}
