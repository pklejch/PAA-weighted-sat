/* 
 * File:   CParser.cpp
 * Author: Petr Klejch (klejcpet@fit.cvut.cz)
 * 
 * Created on 20. ledna 2017, 14:06
 */

#include <string>

#include "CParser.h"
#include "TClause.h"
#include "CFormula.h"
#include <fstream>
#include <sstream>

CParser::CParser(const char * filename, const char* wfilename):
        m_filename(filename), 
        m_wfilename(wfilename)  {}


void CParser::parse(CFormula & formula){
        // precti soubory ve formatu DIMACS s CNF
        std::string line;
        std::ifstream infile(m_filename);
        int a;
        int b;
        int c;
        int numberOfVariables;
        int numberOfClauses;
        char cmd;
        char type[3];
        
        // nacti radek
        while(getline(infile,line)){
            std::istringstream iss(line);
            
            // je to komentar
            if(line.c_str()[0] == 'c'){
                continue;
            }
            
            //je to info radka o velikosti klauzule
            if (line.c_str()[0] == 'p'){
                iss >> cmd;
                iss >> type;
                iss >> numberOfVariables;
                iss >> numberOfClauses;
                
                formula.set(numberOfClauses, numberOfVariables);
                continue;
            }
            
            //3CNF
            iss >> a;
            iss >> b;
            iss >> c;
            
            //vloz klauzuli do formule
            formula.addClause(TClause(a, b, c));
        }
        infile.close();
        
        
        // precti soubor s vahami
        std::ifstream infile2(m_wfilename);
        int weight;
        
        while(getline(infile2,line)){
            std::istringstream iss(line);
            iss >> weight;
            formula.addWeight(weight);
        }
        
        infile.close();
        
        
}