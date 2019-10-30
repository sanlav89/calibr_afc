#ifndef CALIBRATOR2_H
#define CALIBRATOR2_H

#include <QObject>

#define B_NUM_MAX   (4)     // Количество лучей
#define ACCUM_MODES (2)     // Количество режимов (80 и 40 мс)
#define FFT_LENGTH  (1024)  // Длина БПФ
#define SMOOTH_WIN  (2)     // Размер окна сглаживания
#define CUT_AFC_POS (58)    // Номер отсчета "среза" идеальной КХ
#define IGNORE_ZEROZONE_SAMPLES (10)
#define ALL_SPECTR_SIZE (ACCUM_MODES * B_NUM_MAX * FFT_LENGTH * sizeof(double))

class Calibrator2 : public QObject
{
    Q_OBJECT
public:
    explicit Calibrator2(QObject *parent = 0);
    // Загрузить данные средних спектров калибровки и откалибровать АЧХ
    void Calibrate(QByteArray data, int cal_count);
    // Вернуть указатель на средние спектры калибровки
    double* GetSrcSpectrums(quint8 ms40, quint8 b_num);
    // Вернуть указатель на поправочную характеристику АЧХ
    double* GetCompAfc(quint8 ms40, quint8 b_num);
    // Сохранить коэффициенты комп. АЧХ и соответствующие поправки к идеальной
    // КХ в файлы
    void SaveCalibration(bool saveDbgInfo = true);

private:
    // Усредненные (суммарные) спектры калибровки по каждому лучу для каждого
    // режима (40 и 80 мс)
    double spectr[ACCUM_MODES][B_NUM_MAX][FFT_LENGTH];
    // Количество шагов накопления
    int cnt_spectrums;
    // Компенсационная карактеристика АЧХ
    double comp_afc[ACCUM_MODES][B_NUM_MAX][FFT_LENGTH];
    // Сглаженный усредненный спектр (один массив использвется для всех спектров
    // поочередно)
    double spectr_smooth[FFT_LENGTH];
    // Идеальная компенсационная характеристика АЧХ
    static const uint16_t ideal_comp_afc[FFT_LENGTH];
    // Максимум и минимум spectr_smooth
    double spectr_smooth_max;
    double spectr_smooth_min;
    // Сглаживание среднего спектра калибровки и поиск минимума, максимума
    void Smooth(uint8_t ms40, uint8_t b_num, bool smoothEn);
    // Заполнение 0-го (512-го) отсчета спектра
    // Заполняется средним по 10 соседним отсчетам
    void FillSample0(uint8_t ms40, uint8_t b_num);
    // Вычисление коэффициентов, компенсирующих АЧХ
    void CalcAfc(uint8_t ms40, uint8_t b_num);

};

#endif // CALIBRATOR2_H
