#include <rclcpp/rclcpp.hpp> // подключаем заголовочный файл для ROS2 на C++ юзаем по умолчанию
#include <std_srvs/srv/set_bool.hpp>
#include <string>

using namespace std::chrono_literals; // Используем пространство имен литералов для удобных нам интервалов времени

class TestBasicService : public rclcpp::Node // Класс узла, всегда для ноды является производной класса (наследником) класса rclcpp::Node со своим функционалом
{
public: // Открытая часть класса, тут есть конструктор
    TestBasicService(): // Конструктор класса, функция вызывающаяся при создании объекта класса
      Node("test_basic_service_node"),
      flag(false)
    {
        // Исправлено форматирование для читаемости
        service = this->create_service<std_srvs::srv::SetBool>(
            "bool_service",
            std::bind(&TestBasicService::setFlag, this, std::placeholders::_1, std::placeholders::_2)
        );
    }

private:
    bool flag;
    // ИСПРАВЛЕНО: было std_srvs/srv/SetBool (со слэшами), стало std_srvs::srv::SetBool
    rclcpp::Service<std_srvs::srv::SetBool>::SharedPtr service;

    // ИСПРАВЛЕНО: было std::shared_prt, стало std::shared_ptr
    void setFlag(const std::shared_ptr<std_srvs::srv::SetBool::Request> request, std::shared_ptr<std_srvs::srv::SetBool::Response> response)
    {
        flag = request->data;

        // ИСПРАВЛЕНО: success - это тип bool, туда нельзя записать строку
        response->success = true;

        // ИСПРАВЛЕНО: строка должна записываться в поле message, а не success
        response->message = "Флаг изменился на " + std::string(flag ? "1" : "0");

        // ИСПРАВЛЕНО: опечатка responce -> response
        RCLCPP_INFO_STREAM(this->get_logger(), response->message);
    }
};

int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv); // подключаемся к системе с передачей параметров cmd

    // ИСПРАВЛЕНО: было TestService, а класс называется TestBasicService
    rclcpp::spin(std::make_shared<TestBasicService>()); // Функция спин - цикл с указателем на наш узел

    rclcpp::shutdown(); // Как только узел заканчивает работу (сам или после команды или сбой) система выполняет шатдаун
    return 0;
}
