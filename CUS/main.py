import sys
import asyncio
import logging
import uvicorn
import os

from fastapi import FastAPI, WebSocket, Response
from fastapi.staticfiles import StaticFiles
from websocketConnection import WebsocketConnectionManager
from mqttConnection import MqttConnectionManager
from serialConnection import SerialConnectionManager
from fastapi.middleware.cors import CORSMiddleware
from fastapi.responses import FileResponse
from contextlib import asynccontextmanager
from model import State
from config import Config

logging.basicConfig(
    level=logging.INFO,
    format="%(asctime)s - %(levelname)s - %(message)s"
)
logging.getLogger("model").setLevel(logging.DEBUG)

config: Config = Config()
logger = logging.getLogger(__name__)  

state = State()
websocket_manager = WebsocketConnectionManager(state)
mqtt_manager = MqttConnectionManager(config.broker, config.topic, state)
serial_manager = SerialConnectionManager(config.serial_port, config.serial_baudrate, state)

state.set_callable(websocket_manager.handle_water_level_change, 
                   serial_manager.send_mode, 
                   serial_manager.send_valve_state)

@asynccontextmanager
async def lifespan(app: FastAPI):
    logger.info(f"Connecting to broker: {config.broker}, topic: {config.topic}")
    await mqtt_manager.start()
    yield
    logger.info("Shutting down...")
    await mqtt_manager.stop()
    logger.info("MQTT Shutted down...")

    
app = FastAPI(lifespan=lifespan)

app.mount("/dbs", StaticFiles(directory="../DBS"))

app.add_middleware(
    CORSMiddleware,
    allow_origins=["*"],
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"],
)

@app.get("/")
async def get():
    return FileResponse("../DBS/index.html")

@app.websocket("/ws")
async def websocket_endpoint(websocket: WebSocket):
    await websocket_manager.connect(websocket)
    
if __name__ == "__main__":
    if sys.platform == "win32":
        # On Windows, uvicorn uses ProactorEventLoop by default which doesn't support
        # add_reader/add_writer used by aiomqtt. We force the SelectorEventLoop
        # by manually creating the loop before uvicorn does.
        asyncio.set_event_loop_policy(asyncio.WindowsSelectorEventLoopPolicy())
        loop = asyncio.new_event_loop()
        asyncio.set_event_loop(loop)
        config_uvicorn = uvicorn.Config(
            app,
            host=config.host,
            port=config.port,
            timeout_graceful_shutdown=1
        )
        server = uvicorn.Server(config_uvicorn)
        try:
            loop.run_until_complete(server.serve())
        except KeyboardInterrupt:
            logger.info("CTRL+C detected, exiting...")
        finally:
            loop.close()
            logger.info("Server shut down.")
            # Force exit immediately to avoid hanging threads
            os._exit(0)
    else:
        uvicorn.run(app, host=config.host, port=config.port)