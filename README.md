# Описание API

### Краткое описание 

Я буду рассматривать REST API сервис на примере ETH кошельков пользователей. Один юзер имеет один кошелек, поэтому будет иметь user_id и eth_amount. Тогда будут запросы: получить информацию о пользователе, изменить количество криптовалюты у юзера, удалить кошелек юзера, добавить новый кошелек для юзера и получить все кошельки юзеров.

### Представление данных

| Data type | Description | Variable |
| ------ | ------ | ------ |
| int |ID | user_id |
| float | Количество криптовалюты | eth_amount |

Здесь ID у всех юзеров будут уникальными. 

### Пример кошелька юзера

```json
{
  "user_wallet": {
    "user_id": 12,
    "eth_amount": 0.451
  }
  "actions" : [
        {"rel": "add_wallet", "template": "/user_wallets", "method": "POST", "required_data":["user_id", "eth_amount"]},
        {"rel": "edit_wallet", "template": "/user_wallet/{user_id}", "method": "POST", "required_data":["eth_amount"]},
        {"rel": "get_wallet", "template": "/user_wallet/{user_id}", "method": "GET", "required_data":[]},
        {"rel": "delete_wallet", "template": "/user_wallet/{user_id}", "method": "DELETE", "required_data":[]},
        {"rel": "get_all_wallets", "template": "/user_wallets", "method": "GET", "required_data":[]},
  ]
}
```

### Описание возможных запросов

| Query type | URI | Description |
| ------ | ------ | ------ |
| GET| /eth_webservice_queries | Получить все темплейты для запросов к веб-сервису|
|  POST | /user_wallet/<user_id> | Изменить количество криптовалюты |
|  GET | /user_wallet/<user_id> | Получить информацию о количестве криптовалюты в кошельке |
|  DELETE | /user_wallet/<user_id> | Удалить криптовалюту |
|  POST | /user_wallets | Добавить новую криптовалюту. Ожидается, что будут переданы поля user_id и eth_amount |
| GET | /user_wallets | Получить информацию о всех юзерах |

# Описание API и каждого возможного запроса отдельно 

## 0. /eth_webservice_queries (GET) - это будет стандартный URI
По этому URI можно обратиться к сервису, чтобы получить список возможных запросов, например ответ по запросу будет выглядеть так:
```json
{
    "serviceDescription": {
      "base" : "/eth_webservice_queries" 
       "actions" : [
        {"rel": "add_wallet", "template": "/user_wallets", "method": "POST", "required_data":["user_id", "eth_amount"]},
        {"rel": "edit_wallet", "template": "/user_wallet/{user_id}", "method": "POST", "required_data":["eth_amount"]},
        {"rel": "get_wallet", "template": "/user_wallet/{user_id}", "method": "GET", "required_data":[]},
        {"rel": "delete_wallet", "template": "/user_wallet/{user_id}", "method": "DELETE", "required_data":[]},
        {"rel": "get_all_wallets", "template": "/user_wallets", "method": "GET", "required_data":[]},
       ]
    }
}
```
Ко всем ответам по запросу кроме возвращаемого значения, также будут передавать actions,которые можно сделать с кошельком

```json
{
    "user_wallet": { 
        "user_id": 12,
        "eth_amount": 0.451
    }
    "actions" : [
        {"rel": "add_wallet", "template": "/user_wallets", "method": "POST", "required_data":["user_id", "eth_amount"]},
        {"rel": "edit_wallet", "template": "/user_wallet/{user_id}", "method": "POST", "required_data":["eth_amount"]},
        {"rel": "get_wallet", "template": "/user_wallet/{user_id}", "method": "GET", "required_data":[]},
        {"rel": "delete_wallet", "template": "/user_wallet/{user_id}", "method": "DELETE", "required_data":[]},
        {"rel": "get_all_wallets", "template": "/user_wallets", "method": "GET", "required_data":[]},
    ]
}
```

### 1./user_wallet/{user_id} запросы

#### 1.a) Получить информацию о количестве криптовалюты в кошельке юзера (GET)

Результатом будет являться информация о криптовалюте в формате JSON с заданным ID. Если ID не существует или задан некорректно, будут выдаваться соответствующие сообщения.

| Value | Code | Description |
| ------ | ------ | ------ |
|  OK | 200 | Корректный запрос, в результате вернется JSON с описанием криптовалюты с заданным ID |
|  Bad request | 400 | Введен некорректный запрос |
|  Not found | 404 | Нет кошелька, у которого заданный user ID |

#### 1.b) Удалить кошелек юзера(DELETE)

Если кошелек с таким user ID существует, он будет удален. Если такого не было или задан некорректный формат для ID, то будет возвращена соответствующая ошибка.

| Value | Code | Description |
| ------ | ------ | ------ |
|  OK | 200 | Корректный запрос, в результате будет удалена криптовалюта с заданным ID |
|  Bad request | 400 | Введен некорректный запрос |
|  Not found | 404 | Нет кошелька, у которого заданный user ID, ничего не будет удалено |

#### 1.c) Изменить количество криптовалюты в кошельке юзера (POST)

Если кошелек с таким user ID существует и корректно задано поле нового количества (eth_amount), то количество криптовалюты в кошельке с заданным ID будет изменено. Если некорректно заданы параметры или кошелька с таким user ID не существует, будут возвращены соответствующие ошибки. 

В запросе необходимо передать JSON с параметром <eth_amount> - новое количество криптовалюты с заданным user ID. Например,

```json
{
    "eth_amount": ["New amount as <float>"]
}
```

| Value | Code | Description |
| ------ | ------ | ------ |
|  OK | 200 | Корректный запрос, у криптовалюты с заданным ID было изменено поле отвечающее за количество, будет возвращен JSON криптовалюты с измененным количеством|
|  Bad request | 400 | Некорректный запрос или список передаваемых параметров |
|  Not found | 404 | Нет кошелька, у которого заданный user ID |

### 2. /user_wallets запросы

#### 2.a) Получить информацию о всех кошельках (GET)

Результатом будет являться информация о всех криптовалютах в формате JSON.
```json
{
    "user_wallets": [
        "user_wallet": {
          "user_id": 12,
          "eth_amount": 0.451
        },
        "user_wallet": {
          "user_id": 13,
          "eth_amount": 1.34
        },
        ...
    ]
}
```

#### 2.b) Добавить новый кошелек (POST)

В результате запроса в базу будет добавлен новую кошелек, всю информацию про которому нужно передать в JSON. Если поля user_id или eth_amount не будет задано или они заданы некорректно, то новая криптовалюта не будет добавлена.

Если запрос удалось выполнить, возвращается JSON, который будет описывать новую криптовалюту в кошельке. 

| Value | Code | Description |
| ------ | ------ | ------ |
|  OK | 200 | Корректный запрос, будет возвращен JSON, который будет описывать новый кошелек |
|  Bad request | 400 | Некорректный запрос или список передаваемых параметров |

