#include <iostream>
#include <regex>
#include <string>
#include <unordered_map>
#include <utility>



using carBase = std::unordered_map<std::string, std::pair<int, bool>>;
//end time, does it end in the next day

int time_string_to_int(std::string time) {
    int time_in_minutes{};
    const std::regex hours_and_minutes{"([1-9][0-9]|0?[0-9])\\.([0-9][0-9])"};
    std::smatch match{};
    std::regex_search(time, match, hours_and_minutes);
    int hour = std::stoi(match[1]);
    int minutes = std::stoi(match[2]);
    time_in_minutes = (hour - 8) * 60 + minutes;
    return time_in_minutes;
}


void removeOutdatedCars(carBase& cars) {
    
    std::vector<std::string> toRemove{};
    for (auto c : cars) {
			if (!c.second.second) {
				toRemove.push_back(c.first);
			}
			else {
				c.second.second = false;
			}
    }
    for (std::string name : toRemove) {
        cars.erase(name);
    }
}
bool atLeastTenMinutes(int begin_time, int end_time) {
    if (begin_time <= end_time) {
        return end_time - begin_time >= 10;
    }
    return end_time - begin_time + 60 * 12 >= 10;
}


int main() {
    carBase cars{};
    const std::regex check_request{"\\s*([A-Z][A-Z0-9]{2,10})\\s*((?:0?[89]|1[0-9])\\.(?:[0-5][0-9])|20\\.00)\\s*"};
    const std::regex add_request{"\\s*([A-Z][A-Z0-9]{2,10})\\s*((?:0?[89]|1[0-9])\\.(?:[0-5][0-9])|20\\.00)\\s*((?:0?[89]|1[0-9])\\.(?:[0-5][0-9])|20\\.00)\\s*"};
    
    int line_number{1};
    int current_time{};
    std::string line;
    while(std::getline(std::cin, line)) {
        std::smatch match;
        // entering payment data
        if (std::regex_match(line, match, add_request)) {
            std::string name = match[1];
            
            int begin = time_string_to_int(match[2]);
            if (current_time > begin) {
							removeOutdatedCars(cars);
            }
            current_time = begin;
            bool does_it_end_tomorrow = false;
            int end = time_string_to_int(match[3]);
            if (end < begin) {
                does_it_end_tomorrow = true;
            }
            
            if(atLeastTenMinutes(begin, end)) {
                std::cout << "OK " << line_number << '\n';
                std::pair<int, bool> timeData =
                {end, does_it_end_tomorrow};
                cars.insert(std::make_pair(name, timeData));
            }
            else {
                std::cerr << "ERROR " << line_number << '\n';
            }
        }
        // checking payment data
        else if (std::regex_match(line, match, check_request)) {
            std::string name = match[1];
            int check_time = time_string_to_int(match[2]);
            if(check_time < current_time) {
							removeOutdatedCars(cars);
            }
            current_time = check_time;
            int is_the_car_in_system = static_cast<int>(cars.count(name));
            if(is_the_car_in_system == 0) {
                std::cout << "NO " << line_number << '\n';
            }
            else {
                int end_time = cars[name].first;
								bool does_it_end_tomorrow = cars[name].second;
                if (does_it_end_tomorrow || current_time < end_time) {
                    std::cout << "YES " << line_number << '\n';
                }
                else {
                    std::cout << "NO " << line_number << '\n';
                }
            }
        }
        else {
            std::cerr << "ERROR " << line_number << '\n';
        }
        line_number++;
    }
    return 0;
    
}
