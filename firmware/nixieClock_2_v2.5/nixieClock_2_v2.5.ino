/*
  Скетч к проекту "Часы на ГРИ версия 2"
  Страница проекта (схемы, описания): https://alexgyver.ru/nixieclock_v2/
  Исходники на GitHub: https://github.com/AlexGyver/NixieClock_v2
  Нравится, как написан код? Поддержи автора! https://alexgyver.ru/support_alex/
  Автор: AlexGyver Technologies, 2018 
  Добавил коду: Коленка Технолоджис 2020
  https://AlexGyver.ru/
*/

/*
  Управление:
  - Настройка времени:
    - Левая кнопка - выбор, остальные "больше" и "меньше"
    - Удержать "выбор" - настройка времени
    - Клик по "выбору" - смена настройки часы/минуты
    - Клик "больше" и "меньше" - изменение времени
    - Удержать "выбор" - возврат к режиму часов

  - Управление эффектами В РЕЖИМЕ ЧАСОВ:
    - Удержание центральной кнопки включает и выключает "глюки"
    - Клик по центральной кнопке переключает режимы подсветки ламп
      - Дыхание
      - Постоянное свечение
      - Отключена
    - Клик по правой кнопке переключает режимы перелистывания цифр
      - Без эффекта
      - Плавное угасание
      - Перемотка по порядку числа
      - Перемотка по катодам
      - Поезд
      - Резинка
*/
/*
  Версия 2.0 (совместно с Pavel Smelov):
  - Поправлены косяки с подсветкой
  - Добавлены эффекты переключения "поезд" и "резинка"
  - Оптимизирован код

  Версия 2.1:
  - Исправлена ошибка с ходом времени

  Версия 2.2:
  - Сброс секунд при установке времени
  
  Версия 2.3:
  - Добавлены расширенные настройки яркости
  - Исправлены зависания при нулевых значениях некоторых настроек

  Версия 2.4:
  - Продолжаем исправлять баги с нулевыми настройками

  Версия 2.5:
  - Демонстрация эффекта при выборе

  Версия 2.6:
  - Добавлен будильник, звук переключения режима глюков, немного почищен код
  -Удержание кнопки режим - вход в меню настройки, вторые часы - настройка будильника.
  -Клик по кнопке режим не в режиме настройки - включение\отключение будильника. 
   (При включении покажется время срабатывания и произойдет двойной писк)
   -При срабатывании будильника - клик по кнопке режим - прервать срабатывание
*/

// ************************** НАСТРОЙКИ **************************
#define BOARD_TYPE 2
// тип платы часов:
// 0 - IN-12 turned (индикаторы стоят правильно)
// 1 - IN-12 (индикаторы перевёрнуты)
// 2 - IN-14 (обычная и neon dot)
// 3 другие индикаторы

#define DUTY 140        // скважность ШИМ. От скважности зависит напряжение! у меня 175 вольт при значении 180 и 145 вольт при 120

// ======================= ЭФФЕКТЫ =======================
volatile byte FLIP_EFFECT = 1;// эффекты перелистывания часов
// Выбранный активен при первом запуске и меняется кнопками. Запоминается в память
// 0 - нет эффекта
// 1 - плавное угасание и появление (рекомендуемая скорость: 100-150)(60)
// 2 - перемотка по порядку числа (рекомендуемая скорость: 50-80)(50)
// 3 - перемотка по порядку катодов в лампе (рекомендуемая скорость: 30-50)(50)
// 4 - поезд (рекомендуемая скорость: 50-170)(70)
// 5 - резинка (рекомендуемая скорость: 50-150)(50)


// =======================  ЯРКОСТЬ =======================
#define NIGHT_LIGHT 1       // менять яркость от времени суток (1 вкл, 0 выкл)
#define NIGHT_START 18      // час перехода на ночную подсветку (BRIGHT_N)
#define NIGHT_END 10         // час перехода на дневную подсветку (BRIGHT)

#define INDI_BRIGHT 20      // яркость цифр дневная (1 - 24) !на 24 могут быть фантомные цифры!
#define INDI_BRIGHT_N 10     // яркость ночная (1 - 24)

#define DOT_BRIGHT 30       // яркость точки дневная (1 - 255)
#define DOT_BRIGHT_N 20     // яркость точки ночная (1 - 255)

