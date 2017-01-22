/* 
 * File:   CParser.h
 * Author: Petr Klejch (klejcpet@fit.cvut.cz)
 *
 * Created on 20. ledna 2017, 14:06
 */

#ifndef CPARSER_H
#define	CPARSER_H

#include "CFormula.h"

class CParser {
public:
    CParser(const char * filename, const char * wfilename);
    void parse(CFormula & formula);
private:
    const char * m_filename;
    const char * m_wfilename;
    int m_numberOfVariables;
    int m_numberOfClauses;
};

#endif	/* CPARSER_H */

