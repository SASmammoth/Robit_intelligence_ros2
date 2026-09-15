#ifndef HW3_TOPICSLAVE_HPP
#define HW3_TOPICSLAVE_HPP

#include <QObject>
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include "std_msgs/msg/bool.hpp"
#include "std_msgs/msg/int32.hpp"
#include "std_srvs/srv/trigger.hpp"

class TopicSlave : public QObject, public rclcpp::Node
{
    Q_OBJECT
public:
    TopicSlave();
    
    rclcpp::Time last_recv_[3];
    rclcpp::TimerBase::SharedPtr watchdog_;
    rclcpp::Client<std_srvs::srv::Trigger>::SharedPtr revive_cli_[3];

    void watchdog_cb(); // 2초 지났는지 감시하는 감시자

    void request_revive(int idx); // 요청 버튼

signals: // qt로 쏠 시그널
    void strReceived(QString data);
    void boolReceived(bool data);
    void intReceived(int data);
    void aliveChanged(int idx, bool alive);

private:
    void cb1(const std_msgs::msg::String::SharedPtr msg);
    void cb2(const std_msgs::msg::Bool::SharedPtr msg);
    void cb3(const std_msgs::msg::Int32::SharedPtr msg);

    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr sub1_; // 문자열 토픽 듣는 서브스크립션 포인터 생성
    rclcpp::Subscription<std_msgs::msg::Bool>::SharedPtr sub2_; // 부울 토픽 듣는 서브스크립션 포인터 생성
    rclcpp::Subscription<std_msgs::msg::Int32>::SharedPtr sub3_;// 정수 토픽 듣는 서브스크립션 포인터 생성

    bool alive_[3] = {true, true, true};
};

#endif