#include "My_trutle_remote.hpp"
#include <QColor>

static const QColor color_set[18] = { // 색상 테이블, indec를 이용해 변경
        Qt::white, Qt::black, Qt::red, Qt::green, Qt::blue, Qt::cyan, 
        Qt::magenta, Qt::yellow, Qt::gray, Qt::darkRed, Qt::darkGreen, 
        Qt::darkBlue, Qt::darkCyan, Qt::darkMagenta, Qt::darkYellow, 
        Qt::darkGray, Qt::lightGray,Qt::transparent};

My_trutle_remote::My_trutle_remote() : Node("my_trutle_remote")
{
    // 퍼블리셔 생성
    pub_ = create_publisher<geometry_msgs::msg::Twist>("/turtle1/cmd_vel", 10);
    // 서비스 클라이언트 생성
    serv_ = create_client<turtlesim::srv::SetPen>("/turtle1/set_pen");
}

void My_trutle_remote::push_Service(int col_index, int width)
{
    int temp_index = 0;
    if(col_index < 18 && col_index >= 0) temp_index = col_index;
    auto pen = std::make_shared<turtlesim::srv::SetPen::Request>();
    pen->r = color_set[temp_index].red();
    pen->g = color_set[temp_index].green();
    pen->b = color_set[temp_index].blue();

    pen->width = width;

    serv_->async_send_request(pen);
}

void My_trutle_remote::push_publisher(double f_speed, double r_speed)
{
    // 전송할 twist 포뱃 생성
    auto twist = geometry_msgs::msg::Twist();
    twist.linear.x = f_speed; // 전진/후진 속도
    twist.angular.z = r_speed; // 회전속도

    pub_->publish(twist); // 전송
}