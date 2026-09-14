#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

class MySubscriber : public rclcpp::Node
{
public:
    MySubscriber() : Node("mySubscriber")
    {
        lisn_ = create_subscription<std_msgs::msg::String>("topic", 10, std::bind(&MySubscriber::callback, this, std::placeholders::_1));
    }

private:
    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr lisn_;
    void callback(const std_msgs::msg::String::SharedPtr text)
    {
        RCLCPP_INFO(get_logger(), "%s", text->data.c_str());
    }

};



int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<MySubscriber>());
    rclcpp::shutdown();
    return 0;
}