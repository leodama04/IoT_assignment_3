const socketURL = "ws://localhost:8000/ws"; 
let socket;

const logElement = document.getElementById('log');
const statusElement = document.getElementById('status');
const modeText = document.getElementById('mode-text');
const btnAuto = document.getElementById('btn-auto');
const btnManual = document.getElementById('btn-manual');
const waterLevelText = document.getElementById('water-level-text');

const maxMeasurements = 20;
let waterLevelData = [];
let chart;

function initChart() {
    try {
        const canvas = document.getElementById('waterLevelChart');
        if (!canvas) {
            addLog("Error: canvas element not found");
            return;
        }
        const ctx = canvas.getContext('2d');
        chart = new Chart(ctx, {
            type: 'line',
            data: {
                labels: [],
                datasets: [{
                    label: 'Water level (cm)',
                    data: [],
                    borderColor: '#3498db',
                    backgroundColor: 'rgba(52, 152, 219, 0.1)',
                    borderWidth: 2,
                    tension: 0.1,
                    fill: true,
                    pointRadius: 4,
                    pointBackgroundColor: '#3498db',
                    pointBorderColor: '#fff',
                    pointBorderWidth: 2
                }]
            },
            options: {
                responsive: true,
                maintainAspectRatio: true,
                plugins: {
                    legend: {
                        display: true,
                        position: 'top'
                    }
                },
                scales: {
                    y: {
                        beginAtZero: true,
                        title: {
                            display: true,
                            text: 'Level (cm)'
                        }
                    },
                    x: {
                        title: {
                            display: true,
                            text: 'Number of measurements'
                        }
                    }
                }
            }
        });
    } catch (error) {
        addLog("Chart error: " + error.message);
    }
}

function addMeasurement(waterLevel) {
    const timestamp = new Date().toLocaleTimeString();
    waterLevelData.push({ time: timestamp, level: waterLevel });
    if (waterLevelData.length > maxMeasurements) {
        waterLevelData.shift();
    }
    updateChart();
}

function updateChart() {
    if (!chart) return;
    chart.data.labels = waterLevelData.map((_, index) => index + 1);
    chart.data.datasets[0].data = waterLevelData.map(d => d.level);
    chart.update();
}

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
    if (!chart) {
        initChart();
    }
    addMeasurement(currentWaterLevel);
}

function connect() {
    socket = new WebSocket(socketURL);
    socket.onopen = () => {
        statusElement.innerHTML = 'Status: <span style="color: green;">Connected</span>';
        addLog("Connection established.");
    };
    socket.onmessage = (event) => {
        const receivedData = JSON.parse(event.data);
        addLog(`Server broadcast: ${event.data}`);
        if (receivedData.type === "mode") {
            updateMode(receivedData.value);
        } else if (receivedData.type === "water_level") {
            updateWaterLevel(receivedData.value);
        }
    };
    socket.onclose = () => {
        statusElement.innerHTML = 'Status: <span style="color: red;">Disconnected</span>';
        btnAuto.disabled = true;
        btnManual.disabled = true;
        addLog("Connection lost. Reconnecting...");
        setTimeout(connect, 3000);
    };
    socket.onerror = (error) => {
        addLog("WebSocket error.");
    };
}

function sendMode(mode) {
    if (socket && socket.readyState === WebSocket.OPEN) {
        const message = JSON.stringify({ type: "mode", value: mode });
        socket.send(message);
        addLog(`Request sent: ${mode}`);
    }
}

// Initialize chart with a delay to ensure Chart.js library is loaded
setTimeout(() => {
    if (typeof Chart !== 'undefined' && !chart) {
        initChart();
    }
}, 1000);

connect();