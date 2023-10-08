#include <iostream>
#include <string>
#include <regex>


bool isPlateNameGood(std::string& plate) {
    
    
    
    return std::regex_match(plate, std::regex("\s*([A-Z][A-Z0-9]{2,10})\s*((0?[89]|1[0-9])\.([0-5][0-9])|20\.00)\s*((0?[89]|1[0-9])\.([0-5][0-9])|20\.00)\s*"));
}



int main() {
    return 0;
    
}
