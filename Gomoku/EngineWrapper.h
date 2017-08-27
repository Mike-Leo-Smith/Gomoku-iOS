//
//  EngineWrapper.h
//  Gomoku
//
//  Created by Mike Smith on 26/08/2017.
//  Copyright © 2017 Mike Smith. All rights reserved.
//

#import <Foundation/Foundation.h>

struct BoardPoint{
    int row;
    int col;
};

@interface EngineWrapper: NSObject

+ (void)resetGame: (int)firstPlayer;
+ (int)currentPlayer;
+ (int)getPieceAtRow: (int)row col: (int)col;
+ (void)placePieceAtRow: (int)row col: (int)col;
+ (int)gameState;
+ (struct BoardPoint)searchWithAI;
+ (int)cols;
+ (int)rows;

@end
