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
        self.reader_task = None
        self.writer_task = None

    async def connect(self):
        self.reader, self.writer = await serial_asyncio.open_serial_connection(
            url=self.port, baudrate=self.baud
        )
        logger.info(f"Connected to {self.port}")

        self.reader_task = asyncio.create_task(self.reader_loop())
        self.writer_task = asyncio.create_task(self.writer_loop())

        self.state.valve_state = 0
        self.state.handle_valve_state_change(0)

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
            data = await asyncio.wait_for(self.reader.readline(), timeout=5.0)
            return data

    async def reader_loop(self):
        while True:
            try:
                data = await self.read()
                if data:
                    logger.info(f"Serial data received: {data.decode().strip()}")
                    try:
                        parsed = json.loads(data.decode())
                        if "valve_state" in parsed:
                            valve_value = parsed.get("valve_state")
                            logger.info(f"Valve opened to {valve_value :.1f}%")
                            self.state.valve_state = valve_value
                            self.state.handle_valve_state_change(valve_value)
                    except (ValueError, KeyError, json.JSONDecodeError):
                        logger.warning(f"Invalid serial payload: {data}")
            except asyncio.TimeoutError:
                logger.debug("Serial read timeout - no data received")
                continue
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
        # Cancel tasks
        if self.reader_task:
            self.reader_task.cancel()
        if self.writer_task:
            self.writer_task.cancel()
        
        # Wait for tasks to complete
        if self.reader_task:
            try:
                await self.reader_task
            except asyncio.CancelledError:
                pass
        if self.writer_task:
            try:
                await self.writer_task
            except asyncio.CancelledError:
                pass
        
        # Close writer
        if self.writer:
            self.writer.close()
            await self.writer.wait_closed()