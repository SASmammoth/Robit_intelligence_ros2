#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include "std_msgs/msg/bool.hpp"
#include "std_msgs/msg/int32.hpp"
#include "std_srvs/srv/trigger.hpp"

class TopicMaster : public rclcpp::Node
{
public:
    bool is_push_on[3] = {true, true, true}; // 랜덤하게 정지
    TopicMaster();

    // 버튼을 이요해서 켜는 서비스 생성자
    rclcpp::Service<std_srvs::srv::Trigger>::SharedPtr revive_srv0_;
    rclcpp::Service<std_srvs::srv::Trigger>::SharedPtr revive_srv1_;
    rclcpp::Service<std_srvs::srv::Trigger>::SharedPtr revive_srv2_;

    void revive0(const std_srvs::srv::Trigger::Request::SharedPtr req,
                 std_srvs::srv::Trigger::Response::SharedPtr res);
    void revive1(const std_srvs::srv::Trigger::Request::SharedPtr req,
                 std_srvs::srv::Trigger::Response::SharedPtr res);
    void revive2(const std_srvs::srv::Trigger::Request::SharedPtr req,
                 std_srvs::srv::Trigger::Response::SharedPtr res);

private:
    void push1(); // 문자열 퍼블리셔 실행
    void push2();// ㅂ울 퍼블리셔 실행
    void push3();// 정수형 퍼블리셔 실행
    void tick(); // 타이머 틱

    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr pub1_; // 문자열 퍼블리셔
    rclcpp::Publisher<std_msgs::msg::Bool>::SharedPtr pub2_; // 부울 퍼블리셔
    rclcpp::Publisher<std_msgs::msg::Int32>::SharedPtr pub3_; // 정수형 퍼블리셔
    rclcpp::TimerBase::SharedPtr timer_;


};