#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/string.hpp>// подключаем заголовочный файл для ROS2 на C++ юзаем по умолчанию

using namespace std::chrono_literals; // Используем пространство имен литералов для удобных нам инетрвалов времени
class TestPublisher : public rclcpp::Node //Класс узла, всегда для ноды является производной класса (наследником) класса rclcpp::Node со своим функционалом
{
public: // ОткрытаЯ часть класса, тут есть конструктор
    TestPublisher(): //  Конструктор класса, функция вызывающаяся при создании объекта класса
      Node("test_publisher"),
        counter(0)
    {
        publisher  = this->create_publisher<std_msgs::msg::String>("/test_topic", 10); // Вот это пу,ликатор в топик
        timer = this->create_wall_timer(500ms, std::bind(&TestPublisher::timer_callback, this));
        // тут мы впихнули таймер и тут же его конфигурируем
    }
private:
    rclcpp::TimerBase::SharedPtr timer; // Указываем на объект таймера TimerBase
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher;
    unsigned int counter; // Переменная счётчика
    void timer_callback(){ // Функция таймера
      std_msgs::msg::String message;
      message.data = "Counter " + std::to_string(counter);
      publisher->publish(message);
      RCLCPP_INFO_STREAM(this->get_logger(),"Published: "<<message.data);
      counter++;
    }
};

int main(int argc, char *argv[]){
    rclcpp::init(argc, argv); // подключаемся к системе с передачей параметров cmd
    rclcpp::spin(std::make_shared<TestPublisher>()); // Функция спин - цикл с указателем на наш узел
    rclcpp::shutdown(); // Как только узел заканчивает работу (сам или после команды или сбой) система выполняет шатдаун
  return 0;
}
