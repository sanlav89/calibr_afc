//==============================================================================
// (C) Copyright 2019 MIET
// Moscow, Zelenograd, Russia
//
// Device:      DISS
// Module:      MPR
// Component:   AFC calibration utility
// File:        calibrator2.cpp
// Function:    Class, which calculates the compensation characteristic of the
//              AFC
// Notes:
// Author:      A.Lavrinenko
//==============================================================================
#include "calibrator2.h"
#include <QFile>
#include <QTextStream>
#include <QDataStream>
#include <QDebug>
#include <math.h>

//==============================================================================
/*
 *  Идеальная компенсационная характеристика
 */
const uint16_t Calibrator2::ideal_comp_afc[] = {
            1,     1,     1,     1,     1,     1,     1,     1,     1,     1,     1,     1,     1,     1,     1,     1, // 15
            1,     1,     1,     1,     1,     1,     1,     1,     1,     1,     1,     1,     1,     1,     1,     1, // 31
            1,     1, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, // 47
        65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 63625, 61852, 60066, 58286, 56562, // 63
        54937, 53437, 51912, 50392, 48929, 47540, 46255, 44963, 43665, 42450, 41309, 40168, 39019, 37910, 36871, 35898,
        34923, 33940, 32992, 32093, 31263, 30422, 29580, 28763, 27997, 27280, 26560, 25837, 25136, 24470, 23855, 23230,
        22608, 22005, 21434, 20905, 20365, 19827, 19302, 18810, 18352, 17884, 17422, 16970, 16541, 16146, 15742, 15338,
        14943, 14574, 14232, 13880, 13528, 13190, 12870, 12567, 12263, 11958, 11661, 11381, 11121, 10856, 10590, 10331,
        10087,  9858,  9628,  9397,  9168,  8957,  8756,  8554,  8354,  8156,  7968,  7793,  7616,  7437,  7264,  7103,
         6948,  6794,  6638,  6491,  6351,  6210,  6071,  5933,  5803,  5683,  5558,  5435,  5313,  5199,  5091,  4982,
         4873,  4766,  4666,  4572,  4476,  4379,  4285,  4195,  4111,  4027,  3940,  3859,  3778,  3705,  3629,  3553,
         3480,  3411,  3344,  3278,  3210,  3143,  3082,  3025,  2964,  2904,  2847,  2790,  2738,  2687,  2632,  2581,
         2531,  2486,  2439,  2390,  2344,  2300,  2259,  2217,  2175,  2133,  2094,  2057,  2019,  1982,  1945,  1909,
         1876,  1842,  1808,  1775,  1743,  1714,  1684,  1654,  1623,  1595,  1568,  1541,  1513,  1487,  1463,  1439,
         1414,  1388,  1365,  1343,  1320,  1299,  1276,  1255,  1235,  1214,  1194,  1175,  1155,  1137,  1119,  1100,
         1082,  1065,  1048,  1033,  1015,   999,   984,   969,   954,   939,   924,   910,   897,   884,   870,   856,
          843,   832,   819,   807,   794,   782,   771,   760,   749,   739,   728,   717,   708,   697,   687,   678,
          668,   659,   650,   640,   632,   623,   615,   606,   598,   590,   582,   575,   567,   559,   551,   545,
          537,   531,   524,   516,   510,   504,   498,   492,   485,   479,   473,   467,   461,   456,   450,   445,
          440,   435,   429,   424,   419,   414,   409,   405,   400,   395,   390,   387,   382,   378,   373,   369,
          365,   361,   358,   353,   349,   346,   342,   338,   335,   331,   327,   325,   321,   318,   314,   311,
          308,   305,   302,   299,   295,   293,   290,   287,   285,   282,   279,   277,   273,   271,   269,   266,
          264,   261,   259,   257,   254,   252,   250,   247,   245,   243,   241,   239,   237,   235,   233,   231,
          229,   228,   225,   224,   222,   220,   218,   216,   215,   214,   212,   210,   208,   207,   206,   204,
          202,   201,   199,   198,   197,   195,   194,   193,   191,   190,   188,   187,   186,   185,   183,   182,
          181,   180,   179,   178,   177,   176,   174,   173,   172,   171,   170,   169,   168,   168,   167,   166,
          165,   164,   163,   162,   161,   161,   160,   159,   158,   157,   157,   156,   155,   154,   153,   153,
          152,   151,   151,   150,   149,   149,   148,   147,   147,   146,   145,   145,   144,   144,   144,   143,
          142,   142,   141,   141,   140,   140,   139,   139,   138,   138,   137,   137,   136,   136,   136,   136,
          135,   135,   135,   134,   134,   133,   133,   133,   132,   132,   132,   131,   131,   131,   131,   131,
          130,   130,   130,   130,   129,   129,   129,   129,   129,   128,   128,   128,   128,   128,   127,   127,
          127,   127,   127,   127,   127,   127,   127,   127,   127,   127,   127,   127,   127,   127,   127,   127,
          127,   127,   127,   127,   127,   127,   127,   127,   127,   127,   127,   127,   127,   127,   127,   127,
          127,   127,   127,   128,   128,   128,   128,   128,   129,   129,   129,   129,   129,   130,   130,   130,
          130,   131,   131,   131,   131,   131,   132,   132,   132,   133,   133,   133,   134,   134,   135,   135,
          135,   136,   136,   136,   136,   137,   137,   138,   138,   139,   139,   140,   140,   141,   141,   142,
          142,   143,   144,   144,   144,   145,   145,   146,   147,   147,   148,   149,   149,   150,   151,   151,
          152,   153,   153,   154,   155,   156,   157,   157,   158,   159,   160,   161,   161,   162,   163,   164,
          165,   166,   167,   168,   168,   169,   170,   171,   172,   173,   174,   176,   177,   178,   179,   180,
          181,   182,   183,   185,   186,   187,   188,   190,   191,   193,   194,   195,   197,   198,   199,   201,
          202,   204,   206,   207,   208,   210,   212,   214,   215,   216,   218,   220,   222,   224,   225,   228,
          229,   231,   233,   235,   237,   239,   241,   243,   245,   247,   250,   252,   254,   257,   259,   261,
          264,   266,   269,   271,   273,   277,   279,   282,   285,   287,   290,   293,   295,   299,   302,   305,
          308,   311,   314,   318,   321,   325,   327,   331,   335,   338,   342,   346,   349,   353,   358,   361,
          365,   369,   373,   378,   382,   387,   390,   395,   400,   405,   409,   414,   419,   424,   429,   435,
          440,   445,   450,   456,   461,   467,   473,   479,   485,   492,   498,   504,   510,   516,   524,   531,
          537,   545,   551,   559,   567,   575,   582,   590,   598,   606,   615,   623,   632,   640,   650,   659,
          668,   678,   687,   697,   708,   717,   728,   739,   749,   760,   771,   782,   794,   807,   819,   832,
          843,   856,   870,   884,   897,   910,   924,   939,   954,   969,   984,   999,  1015,  1033,  1048,  1065,
         1082,  1100,  1119,  1137,  1155,  1175,  1194,  1214,  1235,  1255,  1276,  1299,  1320,  1343,  1365,  1388,
         1414,  1439,  1463,  1487,  1513,  1541,  1568,  1595,  1623,  1654,  1684,  1714,  1743,  1775,  1808,  1842,
         1876,  1909,  1945,  1982,  2019,  2057,  2094,  2133,  2175,  2217,  2259,  2300,  2344,  2390,  2439,  2486,
         2531,  2581,  2632,  2687,  2738,  2790,  2847,  2904,  2964,  3025,  3082,  3143,  3210,  3278,  3344,  3411,
         3480,  3553,  3629,  3705,  3778,  3859,  3940,  4027,  4111,  4195,  4285,  4379,  4476,  4572,  4666,  4766,
         4873,  4982,  5091,  5199,  5313,  5435,  5558,  5683,  5803,  5933,  6071,  6210,  6351,  6491,  6638,  6794,
         6948,  7103,  7264,  7437,  7616,  7793,  7968,  8156,  8354,  8554,  8756,  8957,  9168,  9397,  9628,  9858,
        10087, 10331, 10590, 10856, 11121, 11381, 11661, 11958, 12263, 12567, 12870, 13190, 13528, 13880, 14232, 14574,
        14943, 15338, 15742, 16146, 16541, 16970, 17422, 17884, 18352, 18810, 19302, 19827, 20365, 20905, 21434, 22005,
        22608, 23230, 23855, 24470, 25136, 25837, 26560, 27280, 27997, 28763, 29580, 30422, 31263, 32093, 32992, 33940,
        34923, 35898, 36871, 37910, 39019, 40168, 41309, 42450, 43665, 44963, 46255, 47540, 48929, 50392, 51912, 53437,
        54937, 56562, 58286, 60066, 61852, 63625, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, // 975
        65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535,     1, // 991
            1,     1,     1,     1,     1,     1,     1,     1,     1,     1,     1,     1,     1,     1,     1,     1, // 1007
            1,     1,     1,     1,     1,     1,     1,     1,     1,     1,     1,     1,     1,     1,     1,     1  // 1023
    };

