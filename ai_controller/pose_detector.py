"""
MediaPipe Pose detection wrapper with Exponential Moving Average (EMA) smoothing.
"""

from typing import Optional, Tuple, NamedTuple, Any

try:
    import numpy as np
except ImportError:
    np = None

try:
    import cv2
    import mediapipe as mp
except ImportError:
    cv2 = None
    mp = None


class NormalizedKeypoints(NamedTuple):
    center_x: float
    center_y: float
    right_wrist_y: float
    left_wrist_y: float
    has_valid_pose: bool


class PoseDetector:
    """
    Encapsulates MediaPipe Pose landmark estimation with temporal jitter filtering.
    """

    def __init__(
        self,
        min_detection_confidence: float = 0.5,
        min_tracking_confidence: float = 0.5,
        smoothing_alpha: float = 0.65,
    ):
        self.min_detection_confidence = min_detection_confidence
        self.min_tracking_confidence = min_tracking_confidence
        self.smoothing_alpha = smoothing_alpha

        # Smoothed coordinate state
        self._filtered_x: Optional[float] = None
        self._filtered_y: Optional[float] = None
        self._filtered_right_wrist_y: Optional[float] = None

        self._mp_pose = None
        self._pose_instance = None
        self._mp_drawing = None
        self._mp_drawing_styles = None

        if mp is not None:
            self._mp_pose = mp.solutions.pose
            self._mp_drawing = mp.solutions.drawing_utils
            self._mp_drawing_styles = mp.solutions.drawing_styles
            self._pose_instance = self._mp_pose.Pose(
                min_detection_confidence=self.min_detection_confidence,
                min_tracking_confidence=self.min_tracking_confidence,
            )

    def close(self):
        if self._pose_instance is not None:
            self._pose_instance.close()

    def process_frame(self, frame_bgr: Any) -> Tuple[Optional[NormalizedKeypoints], Any]:
        """
        Processes a BGR image frame, runs pose estimation, and returns keypoints and annotated image.
        """
        if self._pose_instance is None or frame_bgr is None:
            return None, frame_bgr

        # Convert color space for MediaPipe RGB pipeline
        frame_rgb = cv2.cvtColor(frame_bgr, cv2.COLOR_BGR2RGB)
        frame_rgb.flags.writeable = False

        results = self._pose_instance.process(frame_rgb)

        frame_bgr.flags.writeable = True
        annotated_frame = frame_bgr.copy()

        if not results.pose_landmarks:
            return NormalizedKeypoints(0.5, 0.5, 1.0, 1.0, False), annotated_frame

        landmarks = results.pose_landmarks.landmark

        # Draw MediaPipe skeleton annotation
        if self._mp_drawing and self._mp_drawing_styles:
            self._mp_drawing.draw_landmarks(
                annotated_frame,
                results.pose_landmarks,
                self._mp_pose.POSE_CONNECTIONS,
                landmark_drawing_spec=self._mp_drawing_styles.get_default_pose_landmarks_style(),
            )

        # Extract shoulder and hip coordinates
        # Landmark 11 = LEFT_SHOULDER, 12 = RIGHT_SHOULDER
        # Landmark 23 = LEFT_HIP, 24 = RIGHT_HIP
        # Landmark 16 = RIGHT_WRIST, 15 = LEFT_WRIST
        left_shoulder = landmarks[11]
        right_shoulder = landmarks[12]
        left_hip = landmarks[23]
        right_hip = landmarks[24]
        right_wrist = landmarks[16]
        left_wrist = landmarks[15]

        # Compute raw center of torso
        raw_x = (left_shoulder.x + right_shoulder.x) / 2.0
        raw_y = (left_shoulder.y + right_hip.y) / 2.0
        raw_wrist_y = right_wrist.y

        # Apply Exponential Moving Average (EMA) smoothing to eliminate camera flicker
        if self._filtered_x is None:
            self._filtered_x = raw_x
            self._filtered_y = raw_y
            self._filtered_right_wrist_y = raw_wrist_y
        else:
            self._filtered_x = self.smoothing_alpha * raw_x + (1.0 - self.smoothing_alpha) * self._filtered_x
            self._filtered_y = self.smoothing_alpha * raw_y + (1.0 - self.smoothing_alpha) * self._filtered_y
            self._filtered_right_wrist_y = (
                self.smoothing_alpha * raw_wrist_y + (1.0 - self.smoothing_alpha) * self._filtered_right_wrist_y
            )

        keypoints = NormalizedKeypoints(
            center_x=float(self._filtered_x),
            center_y=float(self._filtered_y),
            right_wrist_y=float(self._filtered_right_wrist_y),
            left_wrist_y=float(left_wrist.y),
            has_valid_pose=True,
        )

        return keypoints, annotated_frame

    def reset_smoothing(self):
        self._filtered_x = None
        self._filtered_y = None
        self._filtered_right_wrist_y = None
