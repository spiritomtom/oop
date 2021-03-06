Задача
======
Да се напише програма за симулация на работа с банкови сметки.

Банкова сметка
--------------
Да се дефинира клас `BankAccount`, представящ банкова сметка на клиент на банка.
Една банкова сметка съдържа следната информация:
* `client_name` – име на клиента (`std::string`)
* `number` – номер на банковата сметка (`std::string`)
* `balance` – налична сума в банковата сметка (`double`)

Член-данните на класа трябва да бъдат скрити (`private`).

Дефинирайте следните селектори (getter методи):
* `get_client_name()` – връща името на клиента
* `get_number()` – връща номера на банковата сметка
* `get_balance()` – връща наличната сума в банковата сметка

Дефинирайте метод `is_empty()`, който проверява дали наличната сума в банковата
сметка е 0.

### Конструиране
Банковата сметка трябва да може да се конструира по следните начини:
* `BankAccount()` – празна банкова сметка (името на клиента и номера на
банковата сметка да са празни низове, а наличната сума да е 0 лева)
* `BankAccount(std::string client_name, std::string number)` – само по име на
клиента и номер, с налична сума 0 лева
* `BankAccount(std::string client_name, std::string number, double amount)` – по
дадено име на клиента, номер и налична сума

Примери за конструиране на банкови сметки:
```cpp
BankAccount empty_bank_account; // конструира празна банкова сметка: името на клиента и номера на сметката са празни низове, а наличната сума е 0 лева

BankAccount bank_account_without_money("Petar", "10AC9D2C"); // конструира банкова сметка с име на клиент "Petar", номер "10AC9D2C" и налична сума 0 лева

BankAccount bank_account("Georgi", "V7RG42NR", 2481.42); // конструира банкова сметка с име на клиент "Georgi", номер "V7RG42NR" и налична сума 2481.42 лева
```

### Внасяне и теглене на пари
Да се напишат методи, чрез които да могат да се внасят и теглят пари от
банковата сметка:
* `deposit(double amount)` – добавя `amount` лева към баланса на банковата
сметка
* `draw(double amount)` – премахва `amount` лева от баланса на банковата сметка

Примери за начина на употреба на методите:
```cpp
BankAccount bank_account("Georgi", "V7RG42NR"); // създаваме банкова сметка на "Georgi" с номер "V7RG42NR" и налична сума 0 лева

bank_account.deposit(100); // внасяме 100 лева в сметката на "Georgi"

bank_account.draw(70); // теглим 70 лева от сметката на "Georgi"; в сметката остават 30 лева
```

### Информация за банкова сметка
Да се напише метод `print()` за извеждане на информация за банковата сметка.

Пример:
```cpp
BankAccount bank_account("Georgi", "V7RG42NR");
bank_account.print(); // извежда на стандартния изход: "Georgi, V7RG42NR, 0 leva"
```

Банка
-----
Да се дефинира клас `Bank`, представящ откритите сметки в дадена банка. Класът
да съдържа член-даннa `bank_accounts`, която да представлява колекцията от
открити банкови сметки в банката. За целта използвайте колекция от стандартната
библиотека (`std::vector`, `std::list`).

### Конструиране
Банката трябва да може да се конструира по следните начини:
* `Bank()` – банка без нито една открита сметка
* `Bank(<тип_колекция> bank_accounts)` – по колекция от банкови сметки

### Функционалности
Да се дефинират следните методи:
* `add_account(BankAccount account)` – добавя банковата сметка `account` към
колекцията от открити сметки в банката
* `remove_account(std::string number)` – премахва банкова сметка с номер
`number` от колекцията с открити сметки в банката
* `get_account_balance(std::string number)` – връща наличната сума пари в
банкова сметка с номер `number`
* `get_client_balance(std::string client_name)` – връща общата сума пари на
даден клиент на банката, налична във всичките му сметки

Пример:
```cpp
BankAccount account_of_ivan("Ivan", "1", 2000);
BankAccount account_of_petar("Petar", "2", 500);
BankAccount second_account_of_ivan("Ivan", "3", 400);

std::vector<BankAccount> accounts = { account_of_ivan, account_of_petar };
Bank bank(accounts);
bank.add_account(second_account_of_ivan); // добавя втора сметка на "Ivan" в банката

bank.get_account_balance("2"); // 500
bank.remove_account("2"); // премахва сметката на "Petar" от банката

bank.get_client_balance("Ivan"); // 2400
```
