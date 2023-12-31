import Foundation

guard let instructionsString = try? String(contentsOfFile: "../../../../../data/day18.txt") else {
    print("could not open data file")
    exit(1)
}

let instructions = instructionsString.split(separator: "\n")

var x = 0
var y = 0
var xDelta: Int = 0
var yDelta: Int = 0

var edge = 0
typealias Point = (x: Int, y: Int)
var points = [Point]()

instructions.forEach { instruction in
    let parts = instruction.split(separator: " ")
    let trueInstruction = String(parts[2].dropFirst(2).dropLast())
    let magnitude = Int(trueInstruction.dropLast(), radix: 16)!
    // turn.
    switch trueInstruction.last! {
    case "0": (xDelta, yDelta) = (1, 0)
    case "2": (xDelta, yDelta) = (-1, 0)
    case "1": (xDelta, yDelta) = (0, 1)
    case "3": (xDelta, yDelta) = (0, -1)
    default: break
    }
    // move.
    edge += magnitude
    x += magnitude * xDelta
    y += magnitude * yDelta
    points.append(Point(x: x, y: y))
}

// https://en.wikipedia.org/wiki/Shoelace_formula

var area = 0
for i in (0 ..< points.count) {
    area += points[i].x * points[(i + 1) % points.count].y - points[(i + 1) % points.count].x * points[i].y
}
area /= 2

let blocks = area + edge / 2 + 1
print("blocks = \(blocks)")
