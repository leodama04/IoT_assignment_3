import asyncio
import aiomqtt
import logging
from model import State

logger = logging.getLogger(__name__) 

class MqttConnectionManager():
    def __init__(self, broker: str, topic: str, state: State):
        self.broker = broker
        self.topic = topic
        self.state = state

    async def start(self):
        self.task = asyncio.create_task(self.connect())
    
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
        try:
            water_level = float(msg.payload.decode("utf-8"))
            self.state.set_water_level(water_level)
        except ValueError:
            logger.warning(f"Payload non valido: {msg.payload}")

    async def stop(self):
        if self.task:
            self.task.cancel()
            try:
                await self.task
            except asyncio.CancelledError:
                logger.info(f"MQTT listener stopped")