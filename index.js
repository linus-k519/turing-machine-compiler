class Tape {
    constructor(config) {
        this.data = config['tape'] || []
        this.emptySymbol = config['machine_params']['empty_symbol'] || '_'
        this.indexOffset = 0
    }

    getRealIndex(index) {
        return index + this.indexOffset
    }

    growLeft() {
        this.data.unshift(this.emptySymbol)
        this.indexOffset++
    }

    growRight() {
        this.data.push(this.emptySymbol)
    }

    growIfNecessary(index) {
        const realIndex = this.getRealIndex(index)
        if (realIndex < 0) {
            this.growLeft()
        } else if (realIndex >= this.data.length) {
            this.growRight()
        }
    }

    get(index) {
        this.growIfNecessary(index)
        const real_index = this.getRealIndex(index);
        return this.data[real_index]
    }

    set(index, value) {
        this.growIfNecessary(index)
        const real_index = this.getRealIndex(index);
        this.data[real_index] = value
    }

    toString(highlightIndex) {
        const realHighlightIndex = this.getRealIndex(highlightIndex);
        // Get first position on the left that has content
        let left = 0
        for (let i = 0; i < this.data.length; i++) {
            if (this.data[i] !== this.emptySymbol) {
                left = i
                break
            }
        }
        if (realHighlightIndex < left) {
            // Show the real highlight index in any case
            left = realHighlightIndex;
        }
        if (left >= 1) {
            // Show one additional item
            left--
        }
        // Get first position on the right that has content
        let right = 0
        for (let i = this.data.length-1; i >= 0; i--) {
            if (this.data[i] !== this.emptySymbol) {
                right = i
                break
            }
        }
        if (realHighlightIndex > right) {
            // Show the real highlight index in any case
            right = realHighlightIndex;
        }
        if (right < this.data.length-1) {
            // Show one additional item
            right++
        }

        // Convert tape to string
        let output = ''
        for (let i = left; i <= right; i++) {
            if (i === realHighlightIndex) {
                // Mark highlight position
                output += `[${this.data[i]}]`
            } else {
                output += this.data[i]
            }
            output += ' '
        }
        return output
    }
}

function runTM(config) {
    const start_time = performance.now()
    let tape = new Tape(config)
    let state = config['machine_params']['start'] || '1'
    let index = 0
    while (true) {
        const read = tape.get(index)
        if (document.getElementById('debug').checked) {
            document.getElementById('output').innerText += `State: ${state}\n`
            document.getElementById('output').innerText += `Tape: ${tape.toString(index)}\n`
            document.getElementById('output').innerText += '\n'
        }
        // Search matching transition
        let foundTransition = false
        for (let transition of config['transitions']) {
            if (transition['from'] === state && transition['read'] === read) {
                foundTransition = true
                tape.set(index, transition['write'])
                state = transition['goto']
                if (transition['move'] === 'left' || transition['move'] === 'l') {
                    index--
                } else if (transition['move'] === 'right' || transition['move'] === 'r') {
                    index++
                }
                break
            }
        }
        if (!foundTransition) {
            const exec_duration = performance.now() - start_time
            document.getElementById('output').innerText += `Finished in ${exec_duration.toFixed(2)} ms\n`
            document.getElementById('output').innerText += `State: ${state}\n`
            document.getElementById('output').innerText += `Tape: ${tape.toString(index)}\n`
            return
        }
    }
}

function isMachineParam(pairs) {
    const machineParams = ['start', 'empty_symbol']
    return machineParams.some(key => pairs[key])
}

function get_pairs(line) {
    let pairs = {}
    for (let i = 0; i < line.length; i += 2) {
        let key = line[i].toLowerCase()
        let value = line[i+1]
        pairs[key] = value
    }
    return pairs
}

function validTransition(transaction) {
    const keys = ['from', 'read', 'write', 'goto', 'move']
    return keys.every(key => transaction[key])
}

function parseMachine(lines) {
    let transitions = []
    let machine_params = {}

    for (let line of lines) {
        line = line.split(' ')
        const pairs = get_pairs(line)
        if (validTransition(pairs)) {
            transitions.push(pairs)
        } else if (isMachineParam(pairs)) {
            for (let key in pairs) {
                machine_params[key] = pairs[key]
            }
        } else {
            // throw new Error('invalid line')
            document.getElementById('output').innerText +=  `error: invalid line '${line}'\n`
        }
    }
    return {transitions, machine_params}
}

function renderTMExecution() {
    document.getElementById('output').innerText = ''
    let tm_description = document.getElementById('tm_description')
        .value
        .split('\n')
        .filter(line => line.trim())
    let parseResult = parseMachine(tm_description)
    parseResult['tape'] = document.getElementById('tape').value.split(' ')
    runTM(parseResult)
}

function updateURL() {
    const tape = document.getElementById('tape').value
    const tm_description = document.getElementById('tm_description').value
    setGetParameter('tape', tape)
    setGetParameter('tm_description', tm_description)
}

function getGetParameters() {
    return location.search
        .substr(1)
        .split("&")
        .filter(x => x) // Ensure item is not empty
        .reduce((acc, cur) => {
            let [key, value] = cur.split("=")
            key = decodeURIComponent(key)
            value = decodeURIComponent(value)
            acc[key] = value
            return acc
        }, {})
}

function setGetParameter(key, value) {
    key = encodeURIComponent(key)
    value = encodeURIComponent(value)
    let params = getGetParameters()
    params[key] = value
    window.history.replaceState(null, '','?' + Object.keys(params)
        .map((key, index) => key + '=' + params[key])
        .join('&'));
}

window.onload = () => {
    const params = getGetParameters()
    if (params['tape'] !== undefined) {
        document.getElementById('tape').value = params['tape']
    }
    if (params['tm_description'] !== undefined) {
        document.getElementById('tm_description').value = params['tm_description']
    }
}
