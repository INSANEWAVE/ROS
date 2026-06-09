#include <rclcpp/rclcpp.hpp> // подключаем заголовочный файл для ROS2 на C++ юзаем по умолчанию

using namespace std::chrono_literals; // Используем пространство имен литералов для удобных нам инетрвалов времени
class TestNode : public rclcpp::Node //Класс узла, всегда для ноды является производной класса (наследником) класса rclcpp::Node со своим функционалом
{
public: // ОткрытаЯ часть класса, тут есть конструктор
    TestNode(): //  Конструктор класса, функция вызывающаяся при создании объекта класса
      Node("test_node"),
        counter(0)
    {
        timer = this->create_wall_timer(500ms, std::bind(&TestNode::timer_callback, this));
        // тут мы впихнули таймер и тут же его конфигурируем
    }
private:
    rclcpp::TimerBase::SharedPtr timer; // Указываем на объект таймера TimerBase
    unsigned int counter; // Переменная счётчика
    void timer_callback(){ // Функция таймера
      RCLCPP_INFO(this->get_logger(),"Test INFO: %d", counter); //1 способ вывода в консоль похожа на принтФ, %d тут говорит про знаковое целове число
      RCLCPP_INFO_STREAM(this->get_logger(),"Test INFO STREAM: "<<counter);
      counter++;
    }
};

int main(int argc, char *argv[]){
    rclcpp::init(argc, argv); // подключаемся к системе с передачей параметров cmd
    rclcpp::spin(std::make_shared<TestNode>()); // Функция спин - цикл с указателем на наш узел
    rclcpp::shutdown; // Как только узел заканчивает работу (сам или после команды или сбой) система выполняет шатдаун
  return 0;
}
