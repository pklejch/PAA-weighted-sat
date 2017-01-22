/* 
 * File:   CFormula.h
 * Author: Petr Klejch (klejcpet@fit.cvut.cz)
 *
 * Created on 20. ledna 2017, 14:11
 */

#ifndef CFORMULA_H
#define	CFORMULA_H

#include "TClause.h"
#include <vector>
#include "ga/GA1DBinStrGenome.h"
#include "ga/GAGenome.h"

class CFormula {
public:
    CFormula();
    virtual ~CFormula();
    void addClause(TClause clause);
    void addWeight(int weight);
    void set(int numberOfClauses, int numberOfVariables);
    void debug();
    int getNumberOfVariables();
    int getNumberOfClauses();
    std::vector<TClause> getClauses();
    std::vector<int> getWeights();
    float fitness(const GAGenome & g);
    float fitness2(const GAGenome & g);
    float fitness3(const GAGenome & g);
    float adaptiveFitness(const GAGenome &g);
    void solve(int popSize, int generations, float probOfCrossover, float propOfMutation);
    void solveBFRec(int * configuration, int depth);
    void solveBF();
private:
    int flipBit(int bit);
    std::vector<TClause> m_clauses;
    std::vector<int> m_weights;
    int m_numberOfClauses;
    int m_numberOfVariables;
    int m_bestWeight;
    bool testSAT(GA1DBinaryStringGenome & g);
    bool testSAT(int * configuration);
    bool testClause(GA1DBinaryStringGenome& g, TClause c);
    bool testClause(int * configuration, TClause c);
    int * m_bestConfiguration;
};

#endif	/* CFORMULA_H */

