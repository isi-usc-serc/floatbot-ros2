from launch import LaunchDescription
from launch.substitutions import Command, PathJoinSubstitution
from launch_ros.actions import Node
from launch_ros.substitutions import FindPackageShare

# Parameters
floatbot_ns = 'floatbot'

def generate_launch_description():
    ld = LaunchDescription()

    # Compile the URDF with Xacro
    package_dir = FindPackageShare('floatbot_description')
    urdf_path = PathJoinSubstitution([package_dir, 'urdf/floatbot.urdf.xacro'])
    compiled_urdf = Command([
        'xacro ', urdf_path,
        ' tag_id:=', '1'
    ])

    ld.add_action(
        Node(
            package = 'robot_state_publisher',
            executable = 'robot_state_publisher',
            namespace = floatbot_ns,
            parameters = [{
                'robot_description': compiled_urdf
            }]
        )
    )

    # Apriltag pose publisher
    ld.add_action(
        Node(
            package = 'floatbot_localization',
            executable = 'camera_pose_publisher',
            name = 'pose_publisher',
            namespace = floatbot_ns,
            parameters = [{
                'tag_frame': 'tag36h11:1',
                'tag_link': 'tag_1_link'
            }]
        )
    )

    # EKF Node
    ld.add_action(
        Node(
            package = 'robot_localization',
            executable = 'ekf_node',
            name = 'ekf',
            namespace = floatbot_ns,
            parameters = [{
                PathJoinSubstitution([FindPackageShare('floatbot_localization'), 'cfg', 'ekf_floatbot.yaml'])
            }]
        )
    )

    return ld