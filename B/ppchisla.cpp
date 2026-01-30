
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <climits>
#include <string>

std::vector<int> primes(int l, int h);

std::vector<int> primes(int l, int h) {
    
    std::vector<bool> primelist( h + 1, true );
    for( int p = 2; p * p <= h; p++ ) {

        if ( primelist[p] == true ) {

            for ( int i = p * p; i <= h; i += p ) {

                primelist[i] = false;
            }
        }
    }


    std::vector<int> res;
    for ( int p = 2; p <= h; p++ ) {

        if ( primelist[p] ) {
            res.push_back(p);
        }
    }
    return res;
}

int main () {

    std::string singlenum;
    std::vector<std::string> str_intervalo;
    std::vector<int> intervalo;
    std::vector<int> nprimes;
    std::ifstream ifile;
    ifile.exceptions(std::ifstream::badbit | std::ifstream::failbit);
    std::ofstream ofile;
    ofile.exceptions(std::ofstream::badbit | std::ofstream::failbit);

    try {

        ifile.open("input.txt");

        if (ifile) {

            std::stringstream ss;
            ss << ifile.rdbuf();
            while ( getline(ss, singlenum, ' ') ) {

                str_intervalo.push_back(singlenum);
            }
        }

        if (str_intervalo.size() == 2) {

            for (int i = 0; i < str_intervalo.size(); i++) {

                int n = 0;
                long long c = 0;
                for (char c : str_intervalo[i]) {
            
                    if ( !(c >= '0' && c <= '9') ) {

                        std::cout << "Not an integer" << std::endl;
                        return 1;
                    }
                }

                try {

                    c = std::stoi(str_intervalo[i]);
                    intervalo.push_back(c);
                } catch (const std::out_of_range& oor) {

                    std::cerr << "Out of Range error: " << oor.what() << std::endl;
                } catch (const std::invalid_argument& ia) {

                    std::cerr << "Invalid Argument error: " << ia.what() << std::endl;
                }
            }
            nprimes = primes(intervalo[0], intervalo[1]);

        } else {

            std::cerr << "Exactly two numbers are needed" << std::endl;
        }


    } catch (const std::ifstream::failure& e) {

        std::cout << "Exception opening/reading input file";
    }

    try {

        ofile.open("output.txt");
        bool at_prime = false;

        if ( ofile.is_open() ) {

            for( auto ele : nprimes ){

                if ( ( ele >= intervalo[0] ) && ( ele <= intervalo[1] )  ) {

                    ofile << ele << ' ';
                    at_prime = true;
                }
            }

            if ( !at_prime ) {
                ofile << "-1" ;
            }
        }

    } catch (const std::ofstream::failure& e) {

        std::cout << "Exception opening/reading output file";
    }

    ofile.close();
    ifile.close();

    /*int n = 1000;
    std::vector<int> res = primes(n);
    for(auto ele : res){
        std::cout << ele << ' ';
    }
    std::cout << std::endl; */

    return 0;

}