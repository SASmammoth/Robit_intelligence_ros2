#include "hw1/imu_watchdog.hpp"

imu_watchdog::imu_watchdog(const rclcpp::NodeOptions &options) : rclcpp_lifecycle::LifecycleNode("imu_watchdog", options)
{
    declare_parameter("subscription_check_rate_hz", 2.0);
    declare_parameter("subscripton_timeout_sec", 5.0);
    declare_parameter("is_wall_timer", false);
}

imu_watchdog::CallbackReturn imu_watchdog::on_configure(const rclcpp_lifecycle::State &)
{
    check_rate_hz = get_parameter("subscription_check_rate_hz").as_double();
    timeout_sec = get_parameter("subscripton_timeout_sec").as_double();
    is_wall_timer_ = get_parameter("is_wall_timer").as_bool();
    if (check_rate_hz <= 0.0) // 주기가 0이하면 FAILURE
    {
        RCLCPP_ERROR(get_logger(), "subscription_check_rate_hz must be > 0 (got %.2f)", check_rate_hz);
        return CallbackReturn::FAILURE; // Unconfigured로 복귀, 재시도 가능
    }
    if (timeout_sec < 0.0) // timeout_sec가 0 미만이면 FAILURE
    {
        RCLCPP_ERROR(get_logger(), "subscripton_timeout_sec must be >= 0 (got %.2f)", timeout_sec);
        return CallbackReturn::FAILURE; // Unconfigured로 복귀, 재시도 가능
    }
    sub_ = create_subscription<sensor_msgs::msg::Imu>("/imu", 10, [this](const sensor_msgs::msg::Imu::SharedPtr msg)
                                                      { imu_subScribe(msg); });
    pub_ = create_publisher<sensor_msgs::msg::Imu>("/imu_checked", 10);

    // 과제 2-1 2-2 2-3 기준
    //imu_state_client_ = create_client<lifecycle_msgs::srv::GetState>("/fake_imu/get_state");

    // 과제 2-4 기준 그룹 나눔
    /*
    client_group_ = create_callback_group(
        rclcpp::CallbackGroupType::MutuallyExclusive);

    imu_state_client_ = create_client<lifecycle_msgs::srv::GetState>(
        "/fake_imu/get_state", rclcpp::ServicesQoS(), client_group_);
    */

    // 과제 2-4 기준 그룹 안 나눔

    imu_state_client_ = create_client<lifecycle_msgs::srv::GetState>(
        "/fake_imu/get_state", rclcpp::ServicesQoS());


    
    RCLCPP_INFO(get_logger(), "on_configure: check_rate = %.1f Hz, timeout_sec = %.2f", check_rate_hz, timeout_sec);
    return CallbackReturn::SUCCESS;
}

imu_watchdog::CallbackReturn imu_watchdog::on_activate(const rclcpp_lifecycle::State &state)
{
    // 부모 구현이 관리 엔티티(LifecyclePublisher)를 활성화

    // 파라미터에 따른 타이머 방식 변경 및 타이머 할당
    if (is_wall_timer_)
        timer_ = create_wall_timer(std::chrono::duration<double>(1.0 / check_rate_hz), [this]()
                                   { check_rate_func(); });
    else
        timer_ = create_timer(std::chrono::duration<double>(1.0 / check_rate_hz), [this]()
                              { check_rate_func(); });

    client_timer_ = create_timer(std::chrono::duration<double>(1.0), [this]()
                                 { count_collback(); });

    // age 검사 용 변수 (NOSIG, TIMEOUT등을 관장) 초기화
    is_firstRecive = false;
    is_timeout = false;
    is_NO_DATA_print = false;

    LifecycleNode::on_activate(state);
    RCLCPP_INFO(get_logger(), "on_activate");
    return CallbackReturn::SUCCESS;
}

imu_watchdog::CallbackReturn imu_watchdog::on_deactivate(const rclcpp_lifecycle::State &state)
{
    LifecycleNode::on_deactivate(state);
    timer_.reset(); // 타이머는 관리 대상이 아니므로 직접 정지
    pub_.reset();
    RCLCPP_INFO(get_logger(), "on_deactivate");
    return CallbackReturn::SUCCESS;
}

