import asyncio
import serial_asyncio
import json
import logging
from model import State
from model import Mode

logger = logging.getLogger(__name__)

class SerialConnectionManager:
    def __init__(self, port: str, baudrate: int, state: State):
        self.port = port
        self.baud = baudrate
        self.state = state
        self.reader = None
        self.writer = None
        self.send_queue = asyncio.Queue()

    async def connect(self):
        self.reader, self.writer = await serial_asyncio.open_serial_connection(
            url=self.port, baudrate=self.baud
        )
        logger.info(f"Connected to {self.port}")

        asyncio.create_task(self.reader_loop())
        asyncio.create_task(self.writer_loop())

    async def send(self, msg: str):
        if self.writer:
            self.writer.write(msg.encode() + b'\n')
            await self.writer.drain()
    
    async def queue_message(self, msg: str):
        await self.send_queue.put(msg)
        logger.debug(f"Message queued: {msg}")

    def send_mode(self, mode: Mode):
        msg = json.dumps({"type": "mode", "value": mode.value})
        asyncio.create_task(self.queue_message(msg))

    def send_valve_state(self, valve_state: float):
        msg = json.dumps({"type": "valve_state", "value": valve_state})
        asyncio.create_task(self.queue_message(msg))
        
    async def read(self) -> str:
        if self.reader:
            data = await asyncio.wait_for(
                self.reader.readline()
            )
            return data

    async def reader_loop(self):
        while True:
            try:
                data = await self.read()
                if data:
                    logger.info(f"Serial data received: {data.decode().strip()}")
                    try:
                        parsed = json.loads(data.decode())
                    except (ValueError, KeyError, json.JSONDecodeError):
                        logger.warning(f"Invalid serial payload: {data}")
            except asyncio.CancelledError:
                logger.info("Serial reader loop cancelled")
                break

    async def writer_loop(self):
        while True:
            try:
                msg = await self.send_queue.get()
                await self.send(msg)
                logger.info(f"Sent to serial: {msg}")
                self.send_queue.task_done()
            except asyncio.CancelledError:
                logger.info("Serial writer loop cancelled")
                break

    async def close(self):
        if self.writer:
            self.writer.close()
            await self.writer.wait_closed()