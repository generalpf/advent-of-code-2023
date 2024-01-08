import Foundation

class Node {
    let x: Int
    let y: Int
    var distance: Int
    var visited: Bool

    public init(x: Int, y: Int, distance: Int, visited: Bool) {
        self.x = x
        self.y = y
        self.distance = distance
        self.visited = visited
    }
}

print(FileManager.default.currentDirectoryPath)

guard let gridString = try? String(contentsOfFile: "../../../../../data/day21.txt") else {
    print("could not open data file")
    exit(1)
}

let grid: [[Character]] = gridString
    .split(separator: "\n")
    .map { Array(String($0)) }

let height = grid.count
let width = grid[0].count

var nodes: [[Node]] = (0 ..< height).map { y in
    (0 ..< width).map { x in
        Node(x: x, y: y, distance: Int.max, visited: false)
    }
}

guard let startingYIndex = grid.firstIndex(where: { $0.contains("S") }),
      let startingXIndex = grid[startingYIndex].firstIndex(where: { $0 == "S" }) else {
    print("no starting spot in grid")
    exit(1)
}
let startingX = grid[startingYIndex].distance(from: grid[startingYIndex].startIndex, to: startingXIndex)
let startingY = grid.distance(from: grid.startIndex, to: startingYIndex)
let startingNode = nodes[startingY][startingX]
startingNode.visited = true
startingNode.distance = 0

var unvisited = unvisitedNodes(from: startingNode, unvisited: []).sorted { $0.distance < $1.distance }

while let next = unvisited.first {
    next.visited = true
    unvisited = Array(unvisited.dropFirst() + unvisitedNodes(from: next, unvisited: unvisited))
        .sorted { $0.distance < $1.distance }
}

let reachable = nodes.reduce(0) { partialResult, row in
    partialResult + row.filter { $0.visited && $0.distance <= 64 && $0.distance % 2 == 0 }.count
}

print("\(reachable) nodes are reachable in 64 moves")

func unvisitedNodes(from: Node, unvisited: [Node]) -> [Node] {
    var un = [Node]()
    // look west.
    if from.x > 0 && grid[from.y][from.x - 1] != "#" {
        let lookHere = nodes[from.y][from.x - 1]
        if !lookHere.visited {
            lookHere.distance = min(lookHere.distance, from.distance + 1)
            if !unvisited.contains(where: { $0.x == lookHere.x && $0.y == lookHere.y }) {
                un.append(lookHere)
            }
        }
    }
    // look east.
    if from.x < width - 1 && grid[from.y][from.x + 1] != "#" {
        let lookHere = nodes[from.y][from.x + 1]
        if !lookHere.visited {
            lookHere.distance = min(lookHere.distance, from.distance + 1)
            if !unvisited.contains(where: { $0.x == lookHere.x && $0.y == lookHere.y }) {
                un.append(lookHere)
            }
        }
    }
    // look north.
    if from.y > 0 && grid[from.y - 1][from.x] != "#" {
        let lookHere = nodes[from.y - 1][from.x]
        if !lookHere.visited {
            lookHere.distance = min(lookHere.distance, from.distance + 1)
            if !unvisited.contains(where: { $0.x == lookHere.x && $0.y == lookHere.y }) {
                un.append(lookHere)
            }
        }
    }
    // look south.
    if from.y < height - 1 && grid[from.y + 1][from.x] != "#" {
        let lookHere = nodes[from.y + 1][from.x]
        if !lookHere.visited {
            lookHere.distance = min(lookHere.distance, from.distance + 1)
            if !unvisited.contains(where: { $0.x == lookHere.x && $0.y == lookHere.y }) {
                un.append(lookHere)
            }
        }
    }
    return un
}
