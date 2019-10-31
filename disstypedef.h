//==============================================================================
// (C) Copyright 2016-2018 ОАО «ЗИТЦ»
// г. Москва, г. Зеленоград, Россия
//
// Блок:        ДИСС-450
// Модуль:      МПР-450
// Компонент:   
// Файл:        disstypedef.h
// Назначение:  Объявления и определения типов, объявление функций.
// Примечания:  Нет
//------------------------------------------------------------------------------
// История изменений:
// версия   дата        автор       CR      описание
//------------------------------------------------------------------------------
//   1    2018.11.15  Карапетян   ----    Создание
//   2    2018.12.04  Лавриненко  ---- Добавлен макрос USE_CONSTANT_ANGLES
//   3    2018.12.04  Лавриненко  ---- Изменены структуры fpga_ctrlreg_s, 
//                                     fpga_result_struct
//   4    2018.12.04  Лавриненко  ---- Добавлена структура wdi_timer_params_s
//   5    2018.12.04  Лавриненко  ---- Добавлены коды ошибок 0xA5-0xAB
//   6    2018.12.04  Лавриненко  ---- Объявлена переменная wdiParams
//   7    2018.12.05  Лавриненко  ---- Добавлена ошибка ERR_SC_EXTBUS_WR
//   8    2019.01.15  Лавриненко  ---- Макросы M_PI, PHI, TETTA -> из algorithms.h
//   9    2019.01.17  Лавриненко  ---- Добавлены макросы ETHERNET_CODE_v1/v2
//  10    2019.01.17  Лавриненко  ---- Перенесены объявления типов из selfcontrol.h
//  11    2019.01.17  Лавриненко  ---- Глобально объявлен указатель sram_data
//  12    2019.01.17  Лавриненко  ---- Макрос TRANSITION40_80 -> из algorithms.h
//  13    2019.01.18  Лавриненко  ---- Подключение string.h перенесено из tcpip.h
//  14    2019.01.18  Лавриненко  ---- Объявление типа comp_coef_send_s перенесено
//                                     из tcpip.h
//  15    2019.01.22  Лавриненко  ---- Добавлены макросы типов пакетов Ethernet
//  16    2019.01.23  Лавриненко  ---- в tel_array_struct добавлены поля type, 
//                                     length,number,struct_cs. Выровнена по 2 байта
//  17    2019.01.23  Лавриненко  ---- в all_algorithms_data добавлены поля type, 
//                                     length,number,struct_cs. Выровнена по 2 байта
//  18    2019.01.23  Лавриненко  ---- Добавлено объявление типа packet_fpga_data_s
//  19    2019.01.23  Лавриненко  ---- Добавлено объявление переменной all_packets_cnt
//  20    2019.01.23  Лавриненко  ---- Добавлен тип telem_mode_ctrl_s
//  21    2019.02.06  Лавриненко  ---- Удалены макросы ETHERNET_CODE_v1/v2
//==============================================================================

#ifndef __DISSTYPEDEF_H__
#define __DISSTYPEDEF_H__

//------------------------------------------------------------------------------
// Включаемые файлы
//------------------------------------------------------------------------------

#include <stdint.h>
#include "string.h"

//------------------------------------------------------------------------------
// Определения макросов
//------------------------------------------------------------------------------

// Версия МПР.
#define MPR_V1          (1)
#define MPR_V2          (2)
#define MPR_V3          (3)
#define MPR_V4          (4)
#define MPR_SELECTED  MPR_V4
// Версия ПО.
#define VERS_SOFT       (11)      // Версия ПО 0...999.

// Режимы полет(40 мс)/висение(40мс).
// Работаем в нормальном режиме, переход полет-висение согласно условию.
#define NORMAL_MODE     (0)
// Боевой цикл только в режиме полет.
#define DBG_MODE40      (1)
// Боевой цикл только в режиме висение.
#define DBG_MODE80      (2)
// Выбранный режим.
#define MODE_MK_WORK    NORMAL_MODE

// Режим РК.
// РК включены.
#define RK_STATUS_OK  (getRK_All())
// РК выключены.
#define RK_STATUS_0   (0xC0)
// Игнорирование IP, IS. Остальные включены.
#define RK_STATUS_IP  (getRK_All() | 0xC0)
// Выбранный режим РК.
#define RK_GET        RK_STATUS_IP
// Внутренний тест ПЛИС
// #define FPGA_INTERN_TEST
// Режим с включенным стартовым контролем.
#define START_CONTROL_EN
// Отключить переходы ПЛИС в режим 40 мс 
#define BAN_40MS
// Расчет скоростей по трем лучам
// #define ONLY_3_BEAMS_CALC
// Номер луча, исключаемого из расчетов в режиме ONLY_3_BEAMS_CALC
#ifdef ONLY_3_BEAMS_CALC
  #define BAD_BEAM_NUM (1)
#endif
// Включить в режиме телеметрии ручную подстройку индекса модуляции.
// #define CNANGE_IND_MOD_IN_TELEM_MODE
// Работа без СВЧМ.
// #define WITHOUT_SHF         // [COMMENT]
// Работа без ошибок пониженных шумов
#define WITHOUT_SVCHMLOWNOISE   // [COMMENT]
// Работа без UART.
#define WITHOUT_UART        // [COMMENT]
// Работа без сторежевых таймеров.
// #define WITHOUT_WDT         // [COMMENT]
// Работа без температурного датчика.
// #define WITHOUT_TEMP_EXT    // [COMMENT]
// Подключает пины для отладки.
// #define EN_TESTING_PINS     // [COMMENT]
// Использовение идеальных коэффициентов АЧХ.
#define USE_HARDCODE_COEFS  // [COMMENT]
// Расчет скоростей при помощи констант (если флеш не прошита)
// #define USE_CONSTANT_ANGLES // [COMMENT]

#define DO_MONITORING_FPGA_COUNT  (400 - 1)    // 16 секунд
#define TRANSITION40_80         (6)   // В переменную transition40_80 будет установлен флаг смещение. Либо 1, либо 2.


#ifndef __MINGW32__
#define M_PI                (3.14159265358979323846)    // PI
#endif  // ! __MINGW32__
#define PHI_DEGREE          (11.5)
#define TETTA_DEGREE        (16.5)

#define PHI                 (PHI_DEGREE * M_PI / 180)
#define TETTA               (TETTA_DEGREE * M_PI / 180)

//#define PHI                 (0.22689280275926285)   // 13 град. - угол положения луча
//#define TETTA               (0.29670597283903605)   // 17 град. - угол положения луча

// Количесто точек калибровки с включенным обогревом
#define ADC_COUNT_WITH_HEAT                 (6)
// Количесто точек калибровки с выключенным обогревом.
#define ADC_COUNT_WITHOUT_HEAT              (12)
// Количество калибровачных данных температурных датчиков.
#define ADC_COUNT                           (ADC_COUNT_WITH_HEAT + ADC_COUNT_WITHOUT_HEAT)
// Количество калибровачных данных температурных датчиков вместе с контрольной суммой.
#define ADC_CRC_COUNT                       (ADC_COUNT + 1)
// Количество калибровачных данных для ИМ.
#define IM_COUNT                            (15)
// Количество калибровачных данных для ИМ вместе с контрольной суммой.
#define IM_CRC_COUNT                        (IM_COUNT + 1)
// Резмер данных
#define ADC_SIZE                            (2)

// FLASH ADDRESS
// Адрес начала ЖО.
#define LOG_ADDR                            (0)
// Количество записей ЖО.
#define LOG_COUNT                           (1000)
// Размер одной записи ЖО.
#define LOG_SIZE                            (8)
// Адрес начала области времени наработки.
#define TIME_ADDR                           (LOG_ADDR + LOG_COUNT * LOG_SIZE)
// Количество записей времени наработки.
#define TIME_COUNT                          (7)
// Адрес начала области углов.
#define ANGLE_ADDR                          (TIME_ADDR + TIME_COUNT * LOG_SIZE)
// Количество углов.
#define ANGLE_COUNT                         (4)
// Размер одной записи углов.
#define ANGLE_SIZE                          (4)
// SENSORS_ADC_ADDR inT, exT, indexModul
// Адрес начала области калибровачных данных температур и ИМ.
#define SENSORS_ADDR                        (ANGLE_ADDR + ANGLE_COUNT * ANGLE_SIZE)
// Количество калибровачных данных температур и ИМ.
#define SENSORS_COUNT                       (2 * ADC_CRC_COUNT + IM_CRC_COUNT)
// Размер одной записи для калибровачных данных температур и ИМ.
#define SENSORS_SIZE                        (ADC_SIZE)
// Заводской номер, REFDAC1, REFDAC2.
// Адрес начала области различных данных. Эта область должна находится последней, тк
// используется для подсчета USE_TOTAL_FLASH_SIZE.
#define SOME_DATA_ADDR                      (SENSORS_ADDR +  SENSORS_COUNT * SENSORS_SIZE)
// Количество различных данных.
#define SOME_DATA_COUNT                     (5)
// Размер данных.
#define SOME_DATA_SIZE                      (2)
// Всего используемых данных
#define USE_TOTAL_FLASH_SIZE                (SOME_DATA_ADDR + SOME_DATA_COUNT * SOME_DATA_SIZE)
// Размер флешки
#define TOTAL_FLASH_SIZE                    (1024 * 8)

#if (USE_TOTAL_FLASH_SIZE >= TOTAL_FLASH_SIZE)
  #error "Flash size overflow"
#endif


// Нижняя граница температуры калибровачных данных.
#define CALIBR_START                (-60)
// Верхняя граница температуры калибровачных данных.
#define CALIBR_END                  (80)
// Шаг калибровачных данных.
#define CALIBR_STEP                 (10.0)
// Нижняя граница температуры калибровачных данных с обогревом.
#define CALIBR_START_WITH_HEAT      (CALIBR_START)
// Верхняя граница температуры калибровачных данных с обогревом.
#define CALIBR_START_WITHOUT_HEAT   (-30)
// Нижняя граница температуры калибровачных данных без обогрева.
#define CALIBR_END_WITH_HEAT        (CALIBR_START_WITH_HEAT + CALIBR_STEP * (ADC_COUNT_WITH_HEAT - 1))
// Верхняя граница температуры калибровачных данных без обогрева.
#define CALIBR_END_WITHOUT_HEAT     (CALIBR_START_WITHOUT_HEAT + CALIBR_STEP * (ADC_COUNT_WITHOUT_HEAT - 1))

// Начало первого луча.
#define N_RAY_START      (0xFFFF)

//  TX
// Количество адресов передоваемых сообщений.
#define ARINC_TX_ADDRESS_CNT                    (15)

