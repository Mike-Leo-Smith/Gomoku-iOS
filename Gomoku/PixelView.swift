//
//  PixelView.swift
//  Gomoku
//
//  Created by Mike Smith on 26/08/2017.
//  Copyright © 2017 Mike Smith. All rights reserved.
//

import Foundation
import UIKit

class PixelView: UIView {
    
    var scale: CGFloat = 5.0 {
        didSet {
            redrawAll()
            render()
        }
    }
    
    var pixelSet: [(CGPoint, UIColor)] = []
    
    func setPixel(at coord: CGPoint, to color: UIColor) {
        pixelSet.append((coord, color))
    }
    
    func getPixel(at coord: CGPoint) -> UIColor {
        for (pixel, color) in pixelSet.reversed() {
            if (pixel == coord) {
                return color
            }
        }
        return backgroundColor ?? UIColor.white
    }
    
    func clear() {
        pixelSet = []
    }
    
    func render() {
        setNeedsDisplay()
    }
    
    private func redrawAll() {
        
        (backgroundColor ?? UIColor.white).inverted.set()
        let lineCount = Int(bounds.width / scale + 0.5)
        let path = UIBezierPath()
        path.lineWidth = 1
        for lineIndex in 0...lineCount {
            path.move(to: CGPoint(x: CGFloat(lineIndex) * scale, y: 0))
            path.addLine(to: CGPoint(x: CGFloat(lineIndex) * scale, y: CGFloat(lineCount) * scale))
            path.move(to: CGPoint(x: 0, y: CGFloat(lineIndex) * scale))
            path.addLine(to: CGPoint(x: CGFloat(lineCount) * scale, y: CGFloat(lineIndex) * scale))
        }
        path.stroke()
        
        for (pixel, color) in pixelSet {
            color.set()
            // let path = UIBezierPath(rect: CGRect(x: pixel.x * scale, y: pixel.y * scale, width: scale, height: scale))
            let path = UIBezierPath(arcCenter: CGPoint(x: pixel.x * scale + scale / 2.0, y: pixel.y * scale + scale / 2.0), radius: scale / 2.0, startAngle: 0, endAngle: 2 * CGFloat.pi, clockwise: true)
            path.lineWidth = 0
            path.fill()
        }
    }
    
    override func draw(_ rect: CGRect) {
        redrawAll()
    }
}

extension UIColor {
    var inverted: UIColor {
        var r: CGFloat = 0.0, g: CGFloat = 0.0, b: CGFloat = 0.0, a: CGFloat = 0.0
        self.getRed(&r, green: &g, blue: &b, alpha: &a)
        return UIColor(red: (1 - r), green: (1 - g), blue: (1 - b), alpha: 1) // Assuming you want the same alpha value.
    }
}

