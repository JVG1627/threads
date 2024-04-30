#include <iostream>
#include <thread>
#include <semaphore.h>
#include <mutex>
#include <chrono>

using namespace std;

mutex mtx;
sem_t sem;

void runMeFirst()
{
    // for (int i=0l; i<100000; i++);  // un comment this to test that runMeFirst always run first
    cout << "Run me first" << endl;
}

void runMeSecond()
{
    sem_wait(&sem);
    cout << "I run second" << endl;
}

void signalSemaphore()
{
    std::this_thread::sleep_for(std::chrono::seconds(1));
    sem_post(&sem);
}

int main()
{
    const int num_threads = 2;
    // initialize the semaphore with the number of threads
    sem_init(&sem, 0, 0);
    thread threads[num_threads];

    threads[0] = thread(runMeFirst);
    threads[1] = thread(runMeSecond);
    thread signalThread(signalSemaphore);

    for (int i = 0; i < num_threads; ++i)
    {
        threads[i].join();
    }
    signalThread.join();

    sem_destroy(&sem); // destroy the semaphore
    return 0;
}