/* 
 * File:   CFormula.cpp
 * Author: Petr Klejch (klejcpet@fit.cvut.cz)
 * 
 * Created on 20. ledna 2017, 14:11
 */

#include <vector>
#include <iostream>

#include "CFormula.h"
#include "ga/GASimpleGA.h"
#include "ga/GA1DBinStrGenome.h"

// globalni formule
CFormula *g_formula;

CFormula::CFormula(): m_bestWeight(0) {
}

CFormula::~CFormula() {
}

void CFormula::addClause(TClause clause){
    m_clauses.push_back(clause);
}

void CFormula::addWeight(int weight){
    m_weights.push_back(weight);
}

void CFormula::set(int numberOfClauses, int numberOfVariables){
    m_numberOfClauses = numberOfClauses;
    m_numberOfVariables = numberOfVariables;
}

void CFormula::debug(){
    std::cout << "*********************** DEBUG ***************************" << std::endl;
    std::cout << "Number of variables: " << m_numberOfVariables << std::endl;
    std::cout << "Number of clauses: " << m_numberOfClauses << std::endl;
    std::cout << "Formula: " << std::endl;
    for(unsigned int i = 0; i < m_clauses.size(); i++){
        int a = m_clauses.at(i).m_variables[0];
        int b = m_clauses.at(i).m_variables[1]; 
        int c = m_clauses.at(i).m_variables[2];
        
        bool neg_a = false;
        if(a<0){
            neg_a = true;
        }
        bool neg_b = false;
        if(b<0){
            neg_b = true;
        }
        bool neg_c = false;
        if(a<0){
            neg_c = true;
        }
        std::cout << "(x" << abs(a);
        if(neg_a){
            std::cout << "'";
        }
        std::cout << " + ";
        std::cout << "x" << abs(b);
        if(neg_b){
            std::cout << "'";
        }
        std::cout << " + ";
        std::cout << "x" << abs(c);
        if(neg_c){
            std::cout << "'";
        }
        std::cout << ")" << std::endl;
    }
    
    std::cout << "Weights of variables: " << std::endl;
    for(unsigned int i = 0; i < m_weights.size(); i++ ){
        std::cout << m_weights.at(i) << " ";
    }
    std::cout << std::endl;
    std::cout << "**********************************************************" << std::endl;
}

int CFormula::getNumberOfClauses(){
    return m_numberOfClauses;
}

int CFormula::getNumberOfVariables(){
    return m_numberOfVariables;
}

std::vector<int> CFormula::getWeights(){
    return m_weights;
}

std::vector<TClause> CFormula::getClauses(){
    return m_clauses;
}



float fitnessWrapper(GAGenome& g){
    return g_formula->fitness(g);
}

float fitnessWrapper2(GAGenome& g){
    return g_formula->fitness2(g);
}

float fitnessWrapper3(GAGenome& g){
    return g_formula->fitness3(g);
}

float fitnessWrapper4(GAGenome& g){
    return g_formula->adaptiveFitness(g);
}

int CFormula::flipBit(int bit){
    if(!bit){
        return 1;
    }else{
        return 0;
    }
}

// fitness funkce podle poctu dosazenych bitu 1
float CFormula::fitness3(const GAGenome & g){
    float scale = 0.5;
    float score = 0;
    int numberOfSATClauses = 0;
    int weight = 0;
    int totalWeight = 0;
    GA1DBinaryStringGenome & genome = (GA1DBinaryStringGenome &)g;
    
    // pro vsechny klauzule
    for(unsigned int i = 0; i < m_clauses.size(); i++){
        if(testClause(genome,m_clauses.at(i))){
            numberOfSATClauses++;
        }
    }
    
    // calculate weight
    for(int i = 0; i < m_numberOfVariables; i++){
        weight += genome.gene(i) * m_weights.at(i);
        totalWeight += m_weights.at(i);
    }
    
    score = scale * ((float)weight/totalWeight) + (1-scale)*((float)numberOfSATClauses/m_clauses.size());
    return score;
}

