Структура сервиса

[main.cpp]
[include/handler.h]
[include/database_header.h]
[src/hander.cpp]
[src/database_impl.cpp]


Компилировать:
cmake .
make

run server 
./Build/bin/restserver

test

generate_get.py generate_put.py генерят запросы для тестов 

Реализованы функции GET,POST.PUT,DELETE /user_wallet

Тестирование

GET запросы с помощью yandex-tank. (test/test_get.yaml)
https://overload.yandex.net/129586 протестировала при возрастании нагрузки от 100 до 2000 юзеров. Как видно из графиков, после 900 rps мы уже уперлись в 100% CPU. Ну и дальше уже идет разлад системы, ошибки, таймауты.
Подтвердим, что мы можем работать с 800 юзеров. https://overload.yandex.net/129592 здесь наблюдается работа CPU 97%,можно в целом говорить, что у нас спокойно будет держать 700 юзеров. Также не было ошибок и таймаутов.

PUT запросы с помощью yandex-tank. (test/test_put.yaml)
https://overload.yandex.net/129688 протестировала при возрастании нагрузки от 10 до 800 юзеров. Как видно из графиков, после 400 rps уже идет разлад системы, ошибки, таймауты.
Подтвердим, что мы можем работать с 300 rps. https://overload.yandex.net/online/129721. Не было ошибок и таймаутов.
