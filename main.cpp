#include <iostream>
#include <stdlib.h>

#include <string>
#include <fstream>
#include "CParser.h"
#include "CFormula.h"

int main(int argc, char ** argv){
    
    if (argc != 3){
        std::cout << "ERROR" << std::endl;
        return (EXIT_FAILURE);
    }
    
    CParser parser = CParser(argv[1],argv[2]);
    CFormula formula = CFormula();
    
    parser.parse(formula);
    
    formula.debug();
    
    std::ofstream time("time.txt");
    
    clock_t begin = clock();
    formula.solve(500,10000,0.3,0.03);
    
    //formula.solveBF();
    clock_t end = clock();
    double len = double(end-begin)/CLOCKS_PER_SEC;
    time << len << std::endl;
    
    time.close();
    
    return(EXIT_SUCCESS);
}