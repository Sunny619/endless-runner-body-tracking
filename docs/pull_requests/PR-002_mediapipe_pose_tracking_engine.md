# PR #002: MediaPipe Optical Pose Tracking Pipeline & Gesture FSM

- **Author**: Computer Vision & AI Team
- **Type**: Feature / HCI
- **Status**: Merged into `develop`

---

## 📌 Description
Establishes the real-time optical computer vision tracking engine:
- `ai_controller/config.py`: Declarative configuration defining boundary thresholds (`top: 0.40`, `bottom: 0.60`, `left: 0.60`, `right: 0.40`).
- `ai_controller/pose_detector.py`: MediaPipe Pose wrapper processing 33 3D skeletal landmarks, extracting shoulder midpoint $X$, torso vertical midpoint $Y$, and wrist elevation.
- `ai_controller/gesture_classifier.py`: Finite state machine converting continuous body coordinates into discrete game commands (`MOVE_LEFT`, `MOVE_RIGHT`, `JUMP`, `CROUCH`, `START_GAME`) with hysteresis.

---

## 🧪 Validation & Testing
- Validated hand-raise start detection: wrist $Y < 0.40$ cleanly triggers game start.
- Verified mirrored camera coordinate space mapping.
