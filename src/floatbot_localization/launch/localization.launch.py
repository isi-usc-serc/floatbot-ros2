from launch import LaunchDescription
from launch.actions import IncludeLaunchDescription, GroupAction
from launch.substitutions import PathJoinSubstitution
from launch_ros.actions import Node, PushRosNamespace
from launch_ros.substitutions import FindPackageShare

floatbot_ns = 'floatbot'

def generate_launch_description():
    ld = LaunchDescription()

    ld.add_action(
        GroupAction(
            actions = [
                PushRosNamespace(floatbot_ns),
                IncludeLaunchDescription(
                    PathJoinSubstitution([FindPackageShare('urdf_launch'), 'launch', 'description.launch.py']),
                    launch_arguments = {
                        'urdf_package': 'floatbot_description',
                        'urdf_package_path': PathJoinSubstitution(['urdf', 'floatbot.urdf.xacro'])
                    }.items()
                )
            ]
        )
    )

    ld.add_action(
        Node(
            package = 'floatbot_localization',
            executable = 'floatbot_pose_publisher',
            name = 'pose_publisher',
            namespace = floatbot_ns,
            parameters = [{
                'tag_frame': 'tag36h11:1',
                'tag_link': 'tag_1_link'
            }]
        )
    )

    return ld