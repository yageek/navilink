#include <stdio.h>
#include <stdlib.h>

#include "NaviGPSapi.h"



#define PRINT_SIZE(x) printf("Size of a "#x": %d\n",sizeof(x))
#define PRINT_STRING(x) printf(#x":%s\n",x);
#define PRINT_NUMBER(x) printf(#x":%d\n",x);

int main()
{
	Byte param[5] = {1,2,3,4,5};
	Byte *ptr = param;
	PRINT_NUMBER(ptr[1]);
	
	return 0;
}

