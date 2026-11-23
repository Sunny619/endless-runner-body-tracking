"""
Root Entry Point for AI Vision Motion Tracking Controller.
Supports seamless execution via the modular ai_controller package with graceful legacy fallback.
"""

import sys
import os

# Add local directory to module search path
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))

try:
    from ai_controller.main import main as run_controller
except ImportError:
    run_controller = None


def run_legacy_pipeline():
    """
    Fallback baseline implementation in case modular package dependencies are unbundled.
    """
    import cv2
    import mediapipe as mp
    import pyautogui

    mp_drawing = mp.solutions.drawing_utils
    mp_drawing_styles = mp.solutions.drawing_styles
    mp_pose = mp.solutions.pose

    boundaries = {"left": 0.6, "right": 0.4, "bottom": 0.6, "top": 0.4}
    states = ("left", "right", "up", "down", "center")
    x, y = 0.5, 0.5
    cur_state = "idle"
    new_state = "idle"
    tutorial = True
    flag = False

    def move(key):
        print(f"[ACTION] Virtual Input: {key}")
        pyautogui.press(key)

    def transition():
        nonlocal cur_state, new_state
        if new_state != cur_state:
            if cur_state == states[4] and (new_state == states[1] or new_state == states[0]):
                move(new_state)
            elif new_state == states[4] and (cur_state == states[1] or cur_state == states[0]):
                if cur_state == states[1]:
                    move(states[0])
                else:
                    move(states[1])
            elif new_state in (states[3], states[2]):
                move(new_state)
            cur_state = new_state

    def check():
        nonlocal cur_state, new_state
        if y < boundaries["top"]:
            new_state = states[2]
        elif y > boundaries["bottom"]:
            new_state = states[3]
        elif x > boundaries["left"]:
            new_state = states[0]
        elif x < boundaries["right"]:
            new_state = states[1]
        else:
            new_state = "center"
        transition()

    def start(y_pos):
        nonlocal flag
        if y_pos < boundaries["top"]:
            flag = True
            pyautogui.press('s')
            print("[STATUS] Game Started via Hand Raise Gesture")

    cap = cv2.VideoCapture(0)
    cap.set(cv2.CAP_PROP_FRAME_WIDTH, 640)
    cap.set(cv2.CAP_PROP_FRAME_HEIGHT, 480)

    with mp_pose.Pose(min_detection_confidence=0.5, min_tracking_confidence=0.5) as pose:
        while cap.isOpened():
            success, image = cap.read()
            if not success:
                continue

            h, w, _ = image.shape
            if tutorial:
                cv2.rectangle(image, (0, int(boundaries["top"] * h)), (int(w * boundaries["right"]), int(boundaries["bottom"] * h)), (0, 255, 0), 1)
                cv2.rectangle(image, (w, int(boundaries["top"] * h)), (int(w * boundaries["left"]), int(boundaries["bottom"] * h)), (0, 255, 0), 1)
                cv2.rectangle(image, (0, 0), (w, int(boundaries["top"] * h)), (0, 255, 0), 1)
                cv2.rectangle(image, (0, int(boundaries["bottom"] * h)), (w, h), (0, 255, 0), 1)

            image.flags.writeable = False
            image = cv2.cvtColor(image, cv2.COLOR_BGR2RGB)
            results = pose.process(image)

            image.flags.writeable = True
            image = cv2.cvtColor(image, cv2.COLOR_RGB2BGR)

            if results.pose_landmarks:
                mp_drawing.draw_landmarks(
                    image,
                    results.pose_landmarks,
                    mp_pose.POSE_CONNECTIONS,
                    landmark_drawing_spec=mp_drawing_styles.get_default_pose_landmarks_style(),
                )
                x = (results.pose_landmarks.landmark[mp_pose.PoseLandmark.LEFT_SHOULDER].x +
                     results.pose_landmarks.landmark[mp_pose.PoseLandmark.RIGHT_SHOULDER].x) / 2.0
                y = (results.pose_landmarks.landmark[11].y + results.pose_landmarks.landmark[24].y) / 2.0

                if flag:
                    check()
                else:
                    start(results.pose_landmarks.landmark[16].y)

            cv2.imshow('MediaPipe Pose - CyberRunner AI', cv2.flip(image, 1))
            if cv2.waitKey(5) & 0xFF == 27:
                break

    cap.release()
    cv2.destroyAllWindows()


if __name__ == "__main__":
    if run_controller is not None:
        run_controller()
    else:
        run_legacy_pipeline()
