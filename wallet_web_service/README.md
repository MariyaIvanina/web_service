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

generate_get.py generate_put.py generate_mixed.py генерят запросы для тестов 

Реализованы функции GET,POST.PUT,DELETE /user_wallet

Проверить запросы можно с помощью следующих запросов

curl -XPOST localhost:34568/user_wallet/ -H "Content-type: application/json" -d '{"user_name": "user_1", "eth_amount": 1.5}'

curl -XGET localhost:34568/user_wallet/5bc9f1baf4e8330d273a23a2 -H "Content-type: application/json"

curl -XDELETE localhost:34568/user_wallet/5bc9f1baf4e8330d273a23a2 -H "Content-type: application/json"

curl -XPUT localhost:34568/user_wallet/5bc9f1baf4e8330d273a23a2 -H "Content-type: application/json" -d '{"eth_amount": 2.5}'


Тестирование

GET запросы с помощью yandex-tank. (test/test_get.yaml)
https://overload.yandex.net/129586 протестировала при возрастании нагрузки от 100 до 2000 юзеров. Как видно из графиков, после 900 rps мы уже уперлись в 100% CPU. Ну и дальше уже идет разлад системы, ошибки, таймауты.
Подтвердим, что мы можем работать с 800 юзеров. https://overload.yandex.net/129592 здесь наблюдается работа CPU 97%,можно в целом говорить, что у нас спокойно будет держать 700 юзеров. Также не было ошибок и таймаутов.

PUT запросы с помощью yandex-tank. (test/test_put.yaml)
https://overload.yandex.net/129688 протестировала при возрастании нагрузки от 10 до 800 юзеров. Как видно из графиков, после 300 rps уже идет разлад системы, ошибки, таймауты.
Подтвердим, что мы можем работать с 150 rps. https://overload.yandex.net/129802 . Не было ошибок и таймаутов. На 200 rps и выше у меня были таймауты и ошибки. Но именно 150 rps получается держать.

GET + PUT  запросы с помощью yandex-tank. (test/test_mixed.yaml)
генерация запросов была в такой пропорции: 70% GET и 30% PUT
Провела тестирование https://overload.yandex.net/130306 нагрузки от 100 до 500 за 2 минуты. После 300 rps начинается разлад системы, таймауты. Проверим, что мы можем держать 200 rps https://overload.yandex.net/130305 . Дейтсвительно можем, ошибок нет, времяответов достаточно приемлемо, CPU занято не на 100%.

