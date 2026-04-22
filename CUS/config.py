from dataclasses import dataclass

@dataclass(frozen=True)
class Config:
    # MQTT
    broker: str = "broker.mqtt-dashboard.com"
    topic: str = "iot-assignment-3"

    # Uvicorn
    host: str = "localhost"
    port: int = 8000

    # Serial 
    serial_port: str = "COM3"
    serial_baudrate: int = 9600