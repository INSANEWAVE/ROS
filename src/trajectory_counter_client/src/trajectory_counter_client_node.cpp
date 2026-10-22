#include <rclcpp/rclcpp.hpp>
#include <trajectory_counter_srv/srv/trajectory_counter.hpp>
#include <random>

using namespace std::chrono_literals;

class TrajectoryCounterClient : public rclcpp::Node
{
public:
    TrajectoryCounterClient():
        Node("trajectory_counter_client_node")
    {
        client =
            this->create_client<trajectory_counter_srv::srv::TrajectoryCounter>("trajectory_counter");
        timer = this->create_wall_timer(2000ms,
                    std::bind(&TrajectoryCounterClient::timer_callback,
                              this));
    }
private:
    rclcpp::TimerBase::SharedPtr timer;
    rclcpp::Client<trajectory_counter_srv::srv::TrajectoryCounter>::SharedPtr client;

    std::random_device rd;
    std::mt19937 gen{rd()};// Случайная генерация для точек идентична работе генератора треугольников
    std::uniform_real_distribution<double> dis{10, 1000}; // От 10 до 1000

    void timer_callback() {
        while(!client->wait_for_service(2000ms)) {
            if(!rclcpp::ok()) {
                RCLCPP_ERROR(this->get_logger(),
                            "Interrupted");
                return;
            }
            RCLCPP_INFO(this->get_logger(),"Service not available, waiting...");
        }
        auto request = std::make_shared<trajectory_counter_srv::srv::TrajectoryCounter::Request>();

        request->pt1.x = dis(gen); request->pt1.y = dis(gen); request->pt1.z = dis(gen);
        request->pt2.x = dis(gen); request->pt2.y = dis(gen); request->pt2.z = dis(gen);
        request->pt3.x = dis(gen); request->pt3.y = dis(gen); request->pt3.z = dis(gen);
        request->pt4.x = dis(gen); request->pt4.y = dis(gen); request->pt4.z = dis(gen);

        client->async_send_request(request,
                    std::bind(&TrajectoryCounterClient::response_callback,
                              this,
                              std::placeholders::_1));
    }

    void response_callback(rclcpp::Client<trajectory_counter_srv::srv::TrajectoryCounter>::SharedFuture future) {
        auto status = future.wait_for(100ms);
        if(status == std::future_status::ready) {
            RCLCPP_INFO_STREAM(this->get_logger(),
                       "Result: "<<future.get()->length);
        }
        else {
            RCLCPP_INFO(this->get_logger(),"Service In-Progress..");
        }
    }
};

int main(int argc, char *argv[]){
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<TrajectoryCounterClient>());
    rclcpp::shutdown();
    return 0;
}
