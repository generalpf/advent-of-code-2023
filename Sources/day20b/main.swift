import Foundation

guard let modulesString = try? String(contentsOfFile: "../../../../../data/day20.txt") else {
    print("could not open data file")
    exit(1)
}

var modulesMap: [String: Module] = [:]
let opQueue = OperationQueue()
opQueue.maxConcurrentOperationCount = 1

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

    func handle(_ pulse: Pulse, from: String, press: Int) {
    }
}

class BlackHole: Module {
}

class Broadcaster: Module {
    override func handle(_ pulse: Pulse, from: String, press: Int) {
        super.handle(pulse, from: from, press: press)
        opQueue.addOperation {
            self.outputs.forEach {
                modulesMap[$0]!.handle(pulse, from: self.id, press: press)
            }
        }
    }
}

class FlipFlop: Module {
    var state = false

    override func handle(_ pulse: Pulse, from: String, press: Int) {
        super.handle(pulse, from: from, press: press)
        if pulse == .low {
            state.toggle()
            opQueue.addOperation {
                self.outputs.forEach {
                    modulesMap[$0]!.handle(self.state ? .high : .low, from: self.id, press: press)
                }
            }
        }
    }
}

class Conjunction: Module {
    var states: [String: Pulse] = [:]
    var firstHigh: [String: Int] = [:]

    func registerInput(from: String) {
        states[from] = .low
        firstHigh[from] = -1
    }

    override func handle(_ pulse: Pulse, from: String, press: Int) {
        super.handle(pulse, from: from, press: press)
        states[from] = pulse
        if id == "tg" {
            if pulse == .high, firstHigh[from] == -1 {
                firstHigh[from] = press
            }
            if firstHigh.allSatisfy({ (key: String, value: Int?) in
                value != -1
            }) {
                let allWillBeHigh = firstHigh.values.reduce(1) { partialResult, value in
                    partialResult * value
                }
                fatalError("all inputs to 'tg' will be high on press \(allWillBeHigh)")
            }
        }
        let allHigh = states.allSatisfy { (key: String, value: Pulse) in
            value == .high
        }
        opQueue.addOperation {
            self.outputs.forEach {
                modulesMap[$0]!.handle(allHigh ? .low : .high, from: self.id, press: press)
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

var i = 0
while true {
    i += 1
    opQueue.addOperation {
        broadcaster.handle(.low, from: "button", press: i)
    }
    opQueue.waitUntilAllOperationsAreFinished()
}

// `rx` receives input from Conjunction module `tg`.
// `tg` receives inputs from [ln, db, vq, tf].
// find when `tg` receives a high from each and multiply together.
