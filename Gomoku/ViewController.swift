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
            let tapRecognizer = UITapGestureRecognizer(target: self, action: #selector(tagGestureResponse))
            tapRecognizer.numberOfTouchesRequired = 1
            pixelView.addGestureRecognizer(tapRecognizer)
        }
    }
    
    override func viewDidLoad() {
        super.viewDidLoad()
        EngineWrapper.resetGame(Int32(Player.black.rawValue))
    }
    
    override func viewDidLayoutSubviews() {
        renderGameBoard()
    }
    
    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
        // Dispose of any resources that can be recreated.
    }
    
    func tagGestureResponse(reactingTo tapRecognizer: UITapGestureRecognizer) {
        if !aiSearching && tapRecognizer.state == .ended && gameState == .inGame {
            let col = Int32(tapRecognizer.location(in: pixelView).x / pixelView.scale)
            let row = Int32(tapRecognizer.location(in: pixelView).y / pixelView.scale)
            EngineWrapper.placePiece(atRow: row, col: col)
            renderGameBoard()
            if (gameState == .inGame) {
                askComputerToGo()
            }
        }
    }
    
    @IBAction func resetGame(_ sender: UIButton) {
        EngineWrapper.resetGame(Int32(Player.black.rawValue))
        renderGameBoard()
    }
    
    @IBOutlet weak var searchingProgress: UIProgressView!
    
    var aiSearching = false
    
    func askComputerToGo() {
        aiSearching = true
        let searchingTimeLimitInSeconds = 15
        searchingProgress.progress = 0
        searchingProgress.setNeedsDisplay()
        searchingProgress.isHidden = false
        
        DispatchQueue.global(qos: .userInitiated).async { [weak self] in
            let solution = EngineWrapper.search(inSeconds: Int32(searchingTimeLimitInSeconds))
            EngineWrapper.placePiece(atRow: solution.row, col: solution.col)
            DispatchQueue.main.async {
                self?.renderGameBoard()
                self?.aiSearching = false
                self?.searchingProgress.isHidden = true
            }
        }
        
        func quadraticSpeedProgress(elapsed time: Double, of total: Double) -> Double {
            return (3.0 * total * time * time - 2.0 * time * time * time) / (total * total * total)
        }
        
        DispatchQueue.global(qos: .background).async { [weak self] in
            for elapsedTime in 0...searchingTimeLimitInSeconds {
                DispatchQueue.main.async {
                    self?.searchingProgress.setProgress(Float(quadraticSpeedProgress(elapsed: Double(elapsedTime), of: Double(searchingTimeLimitInSeconds))), animated: true)
                    self?.searchingProgress.setNeedsDisplay()
                }
                sleep(1)
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
        pixelView.backgroundColor = UIColor(displayP3Red: 0.6, green: 0.7, blue: 0.8, alpha: 1.0)
        pixelView.scale = pixelView.bounds.width / CGFloat(EngineWrapper.cols())
        pixelView.render()
        if gameState == .inGame {
            gameStateLabel?.text = "\(currentPlayer) to go..."
        } else {
            gameStateLabel?.text = String(describing: gameState)
        }
    }
}

