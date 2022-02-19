#include <iostream>
#include <Windows.h>
#include <mutex>
#include <thread>
#include <chrono>

#include "circular_buffer_r1.h"

void writer(unsigned threadId, CircularBufferR1& cb)
{
    char sampleInput[] = "RANDOM STRING";
    unsigned cursor = 0;
    unsigned step = sizeof(sampleInput);
    while (true)
    {
        if (cursor + step > sizeof(sampleInput))
        {
            cursor = 0;
        }

        unsigned writen = cb.tryWrite(step, &sampleInput[cursor]);
        std::this_thread::sleep_for(std::chrono::milliseconds(600));
        if (writen)
        {
            std::cout << "THREAD " << threadId << " WRITED" << writen << " BYTES" << std::endl;
        }
        else
        {
            std::cout << "THREAD " << threadId << " CANNOT WRITE" << std::endl;
        }
    }
}

void reader(CircularBufferR1& cb)
{
    size_t buffer_size = 20;
    char* buffer = new char[buffer_size];
    unsigned read = 0;

    while (true)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(900));
        memset(buffer, 0, buffer_size);
        read = cb.tryRead(10, buffer);
        if (read)
        {
            std::cout << buffer << std::endl;
        }
    }
}
void singleThreadTest(unsigned threadId, CircularBufferR1& cb)
{
    char sampleInput[] = "RANDOM STRING";
    char output[sizeof(sampleInput)];
    srand((unsigned) time(0));

    for (unsigned i = 0; i < 10; i++)
    {
        // random fill
        unsigned inputSize = 1; // rand() % strlen(sampleInput);
        unsigned writen = cb.tryWrite(inputSize, sampleInput);

        if (1 || !threadId)
        {
            std::cout << "[TH: " << threadId << "]: " << "BYTES WRITTEN " << writen << std::endl;
        }

        std::fill(output, output + sizeof(output), 0);
        // read try
        unsigned readBufferSize = 1; // rand() % sizeof(output);
        unsigned read = cb.tryRead(readBufferSize, output);
        
        if (1 || !threadId)
        {
            std::cout << "[TH: " << threadId << "]: " << "BYTES READ TRIED " << readBufferSize << ", READ " << read << ":" << output << std::endl;
        }
    }
}


void multiThreadTest(unsigned threadCount, CircularBufferR1& cb)
{


    std::thread* threads = new std::thread[threadCount];
    cb.tryRead(0, NULL);

    for (unsigned i = 0; i < threadCount; i++)
    {
        // threads[i] = std::thread(singleThreadTest, i, std::ref(cb));
        threads[i] = std::thread(writer, i, std::ref(cb));
    }
    for (unsigned i = 0; i < threadCount; i++)
    {
        threads[i].detach();
    }
}


int main()
{
    std::cout << "Hello World!\n";
    CircularBufferR1 cb(800);
    // CircularBufferR1& refCb = cb;

    multiThreadTest(2, cb);
    reader(cb);
    // singleThreadTest(1);

}
