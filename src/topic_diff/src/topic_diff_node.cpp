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
        subscription_trm = this->create_subscription<std_msgs::msg::Int32>("/msg_tr", 10, std::bind(&TestSubscriber::topic_callback,
                                                                                                     this,
                                                                                                     std::placeholders::_1)); // При вызове функции
        subscription_rcv = this->create_subscription<std_msgs::msg::Int32>("/msg_rcv", 10, std::bind(&TestSubscriber::counter,
                                                                                                     this,
                                                                                                     std::placeholders::_1)); // При вызове функции
         publisher_diff = this->create_publisher<std_msgs::msg::Int32>("/msg_diff", 10); //публикатор счётчика разницы
    }
private:
    rclcpp::Subscription<std_msgs::msg::Int32>::SharedPtr subscription_trm;
    rclcpp::Subscription<std_msgs::msg::Int32>::SharedPtr subscription_rcv;
    rclcpp::Publisher<std_msgs::msg::Int32>::SharedPtr publisher_diff;
unsigned int RECIEVED = 0;
int difference;
int tr;
int rcv;
    void counter(const std_msgs::msg::Int32){ // Функция счётчика - отзывается и инкрементирует счётчик + отсылает его
        std_msgs::msg::Int32 msg_diff;
        int tr = 0;
        int rcv = 0;
        difference = (tr - rcv);// Вот тут бы модуль
        msg_diff.data = std::abc(difference);// Ну тут можно модуль вот
        RCLCPP_INFO_STREAM(this->get_logger(),"Counted: "<<msg_diff;
        publisher_diff->publish(msg_diff);
    }
};

int main(int argc, char *argv[]){
    rclcpp::init(argc, argv); // подключаемся к системе с передачей параметров cmd
    rclcpp::spin(std::make_shared<TestSubscriber>()); // Функция спин - цикл с указателем на наш узел
    rclcpp::shutdown(); // Как только узел заканчивает работу (сам или после команды или сбой) система выполняет шатдаун
  return 0;
}
