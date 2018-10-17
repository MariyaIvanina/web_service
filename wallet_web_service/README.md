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

Реализованы функции GET,POST.PUT,DELETE /user_wallet

Тестирование
GET запросы с помощью yandex-tank. (get_test.yaml)
https://overload.yandex.net/129586 протестировала при возрастании нагрузки от 100 до 2000 юзеров. Как видно из графиков, после 900 rps мы уже уперлись в 100% CPU. Ну и дальше уже идет разлад системы, ошибки, таймауты.
Подтвердим, что мы можем работать с 800 юзеров. https://overload.yandex.net/129592 здесь наблюдается работа CPU 98%,так что я взяла еще чуток с запасом. Также не было ошибок и таймаутов.
