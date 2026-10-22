#include <rclcpp/rclcpp.hpp>
#include <std_srvs/srv/set_bool.hpp>
#include <string>
// Клиентом выполняем периодический вызов сервиса
using namespace std::chrono_literals;// Удобные циферки

class TestBasicClient : public rclcpp::Node
{
public:
    TestBasicClient():
        Node("test_basic_client_node"),
        flag_to_set(true)
    {
        client =
            this->create_client<std_srvs::srv::SetBool>("bool_service");
        timer = this->create_wall_timer(2000ms,
                    std::bind(&TestBasicClient::timer_callback,
                              this));// Таймер и вызов в периоде по таймеру вообще необязателен, идея тут - единичный или НЕ постоянный с
    }
private:
    rclcpp::TimerBase::SharedPtr timer;
    rclcpp::Client<std_srvs::srv::SetBool>::SharedPtr client;
    bool flag_to_set;

    void timer_callback() {
        while(!client->wait_for_service(2000ms)) {
            if(!rclcpp::ok()) {
                RCLCPP_ERROR(this->get_logger(),
                            "Interrupted");
                return;
            }
            RCLCPP_INFO(this->get_logger(),"Service not available, waiting...");
        }
        auto request = std::make_shared<std_srvs::srv::SetBool::Request>();
        request->data = flag_to_set;
        flag_to_set = !flag_to_set;
        client->async_send_request(request,
                    std::bind(&TestBasicClient::response_callback,
                              this,
                              std::placeholders::_1));// Асинхронный запрос на сервис
    }

    void response_callback(rclcpp::Client<std_srvs::srv::SetBool>::SharedFuture future) {
        auto status = future.wait_for(100ms);
        if(status == std::future_status::ready) {
            RCLCPP_INFO_STREAM(this->get_logger(),
                       "Result: "<<future.get()->success
                                 <<" Msg: "<<future.get()->message);
        }
        else {
            RCLCPP_INFO(this->get_logger(),"Service In-Progress..");
        }
    }
};

int main(int argc, char *argv[]){
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<TestBasicClient>());
    rclcpp::shutdown();
    return 0;
}
