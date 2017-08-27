//
// Created by Mike Smith on 26/08/2017.
//

#ifndef GOMOKU_LOGARITHM_H
#define GOMOKU_LOGARITHM_H

namespace watery {
    
    class Logarithm
    {
    private:
        static double *_log_table;
        static Logarithm *_instance;
        static constexpr int _log_table_max = 30000000;
        
        Logarithm();
        ~Logarithm();
        
    public:
        static double get_log(int x);
        
        Logarithm(Logarithm &&) = delete;
        Logarithm(const Logarithm &) = delete;
        Logarithm &operator=(Logarithm &&) = delete;
        Logarithm &operator=(const Logarithm &) = delete;
    };
}

#endif  // GOMOKU_LOGARITHM_H
