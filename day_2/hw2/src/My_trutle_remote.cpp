#include "My_trutle_remote.hpp"
#include <QColor>
#include <cmath>

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

    // 서브스크라이브 생성
    call_sub_ = create_subscription<turtlesim::msg::Pose>("/turtle1/pose", 10,
    std::bind(&My_trutle_remote::turtle_recoder, this,std::placeholders::_1));

    // 초기화 서비스 클라이언트 생성
    reset_ = create_client<std_srvs::srv::Empty>("/reset");
    // 순간이동 서비스 클라이언트 생성
    teleport_ = create_client<turtlesim::srv::TeleportAbsolute>("/turtle1/teleport_absolute");
}

void My_trutle_remote::push_reset()
{
    auto req = std::make_shared<std_srvs::srv::Empty::Request>();
    reset_->async_send_request(req);
}

void My_trutle_remote::push_teleport(double x, double y, double theta)
{
    auto req = std::make_shared<turtlesim::srv::TeleportAbsolute::Request>();
    req->x = x;
    req->y = y;
    req->theta = theta;
    teleport_->async_send_request(req);
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

void My_trutle_remote::recode_init()
{
    is_recoding = true;
    recode_file_.clear(); // 레코드 저장소 초기화
    total_distance_ = 0.0; // 이동 거리 초기화
    r_start_time = this->now(); // 레코드 시작시간 초기화
}

void My_trutle_remote::recode_end()
{
    is_recoding = false;
    r_end_time = this->now(); // 레코드 종료시간 기록

    recode_time = r_end_time - r_start_time; // 총 레코드 시간 저장
} 

void My_trutle_remote::recode_view()
{
    if (timer_) timer_->cancel(); // 기존 타이머 정리
    frame = 0;
    is_playing_ = true; // 재생 시작

    push_reset(); // 거북이 리셋
    timer_ = create_wall_timer(
        std::chrono::milliseconds(16),
        std::bind(&My_trutle_remote::timer_callback, this));
    play_start_time_ = this->now();
}

void My_trutle_remote::timer_callback()
{
    // 모든 프레임 재생 완료 시 타이머 종료
    if (frame >= (int)recode_file_.size())
    {
        timer_->cancel();
        is_playing_ = false; // 재생 종료
        return;
    }

    // 현재 프레임 위치로 순간이동
    const auto &p = recode_file_.at(frame);
    push_teleport(p.x, p.y, p.theta);

    frame++; // 다음 프레임으로
}

void My_trutle_remote::turtle_recoder(const turtlesim::msg::Pose pos_)
{
    // 기록중일때만 저장.
    if (is_recoding)
    {
        // 이전 프레임과의 거리 누적
        if (!recode_file_.empty())
        {
            const auto &prev = recode_file_.back();
            double dx = pos_.x - prev.x;
            double dy = pos_.y - prev.y;
            total_distance_ += std::sqrt(dx * dx + dy * dy);
        }
        recode_file_.push_back(pos_);
    }
}
