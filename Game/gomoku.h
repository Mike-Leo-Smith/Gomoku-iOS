//
// Created by Mike Smith on 2017/6/18.
//

#ifndef GOMOKU_GOMOKU_H
#define GOMOKU_GOMOKU_H

namespace watery {
    
#include <cstdint>
#include <cstring>
    
    constexpr int ROWS = 10;
    constexpr int COLS = 10;
    
    typedef uint_fast16_t Row;
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
        const int _rows;
        const int _cols;
        
        void update_state(int last_row, int last_col);
        
    public:
        Gomoku(int rows = ROWS, int cols = COLS, Player first = BLACK) : _rows(rows), _cols(cols) { reset(first); }
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
        
        int rows() { return _rows; }
        int cols() { return _cols; }
    };
}

#endif  // GOMOKU_GOMOKU_H
