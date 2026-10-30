# ROS2 Node Packages
import rclpy
from rclpy.node import Node
from nav_msgs.msg import Odometry
from geometry_msgs.msg import PoseStamped
import numpy as np

class FloatbotMPC(Node):

    def __init__(self):
        super().__init__('floatbot_mpc')

        # initialize floatbot state
        self.r = np.array([0, 0]) # position (m)
        self.q = np.array([1, 0]) # quaternion orientation
        self.v = np.array([0, 0]) # velocity (m/s)
        self.w = 0 # angular velocity (rad/s)

        # initialize commanded pose
        self.r_cmd = np.array([0, 0, 0]) # position command (m)
        self.q_cmd = np.array([1, 0, 0, 0]) # quaternion orientation command

        # create odometry subscriber
        self.odom_sub = self.create_subscription(
            Odometry,
            'odometry/filtered',
            self.odom_callback,
            10
        )

        # create pose command subscriber
        self.cmd_sub = self.create_subscription(
            PoseStamped,
            'pose_cmd',
            self.cmd_callback,
            10
        )

    def odom_callback(self, msg: Odometry):
        self.get_logger().info('Odometry message recieved')

        # read message and extract planar components
        self.r = np.array([msg.pose.pose.position.x, msg.pose.pose.position.y])
        self.v = np.array([msg.twist.twist.linear.x, msg.twist.twist.linear.y])
        self.w = msg.twist.twist.angular.z

        # normalize quaternion
        q_norm = np.array([msg.pose.pose.orientation.w, msg.pose.pose.orientation.x, msg.pose.pose.orientation.y, msg.pose.pose.orientation.z])
        q_norm /= np.linalg.norm(q_norm)
        self.q = np.array([q_norm[0], q_norm[3]])

    def cmd_callback(self, msg: PoseStamped):
        self.get_logger().info('Command message recieved')

        # read message and extract planar components
        self.r_cmd = np.array([msg.pose.position.x, msg.pose.position.y])

        # normalize quaternion
        q_norm = np.array([msg.pose.orientation.w, msg.pose.orientation.x, msg.pose.orientation.y, msg.pose.orientation.z])
        q_norm /= np.linalg.norm(q_norm)
        self.q_cmd = np.array([q_norm[0], q_norm[3]])

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