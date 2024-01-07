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

typealias Interval = (from: Int, to: Int)

var xIntervals: [Interval] = [(from: 1, to: 4000)]
var mIntervals: [Interval] = [(from: 1, to: 4000)]
var aIntervals: [Interval] = [(from: 1, to: 4000)]
var sIntervals: [Interval] = [(from: 1, to: 4000)]

workflows
    .values
    .flatMap(\.steps)
    .forEach { step in
        guard let v = step.v, let o = step.o, let c = step.c else {
            return
        }
        let intervals: [Interval] = switch v {
        case .x: xIntervals
        case .m: mIntervals
        case .a: aIntervals
        case .s: sIntervals
        }
        let splitMeIndex = intervals.firstIndex(where: { i in c >= i.from && c <= i.to })!
        let splitMe: Interval = intervals[splitMeIndex]
        // there is a bug in here where an interval is replaced with a pair of intervals
        // where one or more of them has .to < .from so maybe we should figure that out if
        // we want to optimize some more.
        let newIntervals = switch o {
        case .greaterThan:
            [Interval(from: splitMe.from, to: c),
             Interval(from: c + 1, to: splitMe.to)]
        case .lessThan:
            [Interval(from: splitMe.from, to: c - 1),
             Interval(from: c, to: splitMe.to)]
        }
        switch v {
        case .x: xIntervals.replaceSubrange(splitMeIndex...splitMeIndex, with: newIntervals)
        case .m: mIntervals.replaceSubrange(splitMeIndex...splitMeIndex, with: newIntervals)
        case .a: aIntervals.replaceSubrange(splitMeIndex...splitMeIndex, with: newIntervals)
        case .s: sIntervals.replaceSubrange(splitMeIndex...splitMeIndex, with: newIntervals)
        }
    }

print("intervals: { x: \(xIntervals.count), m: \(mIntervals.count), a: \(aIntervals.count), s: \(sIntervals.count) }")

let queue = DispatchQueue(label: "com.des.aoc2023.day19b", attributes: .concurrent)

let lock = NSLock()
var allAccepted = 0

let inWorkflow = workflows["in"]!

let group = DispatchGroup()
xIntervals.forEach { xi in
    group.enter()
    queue.async {
        print("x: [\(xi.from), \(xi.to)]")
        var accepted = 0
        mIntervals.forEach { mi in
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
        lock.lock()
        allAccepted += accepted
        lock.unlock()
        group.leave()
    }
}

group.wait()

print("\(allAccepted) parts were accepted")

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
