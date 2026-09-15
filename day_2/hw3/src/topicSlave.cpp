#include "hw3/topicSlave.hpp"

TopicSlave::TopicSlave() : Node("topicSlave")
{
    sub1_ = this->create_subscription<std_msgs::msg::String>(
        "topic_str", 10,
        std::bind(&TopicSlave::cb1, this, std::placeholders::_1));

    sub2_ = this->create_subscription<std_msgs::msg::Bool>(
        "topic_bool", 10,
        std::bind(&TopicSlave::cb2, this, std::placeholders::_1));

    sub3_ = this->create_subscription<std_msgs::msg::Int32>(
        "topic_int", 10,
        std::bind(&TopicSlave::cb3, this, std::placeholders::_1));

    for (int i = 0; i < 3; i++)
        last_recv_[i] = this->now();

    revive_cli_[0] = create_client<std_srvs::srv::Trigger>("revive_pub0");
    revive_cli_[1] = create_client<std_srvs::srv::Trigger>("revive_pub1");
    revive_cli_[2] = create_client<std_srvs::srv::Trigger>("revive_pub2");

    watchdog_ = create_wall_timer(
        std::chrono::milliseconds(200),
        std::bind(&TopicSlave::watchdog_cb, this));
}

void TopicSlave::watchdog_cb()
{
    auto now = this->now();
    for (int i = 0; i < 3; i++) {
        double gap = (now - last_recv_[i]).seconds();
        bool alive = (gap <= 2.0);
        if (alive != alive_[i]) {          // 상태가 바뀔 때만
            alive_[i] = alive;
            emit aliveChanged(i, alive);   // GUI에 알림
        }
    }
}

void TopicSlave::request_revive(int idx)   // 버튼이 부를 함수
{
    if (idx < 0 || idx > 2) return;
    if (!revive_cli_[idx]->service_is_ready()) return;
    auto req = std::make_shared<std_srvs::srv::Trigger::Request>();
    revive_cli_[idx]->async_send_request(req);
    last_recv_[idx] = this->now();         // 부활 요청했으니 시각 갱신
}


void TopicSlave::cb1(const std_msgs::msg::String::SharedPtr msg)
{
    last_recv_[0] = this->now();
    emit strReceived(QString::fromStdString(msg->data));
}

void TopicSlave::cb2(const std_msgs::msg::Bool::SharedPtr msg)
{
    last_recv_[1] = this->now();
    emit boolReceived(msg->data);
}

void TopicSlave::cb3(const std_msgs::msg::Int32::SharedPtr msg)
{
    last_recv_[2] = this->now();
    emit intReceived(msg->data);
}