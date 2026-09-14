from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
    return LaunchDescription([
        Node(
            package='hw1',
            executable='MySubscriber',
            name='mySubscriber'
        ),
        Node(
            package='hw1',
            executable='MyPublisher',
            name='myPublisher'
        ),
    ])