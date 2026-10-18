#include <rclcpp/rclcpp.hpp>// подключаем заголовочный файл для ROS2 на C++ юзаем по умолчанию
#include <std_msgs/msg/string.hpp>
#include <std_msgs/msg/int32.hpp>
#include <cmath> // библиотека для функции модуля

using namespace std::chrono_literals; // Используем пространство имен литералов для удобных нам интервалов времени
class TestSubscriber : public rclcpp::Node //Класс узла, всегда для ноды является производной класса (наследником) класса rclcpp::Node со своим функционалом
{
public: // Открытая часть класса, тут есть конструктор
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

    // Переменные класса для хранения последних полученных значений
    int tr = 0;
    int rcv = 0;
    int difference = 0;

    // Добавили этот колбэк для /msg_tr. Он просто запоминает последнее отправленное значение
    void topic_callback(const std_msgs::msg::Int32 &msg){
        tr = msg.data; // Сохраняем в переменную класса
    }

    // Этот колбэк срабатывает при получении /msg_rcv
    void counter(const std_msgs::msg::Int32 &msg){
        rcv = msg.data; // Сохраняем последнее принятое значение

        // Считаем разницу и берём модуль через std::abs
        difference = std::abs(tr - rcv);

        std_msgs::msg::Int32 msg_diff;
        msg_diff.data = difference;


        RCLCPP_INFO_STREAM(this->get_logger(), "Отправлено:"<< tr<< "Получено:"<< rcv<<"Разница: " << msg_diff.data);

        publisher_diff->publish(msg_diff);
    }
};

int main(int argc, char *argv[]){
    rclcpp::init(argc, argv); // подключаемся к системе с передачей параметров cmd
    rclcpp::spin(std::make_shared<TestSubscriber>()); // Функция спин - цикл с указателем на наш узел
    rclcpp::shutdown(); // Как только узел заканчивает работу (сам или после команды или сбой) система выполняет шатдаун
  return 0;
}
