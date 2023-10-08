#include <iostream>
#include <regex>
#include <string>
#include <unordered_map>

int time_string_to_int(std::string time) {
	int time_in_minutes;
	const std::regex hours_and_minutes("([1-9][0-9]|0?[0-9])\\.([0-9][0-9])");
	std::smatch match;
	std::regex_search(time, match, hours_and_minutes);
	int hour = std::stoi(match[1]);
	int minutes = std::stoi(match[2]);
	time_in_minutes = (hour-8) * 60 + minutes;
	return time_in_minutes;
} 


int main() {
	const std::regex check_request("\\s*([A-Z][A-Z0-9]{2,10})\\s*((?:0?[89]|1[0-9])\\.(?:[0-5][0-9])|20\\.00)\\s*"); 
	const std::regex add_request("\\s*([A-Z][A-Z0-9]{2,10})\\s*((?:0?[89]|1[0-9])\\.(?:[0-5][0-9])|20\\.00)\\s*((?:0?[89]|1[0-9])\\.(?:[0-5][0-9])|20\\.00)\\s*"); 
	
	int line_number = 0;
	while(!std::cin.eof()) {
		std::string line;
		std::getline(std::cin, line);
		std::smatch match;
		int current_time; //minuty od 08.00
		if (std::regex_match(line, match, add_request)) {
			std::string name = match[1];
			int begin = time_string_to_int(match[2]);
			int end = time_string_to_int(match[3]);
			std::cout << "OK " << line_number << std::endl;
		}
		else if (std::regex_match(line, match, check_request)) {
			std::string name = match[1];
			int check_time = time_string_to_int(match[2]);
			std::cout << "CHECK " << line_number << std::endl;

		}
		else {
			std::cout << "ERROR " << line_number << std::endl;
		}
		line_number++;
	}
	return 0;
    
}