// fitness funkce adaptivni
float CFormula::adaptiveFitness(const GAGenome & g){
    unsigned int numberOfSATClauses = 0;
    int weight = 0;
    int totalWeight = 0;
    GA1DBinaryStringGenome & genome = (GA1DBinaryStringGenome &)g;
    
    // pro vsechny klauzule
    for(unsigned int i = 0; i < m_clauses.size(); i++){
        if(testClause(genome,m_clauses.at(i))){
            numberOfSATClauses++;
        }
    }
    
    //dokud nenajdu splnitelnou formuli, vracej pomer #splnenych formuli/#vsech formuli
    //az najdes splnitelnou formuli, vracej 1 + vaha promennych/celkova vaha
    if(numberOfSATClauses != m_clauses.size()){
        return ((float)numberOfSATClauses/m_clauses.size());
    }else{
        // spocti vahu formule
        for(int i = 0; i < m_numberOfVariables; i++){
            weight += genome.gene(i) * m_weights.at(i);
            totalWeight += m_weights.at(i);
        }
        return 1 + ((float)weight/totalWeight);
    }
}

// funkce spocita fitness jako pocet splnitelnych formuli
float CFormula::fitness2(const GAGenome & g){
    int numberOfSATClauses = 0;
    GA1DBinaryStringGenome & genome = (GA1DBinaryStringGenome &)g;
    // pro vsechny klauzule
    for(unsigned int i = 0; i < m_clauses.size(); i++){
        if(testClause(genome,m_clauses.at(i))){
            numberOfSATClauses++;
        }
    }
    return numberOfSATClauses;
}

// funkce spocita vahu splnitelne formule
// pokud neni splnitelna vrati 0
float CFormula::fitness(const GAGenome& g){
    GA1DBinaryStringGenome & genome = (GA1DBinaryStringGenome &)g;
    int weight = 0;
    
    // formule neni splnitelna v tomto ohodnoceni = trest smrti
    if(!testSAT(genome)){
        return 0;
    }
    
    // calculate weight
    for(int i = 0; i < m_numberOfVariables; i++){
        weight+=genome.gene(i) * m_weights.at(i);
    }
    
    if (weight>m_bestWeight){
        m_bestWeight = weight;
        //std::cout << "Found new best weight: " << weight << std::endl;
    }
    
    return weight;
}
bool CFormula::testClause(GA1DBinaryStringGenome& g, TClause clause){
        int a = clause.m_variables[0];
        int b = clause.m_variables[1];
        int c = clause.m_variables[2];
        
        int bit_a = g.gene(abs(a)-1); 
        if (a < 0){
            bit_a = flipBit(bit_a);
        }
        
        
        int bit_b = g.gene(abs(b)-1);
        if (b < 0){
            bit_b = flipBit(bit_b);
        }
        
        int bit_c = g.gene(abs(c)-1);
        if (c < 0){
            bit_c = flipBit(bit_c);
        }
        
        //klauzule neni splnitelna v tomto ohodnoceni
        if (!(bit_a | bit_b | bit_c)){
            return false;
        }
        else{
            return true;
        }
}

bool CFormula::testClause(int * configuration, TClause clause){
        int a = clause.m_variables[0];
        int b = clause.m_variables[1];
        int c = clause.m_variables[2];
        
        int bit_a = configuration[abs(a)-1]; 
        if (a < 0){
            bit_a = flipBit(bit_a);
        }
        
        
        int bit_b = configuration[abs(b)-1];
        if (b < 0){
            bit_b = flipBit(bit_b);
        }
        
        int bit_c = configuration[abs(c)-1];
        if (c < 0){
            bit_c = flipBit(bit_c);
        }
        
        //klauzule neni splnitelna v tomto ohodnoceni
        if (!(bit_a | bit_b | bit_c)){
            return false;
        }
        else{
            return true;
        }
}
bool CFormula::testSAT(GA1DBinaryStringGenome& g){
    for(unsigned int i = 0; i < m_clauses.size(); i++){
        if(!testClause(g, m_clauses.at(i))){
            return false;
        }
    }
    return true;
}

bool CFormula::testSAT(int * configuration){
    for(unsigned int i = 0; i < m_clauses.size(); i++){
        if(!testClause(configuration, m_clauses.at(i))){
            return false;
        }
    }
    return true;
}

