from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
    return LaunchDescription([
        # Первая нода: Ваш C++ узел с орбитой
        Node(
            package="test_tf_broadcaster",
            executable="test_tf_broadcaster_node",
            name="test_tf_broadcaster_node",
            output="screen"
        ),

        # Вторая нода: Стандартный статический публикатор трансформаций из ROS 2
        Node(
            package='tf2_ros',
            executable='static_transform_publisher',
            arguments = ['--x', '0.2',
                         '--y', '0.1',
                         '--z', '0',
                         '--yaw', '0.1',
                         '--pitch', '0',
                         '--roll', '0',
                         '--frame-id', 'base_link',
                         '--child-frame-id', 'sensor_link']
        )
    ])
