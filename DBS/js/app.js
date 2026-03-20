const socketURL = "ws://localhost:8000/ws"; 
let socket;

const logElement = document.getElementById('log');
const statusElement = document.getElementById('status');
const modeText = document.getElementById('mode-text');
const btnAuto = document.getElementById('btn-auto');
const btnManual = document.getElementById('btn-manual');
const waterLevelText = document.getElementById('water-level-text');

function addLog(msg) {
    const time = new Date().toLocaleTimeString();
    logElement.innerHTML += `<div>[${time}] ${msg}</div>`;
    logElement.scrollTop = logElement.scrollHeight;
}

function updateMode(currentMode) {
    modeText.innerText = currentMode;
    if (currentMode === 'AUTOMATIC') {
        btnAuto.disabled = true;
        btnManual.disabled = false;
    } else if (currentMode === 'REMOTE_MANUAL') {
        btnAuto.disabled = false;
        btnManual.disabled = true;
    }
}

function updateWaterLevel(currentWaterLevel) {
    waterLevelText.innerText = `${currentWaterLevel} cm`;
}

function connect() {
    socket = new WebSocket(socketURL);
    socket.onopen = () => {
        statusElement.innerHTML = 'Stato: <span style="color: green;">Connesso</span>';
        addLog("Connessione stabilita.");
    };
    socket.onmessage = (event) => {
        const receivedData = JSON.parse(event.data);
        addLog(`Server Broadcast: ${event.data}`);
        if (receivedData.type === "mode") {
            updateMode(receivedData.value);
        } else if (receivedData.type === "water_level") {
            updateWaterLevel(receivedData.value);
        }
    };
    socket.onclose = () => {
        statusElement.innerHTML = 'Stato: <span style="color: red;">Disconnesso</span>';
        btnAuto.disabled = true;
        btnManual.disabled = true;
        addLog("Connessione persa. Riconnessione...");
        setTimeout(connect, 3000);
    };
    socket.onerror = (error) => {
        addLog("Errore WebSocket.");
    };
}

function sendMode(mode) {
    if (socket && socket.readyState === WebSocket.OPEN) {
        const message = JSON.stringify({ type: "mode", value: mode });
        socket.send(message);
        addLog(`Richiesta inviata: ${mode}`);
    }
}

connect();