Я использовала фреймворк The C++ REST SDK(https://github.com/Microsoft/cpprestsdk) и на его основе буду делать свое API.

Загружала на Ubuntu 16 с помощью sudo apt-get install libcpprest-dev
Далее для запуска сервиса нужно выполнить следующие команды:
cmake .
make
./Build/bin/restserver

Далее чтобы проверить работоспособность нужно запустить curl http://127.0.0.1:34568/
