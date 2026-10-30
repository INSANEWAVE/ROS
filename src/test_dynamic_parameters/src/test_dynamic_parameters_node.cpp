#include <rclcpp/rclcpp.hpp> // Подключаем заголовочный файл для ROS2 на C++
#include <rcl_interfaces/msg/set_parameters_result.hpp> // Необходимо для работы с результатом установки параметров

using namespace std::chrono_literals; // Используем пространство имен для удобной работы с единицами времени (500ms)

class TestDynamicParameters : public rclcpp::Node // Класс узла, наследник rclcpp::Node
{
public:
    TestDynamicParameters(): // Конструктор класса
        Node("test_dynamic_parameters_node") // Инициализируем имя ноды
    {
        // Объявляем параметры (строковый и числовой с плавающей точкой)
        this->declare_parameter("string_parameter", "test_string");
        this->declare_parameter("number_parameter", 15.5);

        // Регистрируем коллбэк, который будет срабатывать при попытке изменить параметры
        callback_handle_ = this->add_on_set_parameters_callback(
            std::bind(&TestDynamicParameters::parametersCallback,
                      this,
                      std::placeholders::_1));
    }

private:
    // Хэндл для хранения коллбэка
    OnSetParametersCallbackHandle::SharedPtr callback_handle_;

    // Функция-коллбэк для обработки изменений параметров
    rcl_interfaces::msg::SetParametersResult parametersCallback(
        const std::vector<rclcpp::Parameter> &parameters) {

        rcl_interfaces::msg::SetParametersResult result;
        result.successful = true;
        result.reason = "success";

        // Проходимся по всем переданным параметрам
        for(auto &param: parameters) {
            if(param.get_name() == "number_parameter") {
                RCLCPP_INFO_STREAM(this->get_logger(),
                                   "New value for number: " << param.as_double());
            }
            if(param.get_name() == "string_parameter") {
                RCLCPP_INFO_STREAM(this->get_logger(),
                                   "New value for string: " << param.as_string());
            }
        }
        return result;
    }
};

int main(int argc, char *argv[]) {
    rclcpp::init(argc, argv); // Инициализация ROS 2
    rclcpp::spin(std::make_shared<TestDynamicParameters>()); // Запуск цикла обработки событий узла
    rclcpp::shutdown(); // Завершение работы ROS 2
    return 0; // Код возврата (на фото обрезан, но обязателен)
}
