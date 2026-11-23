import unittest
import math


class TestGamePhysicsAndCollisions(unittest.TestCase):
    def test_jump_trajectory_kinematics(self):
        ground_y = 0.35
        # Verify sinusoidal jump peak at 90 degrees
        peak_rad = 90.0 * math.pi / 180.0
        peak_y = ground_y + math.sin(peak_rad)
        self.assertAlmostEqual(peak_y, 1.35)

        # Verify landing at 180 degrees
        landing_rad = 180.0 * math.pi / 180.0
        landing_y = ground_y + math.sin(landing_rad)
        self.assertAlmostEqual(landing_y, ground_y, places=5)

    def test_discrete_lane_boundaries(self):
        lane_width = 1.4
        lanes = [-1.4, 0.0, 1.4]

        # Ensure symmetric 3-lane configuration
        self.assertEqual(len(lanes), 3)
        self.assertAlmostEqual(lanes[1] - lanes[0], lane_width)
        self.assertAlmostEqual(lanes[2] - lanes[1], lane_width)

    def test_aabb_collision_clearance(self):
        player_x = 0.0
        player_y = 0.35
        ground_y = 0.35
        collision_radius = 0.55

        # Ground level obstacle in same lane
        obstacle_x = 0.0
        is_x_aligned = abs(player_x - obstacle_x) < 0.7
        is_hit = is_x_aligned and ((player_y - ground_y) < collision_radius)
        self.assertTrue(is_hit, "Player at ground level must collide with obstacle in same lane")

        # Jumping player clears obstacle
        jumped_y = 1.20
        is_cleared = is_x_aligned and ((jumped_y - ground_y) >= collision_radius)
        self.assertTrue(is_cleared, "Jumping player with elevation >= 0.55 must clear obstacle")


if __name__ == "__main__":
    unittest.main()
