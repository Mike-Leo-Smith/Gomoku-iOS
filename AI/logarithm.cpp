//
// Created by Mike Smith on 26/08/2017.
//

#include <cmath>
#include "logarithm.h"

namespace watery {
    
    Logarithm *Logarithm::_instance = new Logarithm;
    double *Logarithm::_log_table = nullptr;
    
    Logarithm::Logarithm()
    {
        _log_table = new double[_log_table_max];
        for (int i = 0; i < _log_table_max; i++) {
            _log_table[i] = log(i);
        }
    }
    
    Logarithm::~Logarithm()
    {
        delete _log_table;
    }
    
    double Logarithm::get_log(int x)
    {
        if (x < _log_table_max) {
            return _log_table[x];
        }
        return log(x);
    }
}
