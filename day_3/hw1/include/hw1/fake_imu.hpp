#include <chrono>
#include <memory>
#include <string>
#include <random>

#include "rclcpp/rclcpp.hpp"
#include "rclcpp_lifecycle/lifecycle_node.hpp"
#include "rclcpp_lifecycle/lifecycle_publisher.hpp"
#include "std_msgs/msg/string.hpp"

// imu 발행 헤더 추가
#include "sensor_msgs/msg/imu.hpp"

using CallbackReturn =
    rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn;


class fake_imu : public rclcpp_lifecycle::LifecycleNode
{
public:
    // 생성자: 파라미터 선언만. 실제 초기화는 on_configure에서.
    explicit fake_imu(const rclcpp::NodeOptions &options = rclcpp::NodeOptions());

    CallbackReturn on_configure(const rclcpp_lifecycle::State &) override;

    CallbackReturn on_activate(const rclcpp_lifecycle::State &state) override;

    CallbackReturn on_deactivate(const rclcpp_lifecycle::State &state) override;

    CallbackReturn on_cleanup(const rclcpp_lifecycle::State &) override;

    CallbackReturn on_shutdown(const rclcpp_lifecycle::State &previous_state) override;

    CallbackReturn on_error(const rclcpp_lifecycle::State &previous_state) override;

private:
    void publish_message();

    double rate_hz_{0.0};
    std::size_t count_{0};

    std::random_device rd; // imu 랜덤 값 등을 생성하기 위해 사용

    double stamp_offset_sec_{0.0}; // 임시 imu 할행시각 오프셋

    

    rclcpp_lifecycle::LifecyclePublisher<sensor_msgs::msg::Imu>::SharedPtr pub_;
    rclcpp::TimerBase::SharedPtr timer_;
    std::mt19937 gen_{std::random_device{}()}; //  난수 엔진 생성 굳이 라이프사이클 내에서 제거 할 필요 없음


};