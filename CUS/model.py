from enum import StrEnum
import logging

logger = logging.getLogger(__name__)  


class Mode(StrEnum):
    AUTOMATIC = "AUTOMATIC"
    REMOTE_MANUAL = "REMOTE_MANUAL"
    LOCAL_MANUAL = "LOCAL_MANUAL"
    UNCONNECTED = "UNCONNECTED"

class State():
    def __init__(self):
        self.mode: Mode = Mode.AUTOMATIC
        self.water_level: float = 0

    def set_mode(self, mode: Mode):
        logger.debug(f"Mode change. From [{self.mode}] to [{mode}]")
        self.mode = mode

    def set_water_level(self, level: float):
        logger.debug(f"Water level change. From [{self.water_level}] to [{level}]")
        self.water_level = level


