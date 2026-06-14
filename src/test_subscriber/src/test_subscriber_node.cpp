#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/string.hpp>// подключаем заголовочный файл для ROS2 на C++ юзаем по умолчанию

using namespace std::chrono_literals; // Используем пространство имен литералов для удобных нам инетрвалов времени
class TestSubscriber : public rclcpp::Node //Класс узла, всегда для ноды является производной класса (наследником) класса rclcpp::Node со своим функционалом
{
public: // ОткрытаЯ часть класса, тут есть конструктор
    TestSubscriber(): //  Конструктор класса, функция вызывающаяся при создании объекта класса
      Node("test_subscriber")
    {
        subscription = this->create_subscription<std_msgs::msg::String>("/test_topic", 10, std::bind // функция которой мы привязываем вызов функции при приёме сообщения
                                                                        (&TestSubscriber::topic_callback,this,std::placeholders::_1)); // При вызове функции
    }
private:
    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr subscription;

    void topic_callback(const std_msgs::msg::String &msg){ // Функция топика
      RCLCPP_INFO_STREAM(this->get_logger(),"Received: "<<msg.data);
    }
};

int main(int argc, char *argv[]){
    rclcpp::init(argc, argv); // подключаемся к системе с передачей параметров cmd
    rclcpp::spin(std::make_shared<TestSubscriber>()); // Функция спин - цикл с указателем на наш узел
    rclcpp::shutdown(); // Как только узел заканчивает работу (сам или после команды или сбой) система выполняет шатдаун
  return 0;
}
