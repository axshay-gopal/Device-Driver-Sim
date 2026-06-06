const fs = require('fs')

const DEVICE = '/dev/mydevice'
const INTERVAL = 2000

function sendCommand(command) {
    fs.writeFileSync(DEVICE, command)
}

function readSensor() {
    return fs.readFileSync(DEVICE, 'utf8').trim()
}

function clearScreen() {
    process.stdout.write('\x1Bc')
}

function getTime() {
    return new Date().toLocaleTimeString()
}

function drawDashboard(data) {
    clearScreen()
    console.log('╔══════════════════════════════════════╗')
    console.log('║     AUTOMOTIVE SENSOR DASHBOARD      ║')
    console.log('║          powered by Node.js          ║')
    console.log('╠══════════════════════════════════════╣')

    data.split('\n').forEach(line => {
        if (line.trim()) {
            const padded = line.padEnd(38)
            console.log(`║  ${padded}  ║`)
        }
    })

    console.log('╠══════════════════════════════════════╣')
    console.log(`║  Last updated : ${getTime().padEnd(21)}║`)
    console.log(`║  Device       : ${DEVICE.padEnd(21)}║`)
    console.log('╚══════════════════════════════════════╝')
    console.log('\n  Press Ctrl+C to exit')
}

function update() {
    try {
        sendCommand('get_all')
        const data = readSensor()
        drawDashboard(data)
    } catch (err) {
        console.error('Error reading device:', err.message)
        console.error('Make sure driver is loaded: sudo insmod mydevice.ko')
        process.exit(1)
    }
}

console.log('Starting Automotive Dashboard...')
console.log('Reading from', DEVICE)

update()
setInterval(update, INTERVAL)
