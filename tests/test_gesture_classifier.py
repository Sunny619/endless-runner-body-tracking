import unittest
import time
from ai_controller.config import ControllerConfig
from ai_controller.gesture_classifier import GestureClassifier, GestureAction, GestureState


class TestGestureClassifier(unittest.TestCase):
    def setUp(self):
        self.config = ControllerConfig(
            boundary_left=0.60,
            boundary_right=0.40,
            boundary_top=0.40,
            boundary_bottom=0.60,
            jump_cooldown_sec=0.01,
            lane_change_cooldown_sec=0.01,
        )
        self.classifier = GestureClassifier(config=self.config)

    def test_initial_state_is_idle(self):
        self.assertEqual(self.classifier.current_state, GestureState.IDLE)
        self.assertFalse(self.classifier.game_started)

    def test_hand_raise_starts_game(self):
        # When game is not started, right wrist raised above top boundary initiates start
        action, state = self.classifier.process_keypoints(center_x=0.5, center_y=0.5, right_wrist_y=0.30)
        self.assertEqual(action, GestureAction.START_GAME)
        self.assertTrue(self.classifier.game_started)

    def test_hand_below_boundary_does_not_start(self):
        action, state = self.classifier.process_keypoints(center_x=0.5, center_y=0.5, right_wrist_y=0.75)
        self.assertEqual(action, GestureAction.NONE)
        self.assertFalse(self.classifier.game_started)

    def test_lean_left_detection(self):
        # Start game first
        self.classifier.game_started = True
        self.classifier.current_state = GestureState.CENTER

        # Leaning left in mirrored camera space shifts center_x > boundary_left (0.60)
        action, state = self.classifier.process_keypoints(center_x=0.70, center_y=0.50, right_wrist_y=0.50)
        self.assertEqual(state, GestureState.LEFT)
        self.assertEqual(action, GestureAction.MOVE_LEFT)

    def test_lean_right_detection(self):
        self.classifier.game_started = True
        self.classifier.current_state = GestureState.CENTER

        # Leaning right in mirrored camera space shifts center_x < boundary_right (0.40)
        action, state = self.classifier.process_keypoints(center_x=0.30, center_y=0.50, right_wrist_y=0.50)
        self.assertEqual(state, GestureState.RIGHT)
        self.assertEqual(action, GestureAction.MOVE_RIGHT)

    def test_return_to_center_from_left(self):
        self.classifier.game_started = True
        self.classifier.current_state = GestureState.LEFT

        action, state = self.classifier.process_keypoints(center_x=0.50, center_y=0.50, right_wrist_y=0.50)
        self.assertEqual(state, GestureState.CENTER)
        self.assertEqual(action, GestureAction.MOVE_RIGHT)

    def test_jump_elevation_detection(self):
        self.classifier.game_started = True
        self.classifier.current_state = GestureState.CENTER

        # Jumping shifts torso center_y < boundary_top (0.40)
        action, state = self.classifier.process_keypoints(center_x=0.50, center_y=0.32, right_wrist_y=0.50)
        self.assertEqual(state, GestureState.UP)
        self.assertEqual(action, GestureAction.JUMP)

    def test_crouch_detection(self):
        self.classifier.game_started = True
        self.classifier.current_state = GestureState.CENTER

        # Ducking / crouching shifts torso center_y > boundary_bottom (0.60)
        action, state = self.classifier.process_keypoints(center_x=0.50, center_y=0.72, right_wrist_y=0.70)
        self.assertEqual(state, GestureState.DOWN)
        self.assertEqual(action, GestureAction.CROUCH)


if __name__ == "__main__":
    unittest.main()