imu_watchdog::CallbackReturn imu_watchdog::on_cleanup(const rclcpp_lifecycle::State &)
{
    sub_.reset();
    pub_.reset();
    imu_state_client_.reset();
    RCLCPP_INFO(get_logger(), "on_cleanup");
    return CallbackReturn::SUCCESS;
}

imu_watchdog::CallbackReturn imu_watchdog::on_shutdown(const rclcpp_lifecycle::State &previous_state)
{
    timer_.reset();
    sub_.reset();
    pub_.reset();
    RCLCPP_INFO(get_logger(), "on_shutdown from [%s]", previous_state.label().c_str());
    return CallbackReturn::SUCCESS;
}

imu_watchdog::CallbackReturn imu_watchdog::on_error(const rclcpp_lifecycle::State &previous_state)
{
    RCLCPP_ERROR(get_logger(), "on_error from [%s]", previous_state.label().c_str());
    timer_.reset();
    sub_.reset();
    return CallbackReturn::SUCCESS; // SUCCESS → Unconfigured, FAILURE → Finalized
}

void imu_watchdog::count_collback()
{
    if (!imu_state_client_->service_is_ready())
    {
        RCLCPP_WARN(get_logger(), "fake_imu get_state service not available");
        return;
    }

    // 응답 생성
    auto request = std::make_shared<lifecycle_msgs::srv::GetState::Request>();

    // 요청 대기 (과제1 수행용)

    //클라이언트 요청 보내기
    auto future = imu_state_client_->async_send_request(request);
    auto response = future.get();
    RCLCPP_INFO(get_logger(), "fake_imu state: %s",
            response->current_state.label.c_str());


    // 비동기 요청 + 응답 콜백 (과제2 수행용)
    // 람다식을 이용해 응답을 기다린다.
    /*
    imu_state_client_->async_send_request(
        request,
        [this](rclcpp::Client<lifecycle_msgs::srv::GetState>::SharedFuture future)
        {
            auto response = future.get();
            RCLCPP_INFO(get_logger(), "fake_imu state: %s",
                        response->current_state.label.c_str());
        });
    */
}

void imu_watchdog::imu_subScribe(const sensor_msgs::msg::Imu::SharedPtr msg)
{
    is_firstRecive = true;
    last_stamp_ = rclcpp::Time(msg->header.stamp, this->get_clock()->get_clock_type());

    double age = (this->now() - last_stamp_).seconds();
    if (age <= timeout_sec) // 신선한 것만
        pub_->publish(*msg);

    RCLCPP_INFO(get_logger(), "IMU data(liner): %.1f, %.1f, %.1f",
                msg->linear_acceleration.x, msg->linear_acceleration.y, msg->linear_acceleration.z);
}

void imu_watchdog::check_rate_func()
{

    if (!is_firstRecive) // 한번도 수신 받은적이 없고, Nodata출력 안 했으면.
    {
        if (!is_NO_DATA_print)
        {
            RCLCPP_INFO(get_logger(), "[NO DATA]");
            is_NO_DATA_print = true;
            return;
        }
        else
        {
            return;
        }
    }

    double age = (this->now() - last_stamp_).seconds();

    if (age > timeout_sec) // 데이터가 오래됨
    {
        RCLCPP_INFO(get_logger(), "[STALE] age=%.2f", age); // 매 검사마다
        is_timeout = true;
    }
    else // 다시? 신선한 데이터 들어옴
    {
        if (is_timeout) // STALE에서 벗어남
        {
            RCLCPP_INFO(get_logger(), "[RECOVERED] age=%.2f", age);
            is_timeout = false;
        }
    }
}

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<imu_watchdog>();
    // LifecycleNode는 rclcpp::Node를 상속하지 않으므로 base interface로 spin
    rclcpp::executors::MultiThreadedExecutor executor;
    executor.add_node(node->get_node_base_interface());
    executor.spin();
    rclcpp::shutdown();
    return 0;
}