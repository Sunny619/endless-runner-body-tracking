"""
Configuration parameters for the AI Vision Motion Controller.
Specifies boundary thresholds, optical sensor metrics, and input mappings.
"""

from dataclasses import dataclass, field
from typing import Dict


@dataclass
class ControllerConfig:
    # Camera capture settings
    camera_id: int = 0
    frame_width: int = 640
    frame_height: int = 480
    target_fps: int = 30

    # MediaPipe detection parameters
    min_detection_confidence: float = 0.5
    min_tracking_confidence: float = 0.5
    model_complexity: int = 1

    # Coordinate thresholds (normalized 0.0 - 1.0)
    # Notice: In mirror selfie mode, user leaning to their left shifts landmark rightwards
    boundary_left: float = 0.60
    boundary_right: float = 0.40
    boundary_top: float = 0.40
    boundary_bottom: float = 0.60

    # Exponential Moving Average (EMA) smoothing factor (0.0 < alpha <= 1.0)
    # Higher alpha = faster response; lower alpha = smoother / less jitter
    smoothing_alpha: float = 0.65

    # Gesture debounce cooldown in seconds
    jump_cooldown_sec: float = 0.35
    lane_change_cooldown_sec: float = 0.20

    # Key binding mappings for OS virtual input injection
    key_move_left: str = "left"
    key_move_right: str = "right"
    key_jump: str = "up"
    key_crouch: str = "down"
    key_start: str = "s"

    # Overlay UI options
    show_tutorial_grid: bool = True
    show_skeleton: bool = True
    show_hud_telemetry: bool = True
    window_title: str = "CyberRunner AI - Vision Tracking Controller"
