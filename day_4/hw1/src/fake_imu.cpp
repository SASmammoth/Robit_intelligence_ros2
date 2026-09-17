#include "hw1/fake_imu.hpp"

fake_imu::fake_imu(const rclcpp::NodeOptions &options) : rclcpp_lifecycle::LifecycleNode("fake_imu", options)
{
    declare_parameter("publish_rate_hz", 2.0);
    declare_parameter("imu_offset", 0.0);
}

fake_imu::CallbackReturn fake_imu::on_configure(const rclcpp_lifecycle::State &)
{

    rate_hz_ = get_parameter("publish_rate_hz").as_double();
    stamp_offset_sec_ = get_parameter("imu_offset").as_double();
    if (rate_hz_ <= 0.0)
    {
        RCLCPP_ERROR(get_logger(), "publish_rate_hz must be > 0 (got %.2f)", rate_hz_);
        return CallbackReturn::FAILURE; // Unconfigured로 복귀, 재시도 가능
    }
    if (stamp_offset_sec_ < 0.0)
    {
        RCLCPP_ERROR(get_logger(), "imu_offset must be > 0 (got %.2f)", stamp_offset_sec_);
        return CallbackReturn::FAILURE; // Unconfigured로 복귀, 재시도 가능
    }

    pub_ = create_publisher<sensor_msgs::msg::Imu>("/imu", 10); // 기존의 string을 imu형식으로 교체
    RCLCPP_INFO(get_logger(), "on_configure: rate = %.1f Hz, offset = %.2f", rate_hz_, stamp_offset_sec_);
    return CallbackReturn::SUCCESS;
}

fake_imu::CallbackReturn fake_imu::on_activate(const rclcpp_lifecycle::State &state)
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

fake_imu::CallbackReturn fake_imu::on_deactivate(const rclcpp_lifecycle::State &state)
{
    LifecycleNode::on_deactivate(state);
    timer_.reset(); // 타이머는 관리 대상이 아니므로 직접 정지
    RCLCPP_INFO(get_logger(), "on_deactivate");
    return CallbackReturn::SUCCESS;
}

fake_imu::CallbackReturn fake_imu::on_cleanup(const rclcpp_lifecycle::State &)
{
    pub_.reset();
    RCLCPP_INFO(get_logger(), "on_cleanup");
    return CallbackReturn::SUCCESS;
}

fake_imu::CallbackReturn fake_imu::on_shutdown(const rclcpp_lifecycle::State &previous_state)
{
    timer_.reset();
    pub_.reset();
    RCLCPP_INFO(get_logger(), "on_shutdown from [%s]", previous_state.label().c_str());
    return CallbackReturn::SUCCESS;
}

fake_imu::CallbackReturn fake_imu::on_error(const rclcpp_lifecycle::State &previous_state)
{
    RCLCPP_ERROR(get_logger(), "on_error from [%s]", previous_state.label().c_str());
    timer_.reset();
    pub_.reset();
    return CallbackReturn::SUCCESS; // SUCCESS → Unconfigured, FAILURE → Finalized
}

void fake_imu::publish_message()
{
    std::uniform_real_distribution<double> dr(0.0, 10.0);
    sensor_msgs::msg::Imu temp_imu;       // 랜덤 imu 데이터

    rclcpp::Duration temp_Duration = rclcpp::Duration::from_seconds(stamp_offset_sec_); // 파라미터로 imu 오프셋 추가
    temp_imu.header.stamp = this->now() - temp_Duration; // offset을 이용하여 지연 데이터를 재현

    temp_imu.angular_velocity.x = dr(gen_); // 랜덤값 입력
    temp_imu.angular_velocity.y = dr(gen_); // 랜덤값 입력
    temp_imu.angular_velocity.z = dr(gen_); // 랜덤값 입력

    temp_imu.linear_acceleration.x = dr(gen_); // 랜덤값 입력
    temp_imu.linear_acceleration.y = dr(gen_); // 랜덤값 입력
    temp_imu.linear_acceleration.z = dr(gen_); // 랜덤값 입력

    pub_->publish(temp_imu); // 비활성 상태라면 발행되지 않음
}

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<fake_imu>();
    // LifecycleNode는 rclcpp::Node를 상속하지 않으므로 base interface로 spin
    rclcpp::spin(node->get_node_base_interface());
    rclcpp::shutdown();
    return 0;
}