/*
 *
 */
Calibrator2::Calibrator2(QObject *parent) : QObject(parent)
{
}

/*
 * Загрузить данные средних спектров калибровки и откалибровать АЧХ
 */
void Calibrator2::Calibrate(QByteArray data, int cal_count)
{
    // Копируем данные
    mempcpy(spectr, data.data(), ALL_SPECTR_SIZE);
    cnt_spectrums = cal_count;

    // Делим каждый отсчет на количество циклов калибровки
    for (int k = 0; k < ACCUM_MODES; k++) {
        for (int j = 0; j < B_NUM_MAX; j++) {
            for (int i = 0; i < FFT_LENGTH; i++) {
                spectr[k][j][i] /= cnt_spectrums;
            }
        }
    }

    // Непосредственно калибровка (расчеты)
    for (int k = 0; k < ACCUM_MODES; k++) {
        for (int j = 0; j < B_NUM_MAX; j++) {
            FillSample0(k, j);
            Smooth(k, j, true);
            CalcAfc(k, j);
        }
    }

}

/*
 * Вернуть указатель на средние спектры калибровки (для отображения на графике)
 */
double* Calibrator2::GetSrcSpectrums(quint8 ms40, quint8 b_num)
{
    return &spectr[ms40][b_num][0];
}

/*
 * Вернуть указатель на поправочную характеристику АЧХ
 * (для отображения на графике)
 */
