#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <climits>

long long gcd(long long a, long long b);

long long gcd(long long a, long long b) {

    if ( a == 0 )
        return b;
    
    if ( b == 0 )
        return a;

    if ( a > b )
        return gcd(b, a % b);

    return gcd(a, b % a);
}

int main () {

    //std::string textline;
    long long gcd_r = 0;
    std::string singlenum;
    std::vector<std::string> numbers;
    std::vector<long long> num64;
    std::ofstream ofile;
    ofile.exceptions(std::ofstream::badbit | std::ofstream::failbit);
    std::ifstream ifile;
    ifile.exceptions(std::ifstream::badbit | std::ifstream::failbit);

    try {

        ifile.open("input.txt");

        if (ifile) {

            std::stringstream ss;
            ss << ifile.rdbuf();
            //textline = ss.str();
            //std::cout << "Text: " << textline << std::endl;
            while ( getline(ss, singlenum, ' ') ) {

                numbers.push_back(singlenum);
            }
        }

        if (numbers.size() == 2) {

            for (int i = 0; i < numbers.size(); i++) {

                //std::cout << numbers[i] << std::endl;
                int n = 0;
                long long c = 0;
                for (char c : numbers[i]) {
            
                    if ( !(c >= '0' && c <= '9') ) {

                        std::cout << "Not an integer" << std::endl;
                        return 1;
                    }
                }

                try {

                    c = std::stoll(numbers[i]);
                    num64.push_back(c);
                } catch (const std::out_of_range& oor) {

                    std::cerr << "Out of Range error: " << oor.what() << std::endl;
                } catch (const std::invalid_argument& ia) {

                    std::cerr << "Invalid Argument error: " << ia.what() << std::endl;
                }

                std::cout << num64[i] << std::endl;
            }
            gcd_r = gcd(num64[0], num64[1]);
            std::cout << "GCD = " << gcd_r << std::endl;

        } else {

            std::cerr << "Exactly two numbers are needed" << std::endl;
        }
    }
    catch (const std::ifstream::failure& e) {

        std::cout << "Exception opening/reading input file";
    }

    try {

        ofile.open("output.txt");

        if ( ofile.is_open() ) {

            ofile << gcd_r;
        }

    }
    catch (const std::ofstream::failure& e) {

        std::cout << "Exception opening/reading output file";
    }

    ofile.close();
    ifile.close();

    return 0;
}
