# Transactions

Реализация проекта Transactions.


## Contents

1. [Chapter I](#chapter-i) \
    1.1. [Introduction](#introduction)
2. [Chapter II](#chapter-ii) \
    2.1. [Information](#information)
3. [Chapter III](#chapter-iii) \
    3.1. [Part 1](#part-1-реализация-in-memory-key-value-хранилища-на-базе-хеш-таблицы)  
    3.2. [Part 2](#part-2-реализация-in-memory-key-value-хранилища-на-базе-самобалансирующегося-бинарного-дерева-поиска)  
    3.3. [Part 3](#part-3-реализация-консольного-интерфейса)  
    3.4. [Part 4](#part-4-дополнительно-реализация-in-memory-key-value-хранилища-на-базе-b-деревьев)  
    3.5. [Part 5](#part-5-дополнительно-исследование-временных-характеристик)


## Chapter I

## Introduction

В данном проекте Вам предстоит вспомнить и подробнее ознакомиться с такими структурами данных как хеш-таблицы и самобалансирующиеся бинарные деревья, а также реализовать на их основе хранилище типа ключ-значение.


## Chapter II

## Information

Хринилища типа «ключ-значение» (англ. key-value storage) предназначенны для хранения, извлечения и управления ассоциативными массивами. Подобное хранилище реализуется во многих языках программирования в виде контейнерного класса *словорь*. Словари содержат коллекцию объектов или записей, которые, в свою очередь, содержат множество различных полей с данными. Эти записи хранятся и извлекаются с использованием ключа, который однозначно идентифицирует запись и используется для быстрого поиска данных в хранилище.

Также, за счёт высокой скорости чтения и записи, идею подобного хранилища реализуют многие NoSQL базы данных такие, как Redis, Memcached, Tarantool и многие-многие другие. Базы данных с использованием пар «ключ‑значение» поддерживают высокую разделяемость и обеспечивают беспрецедентное горизонтальное масштабирование, недостижимое при использовании многих других типов баз данных. 

![key-value-storage](misc/images/key-value-storage.png)

Для реализации key-value хранилищ в преобладающем большинстве используются хеш-таблицы или деревья поиска.

### Хеш-таблица

**Хеш-таблица** - это структура данных, которая используются когда необходимо быстро выполнять операции вставки/удаления/нахождения элементов. 
Скорость выполнения этих операций обусловлена устройством хеш-таблиц. В основе их работы лежит функция хеширования, которая вычисляет значение индекса на базе переданного ей ключа. Благодаря этой функции локализация элемента в таблице по его ключу в общем случае происходит со сложностью О(1), которая не зависит от размера самой хеш-таблицы. Сам же процесс получения индекса по ключу называется **хешированием**. 

![hash-table](misc/images/hash-table.png)

Когда хеш-функция генерирует одинаковый индекс для нескольких разных ключей, возникает конфликт: неизвестно, какое значение нужно сохранить в этом индексе. Подобные ситуации называются **коллизиями**.

Есть несколько методов борьбы с коллизиями:
- метод цепочек;
- метод открытой адресации: линейное пробирование, квадратичное пробирование, двойное хеширование. 

Хеш-таблицы используются для реализации контейнерных классов типа ключ-значения во многих языках программирования, к примеру:
* *C#*: Dictionary, ConcurrentDictionary, HashMap
* *Python*: set, map
* *JavaScript*: Map

### Двоичные деревья поиска

**Двоичное дерево (Бинарное дерево)** — это иерархическая структура данных, в которой каждый узел имеет значение и ссылки на левого и правого потомка. Узел, находящийся на самом верхнем уровне (не являющийся чьим либо потомком) называется корнем. Узлы, не имеющие потомков (оба потомка которых равны `NULL`) называются листьями.

**Бинарное дерево поиска** — это бинарное дерево, обладающее дополнительными свойствами: 
* оба поддерева — левое и правое — являются двоичными деревьями поиска
* у всех узлов левого поддерева произвольного узла X значения ключей данных меньше, нежели значение ключа данных самого узла X
* у всех узлов правого поддерева произвольного узла X значения ключей данных больше либо равны, нежели значение ключа данных самого узла X

![binary-tree](misc/images/binary-tree.png)

В итоге, данные в бинарном дереве поиска хранятся в отсортированном виде. При каждой операции вставки нового или удаления существующего узла отсортированный порядок дерева сохраняется. Благодаря этому появляется возможность формализации алгоритма поиска элементов в бинарном дереве:

* Поиск осуществляется целенаправленным движением по ветвям дерева
* Если ключ поиска равен ключу в вершине, значит, ключ найден и можно вернуть значение узла
* Если ключ поиска меньше ключа в вершине, то осуществляется движение по дереву вниз влево, в противном случае — вниз вправо 
* Если в очередной вершине дальнейшее движение вниз невозможно (указатель равен `NULL`), то это означает, что искомого ключа нет в дереве

Максимальная длина поиска в таком случае равна высоте дерева.

**Сбалансированное бинарное дерево поиска** — это бинарное дерево поиска с логарифмической высотой. Оно применяется, когда необходимо осуществлять быстрый поиск элементов, чередующийся со вставками новых элементов и удалениями существующих. В подобных деревьях для каждой вершины высота двух поддеревьев различается не более чем на 1. Примерами самобалансирующихся деревьев поиска являются *красно-чёрные деревья* или *АВЛ-деревья*.

Красно-чёрные деревья поиска используются в структурах данных различных языков программирования, например:
* *Java*: java.util.TreeMap | java.util.TreeSet
* *C++ STL*: map, multimap, multiset.
* *Linux kernel*: completely fair scheduler, linux/rbtree.h

### Дерево поиска B-tree

Основные операции в деревьях выполняются за время пропорциональное его высоте. Сбалансированные деревья минимизируют свою высоту (к примеру, высота бинарного сбалансированного дерева с _n_ узлами равна _log n_). 

В чем же проблема этих стандартных деревьев поиска? Рассмотрим огромную базу данных, представленную в виде одного из упомянутых деревьев. Очевидно, что мы не можем хранить всё это дерево в оперативной памяти, поэтому в ней храним лишь часть информации, остальное же хранится на стороннем носителе (допустим, на жестком диске, скорость доступа к которому гораздо медленнее). Такие деревья как красно-черное или АВЛ будут требовать от нас _log n_ обращений к стороннему носителю. При больших _n_ это очень много. Как раз эту проблему и призваны решить B-деревья!

B-деревья также представляют собой сбалансированные деревья, поэтому время выполнения стандартных операций в них пропорционально высоте. Но, в отличие от остальных деревьев, они созданы специально для эффективной работы с дисковой памятью, за счёт минимизации числа обращений на чтение или запись на диск.

B деревья используются в key-value хранилищах:
* Apache Ignite
* Tokyo Cabinet

### Описание структуры данных

В качестве ключа в key-value хранилище будут выступать **строки**. В качестве значений (данных) в key-value хранилище будут хранится записи о студентах Школы 21 в следующем виде:
* Фамилия
* Имя
* Год рождения
* Город
* Число текущих коинов

### Описание реализуемых функций key-value хранилища

### SET

Команда используется для установки ключа и его значения. В примере ниже ключом является строка `foo`, значением же является структура, описанная выше. Значение полей новой записи вводится в порядке их описания в структуре. В качестве необязательных параметров используется `EX` для указания срока жизни создаваемой записи. Если необязательное поле не указано, то по умолчанию временя жизни записи никак не ограничено.

Описание параметров команды `SET`:
```
SET <ключ> <Фамилия> <Имя> <Год рождения> <Город> <Число текущих коинов> EX <время в секундах>
```

Пример использования команды `SET` для создания записи без ограничения на время её существования:
```
SET foo Васильев Иван 2000 Москва 55 
> OK
SET foo Васильев 123 aaaaa Москва 55 
> ERROR: unable to cast value "aaaa" to type int 
```

Пример использования команды `SET` для создания записи с ограничением на время её существования. В этом примере запись будет существовать на протяжении 10 секунд, после чего автоматически удалится:
```
SET foo Васильев Иван 2000 Москва 55 EX 10 
> OK
```

### GET

Команда используется для получения значения, связанного с ключом. Если такой записи нет, то будет возвращён `(null)`:
```
GET foo
> Васильев Иван 2000  Москва   55 
GET unknownkey
> (null)
```

### EXISTS

Эта команда проверяет, существует ли запись с данным ключом. Она возвращает `true` если объект существует или `false` если нет:
```
EXISTS foo
> true
```

### DEL

Команда удаляет ключ и соответствующее значение, после чего возращает `true`, если запись успешно удалена, в противном случае - `false`:
```
DEL foo
> true
```

### UPDATE

Команда обновляет значение по соответствующему ключу, если такой ключ существует:
```
SET foo Вас И 20 Мос 5 
> OK
UPDATE foo Васильев Иван 2000 Москва 55
> OK

GET foo
> Васильев Иван 2000 Москва 55 
```

Если же какое-то поле менять не планируется, то на его месте ставится прочерк "-":
```
SET foo Вас И 20 Мос 5 
> OK
UPDATE foo Васильев - - - 55
> OK

GET foo
> Васильев И 20 Мос 55 
```

### KEYS

Возвращает все ключи, которые есть в хранилище:
```
KEYS
1) boo
2) foo
3) bar
```

### RENAME 

Команда используется для переименования ключей:
```
RENAME foo foo2
> OK

GET foo
> (null)

GET foo2
> Васильев И 20 Мос 55 
```

### TTL

Когда ключ установлен с истечением срока действия, эту команду можно использовать для просмотра оставшегося времени. Если записи с заданным ключом не существует, то возвращается `(null)`:
```
SET Васильев Иван 2000 Москва 55 EX 10
> OK
TTL foo
> 6
TTL foo
> 5
TTL foo
> 4
TTL foo
> (null)
```

### FIND

Эта команда используется для восстановления ключа (или ключей) по заданному значению. Аналогично команде `UPDATE` необязательно указывать все значения из структуры студентов школы 21. Если по каким-либо полям не будет выполняться поиск, то на их месте просто проставляются прочерк `-`.

Пример использования команды `FIND` с поиском по всем полям структуры студентов:
```
FIND Васильев Иван 2000 Москва 55
> 1) foo
FIND Васильев Антон 1997 Тверь 55
> 1) boo
```

Пример использования команды `FIND` с поиском по фамилии и числу коинов: 
```
FIND Васильев - - - 55
> 1) foo
> 2) boo
```

### SHOWALL

Команда для получения всех записей, которые содержатся в key-value хранилище на текущий момент:
```
SHOWALL
>  №| Фамилия |   Имя   | год |  город   | Количество коинов |
> 1 "Васильев"  "Иван"   2000   "Москва"           55 
> 2  "Иванов"  "Василий" 2000   "Москва"           55 
```

### UPLOAD

Данная команда используется для загрузки данных из файла. Файл содержит список загружаемых данных в формате:
```
key1 "Васильев" "Иван" 2001 "Ростов" 55
key2 "Иванов" "Василий" 2000 "Москва" 55 
...
key101 "Сидоров" "Сергей" 1847 "Суздаль" 12312313 
```

Вызов команды:
```
UPLOAD ~/Descktop/TestData/file.dat
> OK 101
```

После ответа `OK` выводится число загруженных строк из файла.

### EXPORT
  
Данная команда используется для выгрузки данных, которые находятся в текущий момент в key-value хранилище в файл. Файл должен на выходе содержать список данных в формате:
```
key1 "Васильев" "Иван" 2001 "Ростов" 55
key2 "Иванов" "Василий" 2000 "Москва" 55 
...
key101 "Сидоров" "Сергей" 1847 "Суздаль" 12312313 
```

Вызов команды:
```
EXPORT ~/Descktop/TestData/export.dat
> OK 101
```

После ответа `OK` выводится число выгруженных строк из файла.


## Chapter III

## Part 1. Реализация in-memory key-value хранилища на базе хеш-таблицы

Необходимо реализовать in-memory key-value хранилище на основе хеш-таблицы:
- Программа должна быть разработана на языке C++ стандарта C++17
- Код программы должен находиться в папке src
- Классы должны быть реализованы внутри пространства имен `s21`
- Не использовать устаревшие и выведенные из употребления конструкции языка и библиотеки
- Оформить решение как статическую библиотеку (с заголовочным файлом hash_table.h)
- Библиотека должна быть представлена в виде класса `HashTable`, который хранит в себе информацию в виде хеш-таблицы и содержит все необходимые методы для реализации функционала, описанного [выше](#описание-реализуемых-функций-key-value-хранилища). Выбор хеш-функции и способа разрешения коллизий остаётся за вами
- Класс `HashTable` должен наследоваться от базового абстрактного класса, содержащего все методы, описанные [выше](#описание-реализуемых-функций-key-value-хранилища)
- Предусмотреть Makefile для сборки библиотеки и тестов (с целями all, clean, tests, hash_table.a)
- Должно быть обеспечено полное покрытие unit-тестами методов класса `HashTable`

## Part 2. Реализация in-memory key-value хранилища на базе самобалансирующегося бинарного дерева поиска

Необходимо реализовать in-memory key-value хранилище на основе самобалансирующегося бинарного дерева поиска:
- Программа должна быть разработана на языке C++ стандарта C++17
- Код программы должен находиться в папке src
- Классы должны быть реализованы внутри пространства имен `s21`
- Не использовать устаревшие и выведенные из употребления конструкции языка и библиотеки
- Оформить решение как статическую библиотеку (с заголовочным файлом self_balancing_binary_search_tree.h)
- Библиотека должна быть представлена в виде класса `SelfBalancingBinarySearchTree`, который хранит в себе информацию в виде самобалансирующегося бинарного дерева поиска и содержит все необходимые методы для реализации функционала, описанного [выше](#описание-реализуемых-функций-key-value-хранилища). Выбор типа самобалансирующегося бинарного дерева поиска остаётся за вами.
- Класс `SelfBalancingBinarySearchTree` должен наследоваться от того же базового абстрактного класса, что и в [Part 1](#part-1-реализация-in-memory-key-value-хранилища-на-базе-хеш-таблицы), содержащего все методы, описанные [выше](#описание-реализуемых-функций-key-value-хранилища)
- Предусмотреть Makefile для сборки библиотеки и тестов (с целями all, clean, tests, self_balancing_binary_search_tree.a)
- Должно быть обеспечено полное покрытие unit-тестами методов класса `SelfBalancingBinarySearchTree`

## Part 3. Реализация консольного интерфейса

Должен быть реализован консольный интерфейс, который предоставляет пользователю следующие опции:
- Первоначальный выбор типа хранилища, который будет использоваться в процессе работы программы:
    - хеш-таблица
    - самобалансирующееся бинарное дерево поиска
- Возможность ввода команд, описанных [выше](#описание-реализуемых-функций-key-value-хранилища)

## Part 4. Дополнительно. Реализация in-memory key-value хранилища на базе B+ деревьев

Необходимо реализовать in-memory key-value хранилище на основе самобалансирующегося бинарного дерева поиска:
- Программа должна быть разработана на языке C++ стандарта C++17
- Код программы должен находиться в папке src
- Классы должны быть реализованы внутри пространства имен `s21`
- Не использовать устаревшие и выведенные из употребления конструкции языка и библиотеки
- Оформить решение как статическую библиотеку (с заголовочным файлом b_plus_tree.h)
- Библиотека должна быть представлена в виде класса `BPlusTree`, который хранит в себе информацию в виде B+ дерева и содержит все необходимые методы для реализации функционала, описанного в [выше](#описание-реализуемых-функций-key-value-хранилища).
- Класс `BPlusTree` должен наследоваться от базового абстрактного класса из [Part 1](#part-1-реализация-in-memory-key-value-хранилища-на-базе-хеш-таблицы) и [Part 2](#part-2-реализация-in-memory-key-value-хранилища-на-базе-самобалансирующегося-бинарного-дерева-поиска), содержащего все методы, описанные [выше](#описание-реализуемых-функций-key-value-хранилища)
- Предусмотреть Makefile для сборки библиотеки и тестов (с целями all, clean, tests, b_plus_tree.a)
- Должно быть обеспечено полное покрытие unit-тестами методов класса `BPlusTree`
- Добавить в консольный интерфейс, описанный в [Part 3](#part-3-реализация-консольного-интерфейса), возможность выбора B+ дерева в качестве создаваемого in-memory key-value хранилища

## Part 5. Дополнительно. Исследование временных характеристик

Провести исследование временных характеристик вариантов реализаций in-memory key-value хранилища на базе хеш-таблицы, а также бинарного дерева поиска.

- Пользователем задается количество элементов в хранилище
- Данные в хранилище генерируются случайным образом
- Пользователем задается количество повторений одного действия
- Каждое действие, указанное ниже, выполнить указанное пользователем количество раз, после чего вывести среднее среди полученных значений времени работы
- Изменить время выполнения следующих действих обоими видами хранилищ:
    - Получение произвольного элемента
    - Добавление элемента
    - Удаление элемента
    - Получения списка всех элементов в словаре
    - Поиск ключа элемента по значению
- Быть готовым объяснить полученные результаты
