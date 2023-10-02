#include <cstdio>
#include <cstdlib>
#include <cstdint>

int32_t main(int32_t argc,char* argv[])
{
	for (uint16_t i = 0; i < argc; i++)
	{
		printf(argv[i]);
		
		printf("\n");
	}

	system("pause");
	return 0;
}