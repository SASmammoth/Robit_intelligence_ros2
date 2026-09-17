from launch import LaunchDescription
from launch.actions import EmitEvent, RegisterEventHandler
from launch.events import matches_action
from launch_ros.actions import LifecycleNode
from launch_ros.event_handlers import OnStateTransition
from launch_ros.events.lifecycle import ChangeState
from lifecycle_msgs.msg import Transition


def generate_launch_description():
    watchdog = LifecycleNode(
        package='hw1',
        executable='imu_watchdog',
        name='imu_watchdog',
        namespace='',
        output='screen',
        parameters=[{
            'subscription_check_rate_hz': 2.0,
            'subscripton_timeout_sec': 5.0,
            'is_wall_timer': False,
        }],
    )

    # 1) 노드가 뜨면 configure 요청
    configure = EmitEvent(event=ChangeState(
        lifecycle_node_matcher=matches_action(watchdog),
        transition_id=Transition.TRANSITION_CONFIGURE,
    ))

    # 2) inactive에 도달하면 activate 요청
    activate_when_inactive = RegisterEventHandler(OnStateTransition(
        target_lifecycle_node=watchdog,
        goal_state='inactive',
        entities=[EmitEvent(event=ChangeState(
            lifecycle_node_matcher=matches_action(watchdog),
            transition_id=Transition.TRANSITION_ACTIVATE,
        ))],
    ))

    return LaunchDescription([
        activate_when_inactive,  # 핸들러를 먼저 등록
        watchdog,
        configure,
    ])