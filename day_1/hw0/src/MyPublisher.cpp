#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

class MyPublisher : public rclcpp::Node
{
public:
    MyPublisher() : Node("myPublisher")
    {
        pub_ = create_publisher<std_msgs::msg::String>("topic", 10); // msg::String형식의 퍼블리셔 생성
        timer_ = create_wall_timer(std::chrono::milliseconds(500), [this]
                                   { callback(); }); // 0.5초마다 작동하는 
    }

private:
    void callback()
    {
        auto msg = std_msgs::msg::String();
        msg.data = "설정이 귀찮네요!";
        pub_->publish(msg);
    }
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr pub_;
    rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc, char * argv[]) // 모든 노드 클래스에 필요한 main. 아래 4줄은 기본으로 있어야 함
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<MyPublisher>());
    rclcpp::shutdown();
    return 0;
}