#define BACKL_BRIGHT 200    // макс. яркость подсветки ламп дневная (0 - 255)
#define BACKL_BRIGHT_N 100   // макс. яркость подсветки ламп ночная (0 - 255, 0 - подсветка выключена)
#define BACKL_MIN_BRIGHT 50 // мин. яркость подсветки ламп в режиме дыхание (0 - 255)
#define BACKL_PAUSE 10     // пазуа "темноты" между вспышками подсветки ламп в режиме дыхание, мс

// =======================  ГЛЮКИ =======================
#define GLITCH_MIN 20       // минимальное время между глюками, с
#define GLITCH_MAX 100      // максимальное время между глюками, с

// ======================  МИГАНИЕ =======================
#define DOT_TIME 1000        // время мигания точки, мс
#define DOT_TIMER 5        // шаг яркости точки, мс

#define BACKL_STEP 2        // шаг мигания подсветки
#define BACKL_TIME 10000     // период подсветки, мс

// ==================  АНТИОТРАВЛЕНИЕ ====================
#define BURN_TIME 20        // период обхода индикаторов в режиме очистки, мс
#define BURN_LOOPS 2        // количество циклов очистки за каждый период
#define BURN_PERIOD 10    // период антиотравления, минут


// *********************** ДЛЯ РАЗРАБОТЧИКОВ ***********************
byte BACKL_MODE = 0;                          // Выбранный эффект подсветки индикаторов, активен при запуске и меняется кнопкой 2
byte FLIP_SPEED[] = {0, 60, 50, 50, 70, 50}; // скорость эффектов, мс (количество не меняй)
byte FLIP_EFFECT_NUM = 6;    // количество эффектов
boolean GLITCH_ALLOWED = 1;                   // 1 - включить, 0 - выключить глюки. Управляется кнопкой 2


// --------- БУДИЛЬНИК ---------
int8_t ALM_TIMEOUT = 10;      // время пищания будильника в секундах
// пины
#define PIEZO 2   // пищалка
#define KEY0 3    // часы
#define KEY1 4    // часы 
#define KEY2 5    // минуты
#define KEY3 6    // минуты
#define BTN1 7    // кнопка 1
#define BTN2 8    // кнопка 2
#define GEN 9     // генератор
#define DOT 10    // точка
#define BACKL 11  // подсветка
#define BTN3 12   // кнопка 3

// дешифратор
#define DECODER0 A0
#define DECODER1 A1
#define DECODER2 A2
#define DECODER3 A3

// распиновка ламп
#if (BOARD_TYPE == 0)
const byte digitMask[] = {7, 3, 6, 4, 1, 9, 8, 0, 5, 2};   // маска дешифратора платы in12_turned (цифры нормальные)
const byte opts[] = {KEY0, KEY1, KEY2, KEY3};              // порядок индикаторов слева направо
const byte cathodeMask[] = {1, 6, 2, 7, 5, 0, 4, 9, 8, 3}; // порядок катодов in12

#elif (BOARD_TYPE == 1)
const byte digitMask[] = {2, 8, 1, 9, 6, 4, 3, 5, 0, 7};   // маска дешифратора платы in12 (цифры вверх ногами)
const byte opts[] = {KEY3, KEY2, KEY1, KEY0};              // порядок индикаторов справа налево (для IN-12 turned) и ин-14
const byte cathodeMask[] = {1, 6, 2, 7, 5, 0, 4, 9, 8, 3}; // порядок катодов in12

#elif (BOARD_TYPE == 2)
const byte digitMask[] = {9, 8, 0, 5, 4, 7, 3, 6, 2, 1};   // маска дешифратора платы in14
const byte opts[] = {KEY3, KEY2, KEY1, KEY0};              // порядок индикаторов справа налево (для IN-12 turned) и ин-14
const byte cathodeMask[] = {1, 0, 2, 9, 3, 8, 4, 7, 5, 6}; // порядок катодов in14

#elif (BOARD_TYPE == 3)
const byte digitMask[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};   // тут вводим свой порядок пинов
const byte opts[] = {KEY0, KEY1, KEY2, KEY3};              // свой порядок индикаторов
const byte cathodeMask[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9}; // и свой порядок катодов

#endif

/*
  ард ног ном
  А0  7   4
  А1  6   2
  А2  4   8
  А3  3   1
*/
