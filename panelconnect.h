//==============================================================================
// (C) Copyright 2019 MIET
// Moscow, Zelenograd, Russia
//
// Device:      DISS
// Module:      MPR
// Component:   AFC calibration utility
// File:        panelconnect.h
// Function:    Ethernet communications class
// Notes:
// Author:      A.Lavrinenko
//==============================================================================
#ifndef PANELCONNECT_H
#define PANELCONNECT_H

#include <QObject>
#include <QUdpSocket>
#include "disstypedef.h"

//==============================================================================
/*
 * Объект этого класса обеспечивает выдачу технологических команд, необходимых
 * для процесса калибровки АЧХ, и получение ответа от МПР
 */
class PanelConnect : public QObject
{
    Q_OBJECT
public:
    explicit PanelConnect(QObject *parent = 0);
    void cmdGetVersion();                   // Техн. команда "0xA0"
    void cmdMainModeCheck();                // Техн. команда "0xAD"
    void cmdMainModeStart();                // Техн. команда "0xAC"
    void cmdMainModeSetParams();            // Техн. команда "0xB9"
    void cmdMainModeSetFpga4080(bool ms40); // Техн. команда "0xB6"
    void cmdGetLastLog();                   // Техн. команда "0xC9"
    void cmdCalAfcSetCtrl(int cycles);      // Техн. команда "0xC4"
    void cmdCalAfcGetStatus();              // Техн. команда "0xC5"
    void cmdCalAfcGetData(quint16 part_num);// Техн. команда "0xAE"
    void cmdCalAfcGetDataRepeat();  // Повторяет cmdCalAfcGetData() с последним
                                    // part_num в случае отсутствия ответа
    void cmdProgramResetMpr();              // Техн. команда "0xB9"
    void cmdSetBeamNum();
    quint8 getStatus();     // Вернуть состояние объекта PanelConnect
    quint8 getLogErr();     // Вернуть последний код ошибки (отказа) МПР

    // Коды состояний объекта PanelConnect
    enum {
        ST_CONNECT_FAIL = 0,            // Нет соединения по Ethernet
        ST_CONNECT_READY,               // Соединение по Ethernet есть
        ST_READY_TO_SET_4080MS,         // Готовность выставить режим 40/80 мс
        ST_READY_TO_START_CALIBR,       // Готовность запустить процесс
                                        // суммирования спектров
        ST_ACCUM_CALIBR_PERFOMING,      // МПР выполняет суммирование спектров
        ST_READY_TO_READ_CALIBR_DATA,   // МПР выполнил суммирование спектров
        ST_READING_DATA_PERFOMING,      // Выполняется чтение данных из SRAM
        ST_READING_DATA_ERROR,          // Ошибка чтения данных из SRAM
        ST_READING_DATA_DONE,           // Чтение данных из SRAM выполнено
        ST_TELEM_MODE,                  // МПР в режиме "телеметрии"
        ST_RESET_MPR,                   // МПР в состоянии программного сброса
        ST_LAST_LOG_ERR,                // МПР в отказе
        ST_LAST_LOG_SUCC                // МПР не в отказе
    };

private:
    QUdpSocket* udpConnect;     // Сокет для обмена по Ethernet
    const QHostAddress dissIPAddr = QHostAddress("192.168.1.163");  // IP МПРа
    quint8 cmd_send;            // Код последней отправленной техн. команды
    quint16 part_num_send;      // Номер последнего запрашиваемого блока данных
                                // командой cmdCalAfcGetData(part_num)
    QByteArray calData;         // Весь блок данных, прочитанный из SRAM
    quint8 status;              // Код состояния объекта
    quint8 logErr;              // Код ошибки при отказе МПР
    // Отправка кода команды + параметры (при необходимости, в завис-ти от cmd)
    void panelSendCmd(quint8 cmd, QByteArray data = QByteArray());
    // Обработка ответа на техн. команду
    void panelAnswerProcess(QByteArray datagramRec);

    static const quint16 UDP_PORT_IN = 45454;   // Порт от МПР
    static const quint16 UDP_PORT_OUT = 45454;  // Порт к МПР

signals:
    // Сигналы для функционирования GUI и алгоритма
    void panelStatus(quint8);
    void cmdCalAfcStatusReady(int, bool);
    void cmdCalAfcGetDataPartReady(quint16);
    void cmdCalAfcDataReady(QByteArray);

private slots:
    void processPendingDatagrams(); // Обработка принятых пакетов по Ethernet

};
//==============================================================================

#endif // PANELCONNECT_H
