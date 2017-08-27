//
// Created by Mike Smith on 2017/6/18.
//

#include <iostream>
#include <iomanip>
#include "gomoku.h"

namespace watery {
    
    void Gomoku::reset(Player first)
    {
        _count = 0;
        _player = first;
        _state = IN_GAME;
        
        memset(_board[0], 0, sizeof(_board[0]));
        memset(_board[1], 0, sizeof(_board[1]));
    }
    
    bool Gomoku::place_piece(int row, int col)
    {
        if (!available(row, col))
        {
            return false;
        }
        
        _board[_player][row] |= 1 << col;
        _count++;
        update_state(row, col);
        (int &)_player ^= 1;
        
        return true;
    }
    
    int Gomoku::get_piece(int row, int col)
    {
        if (row < 0 || row >= rows() || col < 0 || col >= cols()) {
            return -1;
        } else if (_board[0][row] & (1 << col)) {
            return Player::BLACK;
        } else if (_board[1][row] & (1 << col)){
            return Player::WHITE;
        } else {
            return -1;
        }
    }
    
    bool Gomoku::available(int row, int col) const
    {
        return (row >= 0 && row < rows()
                && col >= 0 && col < cols()
                && (!((_board[0][row] | _board[1][row]) & (1 << col))));
    }
    
    void Gomoku::update_state(int last_row, int last_col)
    {
        // Vertical.
        Row r = _board[_player][last_row];
        
        if (r & (r >> 1) & (r >> 2) & (r >> 3) & (r >> 4))
        {
            _state = (GameState)_player;
            return;
        }
        
        int min_row = (last_row >= 4 ? last_row - 4 : 0);
        int max_row = (last_row + 4 < rows() ? last_row + 4 : rows() - 1);
        
        int d = max_row - min_row - 3;
        
        for (int i = 0; i < d; i++)
        {
            Row r1 = _board[_player][min_row + i];
            Row r2 = _board[_player][min_row + i + 1];
            Row r3 = _board[_player][min_row + i + 2];
            Row r4 = _board[_player][min_row + i + 3];
            Row r5 = _board[_player][min_row + i + 4];
            
            // Horizontal.
            if (r1 & r2 & r3 & r4 & r5)
            {
                _state = (GameState)_player;
                return;
            }
            
            // Diagonal.
            if ((r1 & (r2 >> 1) & (r3 >> 2) & (r4 >> 3) & (r5 >> 4)) || (r1 & (r2 << 1) & (r3 << 2) & (r4 << 3) & (r5 << 4)))
            {
                _state = (GameState)_player;
                return;
            }
        }
        
        _state = (_count == rows() * cols()) ? TIE : IN_GAME;
        return;
    }
    
    void Gomoku::print(void) const
    {
        std::cout << "   ";
        for (int col = 0; col < cols(); col++)
        {
            std::cout << " " << std::hex << col;
        }
        std::cout << std::endl;
        
        std::cout << "   ";
        for (int col = 0; col < cols(); col++)
        {
            std::cout << " _";
        }
        std::cout << std::endl;
        
        for (int row = 0; row < rows(); row++)
        {
            std::cout << std::hex << row << "  |";
            
            for (int col = 0; col < cols(); col++)
            {
                if (_board[0][row] & (1 << col))
                {
                    std::cout << "X|";
                }
                else if (_board[1][row] & (1 << col))
                {
                    std::cout << "O|";
                }
                else
                {
                    std::cout << "_|";
                }
            }
            std::cout << std::endl;
        }
    }
    
    void Gomoku::random_step(void)
    {
        int rand_row = rand() % rows();
        int rand_col = rand() % cols();
        
        place_piece(rand_row, rand_col);
    }
}
