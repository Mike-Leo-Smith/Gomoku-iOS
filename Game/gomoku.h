//
// Created by Mike Smith on 2017/6/18.
//

#ifndef GOMOKU_GOMOKU_H
#define GOMOKU_GOMOKU_H

namespace watery {
    
#include <cstdint>
#include <cstring>
    
    constexpr int ROWS = 8;
    constexpr int COLS = 8;
    
    typedef uint_fast8_t Row;
    typedef Row Board[2][ROWS];
    
    enum Player
    {
        BLACK = 0,
        WHITE = 1
    };
    
    enum GameState
    {
        IN_GAME = -1,
        BLACK_WIN = 0,
        WHITE_WIN = 1,
        TIE = 2
    };
    
    class Gomoku
    {
    private:
        int _count = 0;
        Board _board = {};
        Player _player = BLACK;
        GameState _state = IN_GAME;
        
        void update_state(int last_row, int last_col);
        
    public:
        Gomoku(Player first = BLACK) { reset(first); }
        ~Gomoku() = default;
        Gomoku(const Gomoku &) = default;
        Gomoku(Gomoku &&) = default;
        Gomoku &operator=(const Gomoku &) = default;
        Gomoku &operator=(Gomoku &&) = default;
        
        void reset(Player first);
        void print() const;
        void random_step();
        bool place_piece(int row, int col);
        int get_piece(int row, int col);
        bool available(int row, int col) const;
        Player player() const { return _player; }
        GameState state() const { return _state; }
        
        static int rows() { return ROWS; }
        static int cols() { return COLS; }
    };
}

#endif  // GOMOKU_GOMOKU_H
