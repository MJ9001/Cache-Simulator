#include "Cache.h"
#include "stdlib.h"
#include <math.h> 

Cache::Cache(std::string filename_, uint32_t wordcount_, uint32_t blockcount_, uint32_t associativity_, bool useFIFO_) :
	filename(filename_), wordcount(wordcount_), useFIFO(useFIFO_), blockcount(blockcount_), associativity(associativity_) {
};
Cache::~Cache(){}
void Cache::Run()
{
	printf("Sorting mode: %s\n", useFIFO ? "RANDOM" : "LRU");
	LoadFile();
	Init();
	Exec();
	PrintResults();
}

void Cache::Exec()
{
	bool hit;
	while (addresses.size() > 0)
	{          
		hit = false;
		uint32_t toreplacei = 0;
		uint32_t toreplacej = 0;
		uint32_t toreplaceval = INT_MAX;
		uint32_t address = addresses.front();
		addresses.pop_front();
		uint32_t set = (address & setbitwise) >> offsetlen;
		uint32_t tag = (address & tagbitwise) >> (offsetlen + setlen);

		for (int i = 0; associativity > i && !hit; i++)
		{
			cacheWord* word = 0;
			//uint32_t offset = address & offsetbitwise;
			for (int j = 0; wordcount > j; j++)
			{
				int movement = (set << (int)log2(associativity)) + i;
				word = &cacheMem.at(movement)[j];
				if (word->valid && (word->tag == tag))
				{
					hit = true;
					break;
						word->insertedtick = cycles;
				}
				if ((toreplaceval > word->insertedtick) || !word->valid)
				{
					toreplaceval = word->insertedtick;
					toreplacei = i;
					toreplacej = j;
					//if(useFIFO)
					//	toreplacej = rand() % wordcount;
					if(!word->valid)
						break;
				}
			}
			cycles++;
			if (!word->valid)
				break;
		}
		printf("address: %08x (%s)\n", address, hit ? "hit" : "miss");
		if (hit)
			hits++;
		else
		{
			misses++;
			cacheWord *word = &cacheMem.at((set << (int)log2f(associativity)) + toreplacei)[toreplacej];
			word->valid = true;
			word->tag = tag;
			word->insertedtick = cycles;
		}
	}
}

void Cache::Init()
{
	cacheWord* block;
	for (int i = 0; blockcount > i; i++)
	{
		block = (cacheWord*)calloc(wordcount, sizeof(cacheWord));
		cacheMem.push_back(block);
		for (int j = 0; wordcount > j; j++)
		{
			block[j] = cacheWord{ false, 0, 0 };
		}
	}
	offsetlen = (int)log2(wordcount);
	setlen = (int)(log2(blockcount)) - (int)log2(associativity);
	taglen = 32 - offsetlen - setlen;

	for (int i = 0; offsetlen > i; i++)
		offsetbitwise |= (1 << i);
	for (int i = offsetlen; (setlen + offsetlen) > i; i++)
		setbitwise |= (1 << i);
	for (int i = (offsetlen + setlen); 32 > i; i++)
		tagbitwise |= (1 << i);
}

void Cache::LoadFile()
{

	FILE *fp;
	char name[50];
	int roll_no, chars;
	float marks;

	fp = fopen(filename.c_str(), "r");

	printf("Attempting to open file %s\n", filename.c_str());
	if (fp == NULL)
	{
		printf("Error opening file\n");
		exit(1);
	}
	printf("Reading file...\n");

	uint32_t readHex = 0;

	while (fscanf(fp, "0x%x\n", &readHex) == 1)
	{
		//printf("Read: %x\n", readHex);
		addresses.push_back(readHex);
	}
	fclose(fp);
	dataSize = addresses.size();
	cacheMax = (int)powf(2, ((log2f(dataSize)) + 1)) + 1;
}

void Cache::PrintResults()
{
	printf("Cache size : %dkb\n", 32*cacheMax/1000);
	printf("Reads : %d\n", dataSize);
	printf("Hits : %d\n", hits);
	printf("Misses : %d\n", misses);
	printf("Hit Rate : %0.2f%%\n", 100*((float)hits)/dataSize);
	printf("Miss Rate : %0.2f%%\n", 100*((float)misses) / dataSize);
}
