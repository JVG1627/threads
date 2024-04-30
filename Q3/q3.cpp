//q3
//Joaquin Vazquez
//04/29/24
#include <iostream>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <random>

using namespace std;

// Structure for the request
struct RequestStructure {
    int reqId;
    string ip;
    string request;
};

// Global variables
queue<RequestStructure> queueMsg;
mutex mtx;
condition_variable cv;
int reqCounter = 0;
string webPages[10] = {"google.com", "yahoo.com", "stackoverflow.com", "github.com", "amazon.com", "ebay.com", "wikipedia.org", "reddit.com", "linkedin.com", "twitter.com"};

// Function to simulate request handling
void do_request(int thread_id) {
    while (true) {
        // Lock the mutex before accessing shared data
        unique_lock<mutex> lock(mtx);

        // Wait if the queue is empty
        cv.wait(lock, [] { return !queueMsg.empty(); });

        // Get the request from the queue
        RequestStructure req = queueMsg.front();
        queueMsg.pop();

        // Unlock the mutex
        lock.unlock();

        // Display the message
        cout << "Thread " << thread_id << " completed request " << req.reqId << " requesting webpage " << req.request << endl;
    }
}
void listen() {
    while (true) {
        // Sleep for random duration between 1-3 seconds
        this_thread::sleep_for(chrono::seconds(rand() % 3 + 1));

        // Create a request
        RequestStructure req;
        req.reqId = ++reqCounter;
        req.ip = "";
        req.request = webPages[rand() % 10];

        // Lock the mutex before modifying shared data
        unique_lock<mutex> lock(mtx);
        queueMsg.push(req);
        lock.unlock();

        // Notify one thread to consume the request
        cv.notify_one();
    }
}

// Function to handle requests

int main() {
    // Create n threads for listening to requests
    const int listenerThreadsNum = 3; // Change this value as needed
    thread listenerThreads[listenerThreadsNum];
    for (int i = 0; i < listenerThreadsNum; ++i) {
        listenerThreads[i] = thread(listen);
    }

    // Create n threads to handle requests
    const int threadsRequestNum = 5; // Change this value as needed
    thread threadsRequest[threadsRequestNum];
    cout<<threadsRequestNum;
    for (int i = 0; i < threadsRequestNum; ++i) {
        threadsRequest[i] = thread(do_request, i + 1);
    }

    // Join the listening threads with the main thread
    for (int i = 0; i < listenerThreadsNum; ++i) {
        listenerThreads[i].join();
    }

    // Join the request handling threads with the main thread
    for (int i = 0; i < threadsRequestNum; ++i) {
        threadsRequest[i].join();
    }

    return 0;
}