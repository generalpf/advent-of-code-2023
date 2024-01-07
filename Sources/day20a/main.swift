import Foundation

guard let modulesString = try? String(contentsOfFile: "../../../../../data/day20.txt") else {
    print("could not open data file")
    exit(1)
}

var modulesMap: [String: Module] = [:]
let opQueue = OperationQueue()
opQueue.maxConcurrentOperationCount = 1
var lows = 0
var highs = 0

enum Pulse {
    case high, low
}

class Module {
    let id: String
    let outputs: [String]

    init(id: String, outputs: [String]) {
        self.id = id
        self.outputs = outputs
    }

    func handle(_ pulse: Pulse, from: String) {
        if pulse == .high {
            highs += 1
        } else {
            lows += 1
        }
        print("\(from) -\(pulse)-> \(id)")
    }
}

class BlackHole: Module {
}

class Broadcaster: Module {
    override func handle(_ pulse: Pulse, from: String) {
        super.handle(pulse, from: from)
        opQueue.addOperation {
            self.outputs.forEach {
                modulesMap[$0]!.handle(pulse, from: self.id)
            }
        }
    }
}

class FlipFlop: Module {
    var state = false

    override func handle(_ pulse: Pulse, from: String) {
        super.handle(pulse, from: from)
        if pulse == .low {
            state.toggle()
            opQueue.addOperation {
                self.outputs.forEach {
                    modulesMap[$0]!.handle(self.state ? .high : .low, from: self.id)
                }
            }
        }
    }
}

class Conjunction: Module {
    var states: [String: Pulse] = [:]

    func registerInput(from: String) {
        states[from] = .low
    }

    override func handle(_ pulse: Pulse, from: String) {
        super.handle(pulse, from: from)
        states[from] = pulse
        let allHigh = states.allSatisfy { (key: String, value: Pulse) in
            value == .high
        }
        opQueue.addOperation {
            self.outputs.forEach {
                modulesMap[$0]!.handle(allHigh ? .low : .high, from: self.id)
            }
        }
    }
}

let modulesStrings: [String] = modulesString
    .split(separator: "\n")
    .map { String($0) }

modulesStrings
    .forEach { moduleString in
        let hyphen = moduleString.firstIndex(of: "-")!
        let id: String = String(moduleString[..<moduleString.index(hyphen, offsetBy: -1)])
        let outputs: [String] = moduleString[moduleString.index(hyphen, offsetBy: 3)...]
            .split(separator: ",")
            .map { String($0).trimmingCharacters(in: .whitespaces) }
        let newModule: Module
        let actualId: String

        if id == "broadcaster" {
            actualId = id
            newModule = Broadcaster(id: actualId, outputs: outputs)
        } else if id.first == "%" {
            actualId = String(id.dropFirst(1))
            newModule = FlipFlop(id: actualId, outputs: outputs)
        } else if id.first == "&" {
            actualId = String(id.dropFirst(1))
            let c = Conjunction(id: actualId, outputs: outputs)
            newModule = c
            modulesMap
                .values
                .filter { $0.outputs.contains(actualId) }
                .forEach { c.registerInput(from: $0.id) }
        } else {
            fatalError("not sure what \(id) is")
        }
        outputs
            .forEach {
                (modulesMap[$0] as? Conjunction)?.registerInput(from: actualId)
            }

        modulesMap[actualId] = newModule
    }

modulesMap
    .values
    .flatMap(\.outputs)
    .forEach {
        if modulesMap[$0] == nil {
            modulesMap[$0] = BlackHole(id: $0, outputs: [])
        }
    }

let broadcaster = modulesMap["broadcaster"]!

for i in (1 ... 1000) {
    print("-- \(i) --")
    opQueue.addOperation {
        broadcaster.handle(.low, from: "button")
    }
    opQueue.waitUntilAllOperationsAreFinished()
}

print("\(highs) highs and \(lows) lows = \(highs * lows)")