double* Calibrator2::GetCompAfc(quint8 ms40, quint8 b_num)
{
    return &comp_afc[ms40][b_num][0];
}

/*
 * Сохранить коэффициенты комп. АЧХ и соответствующие поправки к идеальной КХ
 * в файлы
 */
void Calibrator2::SaveCalibration(const QString &filename, bool saveDbgInfo)
{
    QFile fileCoefs("comp_coefs.c");
    QFile fileCoefCorrs(filename);
    QFile f_sp("CAL_SP.dat"), f_comp("CAL_COMP.dat");
    QTextStream sc(&fileCoefs);
    QTextStream scc(&fileCoefCorrs);
    QString str;
    int comp_coef_value;
    double comp_coef_corr;
    int16_t comp_coef_corr_value;

    normTo2();

    if (!fileCoefs.open(QIODevice::WriteOnly)) {
        qDebug() << "Невозможно открыть файл" << fileCoefs.fileName();
        return;
    }
    else if (!fileCoefCorrs.open(QIODevice::WriteOnly)) {
        qDebug() << "Невозможно открыть файл" << fileCoefCorrs.fileName();
        return;
    }
    else {
        for (int k = 0; k < ACCUM_MODES; k++) {
            for (int j = 0; j < B_NUM_MAX; j++) {
                str.sprintf("// MODE: %d ms; BEAM #%d\r\n", 80 - k * 40, j);
                sc << str;
                sc << "comp_coef[1024] = {\r\n";
                for (int i = 0; i < FFT_LENGTH; i++) {
                    comp_coef_value = (int)(comp_afc[k][j][i] * ideal_comp_afc[i]);
                    if (comp_coef_value > 65535)
                        comp_coef_value = 65535;
                    str.sprintf("%6d", comp_coef_value);
                    sc << str;

                    comp_coef_corr = comp_afc[k][j][i];
                    comp_coef_corr_value = pow(2, 15) * (comp_coef_corr - 1) - 1;

                    if (k == ACCUM_MODES - 1 && j == B_NUM_MAX - 1 && i == FFT_LENGTH - 1)
                        str.sprintf("%6d", (uint16_t) comp_coef_corr_value);
                    else
                        str.sprintf("%6d,", (uint16_t) comp_coef_corr_value);

                    if (i < FFT_LENGTH - 1) {
                        sc << ",";
                        scc << str;
                    }
                    if ((i + 1) % 16 == 0) {
                        sc << "\r\n";
                        scc << "\n";
                    }
                }
                sc << "};\r\n\r\n";
                scc << "\n";
            }
        }
    }
    if (fileCoefs.isOpen())
        fileCoefs.close();
    if (fileCoefCorrs.isOpen())
        fileCoefCorrs.close();

    // Запись в файл промежуточных массивов
    if (saveDbgInfo) {
        if (!f_sp.open(QIODevice::WriteOnly)) {
            qDebug() << f_sp.fileName() << "is not opened";
        }
        else if (!f_comp.open(QIODevice::WriteOnly)) {
            qDebug() << f_comp.fileName() << "is not opened";
        }
        else {
            f_sp.write((char*)GetSrcSpectrums(0, 0), ALL_SPECTR_SIZE);
            f_comp.write((char*)GetCompAfc(0, 0), ALL_SPECTR_SIZE);
        }
        if (f_sp.isOpen())
            f_sp.close();
        if (f_comp.isOpen())
            f_comp.close();
    }
}

/*
 * Заполнение 0-го (512-го) отсчета спектра
 * Заполняется средним по 10 соседним отсчетам
 */