// Адрес времени наработки.
#define ARINC_ADDRESS_TIME                      (06)
// Адрес версии ПО МПР.
#define ARINC_ADDRESS_VERS_SOFT                 (07)
// Адрес версии ПО СВЧМ.
#define ARINC_ADDRESS_VERS_SHF                  (012)
// Адрес версии ПО НИОС.
#define ARINC_ADDRESS_VERS_NIOS                 (015)
// Адрес младших байт CRC МК.
#define ARINC_ADDRESS_CRC_MK_0                  (010)
// Адрес старших байт CRC МК.
#define ARINC_ADDRESS_CRC_MK_1                  (011)
// Адрес младших байт CRC СВЧМ.
#define ARINC_ADDRESS_CRC_SHF_0                 (013)
// Адрес старших байт CRC СВЧМ.
#define ARINC_ADDRESS_CRC_SHF_1                 (014)
// Адрес младших байт CRC НИОС.
#define ARINC_ADDRESS_CRC_NIOS_0                (016)
// Адрес старших байт CRC НИОС.
#define ARINC_ADDRESS_CRC_NIOS_1                (017)
// Адрес скорости X.
#define ARINC_ADDRESS_W_X                       (0260)
// Адрес скорости Z.
#define ARINC_ADDRESS_W_Z                       (0261)
// Адрес скорости Y.
#define ARINC_ADDRESS_W_Y                       (0262) // вертикалка. Сейчас это Z
// Адрес режима работы.
#define ARINC_ADDRESS_MODE                      (0272)
// Адрес статуса состояния.
#define ARINC_ADDRESS_STATUS                    (0356)

//  RX
// Количество адресов принимаемых сообщений
#define ARINC_RX_ADDRESS_CNT                    (4)

// Адрес Q.
#define ARINC_ADDRESS_Q                         (0314)    // 0xCC
// Адрес P.
#define ARINC_ADDRESS_P                         (0324)    // 0xD4
// Адрес R.
#define ARINC_ADDRESS_R                         (0325)    // 0xD5
// Адрес комадного слова.
#define ARINC_ADDRESS_COMAND                    (0371)    // 0xF9

// Размер массива принятых сообщений отправляемых в телеметрию.
#define ARINC_TELEM_ARRAY_SIZE    (4)

// Корректная обработка технологической команды.
#define PANEL_DONE                       (0x55)
// Некорретная обработка технологической команды.
#define PANEL_ERROR                      (0x33)

// Начальная область памяти SRAM
#define ADDRESS_SRAM_START                 (0x60000000)
// Конечная область памяти SRAM
#define ADDRESS_SRAM_END                   (0x60080000)

// Типы пакетов Ethernet в режиме телеметрии
#define TEL_UDP_TYPE_INIT       (0x0)
#define TEL_UDP_TYPE_ARINC      (0x1)
#define TEL_UDP_TYPE_MATH       (0x2)
#define TEL_UDP_TYPE_FAIL       (0x3)
#define TEL_UDP_TYPE_FPGADATA0  (0x8)
#define TEL_UDP_TYPE_FPGADATA1  (0x9)
#define TEL_UDP_TYPE_FPGADATA2  (0xA)
#define TEL_UDP_TYPE_FPGADATA3  (0xB)
#define TEL_UDP_TYPE_FPGADATA4  (0xC)
#define TEL_UDP_TYPE_FPGADATA5  (0xD)
#define TEL_UDP_TYPE_FPGADATA6  (0xE)
#define TEL_UDP_TYPE_FPGADATA7  (0xF)

// Таймаут для эхо-контроля выходных РК (в мс)
#define ECHO_RKOUT_TIMEOUT      (5)

//------------------------------------------------------------------------------
// Объявления и определения типов
//------------------------------------------------------------------------------

// Структура входных РК.
typedef struct {
  // РК молчание.
  uint8_t silence       : 1;
  // РК море.
  uint8_t sea           : 1;
  // РК запрет расширенного контроля.
  uint8_t advControlBan : 1;
  // РК направление вертикальной состовляющей скоростей.
  uint8_t vDir          : 1;
  // РК обновление информации 120 мс.
  uint8_t update120     : 1;
  // РК режим управления РК.
  uint8_t klsRk         : 1;
  // РК IP.
  uint8_t noIp          : 1;
  // РК IS.
  uint8_t noIs          : 1;
} RkInTypeDef;

// Объединение входных РК.
union RkInUnion {
  uint8_t v;            // value
  RkInTypeDef f;        // fields
};

// Структура выходных РК.
#pragma pack(push, 1)
typedef struct {
  // РК отказ.
  uint8_t fail          : 1;
  // РК память.
  uint8_t mem           : 1;
  // РК море.
  uint8_t sea           : 1;
  // РК молчание.
  uint8_t silence       : 1;
  // резерв.
  uint8_t _4_7bit       : 4;
} RkOutTypeDef;
#pragma pack(pop)

// Объединение выходных РК.
#pragma pack(push, 1)
union RkOutUnion {
  uint8_t v;            // value
  RkOutTypeDef f;       // fields
};
#pragma pack(pop)

// Формат режим работы блока. В сообщение смещать на 8.
typedef struct {
  uint8_t _0_1bit       : 2;
  uint8_t fail          : 1;
  uint8_t missInf       : 1;
  uint8_t silence       : 1;
  uint8_t failArinc     : 1;
  uint8_t _6bit         : 1;
  uint8_t memary        : 1;
  uint8_t none8bit      : 1;
  uint8_t sea           : 1;
  uint8_t speedZero     : 1;
  uint8_t _11_15bit     : 5;
} ModeTypeDef;

// Формат режим работы блока. В сообщение смещать на 8.
union ModeUnion {
  uint16_t v;           // value
  ModeTypeDef f;        // fields
};

// Формат состояния блока. В сообщении смещать на 8.
typedef struct {
  uint8_t _0_2bit       : 3;
  uint8_t ioErr         : 1;
  uint8_t powerErr      : 1;
  uint8_t _5bit         : 1;
  uint8_t interfaceErr  : 1;
  uint8_t _7bit         : 1;
} StatusTypeDef;

// Формат состояния блока. В сообщении смещать на 8.
union StatusUnion {
  uint8_t v;            // value
  StatusTypeDef f;      // fields
};

// Формат принятого командного слова.
typedef struct {
  uint8_t _0_1bit       : 2;
  uint8_t silence       : 1;
  uint8_t _3_4bit       : 2;
  uint8_t sea           : 1;
  uint8_t advControl    : 1;
  uint8_t time          : 1;
  uint8_t speedOut1_100 : 1;
  uint8_t speedOut2_100 : 1;
  uint8_t speedIn1_12_5 : 1;
  uint8_t speedIn2_100  : 1;
  uint8_t techTaTb_100  : 1;
  uint8_t _12_15bit     : 3;
} ComandTypeDef;

// Формат принятого командного слова.
union ComandUnion {
  uint16_t v;           // value
  ComandTypeDef f;      // fields
};

// Статус СВЧМ
enum {  // 2 бит. Максимум 3.
  SHF_STATUS_UNKNOWN,         // 0
  SHF_STATUS_OFF,             // 1
  SHF_STATUS_ON,              // 2
  SHF_STATUS_RESERVE
};

// Состояния схемы нагрева МПР-450
typedef enum {
    HEAT_UNKNOWN,   // Неопределенное состояние
    HEAT_ON,        // Схема нагрева включена
    HEAT_OFF,       // Схема нагрева выключена
} HeatStateEnum;

// Указывает местонахождение программы.
enum {  // 4 бит. Максимум 0xF.
  // Начальная инициализация.
  START_INIT_MPR,             // 0
  // Стартовый контроль.
  START_MONITORING0,          // 1
  // Расширенный контроль.
  START_MONITORING1,          // 2
  // После завершения стартового контроля.
  END_MONITORING0,            // 3
  // После завершения расширенного контроля.
  END_MONITORING1,            // 4
  // Резерв.
  PROGRAMM_STATE_RESERVE1,
  // Резерв.
  PROGRAMM_STATE_RESERVE2,
  // Начала математики в основном цикле.
  CYCLE_STATE_START_MATH,     // 7
  // Конец математики в основном цикле.
  CYCLE_STATE_END_MATH,       // 8
  // Начало основного цикла
  MAIN_CYCLE_START,           // 9
};

// Структура статуса МПР записывается в ЖО.
#pragma pack(push, 1)
typedef struct {
  // 0-1 bit - переполнение времени наработки.
  uint8_t timeOverflow      : 2;
  // 2-5 bit - указывает местонахождение программы.
  uint8_t programmState     : 4;
  // 6   bit - перенагрев "TEMPERATURE_OVERHEATING и более".
  uint8_t isOverheating     : 1;
  // 7   bit - переохлаждение "TEMPERATURE_OVERCOOLING и более".
  uint8_t isOvercooling     : 1;
} StatusLogTypeDef;
#pragma pack(pop)

// Структура статуса МПР.
#pragma pack(push, 1)
union StatusLogUnion {
  uint8_t v;
  StatusLogTypeDef f;
};
#pragma pack(pop)

// Структура записи ЖО и времени наработки.
#pragma pack(push, 1)
typedef struct {
  // Время наработки.
  uint32_t total40ms;
  // Количество включений.
  uint8_t totalOn;
  // Статус МПР.
  union StatusLogUnion status;
  // Код ошибки.
  uint8_t error;
  // Контрольная сумма.
  uint8_t CS;
} LogErrorStructTypeDef;
#pragma pack(pop)

// Структура ответа по технологической команде PANEL_LAST_LOG_GET.
#pragma pack(push, 1)
typedef struct {
  LogErrorStructTypeDef log;
  struct {
    // Хранит код ошибки флеш.
    uint8_t fpgaFlashError  : 4;
    // Хранит из какой флеш ф-ции произошла ошибка.
    uint8_t fromFunc        : 4;
  } ethData;
} EthLogErrorStructTypeDef;
#pragma pack(pop)

// Структура трехмерной точки.
#pragma pack(push, 1)
typedef struct {
  double x, y, z;   //  надо было написать x, z, y
} Point3;
#pragma pack(pop)

// Объединение трехмерной точки.
union Point3Union {
  double v[3];
  Point3 f;
};

// Калибровочные данные температурных датчиков.
typedef struct {
  uint16_t withHeat[ADC_COUNT_WITH_HEAT];
  uint16_t withoutHeat[ADC_COUNT_WITHOUT_HEAT];
} CalibrTemp;

// Калибровачные данные температурных датчиков и ИМ.
union TempUnion {
  uint16_t v[2 * ADC_COUNT + IM_COUNT];
  struct {
    CalibrTemp intT;
    CalibrTemp extT;
    uint16_t iMod[IM_COUNT];
  } f;
};

// Число с флагом.
typedef struct {
  uint8_t count : 7;
  uint8_t fl : 1;
} fl_count;

