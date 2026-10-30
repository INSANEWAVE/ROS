from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
    return LaunchDescription([
        Node(
            package="triangle_sides_parameters",
            executable="triangle_sides_parameters_node",
            name="test_param_node1",
            output="screen",
            parameters=[
                {
                    "side_A": 3.0,
                    "side_B": 4.0,
                    "side_С": 5.0
                }
            ]
        )
    ])
