"""
Gesture classification state machine.
Translates normalized torso and wrist keypoints into discrete game control actions.
"""

from enum import Enum, auto
from typing import Optional, Tuple
import time
from .config import ControllerConfig


class GestureState(Enum):
    IDLE = "idle"
    CENTER = "center"
    LEFT = "left"
    RIGHT = "right"
    UP = "up"
    DOWN = "down"


class GestureAction(Enum):
    NONE = auto()
    MOVE_LEFT = auto()
    MOVE_RIGHT = auto()
    JUMP = auto()
    CROUCH = auto()
    START_GAME = auto()


class GestureClassifier:
    """
    State machine that resolves body motion into discrete game commands with hysteresis.
    """

    def __init__(self, config: Optional[ControllerConfig] = None):
        self.config = config or ControllerConfig()
        self.current_state = GestureState.IDLE
        self.previous_state = GestureState.IDLE

        self.last_jump_time = 0.0
        self.last_lane_time = 0.0
        self.game_started = False

    def check_start_gesture(self, right_wrist_y: float) -> bool:
        """
        Detects if user raises their right hand above the top boundary line to start game.
        """
        if not self.game_started and right_wrist_y < self.config.boundary_top:
            self.game_started = True
            return True
        return False

    def evaluate_target_state(self, x: float, y: float) -> GestureState:
        """
        Determines the spatial region corresponding to torso center (x, y).
        """
        if y < self.config.boundary_top:
            return GestureState.UP
        elif y > self.config.boundary_bottom:
            return GestureState.DOWN
        elif x > self.config.boundary_left:
            # Body shifted to right of camera screen (mirrored view = user leaned left)
            return GestureState.LEFT
        elif x < self.config.boundary_right:
            # Body shifted to left of camera screen (mirrored view = user leaned right)
            return GestureState.RIGHT
        else:
            return GestureState.CENTER

    def process_keypoints(
        self,
        center_x: float,
        center_y: float,
        right_wrist_y: float,
    ) -> Tuple[GestureAction, GestureState]:
        """
        Main tick method: transitions state machine and emits actions.
        """
        now = time.time()

        # Check hand raise to initiate game
        if not self.game_started:
            if self.check_start_gesture(right_wrist_y):
                return GestureAction.START_GAME, GestureState.IDLE
            return GestureAction.NONE, GestureState.IDLE

        target_state = self.evaluate_target_state(center_x, center_y)
        action = GestureAction.NONE

        if target_state != self.current_state:
            # Handle lateral lane transitions
            if self.current_state == GestureState.CENTER:
                if target_state == GestureState.LEFT and (now - self.last_lane_time > self.config.lane_change_cooldown_sec):
                    action = GestureAction.MOVE_LEFT
                    self.last_lane_time = now
                elif target_state == GestureState.RIGHT and (now - self.last_lane_time > self.config.lane_change_cooldown_sec):
                    action = GestureAction.MOVE_RIGHT
                    self.last_lane_time = now
            elif target_state == GestureState.CENTER:
                # Returning back to center lane from left or right
                if self.current_state == GestureState.RIGHT:
                    action = GestureAction.MOVE_LEFT
                elif self.current_state == GestureState.LEFT:
                    action = GestureAction.MOVE_RIGHT

            # Handle vertical actions (Jump and Crouch)
            if target_state == GestureState.UP:
                if now - self.last_jump_time > self.config.jump_cooldown_sec:
                    action = GestureAction.JUMP
                    self.last_jump_time = now
            elif target_state == GestureState.DOWN:
                action = GestureAction.CROUCH

            self.previous_state = self.current_state
            self.current_state = target_state

        return action, self.current_state

    def reset(self):
        self.current_state = GestureState.IDLE
        self.previous_state = GestureState.IDLE
        self.game_started = False
        self.last_jump_time = 0.0
        self.last_lane_time = 0.0
