#include <iostream>
#include <regex>
#include <string>
#include <unordered_map>
#include <utility>


// Key: car plate, Values: end time in minutes and boolean whether the
// car is to be removed the next day (not as current day changes).
using car_base = std::unordered_map<std::string, std::pair<int, bool>>;



namespace {

    // Function converting string representing an hour, e.g. "17.45"
    // to number of minutes since 8.00.
    std::int32_t time_string_to_int(const std::string& time) {
        std::int32_t time_in_minutes{};
        const std::regex hours_and_minutes{"([1-9][0-9]|0?[0-9])\\.([0-9][0-9])"};
        std::smatch match{};
        std::regex_search(time, match, hours_and_minutes);
        std::int32_t hour = std::stoi(match[1]);
        std::int32_t minutes = std::stoi(match[2]);
        time_in_minutes = (hour - 8) * 60 + minutes;
        return time_in_minutes;
    }


    // Function that removes cars which are unpaid for from stored data.
    // This should only be called as the day changes.
    void remove_outdated_cars(car_base& cars) {
        std::vector<std::string> to_remove{};
        for (auto c : cars) {
                if (!c.second.second) {
                    to_remove.push_back(c.first);
                }
                else {
                    c.second.second = false;
                }
        }
        for (std::string name : to_remove) {
            cars.erase(name);
        }
    }


    // Function checking if the car request satisfies the
    // 10-minutes-minimum rule.
    bool at_least_ten_minutes(std::int32_t begin_time, std::int32_t end_time) {
        if (begin_time <= end_time) {
            return end_time - begin_time >= 10;
        }
        return end_time - begin_time + 60 * 12 >= 10;
    }
}


std::int32_t main() {
    
    // Regular expressions for identyfying payment for the parked car
    // and checking its status.
    std::string id = "([A-Z][A-Z0-9]{2,10})";
    std::string time = "((?:0?[89]|1[0-9])\\.(?:[0-5][0-9])|20\\.00)";
    const std::regex check_request{"\\s*"+ id + "\\s+" + time +"\\s*"};
    const std::regex add_request{"\\s*" + id + "\\s+" + time + "\\s+" + time +"\\s*"};
    
    car_base cars{};
    std::int32_t line_number{1};
    std::int32_t current_time{};
    std::string line;
   
    while (std::getline(std::cin, line)) {
        
        std::smatch match{};
       
        // Entering payment data.
        if (std::regex_match(line, match, add_request)) {
            std::string name = match[1];
            
            std::int32_t begin = time_string_to_int(match[2]);
            if (current_time > begin) {
                remove_outdated_cars(cars);
            }
            current_time = begin;
            bool does_it_end_tomorrow = false;
            std::int32_t end = time_string_to_int(match[3]);
            if (end < begin) {
                does_it_end_tomorrow = true;
            }
            
            if (at_least_ten_minutes(begin, end)) {
                std::cout << "OK " << line_number << '\n';
                std::int32_t is_the_car_in_system = static_cast<int>(cars.count(name));
                if (is_the_car_in_system == 0) {
                    std::pair<int, bool> time_data =
                    {end, does_it_end_tomorrow};
                    cars.insert(std::make_pair(name, time_data));
                }
                else {
                    std::int32_t current_end_time{cars[name].first};
                    bool current_if_removed_tomorrow{cars[name].second};
                    if ((current_if_removed_tomorrow == does_it_end_tomorrow
                         && current_end_time < end)
                        || (!current_if_removed_tomorrow
                            && does_it_end_tomorrow)) {
                        cars[name] = std::make_pair(end, does_it_end_tomorrow);
                    }
                }
            }
            else {
                std::cerr << "ERROR " << line_number << '\n';
            }
        }
        
        // Checking payment data.
        else if (std::regex_match(line, match, check_request)) {
            std::string name = match[1];
            std::int32_t check_time = time_string_to_int(match[2]);
            if (current_time > check_time) {
                remove_outdated_cars(cars);
            }
            current_time = check_time;
            std::int32_t is_the_car_in_system = static_cast<int>(cars.count(name));
            if (is_the_car_in_system == 0) {
                std::cout << "NO " << line_number << '\n';
            }
            else {
                std::int32_t end_time = cars[name].first;
                bool does_it_end_tomorrow = cars[name].second;
                if (does_it_end_tomorrow || current_time < end_time) {
                    std::cout << "YES " << line_number << '\n';
                }
                else {
                    std::cout << "NO " << line_number << '\n';
                }
            }
        }
        // Unidentified input.
        else {
            std::cerr << "ERROR " << line_number << '\n';
        }
        line_number++;
    }
    return 0;
    
}