// Время наработки с флагом.
#pragma pack(push, 1)
typedef struct {
  uint32_t total40ms;
  uint8_t flTime;
} DataTimeStructTypeDef;
#pragma pack(pop)

// Время наработки с счетчиком таймера.
typedef struct {
  // Время наработки.
  uint32_t total40ms;
  // Счетчик таймера.
  uint32_t cnt;
} DataTimeCntTypeDef;

// Сообщение ARINC со временем.
typedef struct {
  uint32_t mess;
  DataTimeCntTypeDef time;
} ArincMessTypeDef;

// Структура всех выходных ARINC сообщений.
union ArincTxUnion {
  uint32_t v[ARINC_TX_ADDRESS_CNT];
  struct {
    uint32_t time;
    uint32_t vers_soft;
    uint32_t vers_nios;
    uint32_t vers_shf;
    uint32_t crc_mk0;
    uint32_t crc_mk1;
    uint32_t crc_nios0;
    uint32_t crc_nios1;
    uint32_t crc_shf0;
    uint32_t crc_shf1;
    uint32_t wx;
    uint32_t wz;
    uint32_t wy;
    uint32_t mode;
    uint32_t status;
  } f;
};

// Структура флагов калибровки.
union CalibratedFlagsUnion {
  struct {
    // Флаг калибровки углов.
    uint8_t angle       : 1;
    // Флаг калибровки температурного датчика МК.
    uint8_t sensMcu     : 1;
    // Флаг калибровки температурного датчика платы.
    uint8_t sensBoard   : 1;
    // Флаг калибровки индекса модуляции.
    uint8_t indexModel  : 1;
    // Флаг установки серийного номера.
    uint8_t sn          : 1;
    // Флаг калибровки REFDAC.
    uint8_t refdac      : 1;
    // Флаг калибровки СВЧМ.
    uint8_t shf         : 1;
    // Флаг резерв.
    uint8_t configure   : 1;
    // Флаг резерв.
    uint8_t reserve2    : 1;
    // Флаг резерв.
    uint8_t reserve3    : 1;
    // Флаг резерв.
    uint8_t reserve4    : 1;
    // Флаг резерв.
    uint8_t reserve5    : 1;
    // Не используется, тк во флеш хранится 12-битное слово
    // и контрольная сумма.
    uint8_t _12_15bits  : 4;
  } f;
  uint16_t v;
};

// Структура флагов калибровки.
union ConfigureUnion {
  struct {
    uint8_t adcIqMuxSel : 3;
    // Флаг резерв.
    uint16_t reserve    : 9;
    // Не используется, тк во флеш хранится 12-битное слово
    // и контрольная сумма.
    uint8_t _12_15bits  : 4;
  } f;
  uint16_t v;
};

