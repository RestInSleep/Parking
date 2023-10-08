#include <iostream>
#include <regex>
#include <string>
#include <unordered_map>



using carBase = std::unordered_map<std::string, std::tuple<int, int, int,int>>;
using iterator = std::unordered_map<std::string, std::tuple<int, int, int,int>>::iterator;
//start day, start time, end day, end time

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
void removeOutdatedCars(carBase& cars, int current_day,
                        int current_time) {
    for (iterator i = cars.begin(); i != cars.end(); i++) {
        int end_day = std::get<2>(i->second);
        int end_time = std::get<3>(i->second);
        if (current_day > end_day ||(current_day == end_day && current_time > end_time)) {
            cars.erase(i->first);
        }
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
    int current_day{1};
    int current_time{};
    while(!std::cin.eof()) {
        std::string line;
        std::getline(std::cin, line);
        std::smatch match;
        // entering payment data
        if (std::regex_match(line, match, add_request)) {
            std::string name = match[1];
            
            int begin = time_string_to_int(match[2]);
            if (current_time > begin) {
                current_day++;
            }
            current_time = begin;
            
            int end = time_string_to_int(match[3]);
            int start_day = current_day;
            
            int end_day = current_day;
            if (end < begin) {
                end_day++;
            }
            
            if(atLeastTenMinutes(begin, end)) {
                std::cout << "OK " << line_number << '\n';
                std::tuple<int, int, int, int> timeData =
                {start_day, begin, end_day, end};
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
                current_day++;
            }
            current_time = check_time;
            int is_the_car_in_system = static_cast<int>(cars.count(name));
            if(is_the_car_in_system == 0) {
                std::cout << "NO " << line_number << '\n';
            }
            else {
                int end_day = std::get<2>(cars[name]);
                int end_time = std::get<3>(cars[name]);
                if (current_day > end_day || (current_day == end_day && current_time > end_time)) {
                    std::cout << "NO " << line_number << '\n';
                }
                else {
                    std::cout << "YES " << line_number << '\n';
                }
            }
        }
        else {
            std::cerr << "ERROR " << line_number << '\n';
        }
        line_number++;
        removeOutdatedCars(cars, current_day, current_time);
    }
    return 0;
    
}
