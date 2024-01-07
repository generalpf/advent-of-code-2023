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

let inWorkflow = workflows["in"]!
var accepted = 0

class Interval {
    let from: Int
    let to: Int

    public init(from: Int, to: Int) {
        self.from = from
        self.to = to
    }
}

var xIntervals: [Interval] = [Interval(from: 1, to: 4000)]
var mIntervals: [Interval] = [Interval(from: 1, to: 4000)]
var aIntervals: [Interval] = [Interval(from: 1, to: 4000)]
var sIntervals: [Interval] = [Interval(from: 1, to: 4000)]

workflows
    .values
    .flatMap(\.steps)
    .forEach { step in
        guard let v = step.v, let o = step.o, let c = step.c else {
            return
        }
        var intervals: [Interval] = switch v {
        case .x: xIntervals
        case .m: mIntervals
        case .a: aIntervals
        case .s: sIntervals
        }
        let splitMeIndex = intervals.firstIndex(where: { i in c >= i.from && c <= i.to })!
        let splitMe: Interval = intervals[splitMeIndex]
        let newIntervals = switch o {
        case .greaterThan:
            [Interval(from: splitMe.from, to: c),
             Interval(from: c + 1, to: splitMe.to)]
        case .lessThan:
            [Interval(from: splitMe.from, to: c - 1),
             Interval(from: c, to: splitMe.to)]
        }
        intervals.remove(at: splitMeIndex)
        intervals.append(contentsOf: newIntervals)
        switch v {
        case .x: xIntervals = intervals
        case .m: mIntervals = intervals
        case .a: aIntervals = intervals
        case .s: sIntervals = intervals
        }
    }

print("intervals: { x: \(xIntervals.count), m: \(mIntervals.count), a: \(aIntervals.count), s: \(sIntervals.count) }")

xIntervals.forEach { xi in
    print("x: [\(xi.from), \(xi.to)]")
    mIntervals.forEach { mi in
        print("  m: [\(mi.from), \(mi.to)]")
        aIntervals.forEach { ai in
            sIntervals.forEach { si in
                let part = Part(x: xi.from, m: mi.from, a: ai.from, s: si.from)
                var result = inWorkflow.execute(part: part)
                var resolved = false
                while !resolved {
                    switch result {
                    case .accept:
                        accepted += (xi.to - xi.from + 1) * (mi.to - mi.from + 1) * (ai.to - ai.from + 1) * (si.to - si.from + 1)
                        resolved = true
                    case .reject:
                        resolved = true
                    case .goto(let id):
                        result = workflows[id]!.execute(part: part)
                    }
                }
            }
        }
    }
}

print("\(accepted) parts were accepted")

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
