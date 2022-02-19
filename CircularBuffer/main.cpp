#include <iostream>
#include <Windows.h>
#include <mutex>
#include <thread>

#include "circular_buffer_r1.h"

CircularBufferR1 cb(8);

void singleThreadTest(unsigned threadId)
{
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


void multiThreadTest(unsigned threadCount)
{
    std::thread* threads = new std::thread[threadCount];

    for (unsigned i = 0; i < threadCount; i++)
    {
        threads[i] = std::thread(singleThreadTest, i);
    }
    for (unsigned i = 0; i < threadCount; i++)
    {
        threads[i].join();
    }
}


int main()
{
    std::cout << "Hello World!\n";
    
    multiThreadTest(5);
    // singleThreadTest(1);

}
