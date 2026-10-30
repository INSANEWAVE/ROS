from launch import LaunchDescription
from launch_ros.actions import Node
import math

def generate_launch_description():
    return LaunchDescription([
        # Первая нода: C++ узел с движением по восьмёрке
        Node(
            package="custom_tf_broadcaster",
            executable="custom_tf_broadcaster_node",
            name="custom_tf_broadcaster_node",
            output="screen"
        ),

        # Статическая трансформация: base_link -> front_rangefinder
        Node(
            package='tf2_ros',
            executable='static_transform_publisher',
            arguments = ['--x', '0.5',
                         '--y', '0.0',
                         '--z', '0.2',
                         '--yaw', '0.0',
                         '--pitch', '0.0',
                         '--roll', '0.0',
                         '--frame-id', 'base_link',
                         '--child-frame-id', 'front_rangefinder']
        ),

        # Статическая трансформация: base_link -> rear_rangefinder
        # Yaw 180 градусов = math.pi (3.14159)
        Node(
            package='tf2_ros',
            executable='static_transform_publisher',
            arguments = ['--x', '-0.4',
                         '--y', '0.0',
                         '--z', '0.3',
                         '--yaw', str(math.pi),
                         '--pitch', '0.0',
                         '--roll', '0.0',
                         '--frame-id', 'base_link',
                         '--child-frame-id', 'rear_rangefinder']
        )
    ])
