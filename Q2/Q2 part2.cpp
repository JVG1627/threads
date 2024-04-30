#include <iostream>
#include <thread>
#include <semaphore.h>
#include <mutex>

using namespace std;

mutex mtx;
sem_t sem1, sem2;

void runMeFirst()
{
    cout << "Run me first" << endl;
    // for (int i=0; i<100000; i++);    // order tester
    sem_post(&sem1);
}

void runMeSecond()
{
    sem_wait(&sem1);
    cout << "I run second" << endl;
    // for (int i=0; i<100000; i++);    // order tester
    sem_post(&sem2);
}

void runMeThird()
{
    sem_wait(&sem2);
    cout << "I run third" << endl;
}

int main()
{
    const int num_threads = 3;
    // initialize the semaphores with the number of threads
    sem_init(&sem1, 0, 0);
    sem_init(&sem2, 0, 0);
    thread threads[num_threads];

    threads[0] = thread(runMeFirst);
    threads[1] = thread(runMeSecond);
    threads[2] = thread(runMeThird);

    for (int i = 0; i < num_threads; ++i)
    {
        threads[i].join();
    }

    sem_destroy(&sem1); // destroy the semaphores
    sem_destroy(&sem2);
    return 0;
}