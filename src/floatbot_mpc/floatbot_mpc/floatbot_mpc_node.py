# ROS2 Node Packages
import rclpy
from rclpy.node import Node
from nav_msgs.msg import Odometry
import numpy as np

class FloatbotMPC(Node):

    def __init__(self):
        super().__init__('floatbot_mpc')

        # initialize floatbot state
        self.r = np.array([0, 0, 0]) # position (m)
        self.q = np.array([1, 0, 0, 0]) # quaternion orientation
        self.v = np.array([0, 0, 0]) # velocity (m/s)
        self.w = np.array([0, 0, 0]) # angular velocity (rad/s)

        # create odometry subscriber
        self.odom_sub = self.create_subscription(
            Odometry,
            'odometry/filtered',
            self.odom_callback,
            10
        )

    def odom_callback(self, msg: Odometry):
        self.get_logger().info('Odometry message recieved')

        self.r = np.array(msg.pose.pose.position)
        self.q = np.array(msg.pose.pose.orientation)
        self.v = np.array(msg.twist.twist.linear)
        self.w = np.array(msg.twist.twist.angular)

def main(args=None):

    # initialize node
    rclpy.init(args=args)
    floatbot_mpc = FloatbotMPC()

    # spin node
    rclpy.spin(floatbot_mpc)

    # cleanup
    floatbot_mpc.destroy_node()
    rclpy.shutdown()

if __name__ == '__main__':
    main()