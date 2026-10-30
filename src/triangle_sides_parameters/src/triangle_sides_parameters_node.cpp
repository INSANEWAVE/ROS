#include <rclcpp/rclcpp.hpp> // Подключаем заголовочный файл для ROS2 на C++
#include <cmath> // Подключаем математическую библиотеку для вычисления корня (sqrt)

using namespace std::chrono_literals; // Используем пространство имен для удобной работы с единицами времени (500ms)

class TesttriangleSidesParameters : public rclcpp::Node // Класс узла, наследник rclcpp::Node
{
public:
    TesttriangleSidesParameters(): // Конструктор класса
        Node("test_parameters_node") // Инициализируем имя ноды
    {
        // Объявляем параметры (числовые с плавающей точкой)
        // Обратите внимание: имена параметров чувствительны к регистру!
        this->declare_parameter("side_A", 67.5);
        this->declare_parameter("side_B", 15.5);
        this->declare_parameter("side_С", 15.5);

        // Создаем таймер на 1000 мс, который вызывает timer_callback
        timer = this->create_wall_timer(1000ms,
                                std::bind(&TesttriangleSidesParameters::timer_callback,
                                        this));
    }

private:
    rclcpp::TimerBase::SharedPtr timer; // Указатель на объект таймера

    void timer_callback() { // Функция, вызываемая таймером

        double A = this->get_parameter("side_A").as_double();
        double B = this->get_parameter("side_B").as_double();
        double C = this->get_parameter("side_С").as_double();


        if (A > 0 && B > 0 && C > 0 && (A + B > C) && (A + C > B) && (B + C > A)) {
            RCLCPP_INFO_STREAM(this->get_logger(),"Прекрасный треугольник, щас посчитаем:");

            // Формула Герона
            double p = (A + B + C) / 2.0; // Полупериметр
            double area = std::sqrt(p * (p - A) * (p - B) * (p - C)); // Площадь

            RCLCPP_INFO_STREAM(this->get_logger(), "Площадь треугольника: " << area);

        } else {

            RCLCPP_WARN_STREAM(this->get_logger(), "Бро это не треугольник, введи другие стороны");
        }
    }
};

int main(int argc, char *argv[]) {
    rclcpp::init(argc, argv); // Инициализация ROS 2
    rclcpp::spin(std::make_shared<TesttriangleSidesParameters>()); // Запуск цикла обработки событий узла
    rclcpp::shutdown(); // Завершение работы ROS 2
    return 0; // Код возврата
}
