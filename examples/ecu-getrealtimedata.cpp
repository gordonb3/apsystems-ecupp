#include "ecuAPI.hpp"
//#include <unistd.h>
#include <iostream>
#include <sstream>
#include <string.h>
//#include <json/json.h>
#include <ctime>


int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		std::cout << "Usage: ecu-getrealtimedata <ECU address>\n";
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

	std::cout << "ECU id: " << ecuclient->m_apsecu.id << "\n";
	std::cout << "ECU version: " << ecuclient->m_apsecu.version << "\n";

	std::cout << "life time energy: " << ecuclient->m_apsecu.lifetime_energy << " kWh\n";
	std::cout << "current power: " << ecuclient->m_apsecu.current_power << " Watt\n";
	std::cout << "today energy: " << ecuclient->m_apsecu.today_energy << " kWh\n";
	std::cout << "number of inverters online: " << ecuclient->m_apsecu.invertersonline << "/" << ecuclient->m_apsecu.numinverters << "\n";


	statuscode = ecuclient->QueryInverters();
	if (statuscode != 0)
	{
		if (statuscode == -1)
			std::cout << "QueryInverters failed connect with error " << errno << "\n";
		else if (statuscode == 1)
			std::cout << "Received invalid data from QueryInverters\n";
		return statuscode;
	}

	struct tm * tt;
	tt = localtime(&ecuclient->m_apsecu.timestamp);
	char timestring[20];
	strftime(timestring, 20, "%Y-%m-%d %H:%M:%S" , tt);
	std::cout << "timestamp: " << timestring << "\n";

	for (size_t i = 0; i < ecuclient->m_apsecu.inverters.size(); i++)
	{
	
		std::cout << "{\n	\"inverter ID\" : \"" << ecuclient->m_apsecu.inverters[i].id << "\",\n	\"online\" : " << (int)ecuclient->m_apsecu.inverters[i].online_status << ",\n	\"frequency\" : "
		          << ecuclient->m_apsecu.inverters[i].frequency << ",\n	\"temperature\" : " << ecuclient->m_apsecu.inverters[i].temperature << ",\n	\"channels\" : [";
		  
		size_t numchannels = ecuclient->m_apsecu.inverters[i].channels.size();
		for (size_t j = 0; j < numchannels; j++)
		{
			if (j > 0)
				std::cout << ",";
			std::cout << "\n			{\n				\"power\" : " << ecuclient->m_apsecu.inverters[i].channels[j].power
			          << ",\n				\"volt\" : " << ecuclient->m_apsecu.inverters[i].channels[j].volt << "\n			}";
	  
	  	}
	  
		if (numchannels > 0)
			std::cout << "\n			]\n}\n";
		else
			std::cout << "]\n}\n";

	}



	return 0;
}
