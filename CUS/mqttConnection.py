import asyncio
import aiomqtt
import json
import logging
import time
from model import State, Mode, T2

logger = logging.getLogger(__name__) 

class MqttConnectionManager():
    def __init__(self, broker: str, topic: str, state: State):
        self.broker = broker
        self.topic = topic
        self.state = state
        self.last_message_time: float = None
        self.timeout_task = None
        self.is_disconnected = False

    async def start(self):
        self.task = asyncio.create_task(self.connect())
        self.timeout_task = asyncio.create_task(self.monitor_timeout())
    
    async def connect(self):
        try:
            logger.info(f"Connecting...")
            async with aiomqtt.Client(self.broker) as client:
                logger.info(f"Connected to {self.broker}")
                await client.subscribe(self.topic)
                logger.info(f"Subscribed to {self.topic}")
                async for message in client.messages:
                    asyncio.create_task(self.on_message(message))
        except aiomqtt.MqttError as e:
            logger.error(f"MQTT connection failed: {e}")

    async def on_message(self, msg: aiomqtt.Message):
        logger.info(f"MQTT message arrived: {msg.payload.decode('utf-8')}")
        self.last_message_time = time.time()
        
        # If we were in UNCONNECTED state, restore previous mode
        if self.is_disconnected:
            logger.info("Network connection restored, returning to previous mode")
            self.is_disconnected = False
            if self.state.mode_before_disconnect:
                self.state.set_mode(self.state.mode_before_disconnect)
                self.state.handle_mode_change(self.state.mode_before_disconnect)
            else:
                self.state.set_mode(Mode.AUTOMATIC)
                self.state.handle_mode_change(Mode.AUTOMATIC)
        
        try:
            parsed = json.loads(msg.payload.decode("utf-8"))
            if parsed["type"] == "water_level":
                self.state.set_water_level(float(parsed["value"]))
        except (ValueError, KeyError, json.JSONDecodeError):
            logger.warning(f"Invalid payload: {msg.payload}")

    async def monitor_timeout(self):
        """Monitor MQTT connection timeout. If no message for T2 seconds, enter UNCONNECTED state."""
        while True:
            try:
                await asyncio.sleep(1)  # Check every second
                
                if self.last_message_time is not None:
                    time_since_last_message = time.time() - self.last_message_time
                    
                    # If timeout exceeded and not already in UNCONNECTED state
                    if time_since_last_message > T2 and not self.is_disconnected:
                        logger.warning(f"No MQTT message for {time_since_last_message:.1f}s (> {T2}s). Entering UNCONNECTED state")
                        self.is_disconnected = True
                        self.state.mode_before_disconnect = self.state.mode
                        self.state.handle_mode_change(Mode.UNCONNECTED)
                        self.state.set_mode(Mode.UNCONNECTED)
                        
            except asyncio.CancelledError:
                logger.info("MQTT timeout monitor stopped")
                break
            except Exception as e:
                logger.error(f"Error in timeout monitor: {e}")

    async def stop(self):
        if self.task:
            self.task.cancel()
            try:
                await self.task
            except asyncio.CancelledError:
                logger.info(f"MQTT listener stopped")
        
        if self.timeout_task:
            self.timeout_task.cancel()
            try:
                await self.timeout_task
            except asyncio.CancelledError:
                logger.info(f"MQTT timeout monitor stopped")