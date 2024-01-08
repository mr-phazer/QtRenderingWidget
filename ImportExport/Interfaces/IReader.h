#pragma once
/// <summary>
/// Interface for reading data from a buffer
/// </summary>
class IReader
{
public: 
	virtual void Read(void* pData, size_t size = 0) = 0;	
};

