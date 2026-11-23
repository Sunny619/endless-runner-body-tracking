"""
Main execution pipeline for the AI Vision Motion Tracking Controller.
Captures live webcam video, extracts pose landmarks, classifies gestures, and emits OS inputs.
"""

import argparse
import sys
import time
import logging

try:
    import cv2
except ImportError:
    cv2 = None

from .config import ControllerConfig
from .pose_detector import PoseDetector
from .gesture_classifier import GestureClassifier, GestureAction, GestureState
from .input_bridge import InputBridge
from .telemetry_overlay import TelemetryOverlay

logging.basicConfig(level=logging.INFO, format="%(asctime)s [%(levelname)s] %(name)s: %(message)s")
logger = logging.getLogger("MotionController")


class MotionControllerEngine:
    """
    Integrates optical capture, computer vision inference, gesture state machine, and input dispatch.
    """

    def __init__(self, config: ControllerConfig, mock_input: bool = False):
        self.config = config
        self.detector = PoseDetector(
            min_detection_confidence=config.min_detection_confidence,
            min_tracking_confidence=config.min_tracking_confidence,
            smoothing_alpha=config.smoothing_alpha,
        )
        self.classifier = GestureClassifier(config=config)
        self.bridge = InputBridge(config=config, mock_mode=mock_input)
        self.overlay = TelemetryOverlay(config=config)
        self.running = False

    def run(self, no_gui: bool = False):
        """
        Starts the real-time webcam frame acquisition and gesture dispatch loop.
        """
        if cv2 is None:
            logger.error("OpenCV (cv2) is not installed. Please install opencv-python.")
            return

        cap = cv2.VideoCapture(self.config.camera_id)
        cap.set(cv2.CAP_PROP_FRAME_WIDTH, self.config.frame_width)
        cap.set(cv2.CAP_PROP_FRAME_HEIGHT, self.config.frame_height)

        if not cap.isOpened():
            logger.error(f"Cannot open camera index {self.config.camera_id}.")
            return

        logger.info("==========================================================")
        logger.info("  CyberRunner AI Vision Motion Controller Active          ")
        logger.info("  Raise Right Hand above guide line to Start              ")
        logger.info("  Lean Left/Right to change lanes; Jump/Duck to dodge     ")
        logger.info("  Press [ESC] or [Q] in the camera preview to exit        ")
        logger.info("==========================================================")

        self.running = True
        prev_frame_time = time.time()
        latest_action = GestureAction.NONE

        try:
            while self.running and cap.isOpened():
                success, frame = cap.read()
                if not success:
                    logger.warning("Empty camera frame received. Retrying...")
                    time.sleep(0.01)
                    continue

                # Compute frame-rate
                now = time.time()
                fps = 1.0 / max(now - prev_frame_time, 1e-4)
                prev_frame_time = now

                # Run MediaPipe pose detection
                keypoints, annotated = self.detector.process_frame(frame)

                if keypoints and keypoints.has_valid_pose:
                    action, state = self.classifier.process_keypoints(
                        center_x=keypoints.center_x,
                        center_y=keypoints.center_y,
                        right_wrist_y=keypoints.right_wrist_y,
                    )
                    if action != GestureAction.NONE:
                        latest_action = action
                        self.bridge.dispatch_action(action)
                else:
                    state = GestureState.IDLE

                if not no_gui:
                    # Draw visual HUD guidelines and status badge
                    display_frame = self.overlay.render(
                        frame=annotated,
                        center_x=keypoints.center_x if keypoints else 0.5,
                        center_y=keypoints.center_y if keypoints else 0.5,
                        current_state=state,
                        latest_action=latest_action,
                        game_started=self.classifier.game_started,
                        fps=fps,
                    )

                    # Mirror selfie-view horizontally for natural user feedback
                    mirrored = cv2.flip(display_frame, 1)
                    cv2.imshow(self.config.window_title, mirrored)

                    key = cv2.waitKey(5) & 0xFF
                    if key in (27, ord('q'), ord('Q')):
                        logger.info("Exit command received. Shutting down controller...")
                        break

        except KeyboardInterrupt:
            logger.info("Keyboard interrupt received.")
        finally:
            self.running = False
            cap.release()
            self.detector.close()
            if not no_gui:
                cv2.destroyAllWindows()
            logger.info("Motion controller cleanly terminated.")


def parse_arguments():
    parser = argparse.ArgumentParser(description="CyberRunner 3D - AI Vision Tracking Controller")
    parser.add_argument("--camera-id", type=int, default=0, help="Webcam device index (default: 0)")
    parser.add_argument("--width", type=int, default=640, help="Camera capture width (default: 640)")
    parser.add_argument("--height", type=int, default=480, help="Camera capture height (default: 480)")
    parser.add_argument("--no-gui", action="store_true", help="Run in headless mode without preview window")
    parser.add_argument("--mock-input", action="store_true", help="Log virtual keystrokes without injecting OS events")
    return parser.parse_args()


def main():
    args = parse_arguments()
    config = ControllerConfig(
        camera_id=args.camera_id,
        frame_width=args.width,
        frame_height=args.height,
    )
    controller = MotionControllerEngine(config=config, mock_input=args.mock_input)
    controller.run(no_gui=args.no_gui)


if __name__ == "__main__":
    main()
