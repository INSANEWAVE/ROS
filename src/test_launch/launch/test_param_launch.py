from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
    return LaunchDescription([
        Node(
            package="test_parameters",
            executable="test_parameters_node",
            name="test_param_node1",
            output="screen",
            parameters=[
                {"string_parameter": "launch_test1",
                 "number_parameter": 150.4}
            ]
        ),
        Node(
            package="test_parameters",
            executable="test_parameters_node",
            name="test_param_node2",
            output="screen",
            parameters=[
                {"string_parameter": "launch_test11",
                 "number_parameter": 255.4}
            ]
        )
    ])
