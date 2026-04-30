const greenLED_toggle = document.getElementById('greenLED_toggle')
greenLED_toggle.addEventListener('click',toggleGreenLED)

function toggleGreenLED() {
    fetch('/toggleGreenLED')

}

const yellowLED_toggle = document.getElementById('yellowLED_toggle')
yellowLED_toggle.addEventListener('click',toggleYellowLED)

function toggleYellowLED() {
    fetch('/toggleYellowLED')

}

const redLED_toggle = document.getElementById('redLED_toggle')
redLED_toggle.addEventListener('click',toggleRedLED)

function toggleRedLED() {
    fetch('/toggleRedLED')

}

const xmas_toggle = document.getElementById('xmas_toggle')
xmas_toggle.addEventListener('click',toggleXmas)

function toggleXmas() {
    fetch('/toggleXmas')
}