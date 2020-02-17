//==============================================================================
// (C) Copyright 2019 MIET
// Moscow, Zelenograd, Russia
//
// Device:      DISS
// Module:      MPR
// Component:   AFC calibration utility
// File:        calibrator2.h
// Function:    Class, which calculates the compensation characteristic of the
//              AFC
// Notes:
// Author:      A.Lavrinenko
//==============================================================================
#ifndef CALIBRATOR2_H
#define CALIBRATOR2_H

#include <QObject>

//==============================================================================
/*
 * Класс, принимает на вход данные, прочитанные из МПР, исходя из них формирует
 * искомую характеристику
 */
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
    void SaveCalibration(const QString &filename, bool saveDbgInfo = true);

    // Константы
    static const int ACCUM_MODES = 2;   // Количество лучей
    static const int B_NUM_MAX = 4;     // Количество режимов (80 и 40 мс)
    static const int FFT_LENGTH = 1024; // Длина БПФ
    static const int SMOOTH_WIN = 2;    // Размер окна сглаживания
    static const int CUT_AFC_POS = 58;  // Номер отсчета "среза" идеальной КХ
    // Сколько отсчетов около центра не сглаживать
    static const int IGNORE_ZEROZONE_SAMPLES = 10;
    // Размер в байтах всего массива, подлежащего обработке
    static const int ALL_SPECTR_SIZE =
            (ACCUM_MODES * B_NUM_MAX * FFT_LENGTH * sizeof(double));
    static constexpr double DFF = 1000000.0 / 39.0;   // Разрешение спектра

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
    // Заполнение 0-го (512-го) отсчета спектра
    void FillSample0(uint8_t ms40, uint8_t b_num);
    // Сглаживание среднего спектра калибровки и поиск минимума, максимума
    void Smooth(uint8_t ms40, uint8_t b_num, bool smoothEn);
    // Вычисление коэффициентов, компенсирующих АЧХ
    void CalcAfc(uint8_t ms40, uint8_t b_num);
    // Нормировка рассчитанной характеристики в диапазот [0..2]
    void normTo2();

};
//==============================================================================

#endif // CALIBRATOR2_H
