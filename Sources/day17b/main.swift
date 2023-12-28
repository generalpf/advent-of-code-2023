import Foundation

enum Direction: Int, CaseIterable {
    case north, east, south, west
}

class Node {
    let x: Int
    let y: Int
    let direction: Direction
    let distance: Int
    var heatLoss: Int
    var visited: Bool

    public init(x: Int, y: Int, direction: Direction, distance: Int, heatLoss: Int, visited: Bool) {
        self.x = x
        self.y = y
        self.direction = direction
        self.distance = distance
        self.heatLoss = heatLoss
        self.visited = visited
    }
}

print(FileManager.default.currentDirectoryPath)

guard let gridString = try? String(contentsOfFile: "../../../../../data/day17.txt") else {
    print("could not open data file")
    exit(1)
}

let grid: [[Int]] = gridString
    .split(separator: "\n")
    .map {
        $0.map {
            Int($0.asciiValue! - 48)
        }
    }

let height = grid.count
let width = grid[0].count

var nodes: [[[[Node]]]] = (0 ..< height).map { y in
    (0 ..< width).map { x in
        Direction.allCases.map { direction in
            (0 ... 10).map { distance in
                Node(x: x, y: y, direction: direction, distance: distance, heatLoss: Int.max, visited: false)
            }
        }
    }
}

let startingNode = nodes[0][0][Direction.east.rawValue][0]
startingNode.visited = true
startingNode.heatLoss = 0

var unvisited = unvisitedNodes(from: startingNode, unvisited: []).sorted { $0.heatLoss < $1.heatLoss }

while let next = unvisited.first {
    // i wonder if we'll need to check it coming from the west and north?
    if next.x == width - 1 && next.y == height - 1 {
        print("at exit, heat loss = \(next.heatLoss)")
        exit(0)
    }
    next.visited = true
    unvisited = Array(unvisited.dropFirst() + unvisitedNodes(from: next, unvisited: unvisited))
        .sorted { $0.heatLoss < $1.heatLoss }
}

func unvisitedNodes(from: Node, unvisited: [Node]) -> [Node] {
    var un = [Node]()
    // look west.
    if from.x > 0 && from.direction != .east && ((from.direction == .west && from.distance < 10) || (from.direction != .west && from.distance >= 4)) {
        let distance = from.direction == .west ? from.distance + 1 : 1
        let lookHere = nodes[from.y][from.x - 1][Direction.west.rawValue][distance]
        if !lookHere.visited {
            lookHere.heatLoss = min(lookHere.heatLoss, from.heatLoss + grid[from.y][from.x - 1])
            if !unvisited.contains(where: { $0.x == lookHere.x && $0.y == lookHere.y && $0.direction == lookHere.direction && $0.distance == lookHere.distance }) {
                un.append(lookHere)
            }
        }
    }
    // look east.
    if from.x < width - 1 && from.direction != .west && ((from.direction == .east && from.distance < 10) || (from.direction != .east && from.distance >= 4)) {
        let distance = from.direction == .east ? from.distance + 1 : 1
        let lookHere = nodes[from.y][from.x + 1][Direction.east.rawValue][distance]
        if !lookHere.visited {
            lookHere.heatLoss = min(lookHere.heatLoss, from.heatLoss + grid[from.y][from.x + 1])
            if !unvisited.contains(where: { $0.x == lookHere.x && $0.y == lookHere.y && $0.direction == lookHere.direction && $0.distance == lookHere.distance }) {
                un.append(lookHere)
            }
        }
    }
    // look north.
    if from.y > 0 && from.direction != .south && ((from.direction == .north && from.distance < 10) || (from.direction != .north && from.distance >= 4)) {
        let distance = from.direction == .north ? from.distance + 1 : 1
        let lookHere = nodes[from.y - 1][from.x][Direction.north.rawValue][distance]
        if !lookHere.visited {
            lookHere.heatLoss = min(lookHere.heatLoss, from.heatLoss + grid[from.y - 1][from.x])
            if !unvisited.contains(where: { $0.x == lookHere.x && $0.y == lookHere.y && $0.direction == lookHere.direction && $0.distance == lookHere.distance }) {
                un.append(lookHere)
            }
        }
    }
    // look south.
    if from.y < height - 1 && from.direction != .north && ((from.direction == .south && from.distance < 10) || (from.direction != .south && from.distance >= 4)) {
        let distance = from.direction == .south ? from.distance + 1 : 1
        let lookHere = nodes[from.y + 1][from.x][Direction.south.rawValue][distance]
        if !lookHere.visited {
            lookHere.heatLoss = min(lookHere.heatLoss, from.heatLoss + grid[from.y + 1][from.x])
            if !unvisited.contains(where: { $0.x == lookHere.x && $0.y == lookHere.y && $0.direction == lookHere.direction && $0.distance == lookHere.distance }) {
                un.append(lookHere)
            }
        }
    }
    return un
}
