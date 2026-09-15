#include "rclcpp/rclcpp.hpp"

#include "include/hw3/topicMaster.hpp"
#include <random>

TopicMaster::TopicMaster() : Node("topicMaster")
{
    pub1_ = this->create_publisher<std_msgs::msg::String>("topic_str", 10);
    pub2_ = this->create_publisher<std_msgs::msg::Bool>("topic_bool", 10);
    pub3_ = this->create_publisher<std_msgs::msg::Int32>("topic_int", 10);

    revive_srv0_ = create_service<std_srvs::srv::Trigger>(
        "revive_pub0",
        std::bind(&TopicMaster::revive0, this,
                  std::placeholders::_1, std::placeholders::_2));
    revive_srv1_ = create_service<std_srvs::srv::Trigger>(
        "revive_pub1",
        std::bind(&TopicMaster::revive1, this,
                  std::placeholders::_1, std::placeholders::_2));
    revive_srv2_ = create_service<std_srvs::srv::Trigger>(
        "revive_pub2",
        std::bind(&TopicMaster::revive2, this,
                  std::placeholders::_1, std::placeholders::_2));

    timer_ = this->create_wall_timer(
        std::chrono::milliseconds(500),
        std::bind(&TopicMaster::tick, this));
}

void TopicMaster::tick()
{
    static std::mt19937 gen(std::random_device{}());
    std::uniform_int_distribution<int> dist(1, 10);    // 1~1000 범위
    std::uniform_int_distribution<int> dist_index(0, 2); // 1~3 범위

    int value1 = dist(gen); // 1/1000 확률
    int value2 = dist_index(gen);

    if (value1 == 1)
        is_push_on[value2] = false;

    // 토픽 전송
    if (is_push_on[0])
        push1();
    if (is_push_on[1])
        push2();
    if (is_push_on[2])
        push3();
}

void TopicMaster::revive0(
    const std_srvs::srv::Trigger::Request::SharedPtr,
    std_srvs::srv::Trigger::Response::SharedPtr res)
{
    is_push_on[0] = true;
    res->success = true;
}

void TopicMaster::revive1(
    const std_srvs::srv::Trigger::Request::SharedPtr,
    std_srvs::srv::Trigger::Response::SharedPtr res)
{
    is_push_on[1] = true;
    res->success = true;
}

void TopicMaster::revive2(
    const std_srvs::srv::Trigger::Request::SharedPtr,
    std_srvs::srv::Trigger::Response::SharedPtr res)
{
    is_push_on[2] = true;
    res->success = true;
}

void TopicMaster::push1()
{
    std_msgs::msg::String msg;
    msg.data = "Hello " + std::to_string(rand() % 100); // 0~99 까지의 랜덤정수
    pub1_->publish(msg);
}

void TopicMaster::push2()
{
    std_msgs::msg::Bool msg;
    msg.data = (rand() % 2 == 0);
    pub2_->publish(msg);
}

void TopicMaster::push3()
{
    std_msgs::msg::Int32 msg;
    msg.data = rand() % 1000;
    pub3_->publish(msg);
}

int main(int argc, char *argv[]) // 모든 노드 클래스에 필요한 main. 아래 4줄은 기본으로 있어야 함
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<TopicMaster>());
    rclcpp::shutdown();
    return 0;
}