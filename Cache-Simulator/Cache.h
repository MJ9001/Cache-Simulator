#pragma once
#include <iostream>
#include <list>
#include <vector>
class Cache
{
	struct cacheWord {
		bool valid;
		uint32_t tag;
		uint32_t insertedtick;//Used for LRU and FIFO
	};

public:
	Cache(std::string filename_, uint32_t wordcount_, uint32_t blockcount_, uint32_t associativity_,bool useFIFO_);
	~Cache();
	void Run();
private:
	std::vector<cacheWord*> cacheMem;
	std::list<uint32_t> addresses;
	std::string filename;
	uint32_t wordcount = 0;
	uint32_t taglen = 0;
	uint32_t associativity = 0;
	uint32_t tagbitwise = 0;
	uint32_t offsetlen = 0;
	uint32_t offsetbitwise = 0;
	uint32_t setlen = 0;
	uint32_t setbitwise = 0;
	uint32_t dataSize = 0;
	uint32_t blockcount = 0;
	uint32_t hits = 0;
	uint32_t misses = 0;
	uint32_t cacheMax = 0;
	uint32_t cycles = 0;
	bool useFIFO;
	void Init();
	void Exec();
	void LoadFile();
	void PrintResults();
};
 
