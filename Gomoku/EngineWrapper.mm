//
//  EngineWrapper.cpp
//  Gomoku
//
//  Created by Mike Smith on 26/08/2017.
//  Copyright © 2017 Mike Smith. All rights reserved.
//

#import "EngineWrapper.h"

#import "../Game/gomoku.h"
#import "../AI/mct.h"

static watery::Gomoku game;

@implementation EngineWrapper

+ (void)resetGame: (int)firstPlayer  {
    game.reset(watery::Player(firstPlayer));
}

+ (int)getPieceAtRow: (int)row col: (int)col {
    return game.get_piece(row, col);
}

+ (void)placePieceAtRow: (int)row col: (int)col {
    game.place_piece(row, col);
}

+ (int)gameState {
    return game.state();
}

+ (int)currentPlayer {
    return game.player();
}

+ (struct BoardPoint)searchWithAI {
    watery::Point result = watery::MCT::uct_search(game, 15000);
    game.place_piece(result.row, result.col);
    return BoardPoint{result.row, result.col};
}

+ (int)cols {
    return game.cols();
}

+ (int)rows {
    return game.rows();
}

@end
