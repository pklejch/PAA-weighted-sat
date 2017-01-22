/* 
 * File:   CClause.cpp
 * Author: Petr Klejch (klejcpet@fit.cvut.cz)
 * 
 * Created on 20. ledna 2017, 14:11
 */

#include "TClause.h"

TClause::TClause(int a, int b, int c) {
    m_variables[0] = a;
    m_variables[1] = b;
    m_variables[2] = c;
}