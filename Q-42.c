#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<time.h>
#include<pthread.h>
#include<semaphore.h>

void *Barber_func();
void *Customer_func();

sem_t MUTEX;
sem_t CUSTOMER;
sem_t BARBER;

time_t Total_Time;
int Cust_COUNT=0;   /* ThE COUNT of customers waiting for haircuts*/
int Total_Seats;


int main()
{  int open_time;
    printf("===============================================================================\n");
    printf("\t\t\t\tSLEEPING BARBER PROBLEM\n");
    printf("===============================================================================\n");
    printf("Enter total number of seats available in waiting room of Barber shop.\n");
    scanf("%d",&Total_Seats);

    printf(" Time for which the Barber shop is open on a day is = 8\n");
    printf("===============================================================================\n\n\n");

    pthread_t T1,T2;
    Total_Time=time(NULL)+8;

    sem_init(&MUTEX,0,1);
    sem_init(&CUSTOMER,0,0);
    sem_init(&BARBER,0,1);

    pthread_create(&T1,NULL,Barber_func,NULL);

    pthread_create(&T2,NULL,Customer_func,NULL);

    pthread_join(T2,NULL);
    pthread_join(T1,NULL);

    return 0;
}

void *Customer_func()
{
    while(time(NULL)<Total_Time)
    {
        sem_wait(&MUTEX);
        if(Cust_COUNT<Total_Seats)
        {
            Cust_COUNT++;
            printf("Customer:");
            printf("\t Entered Shop \t\t Total customers in waiting room are:     %d\n",Cust_COUNT);
            sem_post(&MUTEX);
            sem_post(&CUSTOMER);
            sem_wait(&BARBER);
        }
        else

            sem_post(&MUTEX);
        sleep(1);
    }
}
void *Barber_func()
{
    while(time(NULL)<Total_Time || Cust_COUNT>0)
    {
        sem_wait(&CUSTOMER);
        sem_wait(&MUTEX);
        Cust_COUNT--;
        printf("Barber:");
        printf("\t\t Finished cutting hair \t Remaining Customers in waiting room are: %d.\n",Cust_COUNT);
        sem_post(&MUTEX);
        sem_post(&BARBER);
        sleep(3);
    }
}