void CFormula::solveBFRec(int * configuration, int depth){
    //pokud se jiz dosahlo maximalni hloubky n
    if(depth==m_numberOfVariables){
        int weight=0;

        //vypoctu cenu formule
        for(int i=0;i<depth;i++){
            weight += configuration[i]*m_weights[i];
        }

        //pokud je formule splnitelna a ma lepsi vahu, uloz ji
        if((testSAT(configuration)) && (weight > m_bestWeight)){
            m_bestConfiguration = new int[m_numberOfVariables];
            m_bestWeight = weight;
            for(int i=0;i<m_numberOfVariables;i++){
                m_bestConfiguration[i]=configuration[i];
            }
        }
        delete [] configuration;
        return;
    }

    //prekopirovani vysledku z rodicovskeho uzlu
    int * configuration0 = new int[m_numberOfVariables];
    int * configuration1 = new int[m_numberOfVariables];
    for(int i=0;i<depth;i++){
        configuration0[i]=configuration[i];
        configuration1[i]=configuration[i];
    }

    //doplneni novych moznosti konfigurace
    configuration0[depth]=0;
    configuration1[depth]=1;

    //opakuj rekurzi
    depth++;
    solveBFRec(configuration0,depth);
    solveBFRec(configuration1,depth);

    delete [] configuration;
}

void CFormula::solveBF(){
    int * configuration = new int[m_numberOfVariables];
    for(int i = 0; i < m_numberOfVariables; i++){
        configuration[i] = 0;
    }
    solveBFRec(configuration,0);
    std::cout << "******************** RESULTS FROM BF *********************" << std::endl;
    std::cout << "Best found weight: " << m_bestWeight << std::endl;
    std::cout << "Configuration: "; 
    for(int i = 0; i < m_numberOfVariables; i++){
        std::cout << m_bestConfiguration[i];
    }
    std::cout << std::endl;
}

void CFormula::solve(int popSize, int generations, float probOfCrossover, float probOfMutation){
    
    g_formula = this;
    
    GARandomSeed();
    GA1DBinaryStringGenome genome(m_numberOfVariables, fitnessWrapper4);
    
    GASimpleGA ga(genome);
    
    ga.populationSize(popSize);
    ga.nGenerations(generations);
    ga.pMutation(probOfMutation);
    ga.pCrossover(probOfCrossover);
    ga.nBestGenomes(popSize);
    
    //ga.crossover(GA1DBinaryStringGenome::OnePointCrossover);
    //ga.crossover(GA1DBinaryStringGenome::TwoPointCrossover);
    ga.crossover(GA1DBinaryStringGenome::UniformCrossover);
    
    // zmena z ruletove selekce na turnajovou
    ga.selector(GATournamentSelector());
    
    // vypnuti elitismu
    //ga.elitist(GABoolean::gaFalse);
    
    ga.evolve();

    
    std::cout << "******************** RESULTS FROM GA *********************" << std::endl;
    // Nejlepsi jedinec
    bool satisfiable = false;
    int bestWeight = 0;
    GA1DBinaryStringGenome best_genome = GA1DBinaryStringGenome(0);
    GAPopulation pop = ga.statistics().bestPopulation();
    for(int i = 0; i < pop.size(); i++){
        int weight = 0;
        GAGenome & g = pop.individual(i);
        GA1DBinaryStringGenome & genome = (GA1DBinaryStringGenome &)g;
        if(!testSAT(genome)){
            continue;
        }else{
            satisfiable = true;
        }
        for(int j = 0; j < m_numberOfVariables; j++){
            weight += genome.gene(j) * m_weights.at(j);
        }
        if (weight > bestWeight){
            bestWeight = weight;
            m_bestWeight = weight;
            best_genome = genome;
        }
        std::cout << genome.fitness() << std::endl;
    }        
    if(satisfiable){
        std::cout << "The best configuration found: " << best_genome << std::endl;
        std::cout << "With weight: " << m_bestWeight << std::endl; 
    }else{
        std::cout << "No solution was found." << std::endl;
    }
    std::cout << ga.statistics();
}