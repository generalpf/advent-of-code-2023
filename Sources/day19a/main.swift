import Foundation

guard let instructionsString = try? String(contentsOfFile: "../../../../../data/day19.txt") else {
    print("could not open data file")
    exit(1)
}

let instructions = instructionsString.split(separator: "\n").map { String($0) }

// it's gobbling up the blank line for some reason.
guard let firstRatingIndex = instructions.firstIndex(where: { $0.starts(with: "{") }) else {
    print("can't find where workflows stop and part ratings start")
    exit(1)
}

enum Variable: String {
    case x, m, a, s
}

enum Comparator: Character {
    case lessThan = "<", greaterThan = ">"
}

struct Part {
    let x: Int
    let m: Int
    let a: Int
    let s: Int

    func value(for variable: Variable) -> Int {
        switch variable {
        case .x: return x
        case .m: return m
        case .a: return a
        case .s: return s
        }
    }

    var rating: Int {
        x + m + a + s
    }
}

enum Result {
    case accept
    case reject
    case goto(id: String)

    static func from(_ s: String) -> Result {
        switch s {
        case "A": return .accept
        case "R": return .reject
        default: return .goto(id: s)
        }
    }
}

struct Step {
    let v: Variable?
    let o: Comparator?
    let c: Int?
    let r: Result

    func execute(part: Part) -> Result? {
        if let v, let o, let c {
            switch o {
            case .lessThan:
                if part.value(for: v) < c {
                    return r
                }
                return nil
            case .greaterThan:
                if part.value(for: v) > c {
                    return r
                }
                return nil
            }
        }
        return r
    }
}

struct Workflow {
    let id: String
    let steps: [Step]

    func execute(part: Part) -> Result {
        for i in (0 ..< steps.count) {
            if let result = steps[i].execute(part: part) {
                return result
            }
        }
        print("reached end of workflow \(id) without result")
        exit(1)
    }
}

var workflows: [String: Workflow] = [:]
instructions
    .prefix(firstRatingIndex)
    .forEach { w in
        let curly = w.firstIndex(of: "{")!
        let id = String(w[..<curly])
        let steps = String(w[curly...])
            .dropFirst()
            .dropLast()
            .split(separator: ",")
            .map(String.init)
            .map(parseStep)
        workflows[id] = Workflow(id: id, steps: steps)
    }

let parts = instructions
    .dropFirst(firstRatingIndex)
    .map(parsePart)

let inWorkflow = workflows["in"]!
var accepted = 0
var totalRating = 0
parts.forEach { part in
    var result = inWorkflow.execute(part: part)
    var resolved = false
    print("in -> ", terminator: "")
    while !resolved {
        switch result {
        case .accept:
            print("A")
            accepted += 1
            totalRating += part.rating
            resolved = true
        case .reject:
            print("R")
            resolved = true
        case .goto(let id):
            print("\(id) -> ", terminator: "")
            result = workflows[id]!.execute(part: part)
        }
    }
}

print("\(accepted) out of \(parts.count) parts were accepted by executing \(workflows.count) workflows, total rating = \(totalRating)")

func parseStep(s: String) -> Step {
    if let comparator = s.firstIndex(of: ">") ?? s.firstIndex(of: "<"), let colon = s.firstIndex(of: ":") {
        return Step(v: Variable(rawValue: String(s[..<comparator])),
                    o: Comparator(rawValue: s[comparator]),
                    c: Int(String(s[comparator..<colon].dropFirst()))!,
                    r: Result.from(String(s[colon...].dropFirst())))
    } else {
        return Step(v: nil, o: nil, c: nil, r: Result.from(s))
    }
}

func parsePart(p: String) -> Part {
    let parts = p.dropFirst().dropLast().split(separator: ",")
    let x = Int(String(parts[0].split(separator: "=")[1]))!
    let m = Int(String(parts[1].split(separator: "=")[1]))!
    let a = Int(String(parts[2].split(separator: "=")[1]))!
    let s = Int(String(parts[3].split(separator: "=")[1]))!
    return Part(x: x, m: m, a: a, s: s)
}
