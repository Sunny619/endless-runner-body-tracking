"""
CyberRunner 3D - AI Vision Motion Tracking Controller
Proprietary Real-Time Computer Vision & Human-Computer Interaction Package
"""

__version__ = "2.1.0"
__author__ = "Engineering Team"

from .config import ControllerConfig
from .pose_detector import PoseDetector
from .gesture_classifier import GestureClassifier, GestureState
from .input_bridge import InputBridge
from .telemetry_overlay import TelemetryOverlay

__all__ = [
    "ControllerConfig",
    "PoseDetector",
    "GestureClassifier",
    "GestureState",
    "InputBridge",
    "TelemetryOverlay",
]
