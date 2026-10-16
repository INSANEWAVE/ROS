#include <rclcpp/rclcpp.hpp>// подключаем заголовочный файл для ROS2 на C++ юзаем по умолчанию
#include <std_msgs/msg/string.hpp>//Подключаем строковые типы сообщений
#include <std_msgs/msg/int32.hpp>//Подключаем целочисленные цифровые сообщения

using namespace std::chrono_literals; // Используем пространство имен литералов для удобных нам итервалов времени
class TestPublisher : public rclcpp::Node //Класс узла, всегда для ноды является производной класса (наследником) класса rclcpp::Node со своим функционалом
{
public: // ОткрытаЯ часть класса, тут есть конструктор
    TestPublisher(): //  Конструктор класса, функция вызывающаяся при создании объекта класса
      Node("test_publisher"),
        counter(0)
    {
    publisher1  = this->create_publisher<std_msgs::msg::String>("/test_topic", 10); // Вот это пу,ликатор в топик
    publisher2  = this->create_publisher<std_msgs::msg::Int32>("/msg_tr", 10); //публикатор по заданиюВ

    timer = this->create_wall_timer(500ms, std::bind(&TestPublisher::timer_callback, this));// тут мы впихнули таймер и тут же его конфигурируем

    }
private:
    rclcpp::TimerBase::SharedPtr timer; // Указываем на объект таймера TimerBase
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher1;
    rclcpp::Publisher<std_msgs::msg::Int32>::SharedPtr publisher2;
    unsigned int counter; // Переменная счётчика
    void timer_callback(){ // Функция таймера

      std_msgs::msg::String msg1;
      std_msgs::msg::Int32 msg2;

      msg1.data = "Counter " + std::to_string(counter);
      msg2.data = counter;

      publisher1->publish(msg1);
      publisher2->publish(msg2);


      RCLCPP_INFO_STREAM(this->get_logger(),
                         "Published: "<<msg1.data<< " | "<<msg2.data);
      counter++;
    }
};

int main(int argc, char *argv[]){
    rclcpp::init(argc, argv); // подключаемся к системе с передачей параметров cmd
    rclcpp::spin(std::make_shared<TestPublisher>()); // Функция спин - цикл с указателем на наш узел
    rclcpp::shutdown(); // Как только узел заканчивает работу (сам или после команды или сбой) система выполняет шатдаун
  return 0;
}
