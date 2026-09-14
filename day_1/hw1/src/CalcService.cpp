#include "rclcpp/rclcpp.hpp"
#include "rclcpp/service.hpp"
#include "hw1/srv/calc2int.hpp" // 내가 생성한 Calc2int 서비스 형을 이용함

class CalcService : public rclcpp::Node
{
public:
    CalcService() : Node("calcService") // 노드 생성자
    {
        srv_ = create_service<hw1::srv::Calc2int>("calc_service", std::bind(&CalcService::calc, this, std::placeholders::_1, std::placeholders::_2));
    }

private:
    rclcpp::Service<hw1::srv::Calc2int>::SharedPtr srv_; // 내가 만든 Calc2int형태의 서비스를 주고받는 서비스포인터

    // 계산하는 부분
    void calc(const std::shared_ptr<hw1::srv::Calc2int::Request> req, const std::shared_ptr<hw1::srv::Calc2int::Response> res)
    {
        switch (req->op)
        {
        case '+':
            res->i3 = req->i1 + req->i2;
            break;
        case '-':
            res->i3 = req->i1 - req->i2;
            break;
        case '*':
            res->i3 = req->i1 * req->i2;
            break;
        case '/':
            if (req->i2 == 0)
            {
                res->i3 = 0;
                RCLCPP_INFO(get_logger(), "/0은 안돼요!"); // 예외처리
                break;
            }
            res->i3 = req->i1 / req->i2;
            break;
        default:
            res->i3 = 0;
            RCLCPP_INFO(get_logger(), "연산자 오류!"); // 예외처리
        }
    }
};

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<CalcService>());
    rclcpp::shutdown();
    return 0;
}