from enum import StrEnum
import logging

logger = logging.getLogger(__name__)  


class Mode(StrEnum):
    AUTOMATIC = "AUTOMATIC"
    REMOTE_MANUAL = "REMOTE_MANUAL"
    LOCAL_MANUAL = "LOCAL_MANUAL"
    UNCONNECTED = "UNCONNECTED"

class State():
    mode: Mode = Mode.AUTOMATIC

    def set_mode(self, mode: Mode):
        logger.debug(f"Mode change. From [{self.mode}] to [{mode}]")
        self.mode = mode


