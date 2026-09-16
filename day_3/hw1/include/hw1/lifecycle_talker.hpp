#include <chrono>
#include <memory>
#include <string>

#include "rclcpp/rclcpp.hpp"
#include "rclcpp_lifecycle/lifecycle_node.hpp"
#include "rclcpp_lifecycle/lifecycle_publisher.hpp"
#include "std_msgs/msg/string.hpp"

using CallbackReturn =
    rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn;


class LifecycleTalker : public rclcpp_lifecycle::LifecycleNode
{
public:
    // 생성자: 파라미터 선언만. 실제 초기화는 on_configure에서.
    explicit LifecycleTalker(const rclcpp::NodeOptions &options = rclcpp::NodeOptions());

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
    rclcpp_lifecycle::LifecyclePublisher<std_msgs::msg::String>::SharedPtr pub_;
    rclcpp::TimerBase::SharedPtr timer_;
};