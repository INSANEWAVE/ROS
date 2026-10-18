#include <rclcpp/rclcpp.hpp>// подключаем заголовочный файл для ROS2 на C++ юзаем по умолчанию
#include <std_msgs/msg/string.hpp>
#include <std_msgs/msg/int32.hpp>


using namespace std::chrono_literals; // Используем пространство имен литералов для удобных нам инетрвалов времени
class TestSubscriber : public rclcpp::Node //Класс узла, всегда для ноды является производной класса (наследником) класса rclcpp::Node со своим функционалом
{
public: // ОткрытаЯ часть класса, тут есть конструктор
    TestSubscriber(): //  Конструктор класса, функция вызывающаяся при создании объекта класса
      Node("test_subscriber")
    {
        subscription1 = this->create_subscription<std_msgs::msg::String>("/test_topic", 10, std::bind(&TestSubscriber::topic_callback,
                                                                                                     this,
                                                                                                     std::placeholders::_1)); // При вызове функции
        subscription2 = this->create_subscription<std_msgs::msg::Int32>("/msg_tr", 10, std::bind(&TestSubscriber::counter,
                                                                                                     this,
                                                                                                     std::placeholders::_1)); // При вызове функции
    }
private:
    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr subscription1;
    rclcpp::Subscription<std_msgs::msg::Int32>::SharedPtr subscription2;
unsigned int RECIEVED = 0;
    void topic_callback(const std_msgs::msg::String &msg){ // Функция топика - отзывается
      RCLCPP_INFO_STREAM(this->get_logger(),"Received: "<<msg.data);
    }
    void counter(const std_msgs::msg::Int32){ // Функция топика - отзывается
        RECIEVED++;
        RCLCPP_INFO_STREAM(this->get_logger(),"Counted: "<<RECIEVED);
    }
};

int main(int argc, char *argv[]){
    rclcpp::init(argc, argv); // подключаемся к системе с передачей параметров cmd
    rclcpp::spin(std::make_shared<TestSubscriber>()); // Функция спин - цикл с указателем на наш узел
    rclcpp::shutdown(); // Как только узел заканчивает работу (сам или после команды или сбой) система выполняет шатдаун
  return 0;
}
