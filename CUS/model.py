from enum import StrEnum
import logging
from typing import Callable
import time

L1 = 25
L2 = 50
T1 = 5 

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
        self.l1_time: float = None  
        self.valve_state: float = 0  

    def set_callable(self, on_water_level_change: Callable[[], None]):
        self.on_water_level_change: Callable[[], None] = on_water_level_change

    def set_mode(self, mode: Mode):
        logger.debug(f"Mode change. From [{self.mode}] to [{mode}]")
        self.mode = mode

    def automatic_monitor_water_level(self, level: float):
        current_time = time.time()
        if level >= L2:
            logger.warning(f"Water level {level} exceeded critical threshold L2 ({L2}). Opening valve to 100%")
            # TODO: VALVE_CONTROL - Open water channel at 100%
            self.valve_state = 100
            self.l1_time = None            
        elif level > L1:
            if self.l1_time is None:
                self.l1_time = current_time
                logger.debug(f"Water level {level} exceeded L1 ({L1}). Starting timer...")
            else:
                if self.valve_state != 50:
                    time_elapsed = current_time - self.l1_time
                    if time_elapsed >= T1:
                        logger.warning(f"Water level {level} exceeded ({L1}) for {time_elapsed:.1f}s (>= {T1}s). Opening valve to 50%")
                        # TODO: VALVE_CONTROL - Open water channel at 50%
                        self.valve_state = 50               
        else:
            if self.valve_state > 0:
                logger.info(f"Water level {level} below ({L1}). Closing valve...")
                # TODO: VALVE_CONTROL - Close water channel (0%)
                self.valve_state = 0
            self.l1_time = None  

    def set_water_level(self, level: float):
        logger.debug(f"Water level change. From [{self.water_level}] to [{level}]")
        self.water_level = level
        if self.mode == "AUTOMATIC": 
            self.automatic_monitor_water_level(level)
        self.on_water_level_change(self.water_level)
        
    
        


