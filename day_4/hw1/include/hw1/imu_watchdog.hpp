#include <chrono>
#include <memory>
#include <string>

#include "rclcpp/rclcpp.hpp"
#include "rclcpp_lifecycle/lifecycle_node.hpp"
#include "rclcpp_lifecycle/lifecycle_publisher.hpp"
#include "sensor_msgs/msg/imu.hpp"
#include "lifecycle_msgs/srv/get_state.hpp"

using CallbackReturn =
    rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn;


class imu_watchdog : public rclcpp_lifecycle::LifecycleNode
{
public:
    // 생성자: 파라미터 선언만. 실제 초기화는 on_configure에서.
    explicit imu_watchdog(const rclcpp::NodeOptions &options = rclcpp::NodeOptions());

    CallbackReturn on_configure(const rclcpp_lifecycle::State &) override;

    CallbackReturn on_activate(const rclcpp_lifecycle::State &state) override;

    CallbackReturn on_deactivate(const rclcpp_lifecycle::State &state) override;

    CallbackReturn on_cleanup(const rclcpp_lifecycle::State &) override;

    CallbackReturn on_shutdown(const rclcpp_lifecycle::State &previous_state) override;

    CallbackReturn on_error(const rclcpp_lifecycle::State &previous_state) override;

private:
    void imu_subScribe(const sensor_msgs::msg::Imu::SharedPtr msg);

    // 검증용 타임아웃 헤르츠와 시간
    double check_rate_hz{0.0}; 
    double timeout_sec{0.0};

    bool is_wall_timer_ = false; // wall timer 사용여부

    void check_rate_func(); // 검증용 타임아웃 헤르츠마다 실행되는 함수

    // age 검사 용 변수 (NOSIG, TIMEOUT등을 관장)
    bool is_firstRecive = false; // 처음 수신 받으면
    bool is_timeout = false; // 타임 아웃 됬으면
    bool is_NO_DATA_print = false; // NoData가 출력이 되었는가

    std::size_t count_{0};
    rclcpp::Subscription<sensor_msgs::msg::Imu>::SharedPtr sub_; // 구독자 포인터 생성
    rclcpp_lifecycle::LifecyclePublisher<sensor_msgs::msg::Imu>::SharedPtr pub_; // 발행자 포인터 생성
    rclcpp::TimerBase::SharedPtr timer_; // 신선한 데이터 발행자
    rclcpp::Time last_stamp_; // 마지막 메시지의 header


    // 4일차 과제 추가
    rclcpp::TimerBase::SharedPtr client_timer_; // 1초단위 클라이언트 타이머
    rclcpp::Client<lifecycle_msgs::srv::GetState>::SharedPtr imu_state_client_; // 1초마다 상태를 묻는 타이머
    void count_collback(); // 클라이언트 타이머 실행시 콜밷 되는 함수
    rclcpp::CallbackGroup::SharedPtr client_group_; // 멀티스래드용 클라이언트 그룹 포인터?
};