"""
Computer vision HUD overlay and debugging telemetry renderer.
Renders boundary thresholds, player center crosshair, FPS, and gesture indicators.
"""

from typing import Tuple, Any

try:
    import numpy as np
except ImportError:
    np = None

try:
    import cv2
except ImportError:
    cv2 = None

from .config import ControllerConfig
from .gesture_classifier import GestureState, GestureAction


class TelemetryOverlay:
    """
    Renders visual guidelines and state badges onto webcam video frames.
    """

    COLOR_GRID = (0, 255, 128)      # Cyan / Neon Green
    COLOR_CENTER = (255, 200, 0)    # Amber
    COLOR_ACTIVE = (0, 100, 255)    # Bright Orange
    COLOR_TEXT = (255, 255, 255)    # White
    COLOR_BG = (20, 20, 30)         # Dark Charcoal

    def __init__(self, config: ControllerConfig):
        self.config = config

    def render(
        self,
        frame: Any,
        center_x: float,
        center_y: float,
        current_state: GestureState,
        latest_action: GestureAction,
        game_started: bool,
        fps: float,
    ) -> Any:
        if cv2 is None or frame is None:
            return frame

        h, w = frame.shape[:2]

        top_y = int(self.config.boundary_top * h)
        bot_y = int(self.config.boundary_bottom * h)
        left_x = int(self.config.boundary_left * w)
        right_x = int(self.config.boundary_right * w)

        if self.config.show_tutorial_grid:
            # Draw boundary partition rectangles
            cv2.rectangle(frame, (0, top_y), (right_x, bot_y), self.COLOR_GRID, 1)
            cv2.rectangle(frame, (w, top_y), (left_x, bot_y), self.COLOR_GRID, 1)
            cv2.rectangle(frame, (0, 0), (w, top_y), self.COLOR_GRID, 1)
            cv2.rectangle(frame, (0, bot_y), (w, h), self.COLOR_GRID, 1)

            # Draw central deadzone box
            cv2.rectangle(frame, (right_x, top_y), (left_x, bot_y), (100, 255, 100), 2)

        # Draw crosshair indicator at player's tracked torso center
        clamped_x = max(0.0, min(1.0, center_x))
        clamped_y = max(0.0, min(1.0, center_y))
        px = int(clamped_x * w)
        py = int(clamped_y * h)
        cv2.circle(frame, (px, py), 6, self.COLOR_CENTER, -1)
        cv2.line(frame, (px - 12, py), (px + 12, py), self.COLOR_CENTER, 2)
        cv2.line(frame, (px, py - 12), (px, py + 12), self.COLOR_CENTER, 2)

        if self.config.show_hud_telemetry:
            # Top HUD Telemetry Bar
            cv2.rectangle(frame, (0, 0), (w, 42), self.COLOR_BG, -1)

            state_label = f"STATE: {current_state.value.upper()}"
            fps_label = f"FPS: {fps:4.1f}"

            if not game_started:
                status_label = "STATUS: WAITING (Raise Right Hand to Start)"
                status_color = (0, 255, 255)
            else:
                status_label = "STATUS: ACTIVE RUN"
                status_color = (0, 255, 0)

            cv2.putText(frame, fps_label, (15, 28), cv2.FONT_HERSHEY_SIMPLEX, 0.6, self.COLOR_TEXT, 2)
            cv2.putText(frame, state_label, (140, 28), cv2.FONT_HERSHEY_SIMPLEX, 0.6, self.COLOR_ACTIVE, 2)
            cv2.putText(frame, status_label, (330, 28), cv2.FONT_HERSHEY_SIMPLEX, 0.55, status_color, 2)

            # Action flash indicator at bottom of screen
            if latest_action != GestureAction.NONE:
                action_text = f">> ACTION: {latest_action.name} <<"
                cv2.putText(frame, action_text, (w // 2 - 120, h - 20), cv2.FONT_HERSHEY_SIMPLEX, 0.7, (0, 255, 255), 2)

        return frame
