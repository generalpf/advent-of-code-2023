import Foundation

guard let instructionsString = try? String(contentsOfFile: "../../../../../data/day18.txt") else {
    print("could not open data file")
    exit(1)
}

let instructions = instructionsString.split(separator: "\n")

let dim = 700

var lagoon: [[String?]] = (0 ..< dim).map({ _ in (0 ..< dim).map({ _ in nil })})
var x: Int = dim / 2
var y: Int = dim / 2
var xDelta: Int = 0
var yDelta: Int = 0

typealias Line = (fromX: Int, fromY: Int, toX: Int, toY: Int)

var verticalLines = [Line]()

instructions.forEach { instruction in
    let parts = instruction.split(separator: " ")
    let direction = parts[0]
    let magnitude = Int(parts[1])!
    let color = String(parts[2].dropFirst().dropLast())
    // turn.
    switch direction {
    case "R": (xDelta, yDelta) = (1, 0)
    case "L": (xDelta, yDelta) = (-1, 0)
    case "D": (xDelta, yDelta) = (0, 1)
    case "U": (xDelta, yDelta) = (0, -1)
    default: break
    }
    // move.
    (1 ... magnitude).forEach { i in
        lagoon[y + i * yDelta][x + i * xDelta] = color
    }
    if xDelta == 0 {
        verticalLines.append(Line(fromX: x, fromY: y, toX: x + magnitude * xDelta, toY: y + magnitude * yDelta))
    }
    x += magnitude * xDelta
    y += magnitude * yDelta
}

var filled = 0
for y in 0 ..< dim {
    let row = lagoon[y]
    var balance = 0
    var lastWas = 0
    for x in 0 ..< dim {
        if row[x] == nil {
            // check if we should fill it.
            if balance != 0 {
                lagoon[y][x] = "#FFFFFF"
                filled += 1
            }
        } else {
            filled += 1
            // is this a vector going up or down?
            if let line = verticalLines.first(where: { $0.fromX == x && ($0.fromY <= y && $0.toY >= y || $0.fromY >= y && $0.toY <= y)}) {
                if line.toY > line.fromY {
                    if lastWas != 1 {
                        balance += 1
                    }
                    lastWas = 1
                } else {
                    if lastWas != -1 {
                        balance -= 1
                    }
                    lastWas = -1
                }
            }
        }
    }
}

print("\(filled) are filled")
