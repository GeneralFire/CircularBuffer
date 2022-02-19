#pragma once
#include <algorithm>
#include <iostream>
#include <mutex>


class CircularBufferR1
{
public:
	// CircularBufferR1& operator=(CircularBufferR1 const&) = delete;
	~CircularBufferR1()	{ delete[] data; };

	CircularBufferR1(unsigned int sizeInBytes) : 
		size(sizeInBytes), 
		data(new char[sizeInBytes]) 
	{
		availableSpace = sizeInBytes;
	};

	/// <summary>
	/// Read buffer
	/// </summary>
	/// <param name="outputBufferSize">Output data buffer size</param>
	/// <param name="buffer">Output buffer data</param>
	/// <returns>Read bytes count</returns>
	unsigned int tryRead(unsigned int outputBufferSize, char* buffer)
	{
		readLock.lock();
		// std::stringstream s;
		// s << "READ ATTEMPT\n\tAvailableData:" << availableData;
		// Logger::log(0, s.str().c_str());

		unsigned i = 0;
		unsigned bytesToRead =
			(outputBufferSize > availableData) ? availableData : outputBufferSize;
		for (; i < outputBufferSize && i < availableData; i++)
		{
			buffer[i] = data[RI];
			RI = (++RI) % size;
		}

		availableData -= i;
		availableSpace += i;
		readLock.unlock();

		return i;
	};

	/// <summary>
	/// Write buffer
	/// </summary>
	/// <param name="inputBufferSize">Input data buffer size</param>
	/// <param name="buffer">Input data buffer</param>
	/// <returns>Written bytes count</returns>
	unsigned int tryWrite(unsigned int inputBufferSize, char* buffer) 
	{
		writeLock.lock();
		// std::stringstream s;
		// s << "WRITE ATTEMPT\n\tAvailableSpace:" << availableSpace;
		// Logger::log(1, s.str().c_str());

		unsigned i = 0;
		unsigned bytesToWrite =
			(availableSpace > inputBufferSize) ? inputBufferSize : availableSpace;
		for (;i < bytesToWrite; i++)
		{
			data[WI] = buffer[i];
			WI = (++WI) % size;
		}
		
		availableData += i;
		availableSpace -= i;

		writeLock.unlock();
		return i;
	};

	/// <summary>
	/// Reset buffer, zeros buffer
	/// </summary>
	/// <returns></returns>
	bool resetBuffer()
	{
		RI = WI = availableData = availableSpace = 0;
		std::fill(data, data + size, 0);
	};

	/// <summary>
	/// Get Write and Read cursors
	/// </summary>
	/// <param name="W">Write cursor value</param>
	/// <param name="R">Read cursor value</param>
	void getWiRi(int* W, int* R)
	{
		*W = WI;
		*R = RI;
	}

private:
	unsigned RI = 0, WI = 0;	// read/write indexies
	unsigned availableData = 0, availableSpace = 0;
	unsigned size;
	char* data;
	std::mutex writeLock, readLock;
};
