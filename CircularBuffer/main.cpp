#include <iostream>
#include <Windows.h>
#include <mutex>
#include "circular_buffer_r1.h"

std::mutex m;

typedef struct
{
    unsigned threadId;
    CircularBufferR1& cb;
} threadArgs;

void singleThreadTest(void* args)
{
    threadArgs* tArgs = (threadArgs*)args;
    unsigned threadId = tArgs->threadId;
    CircularBufferR1 cb = tArgs->cb;

    char sampleInput[] = "RANDOM STRING";
    char output[sizeof(sampleInput)];
    srand((unsigned) time(0));

    for (unsigned i = 0; i < 10; i++)
    {
        // random fill
        unsigned inputSize = rand() % strlen(sampleInput);
        unsigned writen = cb.tryWrite(inputSize, sampleInput);

        if (1 || !threadId)
        {
            std::cout << "[TH: " << threadId << "]: " << "BYTES WRITTEN " << writen << std::endl;
        }

        std::fill(output, output + sizeof(output), 0);
        // read try
        unsigned readBufferSize = rand() % sizeof(output);
        unsigned read = cb.tryRead(readBufferSize, output);
        
        if (1 || !threadId)
        {
            std::cout << "[TH: " << threadId << "]: " << "BYTES READ TRIED " << readBufferSize << ", READ " << read << ":" << output << std::endl;
        }
    }
}

void multiThreadTest(unsigned threadCount, CircularBufferR1& cb)
{   
    HANDLE* hThreads = new HANDLE[threadCount];
    // std::fill(hThreads, hThreads + threadCount, 0);
    threadArgs* args = (threadArgs * )malloc(sizeof(threadArgs)*threadCount);

    for (unsigned i = 0; i < threadCount; i++)
    {
        args[i].cb = cb;
        args[i].threadId = i;

        hThreads[i] = CreateThread(
            NULL,    // Thread attributes
            0,       // Stack size (0 = use default)
            (LPTHREAD_START_ROUTINE) singleThreadTest, // Thread start address
            &args,    // Parameter to pass to the thread
            0,       // Creation flags
            NULL);   // Thread id
    }

    for (unsigned i = 0; i < threadCount; i++)
    {
        if (hThreads[i])
        {
            WaitForSingleObject(hThreads[i], INFINITE);
            CloseHandle(hThreads[i]);
        }
    }
}

int main()
{
    std::cout << "Hello World!\n";
    CircularBufferR1 cb(8);

    multiThreadTest(5, cb);

}
