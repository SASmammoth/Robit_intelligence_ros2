#include "hw1/lifecycle_talker.hpp"

LifecycleTalker::LifecycleTalker(const rclcpp::NodeOptions &options) : rclcpp_lifecycle::LifecycleNode("lifecycle_talker", options)
{
    declare_parameter("publish_rate_hz", 2.0);
}

LifecycleTalker::CallbackReturn LifecycleTalker::on_configure(const rclcpp_lifecycle::State &)
{
    rate_hz_ = get_parameter("publish_rate_hz").as_double();
    if (rate_hz_ <= 0.0)
    {
        RCLCPP_ERROR(get_logger(), "publish_rate_hz must be > 0 (got %.2f)", rate_hz_);
        return CallbackReturn::FAILURE; // Unconfigured로 복귀, 재시도 가능
    }
    pub_ = create_publisher<std_msgs::msg::String>("chatter", 10);
    RCLCPP_INFO(get_logger(), "on_configure: rate = %.1f Hz", rate_hz_);
    return CallbackReturn::SUCCESS;
}

LifecycleTalker::CallbackReturn LifecycleTalker::on_activate(const rclcpp_lifecycle::State &state)
{
    // 부모 구현이 관리 엔티티(LifecyclePublisher)를 활성화
    LifecycleNode::on_activate(state);
    timer_ = create_timer(
        std::chrono::duration<double>(1.0 / rate_hz_),
        [this]()
        { publish_message(); });
    RCLCPP_INFO(get_logger(), "on_activate");
    return CallbackReturn::SUCCESS;
}

LifecycleTalker::CallbackReturn LifecycleTalker::on_deactivate(const rclcpp_lifecycle::State &state)
{
    LifecycleNode::on_deactivate(state);
    timer_.reset(); // 타이머는 관리 대상이 아니므로 직접 정지
    RCLCPP_INFO(get_logger(), "on_deactivate");
    return CallbackReturn::SUCCESS;
}

LifecycleTalker::CallbackReturn LifecycleTalker::on_cleanup(const rclcpp_lifecycle::State &)
{
    pub_.reset();
    RCLCPP_INFO(get_logger(), "on_cleanup");
    return CallbackReturn::SUCCESS;
}

LifecycleTalker::CallbackReturn LifecycleTalker::on_shutdown(const rclcpp_lifecycle::State &previous_state)
{
    timer_.reset();
    pub_.reset();
    RCLCPP_INFO(get_logger(), "on_shutdown from [%s]", previous_state.label().c_str());
    return CallbackReturn::SUCCESS;
}

LifecycleTalker::CallbackReturn LifecycleTalker::on_error(const rclcpp_lifecycle::State &previous_state)
{
    RCLCPP_ERROR(get_logger(), "on_error from [%s]", previous_state.label().c_str());
    timer_.reset();
    pub_.reset();
    return CallbackReturn::SUCCESS; // SUCCESS → Unconfigured, FAILURE → Finalized
}

void LifecycleTalker::publish_message()
    {
        std_msgs::msg::String msg;
        msg.data = "Hello #" + std::to_string(count_++);
        pub_->publish(msg); // 비활성 상태라면 발행되지 않음
    }

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<LifecycleTalker>();
    // LifecycleNode는 rclcpp::Node를 상속하지 않으므로 base interface로 spin
    rclcpp::spin(node->get_node_base_interface());
    rclcpp::shutdown();
    return 0;
}