//
//  ViewController.swift
//  Gomoku
//
//  Created by Mike Smith on 26/08/2017.
//  Copyright © 2017 Mike Smith. All rights reserved.
//

import UIKit

class ViewController: UIViewController {
    
    @IBOutlet weak var pixelView: PixelView! {
        didSet {
            pixelView.backgroundColor = UIColor(displayP3Red: 0.6, green: 0.7, blue: 0.8, alpha: 1.0)
            
            let tapRecognizer = UITapGestureRecognizer(target: self, action: #selector(tagGestureResponse))
            tapRecognizer.numberOfTouchesRequired = 1
            pixelView.addGestureRecognizer(tapRecognizer)
            renderGameBoard()
        }
    }
    
    override func viewDidLoad() {
        super.viewDidLoad()
        EngineWrapper.resetGame(Int32(Player.black.rawValue))
        // Do any additional setup after loading the view, typically from a nib.
    }
    
    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
        // Dispose of any resources that can be recreated.
    }
    
    func tagGestureResponse(reactingTo tapRecognizer: UITapGestureRecognizer) {
        if !aiSearching && tapRecognizer.state == .ended && gameState == .inGame {
            let col = Int32(tapRecognizer.location(in: pixelView).x / pixelView.scale)
            let row = Int32(tapRecognizer.location(in: pixelView).y / pixelView.scale)
            print("Placed piece at(Row: \(row), Col: \(col))")
            EngineWrapper.placePiece(atRow: row, col: col)
            renderGameBoard()
            if (gameState == .inGame) {
                findAISolution()
            }
        }
    }
    
    @IBAction func resetGame(_ sender: UIButton) {
        EngineWrapper.resetGame(Int32(Player.black.rawValue))
        renderGameBoard()
    }
    
    @IBOutlet weak var waitingIndicator: UIActivityIndicatorView!
    var aiSearching = false
    
    func findAISolution() {
        if gameState == .inGame {
            waitingIndicator.startAnimating()
            aiSearching = true
            DispatchQueue.global(qos: .userInitiated).async { [weak self] in
                let solution = EngineWrapper.searchWithAI()
                EngineWrapper.placePiece(atRow: solution.row, col: solution.col)
                DispatchQueue.main.async {
                    self?.renderGameBoard()
                    self?.aiSearching = false
                    self?.waitingIndicator.stopAnimating()
                }
            }
        }
    }
    
    enum Player: Int, CustomStringConvertible {
        case black = 0
        case white = 1
        case neither = -1
        
        var description: String {
            switch self {
            case .black:
                return "Black"
            case .white:
                return "White"
            default:
                return "Neither"
            }
        }
    };
    
    @IBOutlet weak var gameStateLabel: UILabel!
    
    var gameState: GameState {
        return GameState(rawValue: Int(EngineWrapper.gameState())) ?? .unknown
    }
    
    var currentPlayer: Player {
        return Player(rawValue: Int(EngineWrapper.currentPlayer())) ?? .neither
    }
    
    enum GameState: Int, CustomStringConvertible {
        case inGame = -1
        case blackWins = 0
        case whiteWins = 1
        case tie = 2
        case unknown = 3
        
        var description: String {
            switch self {
            case .inGame:
                return "In game... "
            case .blackWins:
                return "Black wins!"
            case .whiteWins:
                return "White wins!"
            case .tie:
                return "Tie!"
            case .unknown:
                return "I don't know what's going on..."
            }
        }
    };
    
    func renderGameBoard() {
        pixelView.clear()
        for row in 0..<EngineWrapper.rows() {
            for col in 0..<EngineWrapper.rows() {
                let piece = Player(rawValue: Int(EngineWrapper.getPieceAtRow(row, col: col)))
                if piece == Player.black {
                    pixelView.setPixel(at: CGPoint(x: Int(col), y: Int(row)), to: UIColor.black)
                } else if piece == Player.white {
                    pixelView.setPixel(at: CGPoint(x: Int(col), y: Int(row)), to: UIColor.white)
                }
            }
        }
        pixelView.scale = pixelView.bounds.width / CGFloat(EngineWrapper.cols())
        pixelView.render()
        if gameState == .inGame {
            gameStateLabel?.text = "\(currentPlayer) to go..."
        } else {
            gameStateLabel?.text = String(describing: gameState)
        }
    }
}

