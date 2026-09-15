#pragma once

#include "rclcpp/rclcpp.hpp"
#include "turtlesim/srv/set_pen.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include "turtlesim/msg/pose.hpp"
#include "std_srvs/srv/empty.hpp"
#include "turtlesim/srv/teleport_absolute.hpp"
#include <vector>

class My_trutle_remote : public rclcpp::Node
{
public:
    My_trutle_remote();
    void push_publisher(double f_speed, double r_speed); // 퍼블리싱 하는 함수 (그리기용)
    void push_Service(int col_index, int width); // 서비스 하는 함수(펜 설정용)
    void turtle_recoder(const turtlesim::msg::Pose pos_); // 거북이 위치 정보 저장.

    void push_reset();                                  // 거북이 초기화(리셋)
    void push_teleport(double x, double y, double theta); // 순간이동

    rclcpp::Time r_start_time; // 기록 시작 시간
    rclcpp::Time r_end_time; // 기록 종료 시간

    bool is_recoding = false; // 기록중인가?

    void recode_init(); // 레코드 초기화 
    void recode_end(); // 레코드 종료
    void recode_view(); // 레코드 보이기

      // 상태 조회용 (GUI 출력에 사용)
    bool get_is_recoding() const { return is_recoding; }        // 기록 중인가
    bool get_is_playing() const { return is_playing_; }         // 재생 중인가
    int  get_frame_count() const { return (int)recode_file_.size(); } // 기록된 프레임 수
    double get_total_distance() const { return total_distance_; }     // 총 이동 거리
    double get_recode_seconds() const { return recode_time.seconds(); } // 총 소요 시간
    int  get_play_frame() const { return frame; }              // 현재 재생 프레임

    

private:
    // 퍼블리셔 생성을 위한: xyz축의 선형 속도와 각속도 형태의 메시지 형식 Twist 의 스마트포인터
    rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr pub_;

    // 서비스 클라이언트 생성을 위한 스마트포인터 : 거북이 팬 형식
    rclcpp::Client<turtlesim::srv::SetPen>::SharedPtr serv_;

    // 서브스크라이브 생성을 위한 스마트 포인터
    rclcpp::Subscription<turtlesim::msg::Pose>::SharedPtr call_sub_;
    
    // 기록한 것 저장
    std::vector<turtlesim::msg::Pose> recode_file_;

    // 거북이 초기화
    rclcpp::Client<std_srvs::srv::Empty>::SharedPtr reset_;

    // 거북이 순간이동
    rclcpp::Client<turtlesim::srv::TeleportAbsolute>::SharedPtr teleport_;

    // 주기 타이머 생성
    rclcpp::TimerBase::SharedPtr timer_;
    void timer_callback();  // 16ms 마다 거북이의 기록을 보여줄 코드

    rclcpp::Duration recode_time = rclcpp::Duration(0, 0); // 총 기록 시간
    rclcpp::Time play_start_time_; // 총 기록 시간
    
    int frame; // 플래이 프레임

    bool is_playing_ = false;      // 재생 중 여부
    double total_distance_ = 0.0;  // 총 이동 거리  
};