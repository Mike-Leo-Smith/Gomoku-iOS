//
// Created by Mike Smith on 2017/6/18.
//

#ifndef GOMOKU_MCT_H
#define GOMOKU_MCT_H

#include "../Clock/clock.h"
#include "../Game/gomoku.h"

namespace watery {
    
    struct MCTNode
    {
        Gomoku game;
        int profit;
        int total;
        
        MCTNode *parent;
        MCTNode *children[ROWS][COLS];
    };
    
    struct Point
    {
        int row;
        int col;
        
        explicit Point(int row = 0, int col = 0) : row(row), col(col) {}
    };
    
    class MCT
    {
    private:
        static MCTNode *tree_policy(MCTNode *node) noexcept;
        static int default_policy(const Gomoku &game) noexcept;
        static MCTNode *create_node(MCTNode *parent, const Gomoku &game) noexcept;
        static void destroy_node(MCTNode *node) noexcept;
        static MCTNode *expand_node(MCTNode *node, int child_row, int child_col) noexcept;
        static MCTNode *best_child(MCTNode *node, double c) noexcept;
        static Point best_child_point(MCTNode *node, double c) noexcept;
        
    public:
        static struct Point uct_search(const Gomoku &game, time_t time);
    };
}

#endif  // GOMOKU_MCT_H
