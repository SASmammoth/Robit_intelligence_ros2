#pragma once

#include "rclcpp/rclcpp.hpp"
#include "turtlesim/srv/set_pen.hpp"
#include "geometry_msgs/msg/twist.hpp"

class My_trutle_remote : public rclcpp::Node
{
public:
    My_trutle_remote();
    void push_publisher(double f_speed, double r_speed); // 퍼블리싱 하는 함수 (그리기용)
    void push_Service(int col_index, int width); // 서비스 하는 함수(펜 설정용)

private:
    // 퍼블리셔 생성을 위한: xyz축의 선형 속도와 각속도 형태의 메시지 형식 Twist 의 스마트포인터
    rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr pub_;

    // 서비스 클라이언트 생성을 위한 스마트포인터 : 거북이 팬 형식
    rclcpp::Client<turtlesim::srv::SetPen>::SharedPtr serv_;
};
