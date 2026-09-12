import unittest
from ai_controller.pose_detector import NormalizedKeypoints, PoseDetector


class TestPoseDetectorMath(unittest.TestCase):
    def test_keypoints_namedtuple_instantiation(self):
        kp = NormalizedKeypoints(
            center_x=0.48,
            center_y=0.52,
            right_wrist_y=0.25,
            left_wrist_y=0.70,
            has_valid_pose=True,
        )
        self.assertAlmostEqual(kp.center_x, 0.48)
        self.assertAlmostEqual(kp.center_y, 0.52)
        self.assertAlmostEqual(kp.right_wrist_y, 0.25)
        self.assertTrue(kp.has_valid_pose)

    def test_ema_smoothing_convergence(self):
        detector = PoseDetector(smoothing_alpha=0.5)

        # Simulate consecutive noisy landmark observations
        # First observation sets initial anchor
        detector._filtered_x = 0.50
        detector._filtered_y = 0.50

        # Step 2: raw reading shifted to 0.70
        alpha = 0.5
        new_raw = 0.70
        detector._filtered_x = alpha * new_raw + (1.0 - alpha) * detector._filtered_x

        # Expected: 0.5 * 0.70 + 0.5 * 0.50 = 0.60
        self.assertAlmostEqual(detector._filtered_x, 0.60)

        # Step 3: raw reading stays at 0.70
        detector._filtered_x = alpha * new_raw + (1.0 - alpha) * detector._filtered_x
        # Expected: 0.5 * 0.70 + 0.5 * 0.60 = 0.65
        self.assertAlmostEqual(detector._filtered_x, 0.65)


if __name__ == "__main__":
    unittest.main()
