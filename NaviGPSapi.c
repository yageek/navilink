#include "NaviGPSapi.h"




NaviGPS *get_new_GPS(const char* dev){
	
	
	
		NaviGPS * ptr = malloc(sizeof(NaviGPS));
	
		ptr->informations = malloc(sizeof(T_INFORMATION));
		ptr->waypoints = malloc(sizeof(T_WAYPOINT));
		ptr->routes = malloc(sizeof(T_ROUTE));
		ptr->tracks = malloc(sizeof(T_TRACKPOINT));
	
		strcpy(ptr->deviceName,dev);
		
		return ptr;
	

}


void display_gps_info(NaviGPS *dev){
	printf(" ===== Informations of %s ====\n",dev->deviceName);
	printf("\tTotal waypoints   : %20d\n",dev->informations->totalWaypoint);
	printf("\tTotal route       : %20d\n",dev->informations->totalRoute);
	printf("\tTotal track       : %20d\n",dev->informations->totalTrack);
	printf("\tAdressTrack (hex) : %20X\n",dev->informations->startAdrOfTrackBuffer);
	printf("\tDevice serial num : %20d\n",dev->informations->deviceSerialNum);
	printf("\tProtocol version  : %20d\n",dev->informations->protocolVersion);
	printf("\tUser name         : %20s\n",dev->informations->username);
}


void queryWaypoints(NaviGPS *dev,DoubleWord first, Word size ){


	
		
}
void free_GPS(NaviGPS * dev){
	/* Test if connected */
	free(dev);
	
}
