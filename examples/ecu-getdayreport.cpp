#include "ecuAPI.hpp"
#include <iostream>
#include <string.h>
#include <ctime>


int main(int argc, char *argv[])
{
	if ( (argc < 2) || ( (argc == 3) &&(strlen(argv[2]) != 10) ) )
	{
		std::cout << "Usage: ecu-getdayreport <ECU address> [ISO-date]\n";
		return 1;
	}

	int year;
	uint8_t month;
	uint8_t day;

	if (argc == 3)
	{
		char syear[5] = {0,0,0,0,0};
		char smonth[3] = {0,0,0};
		char sday[3] = {0,0,0};
		char* source = argv[2];
		bcopy(source, syear, 4);
		source += 5;
		bcopy(source, smonth, 2);
		source += 3;
		bcopy(source, sday, 2);
		year = atoi(syear);
		month = static_cast <uint8_t>(atoi(smonth));
		day = static_cast <uint8_t>(atoi(sday));
	}
	else
	{
		time_t now = time(NULL);
		struct tm * tt;
		tt = localtime(&now);
		year = tt->tm_year + 1900;
		month = static_cast <uint8_t>(tt->tm_mon + 1);
		day = static_cast <uint8_t>(tt->tm_mday);
	}


	int statuscode;
	ecuAPI *ecuclient;
	ecuclient = new ecuAPI();

	ecuclient->SetTargetAddress(argv[1]);
	
	statuscode = ecuclient->QueryECU();
	if (statuscode != 0)
	{
		if (statuscode == -1)
			std::cout << "QueryECU failed connect with error " << errno << "\n";
		else if (statuscode == 1)
			std::cout << "Received invalid data from QueryECU\n";
		return statuscode;
	}

	std::string jsondata;

	ecuclient->GetDayReport(year, month, day, jsondata);
	std::cout << " dayreport : "<< jsondata;

	return 0;
}
