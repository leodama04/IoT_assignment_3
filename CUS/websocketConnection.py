from fastapi import WebSocket, WebSocketDisconnect
from typing import Set
from model import State, Mode
import logging
import asyncio

logger = logging.getLogger(__name__)  

class WebsocketConnectionManager:
    def __init__(self, state : State):
        self.active_connections: Set[WebSocket] = set()
        self.state = state

    async def connect(self, websocket: WebSocket):
        await websocket.accept()
        self.active_connections.add(websocket)
        await self.broadcast(self.state.mode)
        logger.info(f"Client connesso. Connessioni attive: {len(self.active_connections)}")
        await self.handle_msg(websocket)

    def disconnect(self, websocket: WebSocket):
        self.active_connections.remove(websocket)
        logger.info(f"Client disconnesso. Connessioni attive: {len(self.active_connections)}")

    async def broadcast(self, message: str):
        logger.info(f"Server invia a tutti: '{message}'")
        for connection in self.active_connections:
            await connection.send_text(message)

    async def handle_msg(self, websocket : WebSocket):
        try:
            while True:
                data = await websocket.receive_text()
                logger.info(f"Browser invia: '{data}'")
                asyncio.create_task(self.process_data(data))
                await self.broadcast(data)
        except WebSocketDisconnect:
            self.disconnect(websocket)

    async def process_data(self, data: str):
        if(data == Mode.AUTOMATIC):
            self.state.set_mode(Mode.AUTOMATIC)
        elif(data == Mode.REMOTE_MANUAL):
            self.state.set_mode(Mode.REMOTE_MANUAL)

        