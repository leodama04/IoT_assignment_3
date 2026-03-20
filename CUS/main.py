import sys
import asyncio
import logging
import uvicorn

from fastapi import FastAPI, WebSocket
from fastapi.staticfiles import StaticFiles
from websocketConnection import WebsocketConnectionManager
from mqttConnection import MqttConnectionManager
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

state.set_callable(websocket_manager.handle_water_level_change)

@asynccontextmanager
async def lifespan(app: FastAPI):
    logger.info(f"Connecting to broker: {config.broker}, topic: {config.topic}")
    await mqtt_manager.start()
    yield
    await mqtt_manager.stop()

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
        # Su Windows, uvicorn usa di default il ProactorEventLoop che non supporta
        # add_reader/add_writer usati da aiomqtt. Forziamo il SelectorEventLoop
        # creando manualmente il loop prima che uvicorn lo faccia.
        asyncio.set_event_loop_policy(asyncio.WindowsSelectorEventLoopPolicy())
        loop = asyncio.new_event_loop()
        asyncio.set_event_loop(loop)
        config_uvicorn = uvicorn.Config(app, host=config.host, port=config.port)
        server = uvicorn.Server(config_uvicorn)
        try:
            loop.run_until_complete(server.serve())
        except KeyboardInterrupt:
            pass  # Uscita pulita con CTRL+C
    else:
        uvicorn.run(app, host=config.host, port=config.port)