// Объединение с различными флагами.
#pragma pack(push, 1)
union FlagsUnion{
#ifdef __MINGW32__
  struct __attribute__((gcc_struct, packed)) {
#else   // !__MINGW32__
  struct {
#endif  // !__MINGW32__
    // Код ошибки.
    uint16_t logError           : 8;
    // Код перехода в память.
    uint16_t logMem             : 8;
    // Флаг события перехода в отказ.
    uint16_t errorEvent         : 1;
    // Флаг завершения записи в ЖО ошибки.
    uint16_t errorWriteDone     : 1;
    // Флаг необходимости записи в ЖО перехода в память.
    uint16_t logMemEvent        : 1;
    // Флаг необходимости времени наработки во флеш.
    uint16_t timeTotalEvent     : 1;
    // Флаг состояния СВЧМ. 0 - неизв, 1 - выкл, 2 - вкл.
    uint16_t shfStatus          : 2;
    // Флаг необходимости проверить состояния CTRL при выключении СВЧМ.
    uint16_t controlOffShfEvent : 1;
    // Флаг изменения значений выходных РК.
    uint16_t echoRkEvent        : 1;
    // Флаг попытка захода в контроль.
    uint16_t tryToMon           : 2;
    // Флаг записи байта.
    uint8_t writeByte           : 1;
    // Флаг записи страницы.
    uint8_t writePage           : 1;
    // Флаг обогрева. 1 - обогрев вЫключен.
    uint8_t tofflv              : 1;
    // Схема обогрева.
    uint8_t heatSchemeState     : 2;
    // Флаг выхода в боевой по команде PANEL_MAIN_MODE_SET.
    uint8_t toMain              : 1;
  } f;
  uint32_t v;
};

// Флаги для технологического боевого режима.
union TechModeUnion {
  uint8_t v;
  struct {
    // 0 bit - пропуск стартового контроля.
    uint8_t missStartControl    : 1;
    // 1 bit - пропуск ведение журнала.
    uint8_t missLog             : 1;
    // 2 bit - запрет смены режима 40 80.
    uint8_t banChange40_80      : 1;
    // 3 bit - запрет смены ИД.
    uint8_t banChangeIm         : 1;
    // 4 bit - вызвать resetMk.
    uint8_t resetMK             : 1;
    // 5 bit - запрет использования поправок углов.
    uint8_t banUseDAngles       : 1;
    // 6 bit - запрет перехода ПЛИС в 40мс.
    uint8_t banFpga40ms         : 1;
    // 7 bit - резерв.
    uint8_t startFpgaIn40       : 1;
  } f;
};
#pragma pack(pop)

// Стуктура статуса МПР.
#pragma pack(push, 2)
typedef struct{
  union {
    struct {
      uint8_t comand;
      uint8_t status;
    } panel;
    struct {
      // Тип пакета
    uint16_t type   : 4;
    // Длина пакета
    uint16_t length : 12;
    } tech;
  } title;
  // Номер пакета (сквозной)
  uint32_t number : 32;
  // Q по могли учавствовать в подсчете скорости.
  ArincMessTypeDef mathQ;   // 12 byte
  // P по могли учавствовать в подсчете скорости.
  ArincMessTypeDef mathP;
  // R по могли учавствовать в подсчете скорости.
  ArincMessTypeDef mathR;
  // Комадное слово.
  ArincMessTypeDef lastComand;
  // Выходные сообщения ARINC.
  union ArincTxUnion arincTx;
  // Время последнего прерывания ПЛИС.
  DataTimeCntTypeDef timeFpga;
  // Входные РК.
  union RkInUnion rkIn;
  // Номер луча.
  uint16_t n_ray;
  // Выходные РК.
  union RkOutUnion rkOut;
  // Значение АЦП для температурного датчика МК.
  uint16_t countAdcTemperInt;
  // Значение АЦП для температурного датчика платы.
  uint16_t countAdcTemperExt;
  // Температура МК датчика * 10.
  int16_t temperInt;
  // Температура датчика платы * 10.
  int16_t temperExt;
  // Массив последних принятых углов Q.
  ArincMessTypeDef arrayQ[ARINC_TELEM_ARRAY_SIZE];
  // Количество принятых углов Q.
  uint8_t cntQ;
  // Массив последних принятых углов P.
  ArincMessTypeDef arrayP[ARINC_TELEM_ARRAY_SIZE];
  // Количество принятых углов P.
  uint8_t cntP;
  // Массив последних принятых углов R.
  ArincMessTypeDef arrayR[ARINC_TELEM_ARRAY_SIZE];
  // Количество принятых углов R.
  uint8_t cntR;
  // Массив последних принятых комадных слов.
  ArincMessTypeDef arrayComand[ARINC_TELEM_ARRAY_SIZE];
  // Количество принятых комадных слов.
  uint8_t cntComand;
  // Количество других принятых сообщений.
  uint8_t cntOther;       // must be zero
  // Количество возникновений ошибок на шине по каналу 1.
  uint8_t cntErrRxArinc1; // must be zero
  // Количество возникновений ошибок на шине по каналу 2.
  uint8_t cntErrRxArinc2; // must be zero
  // Время отправки сообщений ARINC.
  uint32_t tp4_arinc_send;
  // Флаги калибровки.
  union CalibratedFlagsUnion calibrated;
  // Флаги работы алгоритма.
  union FlagsUnion flags;
  // Флаги для технологического-боевого режима.
  union TechModeUnion techMode;
  // Статус МПР для записи в ЖО.
  union StatusLogUnion statusLog;
  // Последняя запись в ЖО с полем для технологической команды
  EthLogErrorStructTypeDef lastLog;   // последняя запись в ЖО
  // Для отладки.
  uint32_t forDebug[2];
  // Контрольная сумма структуры
  uint16_t struct_cs : 16;
} tel_array_struct;
#pragma pack(pop)

// Скорочти в формате отправки по ARINC.
typedef struct {
    uint32_t x, y, z;
} Point3_protocol;

// Структура данных алгоритмов селекции часторы доплера и вычисления вектора скорости
#pragma pack(push, 1)
typedef struct{
    uint64_t max_spectr_mid_smooth_val[4];  // Значения максимума среднего сглаженного спектра (мссс)
    uint16_t max_spectr_mid_index[4];       // Номер максимума среднего сглаженного спектра
    uint16_t max_spectr_mid_index_done[4];  // Индекс максимума среднего сглаженного спектра
    uint8_t beam_spectr_mid_mode[4];        // Флаги превышения порога мссс [0..1023]
    uint8_t spectr_cnt[4];                  // Номер спектра по каждуму лучу
    uint8_t accum_full[4];                  // Флаг того, что как минимум 8 спектров уже накопилось
    uint8_t bad_b_num;                      // Номер "плохого" луча по критерию минимального из мссс по всем лучам
    uint64_t spectr_mid_mean;               // Среднее значение среднего сглаженного спектра
    uint8_t beam_mode_3;                    // Количество лучей в состоянии "Слежение"
    uint8_t near_zone_beams;                // Количество лучей в "ближней" зоне
    uint8_t stoppage_mode;                  // Индикатор режима "остановки"
    double Fdopler[4];                      // Частоты доплера по 4-м лучам
    Point3 W;                               // Вектор скорости
    uint16_t max_otschet[4];                // Номер отсчета максимума спектра по каждому из лучей
    uint64_t noise_level[4];                // Уровень шума текущего спектра по каждому из лучей
    uint64_t noise_level_mid[4];            // Вспомогательный массив
    uint64_t sig_level[4];                  // Уровень сигнала (числитель из формулы ЦТ)
    uint8_t over_noise_thres_cnt[4];        // Счетчик превышений верхнего порога по уровню шума
    uint8_t under_noise_thres_cnt[4];       // Счетчик НЕпревышений нижнего порога по уровню шума
    uint32_t cs_sram_rd_cur[8][4];          // Контрольные суммы 8ми последних спектров по каждому из 4 лучей при чтении из SRAM
    uint32_t cs_sram_wr_cur[8][4];          // Контрольные суммы 8ми последних спектров по каждому из 4 лучей при записи в SRAM
    uint32_t cs_sram_rd_ct[8][4];           // Контрольные суммы 8ми последних спектров по каждому из 4 лучей при вычислении ЦТ
    uint32_t cs_sram_rd_mid[4];             // Контрольные суммы среднего спектра по каждому из 4 лучей при чтении из SRAM
    uint32_t cs_sram_wr_mid[4];             // Контрольные суммы среднего спектра по каждому из 4 лучей при записи в SRAM
    uint8_t cs_sram_status;                 // Тип несовпадения контрольных сумм
} accum_result_struct;
#pragma pack(pop)

// Регистр статуса ПЛИС
#pragma pack(push, 1)
typedef struct {
    uint8_t statusRegCS         : 4; // [15:12] - контрольная сумма регистра
    uint8_t pll2Status          : 1; // [11]    - состояние PLL1 LOCKED (должно быть 1)
    uint8_t pll1Status          : 1; // [10]    - состояние PLL1 LOCKED (должно быть 1)
    uint8_t resvBit0            : 1; // [9]     - резерв 
    uint8_t resvBit1            : 1; // [8]     - резерв
    uint8_t flErrorFifoCounter  : 1; // [7]     - флаг ошибки счетчиков FIFO в Арбитре
    uint8_t ADCInitError        : 1; // [6]     - инициализация АЦП не пройдена
    uint8_t flResetAfterMCUbyMF : 1; // [5]     - флаг того, ПЛИС проснулась после сброса по инициативе МК (по линии MF[1])
    uint8_t flResetAfterExtNWDO : 1; // [4]     - флаг того, что был выработан импульс nWDO внешнего СТ
    uint8_t flResetAfterExtNWDR : 1; // [3]     - флаг того, что был выработан импульс nWDR внешнего СТ по ошибке питания 5В
    uint8_t flResetAfterIntNWDO : 1; // [2]     - флаг того, что был выработан импульс nWDO внутреннего СТ в ПЛИС
    uint8_t flErrPll2           : 1; // [1]     - флаг того, что было падение сигнала PLL_LOCKED для CLK12M1
    uint8_t flErrPll1           : 1; // [0]     - флаг того, что было падение сигнала PLL_LOCKED для CLK12M2
} fpga_main_statusreg_s;
#pragma pack(pop)
// Регистр статуса СВЧМ

#pragma pack(push, 1)
typedef struct {
    uint8_t statusRegCS         : 4; // [15:12] - контрольная сумма регистра
    uint8_t resvBit0            : 1; // [11]    - резерв 
    uint8_t resvBit1            : 1; // [10]    - резерв    
    uint8_t resvBit2            : 1; // [9]     - резерв 
    uint8_t flUartEmpty         : 1; // [8]     - флаг отсутствия пакетов по UART
    uint8_t vukState            : 2; // [7:6]   - состояние сигналов {VUK1, VUK0}
    uint8_t uartProtocolStatus  : 2; // [5:4]   - статус контроллера протокола СВЧМ-МПР
    uint8_t EN_HFS              : 1; // [3]     - признак включения приемника (для проверки того, что ПЛИС выставила его)
    uint8_t EN_TEST             : 1; // [2]     - признак включения приемника, передатчика, генератора (-//-)
    uint8_t CNTRL2_AP           : 1; // [1]     - наличие мощности усилителя
    uint8_t CNTRL3_CL           : 1; // [0]     - превышение тока
} svchm_statusreg_s;
#pragma pack(pop)

// Регистр счетчика реального времени
#pragma pack(push, 1)
typedef struct {
    uint8_t statusRegCSh        : 4; // r1[15:12] - контрольная сумма регистра
    uint8_t statusRegCSl        : 4; // r0[15:12] - контрольная сумма регистра
    uint32_t rt_counter_ms      : 32; // r1[11:0], r2[11:0]  - к-во разрядов амплитуды полезного сигнала СГТС
} rt_counter_ms_s;
#pragma pack(pop)


// Регистр статуса схемы ЦОС
#pragma pack(push, 1)
typedef struct {
    uint16_t statusRegCS         : 4; // [15:12] - контрольная сумма регистра
    uint16_t scale               : 8; // [11]    - параметр масштабирования полученного результата
    uint16_t b_num               : 2; // [10]    - номер луча, для которого обработан полученный результат
    uint16_t ms40                : 1; // [9]     - Режим (40мс или 80мс), в котором обработан полученный результат
    uint16_t startgo             : 1; // [8]     - индикатор запуска алгоритма ЦОС
} dsp_statusreg_s;
#pragma pack(pop)

// Регистр статуса СГТС
#pragma pack(push, 1)
typedef struct {
    uint8_t statusRegCS1        : 4; // r1[15:12] - контрольная сумма регистра
    uint8_t statusRegCS0        : 4; // r0[15:12] - контрольная сумма регистра
    uint8_t test_signal_amp     : 4; // r1[11:8]  - к-во разрядов амплитуды полезного сигнала СГТС
    uint8_t test_noise_amp      : 4; // r1[7:4]   - к-во разрядов амплитуды шума сигнала СГТС
    int16_t test_nco_df         : 16; // r0[11:0], r1[3:0] - параметр сдвига частоты в Гц
} sgts_statusreg_s;
#pragma pack(pop)

// Регистр состояний линий MF-FM
#pragma pack(push, 1)
typedef struct {
    uint8_t statusRegCS         : 4; // [15:12] - контрольная сумма регистра
    uint8_t JM_NRST_B           : 1; // [11]    - резерв 
    uint8_t TMODE               : 1; // [10]    - резерв    
    uint8_t READY               : 1; // [9]     - резерв 
    uint8_t FM                  : 4; // [8:5]   - состояния линий FM[4:1]
    uint8_t MF                  : 4; // [3:0]   - состояния линий MF[3:0]
} mffm_statusreg_s;
#pragma pack(pop)

// Регистр статуса схемы ШИМ
#pragma pack(push, 1)
typedef struct {
    uint16_t statusRegCS        : 4; // [15:12] - контрольная сумма регистра
    uint16_t pwm_sine_scale     : 12; // [11:0]   - размах амплитуды модулирующего сигнала
} pwmsine_statusreg_s;
#pragma pack(pop)

// Регистр статуса flash-контроллера
#pragma pack(push, 1)
typedef struct {
    uint8_t statusRegCS0        : 4; // [15:12] - контрольная сумма регистра
    uint8_t statusRegCS1        : 4; // [15:12] - контрольная сумма регистра
    uint8_t missed_reads        : 1; // [11] - флаг пропущенных байтов при чтении в МК
    uint8_t check_wr_page       : 1; // [10] - флаг успешного контроля записи страници ее чтением
    uint8_t check_wr_byte       : 1; // [9] - флаг успешного контроля записи байта его чтением
    uint8_t error_op            : 1; // [4] - флаг ошибки транзакции
    uint8_t busy                : 1; // [3] - флаг недоступности контроллера (busy)
    uint8_t CMD                 : 3; // [2:0] - команда-тип транзакции
    uint8_t error_state         : 4; // [8:5] - состояние контроллера PHY-уровня, в котором произошла ошибка
    uint8_t ctrl_state          : 4; // [11:8]  - текущее состояние flash_controller
    uint8_t rd_byte             : 8; // [7:0] - прочитанныый байт из ПЗУ
} flash_statusreg_s;
#pragma pack(pop)

// Регистр управления ПЛИС
#pragma pack(push, 1)
typedef struct {
  uint8_t WD_EXT_MASK     : 1; // FPGA_CTRLREG0[15] - Маска сторожевого таймера (при WD_MASK = 1 отключается сброс от СТ)
  uint8_t IRQ_DISABLE     : 1; // FPGA_CTRLREG0[14] - выключение прерываний от ПЛИС
  uint8_t MANUAL_FIX      : 1; // FPGA_CTRLREG0[13] - зафиксировать группу регистров не дожидаясь прерывания от ПЛИС
  uint8_t RESET           : 1; // FPGA_CTRLREG0[12] - Синхронный сброс всей логики ЛИС, кроме внешней шины
  uint8_t BEAM            : 2; // FPGA_CTRLREG0[11:10] - номер луча (принудительно заданный/текущий для контроля)
  uint8_t BEAM_EN         : 1; // FPGA_CTRLREG0[9] - Сигнал разрешения установки принудительно заданного номера луча
  uint8_t EN_TEST         : 1; // FPGA_CTRLREG0[8] - Дискретный сигнал к СВЧМ "включение приемника"
  uint8_t EN_HFS          : 1; // FPGA_CTRLREG0[7] - Дискретный сигнал к СВЧМ "включение передатчика, приемника, генератора"
  uint8_t RESV2           : 1; // FPGA_CTRLREG0[6] - Дискретный сигнал к СВЧМ RESV2
  uint8_t RESV1           : 1; // FPGA_CTRLREG0[5] - Дискретный сигнал к СВЧМ RESV1
  uint8_t resvBit1        : 1; // FPGA_CTRLREG0[4] - резерв
  uint8_t SW_SGTS_ADC     : 1; // FPGA_CTRLREG0[3] - Переключение исходного сигнала для схемы ЦОС
  uint8_t ADC_INIT_EN     : 1; // FPGA_CTRLREG0[2] - включение схемы инициализации АЦП
  uint8_t WD_INT_MASK     : 1; // FPGA_CTRLREG0[1] - Сброс контроллера АЦП (состояние сброса - "0")
  uint8_t WD_EXT_START    : 1; // FPGA_CTRLREG0[0] - резерв
  uint8_t IRQ_CYCLES      : 8; // IRQ_CYCLES[7:0] - Длительность прерывания от плис в тактах частоты 80 МГц
} fpga_ctrlreg_s;
#pragma pack(pop)

// Регистры инициализации параметров АЦП
#pragma pack(push, 1)
#ifdef __MINGW32__
typedef struct __attribute__((gcc_struct, packed)) {
#else   // !__MINGW32__
typedef struct {
#endif  // !__MINGW32__
  uint16_t ADC_CONTROL_CMD    : 12; // ADC_CONTROL_CMD[15:4] 
  uint16_t ADC_REFDAC1_CMD    : 10; // REFDAC1[9:0]
  uint16_t ADC_REFDAC2_CMD    : 10; // REFDAC2[9:0]
  uint16_t CIC_DELAY_US       : 12; // ADC_CTRLREG[15:4] - Задержка для вырезания переходного процесса на входе CIC
  uint16_t REIN_MUX_SEL       : 4;  // ADC_CTRLREG[3:0] - Сигнал выбора комбинации сигналов {I, Q} на вход схемы ЦОС
} adc_init_s;
#pragma pack(pop)

// Регистр управления схемой ЦОС
#pragma pack(push, 1)
typedef struct {
  uint8_t resvBits0_7     : 8; // DSP_CTRLREG[15:8] - резерв
  uint8_t resvBit8        : 1; // DSP_CTRLREG[7] - резерв
  uint8_t MS80            : 1; // DSP_CTRLREG[6] - Режим накопления сигнала схемой ЦОС (80/40 мс)
  uint8_t HARD_STOP_EN    : 1; // DSP_CTRLREG[5] - Разрешение остановки алгоритма сразу после того, как пройдет только один цикл работы алгоритма по всем четырем лучам 
  uint8_t BEAM_NUM_IN     : 2; // DSP_CTRLREG[4:3] - Номер луча, с которого начнется следующий такт обработки при условии активного сигнала BEAM_CHANGE (BEAM_CHANGE = 1)
  uint8_t BEAM_CHANGE     : 1; // DSP_CTRLREG[2] - Разрешение принудительной смены номера луча на значение BEAM_NUM_IN, с которого начнется следующий такт обработки
  uint8_t STOP            : 1; // DSP_CTRLREG[1] - Остановка работы схемы ЦОС (по переднему фронту сигнала)
  uint8_t START           : 1; // DSP_CTRLREG[0] - Старт работы схемы ЦОС (по переднему фронту сигнала)
} dsp_ctrlreg_s;
#pragma pack(pop)

// Регистры инициализации СГТС
#pragma pack(push, 1)
typedef struct {
  uint8_t resvBits0_7     : 8; // SGTS_PARAMS[15:8] - резерв
  uint8_t TEST_NOISE_AMP  : 4; // SGTS_PARAMS[7:4] - Количество разрядов амплитуды цифрового шума для СГТС
  uint8_t TEST_SIG_AMP    : 4; // SGTS_PARAMS[3:0] - Количество разрядов амплитуды полезного сигнала для СГТС
  int16_t TEST_NCO_DF     : 16; // TEST_NCO_DF[15:0] - Значение частотного сдвига в Гц
} sgts_init_s;
#pragma pack(pop)

// Регистр управления контроллером ШИМ
#pragma pack(push, 1)
typedef struct {
  uint16_t PWMSINE_SCALE  : 12;  // PWMSINE_PARAMS[15:4] Размах амплитуды сигнала, генерирующегося при помощи схемы ШИМ
  uint16_t PWMSINE_MEAN   : 4;   // PWMSINE_PARAMS[3:0] Постоянная составляющая сигнала, генерирующегося при помощи схемы ШИМ
} pwm_ctrlreg_s;
#pragma pack(pop)

// Структура результатов обработки ПЛИС
#pragma pack(push, 1)
typedef struct{
  uint16_t cnt_irqs;            // Счетчик приерываний от ПЛИС
  int16_t maximum1;             // Максимум спектра №1
  int16_t maximum2;             // Максимум спектра №2
  int16_t maximum3;             // Максимум спектра №3
  int const_25_re_p;            // Постоянка действ. части 25кГц
  int const_25_im_p;            // Постоянка мн. части 25кГц
  int const_25_re_m;            // Постоянка действ. части -25кГц
  int const_25_im_m;            // Постоянка мн. части -25кГц
  int const_50_re_p;            // Постоянка действ. части 50кГц
  int const_50_im_p;            // Постоянка мн. части 50кГц
  int const_50_re_m;            // Постоянка действ. части -50кГц
  int const_50_im_m;            // Постоянка мн. части -50кГц
  int const_0_re;               // Постоянка действ. части 0кГц
  int const_0_im;               // Постоянка мн. части 0кГц
  int const_cic_re_p;           // Постоянка действ. части после CIC 
  int const_cic_im_p;           // Постоянка мн. части после CIC 
  int const_cic_re_m;           // Постоянка действ. части после CIC 
  int const_cic_im_m;           // Постоянка мн. части после CIC
  uint16_t svchm_temp;          // Температура СВЧМ
  uint16_t svchm_power;         // Мощность сигнала СВЧМ
  uint16_t svchm_p6V;           // Напряжение СВЧМ в цепи "+6В"
  uint16_t svchm_p4V;           // Напряжение СВЧМ в цепи "+4В"
  uint16_t svchm_m6V;           // Напряжение СВЧМ в цепи "-6В"
  uint16_t svchm_p6C;           // Ток СВЧМ в цепи "+6В"
  uint16_t svchm_p4C;           // Ток СВЧМ в цепи "+4В"
  // Регистры статуса
  fpga_main_statusreg_s fpgaStatus; // Главный регистр состояния ПЛИС
  svchm_statusreg_s svchmStatus;// Регистр статуса СВЧМ
  rt_counter_ms_s realtime_counter_ms; // Счетчик реального времени в ПЛИС
  dsp_statusreg_s dspStatus;    // Регистр статуса схемы ЦОС
  sgts_statusreg_s sgtsStatus;  // Регистр статуса СГТС
  mffm_statusreg_s mffmStatus;  // Регистр состояний линий MF-FM
  pwmsine_statusreg_s pwmsineStatus; // Регистр статуса схемы ШИМ
  flash_statusreg_s flashStatus;    // Регистр статуса flash-контроллера
  // Контрольные суммы
  uint16_t ram_cs_fpga_pre[4];  // Контролные суммы внутренней памяти ПЛИС, подсчитанная в ПЛИС при запуске FFT
  uint16_t ram_cs_fpga[4];      // Контролные суммы внутренней памяти ПЛИС, подсчитанная в ПЛИС при чтении со стороны МК
  uint16_t ram_cs_mcu[4];       // Контролные суммы внутренней памяти ПЛИС, подсчитанная в МК
  uint16_t struct_cs_fpga;      // КС блока регистров ПЛИС, подсчитанная в ПЛИС
  uint16_t struct_cs_mcu;       // КС блока регистров ПЛИС, подсчитанная в ПЛИС
  // Управление ПЛИС
  fpga_ctrlreg_s fpgaCtrlReg;   // Содержимое регистра управления ПЛИС
  dsp_ctrlreg_s dspCtrlReg;     // Содержимое регистра управления схемой ЦОС
  pwm_ctrlreg_s pwmCtrlReg;     // Содержимое регистра управления контроллером ШИМ
  adc_init_s adcInitParams;     // Параметры инициализации микросхемы АЦП и контроллера АЦП в ПЛИС
  sgts_init_s sgtsInitParams;   // Параметры СГТС
  uint16_t irq_cycles;          // 
  // Временные метки по таймеру-4
  uint32_t tp0_fpga_irq_0;      
  uint32_t tp1_fpga_irq_1;
  uint32_t tp2_begin_calc;
  uint32_t tp3_end_calc;
  // Были ли ошибки при заполнении буфера в обработчике прерывания от ПЛИС
  uint8_t irq_status;
  uint32_t irq_time;
  uint8_t transition40_80;
  uint16_t irq_timeout_us;
  uint16_t dbg_word16;
  uint8_t irq_no_control_fl;
  uint8_t no_wdi_fl;
  uint8_t errors[8];
} fpga_result_struct; // Структура результатов обработки ПЛИС
#pragma pack(pop)

#pragma pack(push, 1)
#ifdef __MINGW32__
typedef struct __attribute__((gcc_struct, packed)) {
#else   // !__MINGW32__
typedef struct {
#endif  // !__MINGW32__
    uint8_t cmd : 8;
    uint8_t fl_resv0 : 1;
    uint8_t fl_resv1 : 1;
    uint8_t fl_resv2 : 1;
    uint8_t fl_resv3 : 1;
    uint8_t fl_resv4 : 1;
    uint8_t fl_resv5 : 1;
    uint8_t fl_resv6 : 1;
    uint8_t fl_resv7 : 1;
    uint8_t fl_im_calibr_mode : 1;
    uint8_t fl_telemetry_mode : 1;
    uint8_t fl_advcontrol_mode : 1;
    uint8_t fl_resv9 : 1;
    uint16_t temp_IM : 12;
    uint32_t number : 32;
} telem_mode_ctrl_s;
#pragma pack(pop)

// Структура информации об Ethernet
#pragma pack(push, 1)
#ifdef __MINGW32__
typedef struct __attribute__((gcc_struct, packed)) {
#else   // !__MINGW32__
typedef struct {
#endif  // !__MINGW32__
    telem_mode_ctrl_s telemModeCtrl;
    uint32_t processedIrqs;
    uint32_t processedReqs;
    uint16_t ethCtrlResetCnt;
    uint16_t wd_eth;
} ethernet_status_s;
#pragma pack(pop)

// Объединенная структура данных ПЛИС и результатов работы алгоритмов
#pragma pack(push, 2)
typedef struct{
    uint16_t type   : 4;  // Тип пакета
    uint16_t length : 12; // Длина пакета
    uint32_t number : 32; // Номер пакета (сквозной)
    fpga_result_struct FRS;   // Структура данных ПЛИС
    accum_result_struct ARS;  // Структура данных алгоритмов селекции часторы доплера и вычисления вектора скорости
    ethernet_status_s ethInfo;// Информация об обмене по Ethernet
    uint16_t struct_cs : 16;  // КС структуры
} all_algorithms_data;
#pragma pack(pop)


// Структура параметров сторожевых таймеров и схемы сброса
#pragma pack(push, 1)
typedef struct{
    uint8_t wdiIntSource : 1;   // источник импульсов WDI для внешнего СТ (1 - удлиненный импульс, 0 - импульс от МК)
    uint8_t wdiExtSource : 1;   // источник импульсов WDI для внешнего СТ (1 - ПЛИС, 0 - МК)
    uint8_t wdtIntMask   : 1;   // значение, записываемое в бит маски внутреннего СТ
    uint8_t wdtExtMask   : 1;   // значение, записываемое в бит маски внешнего СТ
    uint8_t wdiEn        : 2;   // разрешение выработки импульсов WDI в обработчике прерывания таймера-2 МК
    uint8_t cntEn        : 2;   // разрешение задания предельного количества импульсов WDI в обработчике прерывания таймера-2
    uint8_t cnt          : 8;   // предельное значение количества импульсов WDI в обработчике прерывания таймера-2
    uint8_t wdiDividor   : 8;   // делитель частоты импульсов WDI (работает при wdiIntSource = 1)
} wdi_timer_params_s;
#pragma pack(pop)

// Структура информации об инициализации устройств при включении
#pragma pack(push, 1)
typedef struct {
    uint16_t type   : 4;  // Тип пакета
    uint16_t length : 12; // Длина пакета
    uint32_t number : 32; // Номер пакета (сквозной)
    uint8_t status_ponsys;      // Состояние схемы включения МК
    uint8_t status_plllocked;   // Состояние сигналов PLL_Locked ПЛИС и МК
    uint8_t status_periph;      // Состояние инициализации и настройки остальных периферийных устройств МПР-450
    uint32_t tp[11];            // Временные отметки процесса инициализации МПР-450
} PowerOnControlStatus_s;
#pragma pack(pop)

// Структура пакета с данными Ethernet для режима телеметрии
#pragma pack(push, 2)
typedef struct {
  uint16_t type   : 4;  // Тип пакета
  uint16_t length : 12; // Длина пакета
  uint32_t number : 32; // Номер пакета (сквозной)
  uint16_t data[512];   // Данные ПЛИС
  uint16_t struct_cs;           // Контрольная сумма
} packet_fpga_data_s;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct {
  uint8_t comp_coef_num;
  uint8_t b_num;
  uint8_t part;
  uint16_t coef[512];
} comp_coef_send_s;
#pragma pack(pop)

#pragma pack(push, 1)
#ifdef __MINGW32__
typedef struct __attribute__((gcc_struct, packed)) {
#else   // !__MINGW32__
typedef struct {
#endif  // !__MINGW32__
  uint8_t get_status_only : 1;
  uint8_t calibr_en       : 1;
  uint32_t cnt_max        : 30;
} calibr_afc_s;
#pragma pack(pop)

// Состояния схемы проверки внешнего СТ
typedef enum {
    STATE_IDLE  = 0,     // Начальное состояние
    STATE_GEN_WDI,       // Генерация импульса WDI
    STATE_WAIT_NWDO,     // Ожидание активного уровня сигнала nWDO
    STATE_NWDO_ACTIVE,   // Состояние при активном уровне сигнала nWDO
    STATE_WAIT_NWDO_EDGE,// Максимальная ожидаемая задержка положит. фронта nWDO относительно фронта (любого) WDI
    STATE_CHECK_NWDO     // Проверка того, что сигнал nWDO вернулся в пассивное состояние
} ExtWdtStateEnum;

// Статус тестирования внутреннего СТ
typedef enum {
    STATUS_IDLE = 0, 
    STATUS_BUSY,
    STATUS_NOWDMASK,
    STATUS_NWDOIS0,
    STATUS_NWDOIS0_EARLY,
    STATUS_NWDOIS1_LATE,
    STATUS_NWDOIS0_LATE,
    STATUS_SUCCESS
} ExtWdtStatusEnum;

// Перечисление расположение различных данных начиная с адреса SOME_DATA_ADDR
enum {
  CALIBRATED_POS, // 0
  SN_POS,         // 1
  REFDAC1_POS,    // 2
  REFDAC2_POS,    // 3
  CONFIGURE_POS,  // 4
  //SOME_DATA_COUNT обновлять вверху дефайн! не добовлять сюда для проверки size overflow.
};

// Ошибка флеш.
typedef enum { // 4 бит. НЕ должно быть больше 0xF.
  // ФЛАГ МИНИМУМ.
  MIN_FPGA_FLASH_ERROR = 0,
  // (0x1) Ошибка контроллера FLASH PHY при записи.
  ERR_WROP,
  // (0x2) Ошибка контрольного чтения при записи байта.
  ERR_WRBYTE,
  // (0x3) Ошибка контрольного чтения при записи страницы.
  ERR_WRPAGE,
  // (0x4) Ошибка контроллера FLASH PHY при чтении.
  ERR_RDOP,
  // (0x5) Ошибка контроллера FLASH: не выставлен BUSY.
  ERR_NOBUSY,
  // (0x6) Неожиданно выставлен флаг готовности чтения байта из FLASH.
  ERR_UNEXPRDY,
  // (0x7) Пропущены байты при чтении FLASH.
  ERR_MISSED,
  // (0x8) Операция чтения завершена, но нет готовности данных.
  ERR_NORDY,
  // (0x9) Ошибка операции записи в регистр ПЛИС.
  ERR_SC_FPGARDOP,
  // (0xA) Ошибка операции записи в регистр ПЛИС.
  ERR_SC_FPGAWROP,
  // (0xB) Долгая запись.
  ERR_BUSY,
  // ФЛАГ МАКСИМУМ.
  MAX_FPGA_FLASH_ERROR
} enumFpgaFlashError;
// Функция, в которой произошла ошибка.
typedef enum {  // 4 бит. НЕ должно быть больше 0xF
  FUNC_OTHER = 0,
  // (0x1)
  FLASH_FIND_INDEX,
  // (0x2)
  FLASH_SAVE_LOG,
  // (0x3)
  GET_ANGLE,
  // (0x4)
  GET_CALIB_TEMP,
  // (0x5)
  CHECK_RESET_FPGA,
  // (0x6)
  FUNC_SELFCONTROL,
  // (0x7)
  FLASH_INIT_DATA,
  // (0x8)
  CHECK_INT_WDOARISE,
  // (0x9)
  INIT_FLASH,
  // (0xA)
  CHECK_SAVE_LOG,
  // (0xB)
  CHECK_EXT_WDOARISE,
  // (0xC)
  READ_FPGA_INTO_BUFFER,
  // (0xD)
  CHECK_AND_CONF_PERIPH,
  // (0xE)
  INIT_FPGA_ADC,
} enumFpgaFlashFunc;

// Коды перехода в память и коды ошибок.
typedef enum {
  // Код времени наработки.
  LOG_TIME_TOTAL = 0x0,
  // min Memary.
  FLAG_LOG_MEMORY_MIN = 0x0,
  // 0x01 Потерян сигнал.
  LOG_MEM_MISS_INF,
  // 0x02 Команда молчание.
  LOG_MEM_SILENCE,
  // 0x03 Выход из стартового контроля.
  LOG_MEM_START_CONTROL_DONE,
  // 0x04 Выход из расширенного контроля.
  LOG_MEM_ADV_CONTROL_DONE,
  // max Memary.
  FLAG_LOG_MEMORY_MAX,
  // min Power.
  FLAG_LOG_ERR_IP_MIN = 0xA,
  // 0xB отказ питания при IS = 0
  LOG_ERR_IP_IS0,
  // 0xC отказ питания при IS = 1
  LOG_ERR_IP_IS1,
  // max Power.
  FLAG_LOG_ERR_IP_MAX,
  // min SHF.
  FLAG_LOG_ERR_SHF_MIN = 0x10,
  // (0x11) Не выставилась РК мощность усилителя. Осталось в значении 0.
  LOG_ERR_CNTRL2_IS_0,
  // (0x12) Неожидано мощность исчезла.
  LOG_ERR_CNTRL2_IS_0_WOW,
  // (0x13) Не выставилась РК мощность усилителя.Осталось в значении 1.
  LOG_ERR_CNTRL2_IS_1,
  // (0x14) Неожидано появилась мощность.
  LOG_ERR_CNTRL2_IS_1_WOW,
  // (0x15) Превышение тока.
  LOG_ERR_CNTRL3,
  // (0x16) Резерв
  RESERVE_16,
  // (0x17) Ошибка отсутствия превышения по току СВЧМ.
  ERR_SC_CNTRL3CL,
  // (0x18) Ошибка контрольной суммы пакета по протоколу СВЧМ-МПР (UART).
  ERR_SC_UARTPACKETCS,
  // (0x19) Ошибка напряжения питания цепи +6В СВЧМ.
  ERR_SC_SVCHMP6V,
  // (0x1A) Ошибка напряжения питания цепи +4В СВЧМ.
  ERR_SC_SVCHMP4V,
  // (0x1B) Ошибка напряжения питания цепи -6В СВЧМ.
  ERR_SC_SVCHMM6V,
  // (0x1C) Превышение по току в цепи +6В СВЧМ.
  ERR_SC_SVCHMP6C,
  // (0x1D) Превышение по току в цепи +4В СВЧМ.
  ERR_SC_SVCHMP4C,
  // (0x1E) Превышение по мощности СВЧМ.
  ERR_SC_SVCHMPOWERHIGH,
  // (0x1F) Мощность СВЧМ меньше допустимой.
  ERR_SC_SVCHMPOWERLOW,
  // (0x20) Пониженный шум по каналу 1 луча.
  ERR_SC_SVCHMLOWNOISE_B0,
  // (0x21) Пониженный шум по каналу 2 луча.
  ERR_SC_SVCHMLOWNOISE_B1,
  // (0x22) Пониженный шум по каналу 3 луча.
  ERR_SC_SVCHMLOWNOISE_B2,
  // (0x23) Пониженный шум по каналу 4 луча.
  ERR_SC_SVCHMLOWNOISE_B3,
  // (0x24) Повышенный шум по каналу 1 луча.
  ERR_SC_SVCHMHIGHNOISE_B0,
  // (0x25) Повышенный шум по каналу 2 луча.
  ERR_SC_SVCHMHIGHNOISE_B1,
  // (0x26) Повышенный шум по каналу 3 луча.
  ERR_SC_SVCHMHIGHNOISE_B2,
  // (0x27) Повышенный шум по каналу 4 луча.
  ERR_SC_SVCHMHIGHNOISE_B3,
  // (0x28) Отсутствие информации по UART при включении.
  ERR_SC_UARTEMPTY,
  // max SHF.
  FLAG_LOG_ERR_SHF_MAX,
  // min MPR noData.
  FLAG_LOG_ERR_MPR_MIN = 0x40,
  // (0x41) Резерв
  RESERVE_41,
  // (0x42) не прошел стартовый контроль.
  LOG_ERR_FPGA,
  // (0x43) не прошел стартовый контроль.
  LOG_ERR_SRAM,
  // (0x44) ошибка при записи/чтении.
  LOG_ERR_FPGA_FLASH,
  // (0x45) получено сообщение по другим адресам (не работает фильтр).
  LOG_ERR_ARINC,
  // (0x46) номер луча.
  LOG_ERR_N_RAY,
  // (0x47) ошибка перехода.
  LOG_ERR_80TO40,
  // (0x48) ошибка перехода.
  LOG_ERR_40TO80,
  // (0x49) ошибки в прошивке. Они не должны происходить..
  LOG_ERR_OTHER,
  // (0x4A) не совпадение CS.
  LOG_ERR_CALIBR_CS,
  // (0x4B) Ошибка EEPROM.
  LOG_ERR_EEPROM,
  // (0x4C) Ошибка схемы включения.
  ERR_SC_POWERONSYS,
  // (0x4D) Ошибка входной частоты 12 МГц.
  ERR_SC_PLLTIMEOUT,
  // (0x4E) Ошибка конфигурации ПЛИС.
  ERR_SC_FPGACONFTIMEOUT,
  // (0x4F) Ошибка связи с ПЛИС по внешней шине AD.
  ERR_SC_EXTBUS,
  // (0x50) Ошибка генератора 12 МГц на PLL1 ПЛИС.
  ERR_SC_FPGAPLL1LOCK,
  // (0x51) Ошибка генератора 12 МГц на PLL2 ПЛИС.
  ERR_SC_FPGAPLL2LOCK,
  // (0x52) Ошибка контроллера внешнего ПЗУ.
  ERR_SC_FLASH,
  // (0x53) Ошибка взаимодействия с внешней SRAM.
  ERR_SC_SRAM,
  // (0x54) Ошибка вырабатывания импульсов nWDO внутреннего СТ.
  ERR_SC_INTNWDO,
  // (0x55) Ошибка вырабатывания импульсов nWDO внешнего СТ.
  ERR_SC_EXTNWDO,
  // (0x56) Ошибка сброса ПЛИС.
  ERR_SC_FPGARST,
  // (0x57) Ошибка соответствия состояний порта FM[1] в МК и ПЛИС.
  ERR_SC_PORTFM_1,
  // (0x58) Ошибка выполнения команды "Старт" схемы ЦОС.
  ERR_SC_DSPSTART,
  // (0x59) Ошибка запуска схемы генерации тестового сигнала (СГТС).
  ERR_SC_SGTS,
  // (0x5A) Ошибка вырабатывания внешних прерываний от ПЛИС.
  ERR_SC_FPGAEXTIRQ,
  // (0x5B) Ошибка номера луча.
  ERR_SC_BEAMNUM,
  // (0x5C) Ошибка состояний режимов "ПОЛЕТ" или "ВИСЕНИЕ".
  ERR_SC_MODES40MS80MS,
  // (0x5D) Ошибка выполнения команды "Стоп" схемы ЦОС.
  ERR_SC_DSPSTOP,
  // (0x5E) Ошибка результатов тестовой задачи.
  ERR_SC_TESTTASK,
  // (0x5F) Ошибка выставления выходных LVTTL EN_HFS.
  ERR_SC_ENHFS,
  // (0x60) Ошибка параметра индекса модуляции.
  ERR_SC_INDMOD,
  // (0x61) Ошибка операции записи в регистры ПЛИС по внешней шине
  ERR_SC_EXTBUS_WR,
  // (0x62) Ошибка соответствия состояний порта FM[2] в МК и ПЛИС.
  ERR_SC_PORTFM_2,
  // (0x63) Ошибка чтения блока регистров.
  ERR_SC_FPGARDSTRUCT,
  // (0x64) Ошибка счетчиков FIFO в ПЛИС.
  ERR_SC_FIFOCNTRS,
  // (0x65) Ошибка стартового/стопового байтов пакета по протоколу СВЧМ-МПР (UART).
  ERR_SC_STARTSTOPBITS,
  // (0x66) Ошибка выставления выходных LVTTL EN_TEST.
  ERR_SC_ENTEST,
  // (0x67) Ошибка контрольных сумм в ПЛИС.
  ERR_SC_FPGA_CS,
  // (0x68) Ошибка контрольных сумм при чтении внутреннего ОЗУ ПЛИС со стороны МК.
  ERR_SC_FPGA_MCU_CS,
  // (0x69) Ошибка контрольной суммы группы регистров.
  ERR_SC_FPGA_REGS_CS,
  // (0x6A) Ошибка инициализации внешней тактовой частоты МК.
  ERR_SC_HSE_CONFIG,
  // (0x6B) Ошибка выставления PLL LOCKED От внешней тактовой частоты МК.
  ERR_SC_HSE_PLLLOCKED,
  // (0x6C) Ошибка питания 5В МПР.
  ERR_SC_EXTNWDR,
  // (0x6D) Ошибка схемы ШИМ.
  ERR_SC_PWMSINE,
  // (0x6E) Ошибка контрольных сумм при записи/чтении внешнего ОЗУ по адресам текущих спектров.
  ERR_SC_SRAMCURSPCS,
  // (0x6F) Ошибка контрольных сумм при записи/чтении внешнего ОЗУ при вычислении центра тяжести.
  ERR_SC_SRAMCTSPCS,
  // (0x70) Ошибка контрольных сумм при записи/чтении внешнего ОЗУ по адресам средних спектров.
  ERR_SC_SRAMMIDSPCS,
  // (0x71) Сработал таймаут по прерыванию и ПЛИС.
  ERR_SC_FPGAIRQTIMEOUT,
  // (0x72) Период прерываний от ПЛИС слишком мал.
  ERR_SC_FPGAIRQLOW,
  // (0x73) Ошибка конфигурации ПЛИС.
  ERR_SC_FPGACONF,
  // (0x74) Ошибка инициализации АЦП.
  ERR_SC_FPGAADCINIT,
  // (0x75) МК запустился после сброса от внешнего СТ.
  ERR_SC_STARTAFTEREXTNWDO,
  // (0x76) МК запустился после сброса от внутреннего СТ.
  ERR_SC_STARTAFTERINTNWDO,
  // (0x77) Ошибка соответствия состояния порта TMODE в МК и ПЛИС.
  ERR_SC_PORTTMODE,
  // (0x78) Ошибка соответствия состояния порта READY в МК и ПЛИС.
  ERR_SC_PORTREADY,
  // (0x79) Ошибка соответствия состояний порта MF[0] в МК и ПЛИС.
  ERR_SC_PORTMF_0,
  // (0x7A) Ошибка вычисления скорости Wx в тестовой задаче.
  ERR_SC_CTRL_WX,
  // (0x7B) Ошибка вычисления скорости Wz в тестовой задаче.
  ERR_SC_CTRL_WZ,
  // (0x7C) Ошибка вычисления скорости Wy в тестовой задаче.
  ERR_SC_CTRL_WY,
  // (0x7D) Ошибка счетчика прерываний ПЛИС.
  ERR_SC_FPGACNTIRQS,
  // (0x7E) Ошибка схемы нагрева: неожиданно включена.
  ERR_SC_HEATON,
  // (0x7F) Ошибка схемы нагрева: неожиданно вЫключена.
  ERR_SC_HEATOFF,
  // (0x80) Ошибка уровня шумов АЦП.
  ERR_SC_ADCNOISEHIGH,
  // (0x81) Ошибка уровня шумов АЦП.
  ERR_SC_ADCNOISELOW,
  // (0x82) Ошибка показаний температурных датчиков.
  ERR_SC_TEMPSENSORS,
  // (0x83) Ошибка контрольных сумм при записи/чтении компенсирующих характеристик АЧХ из ОЗУ.
  ERR_SC_SRAMCOMPCOEF,
  // (0x84) Ошибка эхо-контроля приемника 1 канала КЛС.
  ERR_SC_ARINC429R1,
  // (0x85) Ошибка эхо-контроля приемника 2 канала КЛС.
  ERR_SC_ARINC429R2,
  // (0x86) Ошибка эхо-контроля передатчика двух каналов КЛС (бит регистра аринк).
  ERR_SC_ARINC429T_REG,
  // (0x87) Ошибка эхо-контроля передатчика 1 канала КЛС (бит регистра аринк).
  ERR_SC_ARINC429T1_REG,
  // (0x88) Ошибка эхо-контроля передатчика 2 канала КЛС (бит регистра аринк).
  ERR_SC_ARINC429T2_REG,
  // (0x89) Ошибка эхо-контроля передатчика двух каналов КЛС (пришло сообщение которое не отправляли).
  ERR_SC_ARINC429T_MSG,
  // (0x8A) Ошибка эхо-контроля передатчика 1 канала КЛС (пришло сообщение которое не отправляли).
  ERR_SC_ARINC429T1_MSG,
  // (0x8B) Ошибка эхо-контроля передатчика 2 канала КЛС (пришло сообщение которое не отправляли).
  ERR_SC_ARINC429T2_MSG,
  // (0x8C) Ошибка эхо-контроля передатчика двух каналов КЛС (не пришло сообщение которое отправляли).
  ERR_SC_ARINC429T_NO_MSG,
  // (0x8D) Ошибка эхо-контроля передатчика 1 канала КЛС (не пришло сообщение которое отправляли).
  ERR_SC_ARINC429T1_NO_MSG,
  // (0x8E) Ошибка эхо-контроля передатчика 2 канала КЛС (не пришло сообщение которое отправляли).
  ERR_SC_ARINC429T2_NO_MSG,
  // (0x8F) Ошибка показаний внутреннего датчика температуры МК.
  ERR_SC_EXTTEMPSENSOR,
  // (0x90) Ошибка показаний внешнего датчика температуры МПР.
  ERR_SC_INTTEMPSENSOR,
  // (0x91) Ошибка ПО/оператора.
  ERR_SC_SF,
  // (0x92) Ошибка контрольных сумм при чтении данных из флеш ТД.
  ERR_SC_FLASHDATACS_TEMP,
  // (0x93) Ошибка контрольных сумм при чтении данных из флеш углов.
  ERR_SC_FLASHDATACS_ANGLE,
  // (0x94) Ошибка контрольных сумм при чтении данных слов.
  ERR_SC_FLASHDATACS_DATA,
  // (0x95) Ошибка обогрева. Обогрев не включился.
  ERR_SC_TOFFLS_IS_1,
  // (0x96) Ошибка обогрева. Обогрев включился.
  ERR_SC_TOFFLS_IS_0,
  // (0x97) Ошибка температурного датчика. Перегрев ТД МК.
  ERR_SC_HEAD_INT,
  // (0x98) Ошибка температурного датчика. Перегрев ТД платы.
  ERR_SC_HEAD_EXT,
  // (0x99) Ошибка температурного датчика. Переохлаждение ТД МК.
  ERR_SC_COOL_INT,
  // (0x9A) Ошибка температурного датчика. Переохлаждение ТД платы.
  ERR_SC_COOL_EXT,
  // (0x9B) Ошибка номера луча по информации от СВЧМ.
  ERR_SC_SVCHMBEAMNUM,
  // (0x9C) Ошибка отсутствия информации по UART в цикле работы.
  ERR_SC_UARTEMPTY_2,
  // (0x9D) Ошибка резерв.
  RESERVE5,
  // (0x9E) Ошибка переполнения времени наработки.
  ERR_SC_TIME_OVERFLOW,
  // (0x9F) Ошибка при чтении поправок АЧХ для 1 луча.
  ERR_SC_COMPCOEFB0CS,
  // (0xA0) Ошибка при чтении поправок АЧХ для 2 луча.
  ERR_SC_COMPCOEFB1CS,
  // (0xA1) Ошибка при чтении поправок АЧХ для 3 луча.
  ERR_SC_COMPCOEFB2CS,
  // (0xA2) Ошибка при чтении поправок АЧХ для 4 луча.
  ERR_SC_COMPCOEFB3CS,
  // (0xA3) Ошибка при чтении коэффициентов АЧХ из SRAM.
  ERR_SC_COMPCOEFSRAM,
  // (0xA4) Ошибка частой отправки аринк.
  ERR_SC_ARINC_TOO_MUCH,
  // (0xA5) Внешний СТ. Тест не пройден: состояние после сброса
  ERR_SC_WDT_IDLE,
  // (0xA6) Внешний СТ. Тест не пройден: маска СТ не выставлена
  ERR_SC_WDT_NOMASK,
  // (0xA7) Внешний СТ. Тест не пройден: nWDO в состоянии "0" при запуске
  ERR_SC_WDT_NWDOIS0,
  // (0xA8) Внешний СТ. Тест не пройден: nWDO стал активным раньше регламентированного минимального времени
  ERR_SC_WDT_NWDOIS0_EARLY,
  // (0xA9) Внешний СТ. Тест не пройден: nWDO пассивен больше регламентированного максимального времени после фронта WDI
  ERR_SC_WDT_NWDOIS1_LATE,
  // (0xAA) Внешний СТ. Тест не пройден: nWDO не получилось сделать пасивным после 2х фронтов WDI в периодом
  ERR_SC_WDT_NWDOIS0_LATE,
  // (0xAB) Внешний СТ. Тест не пройден: истекло отведенное для теста время
  ERR_SC_WDT_TESTTIMEOUT,
  // (0xAC) Ошибка конфигурации ПЛИС при чтении регистров статуса ПЛИС
  ERR_SC_FPGACONF_RD,
  // (0xAD) Ошибка занятости флешки перед стартовым контролем.
  ERR_SC_START_FLASH_BUSY,
  // (0xAE) Ошибка соответствия состояний порта FM[3] в МК и ПЛИС.
  ERR_SC_PORTFM_3,
  // (0xAF) Ошибка соответствия состояний порта FM[4] в МК и ПЛИС.
  ERR_SC_PORTFM_4,
  // (0xB0) Ошибка соответствия состояний порта MF[1] в МК и ПЛИС.
  ERR_SC_PORTMF_1,
  // (0xB1) Ошибка соответствия состояний порта MF[2] в МК и ПЛИС.
  ERR_SC_PORTMF_2,
  // (0xB2) Ошибка соответствия состояний порта MF[3] в МК и ПЛИС.
  ERR_SC_PORTMF_3,
  // max MPR noData.
  FLAG_LOG_ERR_MPR_MAX,
  // min MPR Data.
  FLAG_LOG_ERR_MPR_DATA_MIN = 0xC0,
  // (0xC1) Ошибка эхо-контроля РК.
  ERR_SC_RKOLV0001,
  // (0xC2) Ошибка эхо-контроля РК.
  ERR_SC_RKOLV0010,
  // (0xC3) Ошибка эхо-контроля РК.
  ERR_SC_RKOLV0011,
  // (0xC4) Ошибка эхо-контроля РК.
  ERR_SC_RKOLV0100,
  // (0xC5) Ошибка эхо-контроля РК.
  ERR_SC_RKOLV0101,
  // (0xC6) Ошибка эхо-контроля РК.
  ERR_SC_RKOLV0110,
  // (0xC7) Ошибка эхо-контроля РК.
  ERR_SC_RKOLV0111,
  // (0xC8) Ошибка эхо-контроля РК.
  ERR_SC_RKOLV1000,
  // (0xC9) Ошибка эхо-контроля РК.
  ERR_SC_RKOLV1001,
  // (0xCA) Ошибка эхо-контроля РК.
  ERR_SC_RKOLV1010,
  // (0xCB) Ошибка эхо-контроля РК.
  ERR_SC_RKOLV1011,
  // (0xCC) Ошибка эхо-контроля РК.
  ERR_SC_RKOLV1100,
  // (0xCD) Ошибка эхо-контроля РК.
  ERR_SC_RKOLV1101,
  // (0xCE) Ошибка эхо-контроля РК.
  ERR_SC_RKOLV1110,
  // (0xCF) Ошибка эхо-контроля РК.
  ERR_SC_RKOLV1111,
  // max MPR Data.
  FLAG_LOG_ERR_MPR_DATA_MAX,
} LogErrEnum;     // Код "ошибки" в журнале отказа

// Коды технологических команд.
enum {
  // НЕ КОМАНДА! флаг минимальной команды.
  FLAG_PANEL_MIN = 0xA0 - 1,
  // A0 отправляет версию МК.
  PANEL_BUILD_MK,
  // A1 отправляет RK_IN.
  PANEL_RK_IN,
  // A2 выставляет RK_OUT (светодиоды)  (только в техн).
  PANEL_RK_OUT_SET,
  // A3 отправка RK_OUT (светодиоды).
  PANEL_RK_OUT_GET,
  // A4 выставляет индекс модуляции.
  PANEL_INDEX_MODULATION_SET,
  // A5 отправляет индекс модуляции.
  PANEL_INDEX_MODULATION_GET,
  // A6 отправляет температуру 3х датчиков.
  PANEL_TEMPERATURE,
  // A7 Резерв, всегда отвечает "Успех".
  PANEL_COEFF_GET,
  // A8 отправляет 1 Кб журнала отказа.
  PANEL_LOG_ERROR_GET,
  // A9 очищает журнал отказа.
  PANEL_LOG_ERROR_CLEAR,
  // AA отправляет смещение по 4ем лучам.
  PANEL_DOPLER_GET,
  // AB Запрос структуры телеметрии МПР.
  PANEL_TEL_ARRAY,
  // * AC выход в боевой.
  PANEL_MAIN_MODE_SET,
  // AD возвращаем режим работы.
  PANEL_MAIN_MODE_GET,
  // AE Запросить данные калибровки АЧХ (средние спектры)
  PANEL_AFCCAL_GET_DATA,
  // AF Установка тестового сигнала.
  PANEL_TEST_SIGNAL_SET,
  // B0 Запросить параметр тестового сигнала.
  PANEL_TEST_SIGNAL_GET,
  // B1 Установка номер луча.
  PANEL_N_RAY_SET,
  // B2 Прочитать статус номер луча.
  PANEL_N_RAY_GET,
  // B3 Установить выходные LVTTL.
  PANEL_LVTTL_OUT_SET,
  // B4 Запросить состояние выходных LVTTL.
  PANEL_LVTTL_OUT_GET,
  // B5 Запросить состояние входных LVTTL.
  PANEL_LVTTL_IN,
  // B6 Выставить плис в режиме 40/80.
  PANEL_40_80_SET,
  // B7 Запрос режима плис.
  PANEL_40_80_GET,
  // B8 переходи в отказ, только в боевом.
  PANEL_GO_TO_ERROR,
  // * B9 "запрет СС, запрет ЖуО, запрет перехода 40/80" in 1 byte.
  PANEL_TECH_MODE_SET,
  // BA "запрет СС, запрет ЖуО, запрет перехода 40/80" in 1 byte.
  PANEL_TECH_MODE_GET,
  // BB Выставление углов.
  PANEL_ANGLE_ALL_SET,
  // BC Прочитать значение углов всех лучей.
  PANEL_ANGLE_ALL_GET,
  // BD Выставить калибровачные данные для темп датчиков.
  PANEL_TEMP_CALIB_SET,
  // BE Прочитать калибровачные данные для темп датчиков.
  PANEL_TEMP_CALIB_GET,
  // BF Прочитать питание выше порога.
  PANEL_5V_MPR,
  // C0 Прочитать питание СВЧМ и температуру.
  PANEL_POWER_SHF,
  // C1 Очитить компенсирующих коэф. (пока в EEPROM).
  PANEL_COMP_COEFF_CLEAR,
  // C2 Записать компенсирующих коэф. (пока в EEPROM).
  PANEL_COMP_COEFF_SET,
  // C3 Чтение компенсирующих коэф. (пока в EEPROM).
  PANEL_COMP_COEFF_GET,
  // С4 Установить параметры процесса калибровки АЧХ
  PANEL_AFCCAL_SETCTRL,
  // С5 Прочитать состояние процесса калибровки
  PANEL_AFCCAL_GETSTAT,
  // C6 Установить флаг калибровки данных.
  PANEL_CALIBRATED_SET,
  // C7 Установить данные CALIBRATED, серийный номер и REFDAC'и.
  PANEL_SOME_DATA_SET,
  // C8 Прочитать данные CALIBRATED, серийный номер и REFDAC'и.
  PANEL_SOME_DATA_GET,
  // C9 Прочитать последнюю запись ЖО.
  PANEL_LAST_LOG_GET,
  // CA Контроль обогревом и внешним температурным датчиком.
  PANEL_CONTROL_UT,
  // CB Статус обогрева.
  PANEL_CONTROL_UT_GET,
  // CC Настройка аринк.
  PANEL_ARINC_CONFIG_SET,
  // CD Возврат настроек аринк.
  PANEL_ARINC_CONFIG_GET,
  // CE Отправить тестовое сообщение.
  PANEL_TEST_TX,
  // CF Принять тестовое сообщение.
  PANEL_TEST_RX,
  // Не команда! флаг макс команды.
  FLAG_PANEL_MAX
};

//------------------------------------------------------------------------------
// Объявления функций
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Объявления переменных
//------------------------------------------------------------------------------

// Структура состояний МПР.
extern tel_array_struct telem;          // телеметрия МК
// При ошибках в диапазоне FLAG_LOG_ERR_MPR_DATA_MIN..FLAG_LOG_ERR_MPR_DATA_MAX
// записывабтся данные условия ошибки, вместо totalOn
extern uint8_t totalOnData;             // байт для записи в журнал отказа, в поле totalOn
// Последняя запись времени наработки
extern LogErrorStructTypeDef lastTimeTotal;
// Структура данных ПЛИС и структура данных алгоритмов селекции часторы доплера
// и вычисления вектора скорости.
extern all_algorithms_data AAD;
// Структура данных ПЛИС
extern fpga_result_struct * pFpgaRes;
// Структура данных алгоритмов селекции часторы доплера и вычисления вектора
// скорости.
extern accum_result_struct * pAccumRes;
extern wdi_timer_params_s wdiParams;    // Параметры формирования импульса WDI
extern uint16_t *sram_data;
extern uint32_t all_packets_cnt;

//#define USE_DEBUG_ARRAY   // [COMMENT]

extern const uint16_t SIZE_ARRAY_DEBUG;

enum DebugArray {
  funcEXT_INT1_IRQHandler = 1,
  funcCurrentControl1,
};

extern uint16_t indArrDebug;
extern uint8_t arrDebug[];

#ifndef __MINGW32__
__inline void WriteDebugArray(enum DebugArray func) {
#ifdef USE_DEBUG_ARRAY
  if (!telem.rkOut.f.fail) {
    arrDebug[indArrDebug] = func;
    if (++indArrDebug >= SIZE_ARRAY_DEBUG) {
      indArrDebug = 0;
    }
  }
#endif  // USE_DEBUG_ARRAY
}
#endif  // ! __MINGW32__

#endif   // __DISSTYPEDEF_H__
