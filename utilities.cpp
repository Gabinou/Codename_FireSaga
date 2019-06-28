#include <stdio.h>
#include <iostream>
#include <vector>
#include <bits/stdc++.h> 
using namespace std;
std::vector<int> ids;



std::vector<int> ids_list() {
    // Want to make an lgorithm that prints all numbers whose sum of digits is moduloable by 7
    int digit;
    int ids;
    std::vector<int> out;
    for (int id = 1e6; id<9999999 ; id++) {
        // std::cout << id << "\n";
        int value = id;
        int digit_sum=0;
        int digits = log10((float)value) + 1; //this determines the number of digits
        // std::cout << id << "\n";
        // std::cout << digits << "\n";
        for (int i = digits - 1; i >= 0; i--) {
            int divisor = pow((float)10, i);
            int digit = value / divisor;
            digit_sum += digit;
            // std::cout << digit << "\n";
            value -= digit * divisor;

        }
    // std::cout << digit_sum << "\n";
    if ( digit_sum % 7 == 0 ) {
        out.push_back(id);
    }
    }
   return out;
};

int digit_sum(int value) {
    int sum = 0;
    int digits = log10((float)value) + 1; //this determines the number of digits
    for (int i = value - 1; i >= 0; i--) {
        int divisor = pow((float)10, i);
        int digit = value / divisor;
        sum += digit;
        // std::cout << digit << "\n";
        value -= digit * divisor;
        } 
    return sum;
};

int random_id() {
    uint32_t seed_val; 
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist(1e6, 1e7); // distribution in range [1, 6]

    int not_id = dist(rng);
    std::cout << not_id << "\n";
    int new_id = not_id + ((7 - digit_sum(not_id)) % 7);
    ::ids;
    // extern std::vector<int> ids;
    return new_id;
};

main() {
    
    std::vector<int> ids = {1, 2};
    int new_id = random_id();
    std::cout << new_id;
    // ids = ids_list();
    // // for (auto i: ids)
        // // std::cout << i << ' ';
    
    // std::ofstream outFile("idslist.txt");
    // for (const auto &e : ids) outFile << e << "\n";
    return 0 ;
}