"""
Virtual input emitter and OS bridge.
Dispatches simulated keyboard events with safety rate-limiting and headless mocking.
"""

from typing import List, Optional
import time
import logging

try:
    import pyautogui
    # Configure safety defaults for PyAutoGUI
    pyautogui.PAUSE = 0.01
    pyautogui.FAILSAFE = False
except ImportError:
    pyautogui = None

from .config import ControllerConfig
from .gesture_classifier import GestureAction

logger = logging.getLogger("InputBridge")


class InputBridge:
    """
    Translates GestureAction enums to hardware or OS virtual keystrokes.
    """

    def __init__(self, config: Optional[ControllerConfig] = None, mock_mode: bool = False):
        self.config = config or ControllerConfig()
        self.mock_mode = mock_mode or (pyautogui is None)
        self.history: List[str] = []
        self._last_dispatch_time: float = 0.0
        self._min_dispatch_interval: float = 0.05

    def dispatch_action(self, action: GestureAction) -> Optional[str]:
        """
        Dispatches keystroke corresponding to the given GestureAction.
        """
        if action == GestureAction.NONE:
            return None

        key_to_press: Optional[str] = None
        if action == GestureAction.MOVE_LEFT:
            key_to_press = self.config.key_move_left
        elif action == GestureAction.MOVE_RIGHT:
            key_to_press = self.config.key_move_right
        elif action == GestureAction.JUMP:
            key_to_press = self.config.key_jump
        elif action == GestureAction.CROUCH:
            key_to_press = self.config.key_crouch
        elif action == GestureAction.START_GAME:
            key_to_press = self.config.key_start

        if not key_to_press:
            return None

        now = time.time()
        if now - self._last_dispatch_time < self._min_dispatch_interval:
            return None

        self._last_dispatch_time = now
        self.history.append(key_to_press)

        if self.mock_mode:
            logger.debug(f"[MOCK INPUT] Key Press: {key_to_press}")
            return key_to_press

        try:
            if pyautogui is not None:
                pyautogui.press(key_to_press)
                logger.info(f"[INPUT DISPATCH] Key: {key_to_press}")
        except Exception as e:
            logger.warning(f"Failed to dispatch key {key_to_press}: {e}")

        return key_to_press

    def clear_history(self):
        self.history.clear()
