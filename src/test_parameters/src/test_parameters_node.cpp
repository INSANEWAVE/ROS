#include <rclcpp/rclcpp.hpp> // Подключаем заголовочный файл для ROS2 на C++

using namespace std::chrono_literals; // Используем пространство имен для удобной работы с единицами времени (500ms)

class TestParameters : public rclcpp::Node // Класс узла, наследник rclcpp::Node
{
public:
    TestParameters(): // Конструктор класса
        Node("test_parameters_node") // Инициализируем имя ноды
    {
        // Объявляем параметры (строковый и числовой с плавающей точкой)
        this->declare_parameter("string_parameter", "test_string");
        this->declare_parameter("number_parameter", 15.5);

        // Создаем таймер на 500 мс, который вызывает timer_callback
        timer = this->create_wall_timer(500ms,
                                std::bind(&TestParameters::timer_callback,
                                        this));
    }

private:
    rclcpp::TimerBase::SharedPtr timer; // Указатель на объект таймера

    void timer_callback() { // Функция, вызываемая таймером
        // Получаем значения параметров из системы
        std::string str = this->get_parameter("string_parameter").as_string();
        double num = this->get_parameter("number_parameter").as_double();

        // Выводим полученные параметры в консоль (лог)
        RCLCPP_INFO_STREAM(this->get_logger(),"Params: str: "<<str<<" num: "<<num);
    }
};

int main(int argc, char *argv[]) {
    rclcpp::init(argc, argv); // Инициализация ROS 2
    rclcpp::spin(std::make_shared<TestParameters>()); // Запуск цикла обработки событий узла
    rclcpp::shutdown(); // Завершение работы ROS 2
    return 0; // Код возврата
}
