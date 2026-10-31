# ROS2 Node Packages
import rclpy
from rclpy.node import Node
from nav_msgs.msg import Odometry
from geometry_msgs.msg import PoseStamped
from floatbot_interfaces.msg import ThrusterCommand
import numpy as np

class FloatbotMPC(Node):

    def __init__(self):
        super().__init__('floatbot_mpc')

        # initialize floatbot state
        self.r_meas = np.array([0, 0]) # position (m)
        self.q_meas = np.array([1, 0]) # quaternion orientation
        self.v_meas = np.array([0, 0]) # velocity (m/s)
        self.w_meas = 0 # angular velocity (rad/s)

        # initialize pose command
        self.r_cmd = np.array([0, 0, 0]) # position command (m)
        self.q_cmd = np.array([1, 0, 0, 0]) # quaternion orientation command

        # initialize thruster commands
        self.u_cmd = np.array([0, 0, 0, 0, 0, 0, 0, 0])

        # create control loop timer
        control_period = .1 # seconds
        self.timer = self.create_timer(
            control_period,
            self.ctrl_callback
        )

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

        # create thruster publisher
        self.thruster_pub = self.create_publisher(
            ThrusterCommand,
            'thruster_cmd',
            10
        )

    def ctrl_callback(self):

        # create thrust command message
        msg = ThrusterCommand()
        msg.thrust = self.u_cmd.tolist()

        # publish message
        self.thruster_pub.publish(msg)
        self.get_logger().info(f'Publishing thruster command: {msg.thrust}')

    def odom_callback(self, msg: Odometry):

        # read message and extract planar components
        self.r_meas = np.array([msg.pose.pose.position.x, msg.pose.pose.position.y])
        self.v_meas = np.array([msg.twist.twist.linear.x, msg.twist.twist.linear.y])
        self.w_meas = msg.twist.twist.angular.z

        # normalize quaternion
        q_norm = np.array([msg.pose.pose.orientation.w, msg.pose.pose.orientation.x, msg.pose.pose.orientation.y, msg.pose.pose.orientation.z])
        q_norm /= np.linalg.norm(q_norm)
        self.q_meas = np.array([q_norm[0], q_norm[3]])

    def cmd_callback(self, msg: PoseStamped):

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