#include <stdio.h>
#include <stdlib.h>

#include "api.h"
#include "endianness.h"


#define PRINT_SIZE(x) printf("Size of a "#x": %d\n",sizeof(x))
#define PRINT_STRING(x) printf(#x":%s\n",x);
#define PRINT_NUMBER(x) printf(#x":%s\n",x);

int main()
{
	NaviGPS * gps = initGPS("");
	if(getInfo(gps) < 0) printf("Error : %s", NavilinkError());
	else displayInfo(gps);
	
	
	

	
	
	
  return 0;
}

