# IMDBL
My first experience connecting sql with program code

Идея была создать базу данных оценок и отзывов на фильмы. Где множество пользователей могли бы оставлять и редактировать свои оценки/отзывы. А также где администратор мог бы их вручную регулировать.

Для начала была создана примерная схема расположения связей от сущностей. А также созданы "прокладки" для преобразования связей "многое ко многим" к связям "один ко многим". По итогу получилось 5 сущностей, а именно база данных зарегистрированных пользователей, созданных кинопродуктов, таблица оценок и таблица отзывов, а также журнал, в котором хранились все существующий на данный момент оценки/отзывы. Также для усложнения было придумано создать отдельные базу данных съемочной команды, которая содержит в себе людей (которые подгружаются из отдельной бд) а также их профессии в конкретном фильме (профессии подгружаются из отдельной бд). К примеру в "Braveheart" Мэл Гибсон был и режиссером и продюсером, и это относительно легко указать с таким типом связи.

После создания связей, и примитивного (в плане не очень много информации) наполнения базы данных, я приступил к созданию программы на c++ в среде разработки Qt.
Интерфейс приложения состоит из двух частей, отделенных друг от друга вертикальной линей, из условной левой и правой части. В левой расположен интерфейс авторизации в двух режимах, для пользователя, в том числе его регистрация, а также возможность авторизоваться как администратор.

В правой части содержаться 3 взаимоисключающие кнопки, каждая из которых включает определённый интерфейс взаимодействия с данными.

Кнопка № 1 отвечает за прямое взаимодействие с оценками, а именно, со стороны пользователя это возможность выбрать из списка фильм, и поставить/изменить свою оценку или отзыв. А со стороны администратора это возможность отдельно по каждому фильму просмотреть, и возможно даже отредактировать или удалить, оценку/отзыв каждого пользователя в отдельности.

Кнопка № 2 со стороны пользователя отвечает за возможность просмотреть списком все свои оценки/отзывы, которые содержаться в базе, а также содержат возможность удалить свою отдельно взятую оценку. Со стороны администратора кнопка открывает возможность создавать новые карточки фильмов, и удалять уже созданные карточки фильмов, в том числе удалять из бд все возможные оценки/отзывы на этот фильм. Также в интерфейсе этой кнопки можно редактировать информацию о фильме, а именно его описание и количество полученных им наград. Также доступно просмотреть прикрепленный актерский состав.

Кнопка № 3 для администратора содержит возможность почти полного взаимодействия (почти, потому что нет возможности вносить в бд новые профессии) с съемочной командой. Появляется возможность выбрать фильм, и удалить уже прикрепленного режиссера/сценариста/продюсера или прикрепить нового из списка лиц, который тоже можно расширить, добавив в него нового человека.
Для пользователя кнопка предоставляет возможность просмотреть описание/характеристики фильма/сериала, в том числе среднюю оценку этого продукта, или процент положительных рецензий. А также съемочную команду 

Недостатки моей реализации:
1) В некоторых местах (во многих) присутствует не оптимизированный программный код
2) Так как это первый опыт SQL запросов, то вместо соединения таблиц и вложенных запросов, создавал несколько примитивных запросов подряд, и сохранял данные в переменные или массивы переменных внутри кода
3) В дополнение к п. 1 и п. 2 следует добавить, что среднее считал не с помощью запросов, а запрашивая всё и внутри кода, вместе с проверками на "NULL" считал и количество, и среднее. Что тоже явно нельзя назвать высоким уровнем программирования. 
