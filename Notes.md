RQT можно запустить через Х11

QT_QPA_PLATFORM=xcb rqt

Вот так инициируем новый терминал

source install/setup.bash

Присвоение параметра командной строкой

$ ros2 run test_parameters test_parameters_node --ros-args -p number_parameter:=100.0

ros2 param list (get)

 ros2 param get /test_parameters_node string_parameter
 
 ros2 param set /test_parameters_node string_parameter "XX"
 

ros2 param dump /test_parameters_node > test_dump.yaml

ros2 param load /test_parameters_node test_dump.yaml

 ros2 launch test_launch test_param_launch.py 

Для набора:


Набор 1: Стороны 3.0, 4.0, 5.0. Площадь: 6.00
Набор 2: Стороны 5.0, 5.0, 5.0. Площадь: 10.83
Набор 3: Стороны 7.5, 8.2, 10.1. Площадь: 30.28
