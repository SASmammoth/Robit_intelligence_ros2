import os
from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch.substitutions import Command, FindExecutable, PathJoinSubstitution
from launch_ros.actions import Node
from launch_ros.parameter_descriptions import ParameterValue
from launch_ros.substitutions import FindPackageShare

rviz_config = PathJoinSubstitution(
    [FindPackageShare('robot_description'), 'config', 'eclipse.rviz'])

def generate_launch_description():
    robot_description = ParameterValue(
        Command([
            FindExecutable(name='xacro'), ' ',
            PathJoinSubstitution(
                [FindPackageShare('robot_description'), 'urdf', 'eclipse.xacro']),
            ' with_base:=true with_arm:=true with_camera_tower:=true',
            ' use_mock_base:=true use_mock_arm:=true use_mock_ct:=true',
        ]),
        value_type=str,
    )
    controllers_yaml = PathJoinSubstitution(
        [FindPackageShare('eclipse_bringup_JKM'), 'config', 'controllers.yaml'])

    return LaunchDescription([
        Node(
            package='robot_state_publisher',
            executable='robot_state_publisher',
            parameters=[{'robot_description': robot_description}],
        ),
        
        # controller_manager는 /robot_description 토픽으로 URDF를 받음
        Node(
            package='controller_manager',
            executable='ros2_control_node',
            parameters=[{'robot_description': robot_description}, controllers_yaml],
            output='screen',
        ),
        Node(
            package='controller_manager',
            executable='spawner',
            arguments=['joint_state_broadcaster'],
        ),
        Node(
            package='controller_manager',
            executable='spawner',
            arguments=['camera_tower_controller'],
        ),
        Node(
        package='rviz2', executable='rviz2',
        arguments=['-d', rviz_config],
        ),
        
        Node(package='controller_manager', executable='spawner',
             arguments=['arm_controller']),
        Node(
            package='controller_manager',
            executable='spawner',
            arguments=['diff_drive_controller'],
        ),

        Node(package='controller_manager', executable='spawner',
             arguments=['flipper_controller']),
    ])