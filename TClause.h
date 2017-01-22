/* 
 * File:   CClause.h
 * Author: Petr Klejch (klejcpet@fit.cvut.cz)
 *
 * Created on 20. ledna 2017, 14:11
 */

#ifndef CCLAUSE_H
#define	CCLAUSE_H

#define NUMBER_OF_LITERALS 3

struct TClause {
public:
    TClause(int a, int b, int c);
    int m_variables[NUMBER_OF_LITERALS];
};

#endif	/* CCLAUSE_H */

