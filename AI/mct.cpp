//
// Created by Mike Smith on 2017/6/18.
//

#include <cmath>
#include <iostream>
#include "mct.h"
#include "logarithm.h"

namespace watery {
    
    constexpr int iterations_per_simulation = 10;
    
    struct Point MCT::uct_search(const Gomoku &game, time_t time)
    {
        Clock clock;
        MCTNode *root = create_node(nullptr, game);
        
        clock.reset();
        while (clock.tick() < time) {
            for (int i = 0; i < 1000; i++) {
                MCTNode *node = tree_policy(root);
                int delta = default_policy(node->game);
                
                // Backup
                while (node != nullptr) {
                    node->total += iterations_per_simulation;
                    node->profit += delta;
                    delta = -delta;
                    node = node->parent;
                }
            }
        }
        
        Point best = best_child_point(root, 0);
        destroy_node(root);
        return best;
    }
    
    inline MCTNode *MCT::tree_policy(MCTNode *node) noexcept
    {
        while (node->game.state() == IN_GAME) {
            for (int row = 0; row < Gomoku::rows(); row++) {
                for (int col = 0; col < Gomoku::cols(); col++) {
                    if (node->children[row][col] == nullptr && node->game.available(row, col)) {
                        return expand_node(node, row, col);
                    }
                }
            }
            
            node = best_child(node, 0.5);
        }
        
        return node;
    }
    
    inline MCTNode *MCT::expand_node(MCTNode *node, int child_row, int child_col) noexcept
    {
        MCTNode *child = create_node(node, node->game);
        
        node->children[child_row][child_col] = child;
        
        child->game.place_piece(child_row, child_col);
        child->profit = default_policy(child->game);
        child->total = iterations_per_simulation;
        
        return child;
    }
    
    inline MCTNode *MCT::best_child(MCTNode *node, double c) noexcept
    {
        MCTNode *best_child = nullptr;
        double best_confidence = -INFINITY;
        
        for (auto &row : node->children) {
            for (auto child : row) {
                if (child != nullptr) {
                    double
                    confidence = child->profit / (double)child->total + c * sqrt(2 * Logarithm::get_log(node->total) / child->total);
                    
                    if (confidence >= best_confidence) {
                        best_confidence = confidence;
                        best_child = child;
                    }
                }
            }
        }
        
        return best_child;
    }
    
    inline Point MCT::best_child_point(MCTNode *node, double c) noexcept
    {
        int best_row = 0, best_col = 0;
        double best_confidence = -INFINITY;
        
        for (int row = 0; row < Gomoku::rows(); row++) {
            for (int col = 0; col < Gomoku::cols(); col++) {
                MCTNode *child = node->children[row][col];
                
                if (child != nullptr) {
                    double confidence =
                    child->profit / (double)child->total + c * sqrt(2 * Logarithm::get_log(node->total) / child->total);
                    
                    if (confidence >= best_confidence) {
                        best_confidence = confidence;
                        best_row = row;
                        best_col = col;
                    }
                }
            }
        }
        
        return Point(best_row, best_col);
    }
    
    inline int MCT::default_policy(const Gomoku &game) noexcept
    {
        // End of a game.
        if (game.state() != IN_GAME) {
            return static_cast<int>((int)game.state() != (int)game.player());
        }
        
        int parent_lose = 0;
        
        for (int i = 0; i < iterations_per_simulation; i++) {
            Gomoku game_copy = game;
            
            while (game_copy.state() == IN_GAME) {
                game_copy.random_step();
            }
            
            if ((int)game_copy.state() == (int)game.player()) {
                parent_lose++;
            }
        }
        
        return iterations_per_simulation - parent_lose;
    }
    
    inline MCTNode *MCT::create_node(MCTNode *parent, const Gomoku &game) noexcept
    {
        auto *node = new MCTNode;
        
        node->parent = parent;
        node->game = game;
        node->profit = 0;
        node->total = 0;
        memset(node->children, 0, sizeof(node->children));
        
        return node;
    }
    
    void MCT::destroy_node(MCTNode *node) noexcept
    {
        for (auto &row : node->children) {
            for (auto &col : row) {
                if (col != nullptr) {
                    destroy_node(col);
                }
            }
        }
        delete node;
    }
}
