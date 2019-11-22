#include <iostream>
#include <string>
#include "Cache.h"
int main(int argc, char *argv[])
{
	if (argc == 6)
	{
		std::string filename(argv[1], (argv[2] - argv[1]));
		Cache cache(filename, atoi(argv[2]), atoi(argv[3]), atoi(argv[4]), std::string(argv[5]).find("lru") == std::string::npos);
		cache.Run();
	}
	else
		std::cout << "cache-simulator [filename] [blocksize] [blockcount] [associativity] [lru or fifo]\n";
	system("PAUSE");
	return 0;
}