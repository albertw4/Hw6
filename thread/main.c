#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void* produce(void* value)
{
    printf("Produced\n");

}

void* produceWithDelay(void* value)
{
    printf("Produced with Delay\n");


}

void* consume(void* value)
{
    printf("Consumed\n");
}

void* consumeWithDelay(void* value)
{
    printf("Consumed with Delay\n");
}

void printBuffer(int buffer[8])
{
    int i;
    for(i = 0; i < 8; ++i)
    {
        printf("[%d]  ", buffer[i]);
    }
    printf("\n");
}

void fillBuffer(int buffer[8])
{
    int i;
    for(i = 0; i < 8; ++i)
    {
        buffer[i] = 0;
    }
}

int main(int argc, char **argv)
{
    int producerNum = 2;        //change this to argv[1] later
    int consumerNum = 4;        //change this to argv[2] later
    int itemNum = 10;           //change this to argv[3] later
    int delay = 0;              //change this to argv[4] later IF == 0 then producer delay, IF == 1 then consumer delay

    int buffer[8];
    pthread_t producer;
    pthread_t consumer;

    printf("In main\n");
    fillBuffer(buffer);

    if(delay == 0)
    {
        pthread_create(&producer, NULL, produceWithDelay, &buffer);
        pthread_create(&consumer, NULL, consume, &buffer);
        pthread_join(producer, NULL);
        pthread_join(consumer, NULL);
    }else{
        pthread_create(&producer, NULL, produce, &buffer);
        pthread_create(&consumer, NULL, consumeWithDelay, &buffer);
        pthread_join(producer, NULL);
        pthread_join(consumer, NULL);
    }
    return 0;
}
