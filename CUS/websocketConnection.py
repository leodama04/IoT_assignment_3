from fastapi import WebSocket, WebSocketDisconnect
from typing import Set
from model import State, Mode
import logging
import asyncio
import json

logger = logging.getLogger(__name__)  

class WebsocketConnectionManager:
    def __init__(self, state : State):
        self.active_connections: Set[WebSocket] = set()
        self.state = state

    async def connect(self, websocket: WebSocket):
        await websocket.accept()
        self.active_connections.add(websocket)
        await self.broadcast("mode", self.state.mode) 
        await self.broadcast("water_level", self.state.water_level)
        await self.handle_msg(websocket)

    def disconnect(self, websocket: WebSocket):
        self.active_connections.remove(websocket)
        logger.info(f"Client disconnected. Active connections: {len(self.active_connections)}")

    async def broadcast(self, message_type: str, value):
        payload = json.dumps({"type": message_type, "value": value})
        logger.info(f"Broadcasting to all clients: '{payload}'")
        for connection in self.active_connections:
            await connection.send_text(payload)

    async def handle_msg(self, websocket : WebSocket):
        try:
            while True:
                data = await websocket.receive_text()
                logger.info(f"Received from client: '{data}'")
                asyncio.create_task(self.process_data(data))
        except WebSocketDisconnect:
            self.disconnect(websocket)

    async def process_data(self, data: str):
        parsed = json.loads(data)
        if parsed["type"] == "mode":
            mode = parsed["value"]
            if mode == Mode.AUTOMATIC:
                self.state.set_mode(Mode.AUTOMATIC)
            elif mode == Mode.REMOTE_MANUAL:
                self.state.set_mode(Mode.REMOTE_MANUAL)
            await self.broadcast("mode", mode)  
    
    def handle_water_level_change(self, water_level: float):
        try:
            asyncio.create_task(self.broadcast("water_level", water_level))
        except RuntimeError:
            logger.warning("Could not create task for water level change (event loop might be closing)")
    