void Calibrator2::FillSample0(uint8_t ms40, uint8_t b_num)
{
    double sample0 = 0;
    for (int i = 507; i <= 511; i++)
        sample0 += spectr[ms40][b_num][i];
    for (int i = 513; i < 518; i++)
        sample0 += spectr[ms40][b_num][i];
    sample0 /= 10.0;
    spectr[ms40][b_num][512] = sample0;
}

/*
 * Сглаживание среднего спектра калибровки и поиск минимума, максимума
 */
void Calibrator2::Smooth(uint8_t ms40, uint8_t b_num, bool smoothEn)
{
    int k;
    for (int i = 0; i < FFT_LENGTH; i++) {
        if (smoothEn) {
            if (i <= FFT_LENGTH / 2 - IGNORE_ZEROZONE_SAMPLES ||
                i >= FFT_LENGTH / 2 + IGNORE_ZEROZONE_SAMPLES) {
                spectr_smooth[i] = 0;
                for (int j = i - SMOOTH_WIN / 2; j < i + SMOOTH_WIN / 2; j++) {
                    if (j < 0)
                        k = FFT_LENGTH + j;
                    else if (j >= FFT_LENGTH)
                        k = j - FFT_LENGTH;
                    else
                        k = j;
                    spectr_smooth[i] += spectr[ms40][b_num][k];
                }
                spectr_smooth[i] /= SMOOTH_WIN;
            }
            else {
                spectr_smooth[i] = spectr[ms40][b_num][i];
            }
        }
        else {
            spectr_smooth[i] = spectr[ms40][b_num][i];
        }
    }

    // Поиск максимума и минимума spectr_smooth
    spectr_smooth_max = spectr_smooth[CUT_AFC_POS];
    spectr_smooth_min = spectr_smooth[CUT_AFC_POS];
    for (int i = CUT_AFC_POS + 1; i < FFT_LENGTH - CUT_AFC_POS; i++) {
        if (spectr_smooth[i] > spectr_smooth_max) {
            spectr_smooth_max = spectr_smooth[i];
        }
        if (spectr_smooth[i] < spectr_smooth_min) {
            spectr_smooth_min = spectr_smooth[i];
        }
    }
}

/*
 * Вычисление коэффициентов, компенсирующих АЧХ
 */
void Calibrator2::CalcAfc(uint8_t ms40, uint8_t b_num)
{
    // Нормировка компенсационной характеристики АЧХ по краям рабочей полосы
    // спектра
    // 1) находим максмимумы спектра на 3 краевых отсчетах справа и слева;
    // 2) находим большее из двух краевых значений и относительно него проводим
    // нормировку.
    double level_norm, level_norm_left, level_norm_right;
    level_norm_left = spectr_smooth[CUT_AFC_POS - 2];
    level_norm_right = spectr_smooth[FFT_LENGTH - CUT_AFC_POS - 2];
    for (int i = CUT_AFC_POS - 2; i < CUT_AFC_POS + 2; i++) {
        if (spectr_smooth[i] > level_norm_left) {
            level_norm_left = spectr_smooth[i];
        }
    }
    for (int i = FFT_LENGTH - CUT_AFC_POS - 2;
         i < FFT_LENGTH - CUT_AFC_POS + 2; i++) {
        if (spectr_smooth[i] > level_norm_right) {
            level_norm_right = spectr_smooth[i];
        }
    }
    level_norm = (level_norm_left > level_norm_right)
                  ? level_norm_left : level_norm_right;

    // Вычисление компенсационной характеристики АЧХ
    for (int i = 0; i < CUT_AFC_POS; i++)
        comp_afc[ms40][b_num][i] = 1.0;
    for (int i = CUT_AFC_POS; i <= FFT_LENGTH - CUT_AFC_POS; i++)
        comp_afc[ms40][b_num][i] = level_norm / spectr_smooth[i];
    for (int i = FFT_LENGTH - CUT_AFC_POS + 1; i < FFT_LENGTH; i++)
        comp_afc[ms40][b_num][i] = 1.0;
}

/*
 * Нормировка рассчитанной характеристики в диапазот [0..2]
 */
void Calibrator2::normTo2()
{
    for (int k = 0; k < ACCUM_MODES; k++) {
        for (int j = 0; j < B_NUM_MAX; j++) {
            double max = 0;
            for (int i = 0; i < FFT_LENGTH; i++) {
                if (comp_afc[k][j][i] > max) {
                    max = comp_afc[k][j][i];
                }
            }
            if (max > 2) {
                for (int i = 0; i < FFT_LENGTH; i++) {
                    comp_afc[k][j][i] *= 2 / max;
                }
            }
        }
    }
}

//==============================================================================
