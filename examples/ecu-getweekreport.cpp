#include "ecuAPI.hpp"
#include <iostream>
#include <string.h>


int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		std::cout << "Usage: ecu-getweekreport <ECU address>\n";
		return 1;
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

	ecuclient->GetPeriodReport(0, jsondata);
	std::cout << " weekreport : "<< jsondata;
	
	return 0;
}
