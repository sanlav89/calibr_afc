#include "panelconnect.h"
#include <QDebug>

/*
 * Создание объекта
 */
PanelConnect::PanelConnect(QObject *parent) : QObject(parent)
{
    udpConnect = new QUdpSocket(this);
    udpConnect->bind(UDP_PORT_TELEM_MODE_IN, QUdpSocket::ShareAddress);
    dissIPAddr = QHostAddress("192.168.1.163");
    connect(udpConnect, SIGNAL(readyRead()),
            this, SLOT(processPendingDatagrams()));
    status = ST_CONNECT_FAIL;
    logErr = 0x0;
}

/*
 * Отправка кода команды (cmd) с сопутствующими параметрами (data)
 */
void PanelConnect::panelSendCmd(quint8 cmd, QByteArray data)
{
    cmd_send = cmd;
    data.prepend(QByteArray::fromRawData((char*)&cmd, 1));
    udpConnect->writeDatagram(data, dissIPAddr, UDP_PORT_TELEM_MODE_OUT);
//    qDebug() << data.toHex();
}

/*
 * Команда 0xA0 - запрос версий ПО, CRC
 */
void PanelConnect::cmdGetVersion()
{
    status = ST_CONNECT_FAIL;
    panelSendCmd(PANEL_BUILD_MK);
}

/*
 * Команда 0xAD - Проверка находится ли МПР в "технологическом боевом" режиме
 */
void PanelConnect::cmdMainModeCheck()
{
    panelSendCmd(PANEL_MAIN_MODE_GET);
}

/*
 * Команда 0xAC - Выход в "технологический боевой" режим
 */
void PanelConnect::cmdMainModeStart()
{
    panelSendCmd(PANEL_MAIN_MODE_SET, QByteArray::fromHex("01"));
}

/*
 * Команда 0xB9 - выставить параметры для "технологического боевого" режима
 */
void PanelConnect::cmdMainModeSetParams()
{
    TechModeUnion mainModeParams;
    mainModeParams.f.missStartControl = 1;
    mainModeParams.f.missLog = 1;
    mainModeParams.f.banChange40_80 = 1;
    mainModeParams.f.banChangeIm = 1;
    mainModeParams.f.resetMK = 0;
    mainModeParams.f.banFpga40ms = 0;
    mainModeParams.f.banUseDAngles = 0;
    mainModeParams.f.startFpgaIn40 = 0;
    panelSendCmd(PANEL_TECH_MODE_SET,
                 QByteArray::fromRawData((char*)&mainModeParams.v, 1));
}

/*
 * Команда 0xB6 - выставить режим накопления сигнала в ПЛИС (40 или 80 мс)
 */
void PanelConnect::cmdMainModeSetFpga4080(bool ms40)
{
    QByteArray data = ms40 ? QByteArray::fromHex("00") :
                             QByteArray::fromHex("01");
    panelSendCmd(PANEL_40_80_SET, data);
}

/*
 * Команда 0xC9 - прочитать последний код ошибки (если ошибок не было - 0x00)
 */
void PanelConnect::cmdGetLastLog()
{
    panelSendCmd(PANEL_LAST_LOG_GET);
}

/*
 * Команда 0xD0 - начать калибровку поправок АЧХ
 */
void PanelConnect::cmdCalAfcSetCtrl(int cycles)
{
    QByteArray data;
    calibr_afc_s calibr_afc;
    calibr_afc.get_status_only = 0;
    calibr_afc.calibr_en = 1;
    calibr_afc.cnt_max = cycles;
    data.append(QByteArray::fromRawData((char*)&calibr_afc,
                                        sizeof(calibr_afc_s)));
    panelSendCmd(PANEL_AFCCAL_SETCTRL, data);
}

void PanelConnect::cmdProgramResetMpr()
{
    TechModeUnion mainModeParams;
    mainModeParams.f.missStartControl = 0;
    mainModeParams.f.missLog = 0;
    mainModeParams.f.banChange40_80 = 0;
    mainModeParams.f.banChangeIm = 0;
    mainModeParams.f.resetMK = 1;
    mainModeParams.f.banFpga40ms = 0;
    mainModeParams.f.banUseDAngles = 0;
    mainModeParams.f.startFpgaIn40 = 0;
    panelSendCmd(PANEL_TECH_MODE_SET,
                 QByteArray::fromRawData((char*)&mainModeParams.v, 1));
    status = ST_RESET_MPR;
}

/*
 * Команда 0xD0 - начать калибровку поправок АЧХ
 */
void PanelConnect::cmdCalAfcGetStatus()
{
    panelSendCmd(PANEL_AFCCAL_GETSTAT);
}

/*
 * Команда 0xD2 - запрос анных спектров калибровки
 */
void PanelConnect::cmdCalAfcGetData(quint16 part_num)
{
    part_num_send = part_num;
    panelSendCmd(PANEL_AFCCAL_GET_DATA,
                 QByteArray::fromRawData((char*)&part_num, 2));
}

void PanelConnect::cmdCalAfcGetDataRepeat()
{
    cmdCalAfcGetData(part_num_send);
}

quint8 PanelConnect::getStatus()
{
    return status;
}

quint8 PanelConnect::getLogErr()
{
    return logErr;
}

/*
 * Чтение ответного пакета из сокета и обработка ответа
 */
void PanelConnect::processPendingDatagrams()
{
    QByteArray datagramRec;
    // Чтение пакета из сокета
    while (udpConnect->hasPendingDatagrams()) {
        datagramRec.resize(udpConnect->pendingDatagramSize());
        udpConnect->readDatagram(datagramRec.data(), datagramRec.size());
    }
    // Обработка ответа
    panelAnswerProcess(datagramRec);
}

/*
 * Обработчик ответа от МПР
 */
void PanelConnect::panelAnswerProcess(QByteArray datagramRec)
{
    int calibrate_afc_cnt;
    EthLogErrorStructTypeDef* lastLog;
    bool cal_done = false;
    quint16 part_num_rec;

    quint8* data_rec = (quint8*)datagramRec.data();
    cmd_rec = data_rec[0];
    cmd_rec_status = data_rec[1];
//    qDebug() << datagramRec.toHex();
    if (cmd_rec == cmd_send) {
        // Check error in answer
        if (cmd_rec_status == PANEL_DONE) {
            //            qDebug() << "Success";
            // Answer processing
            switch (cmd_rec) {
            case PANEL_BUILD_MK:
                qDebug() << "Version is read";
                cmdMainModeCheck();
                status = ST_CONNECT_READY;
                break;
            case PANEL_MAIN_MODE_GET:
                if ((data_rec[2] >> 0) & 0x1) {
                    status = ST_READY_TO_SET_4080MS;
                } else if ((data_rec[2] >> 1) & 0x1) {
                    status = ST_TELEM_MODE;
                }
                break;
            case PANEL_TECH_MODE_SET:
                if (status == ST_RESET_MPR) {
                    status = ST_CONNECT_FAIL;
                } else {
                    qDebug() << "Main mode params is setted...";
                    cmdMainModeStart();
                }
                break;
            case PANEL_MAIN_MODE_SET:
                qDebug() << "Main mode is started...";
                status = ST_READY_TO_SET_4080MS;
                break;
            case PANEL_40_80_SET:
                qDebug() << "Mode 40/80 is set...";
                status = ST_READY_TO_START_CALIBR;
                break;
            case PANEL_AFCCAL_SETCTRL:
                status = ST_ACCUM_CALIBR_PERFOMING;
                break;
            case PANEL_AFCCAL_GETSTAT:
                cal_done = data_rec[2];
                calibrate_afc_cnt = *((int*)&data_rec[3]);
                status = ST_ACCUM_CALIBR_PERFOMING;
                if (cal_done)
                    status = ST_READY_TO_READ_CALIBR_DATA;
                emit cmdCalAfcStatusReady(calibrate_afc_cnt, cal_done);
                break;
            case PANEL_AFCCAL_GET_DATA:
                part_num_rec = *((quint16*)&data_rec[2]);
                if (part_num_rec == part_num_send) {
                    if (part_num_rec == 0) {
                        calData.resize(0);
                    }
                    calData.append(QByteArray::fromRawData(
                                       (char*)&data_rec[4], 64));
                    qDebug() << "Read part" << part_num_rec + 1 << "of" << 512;
                    status = ST_READING_DATA_PERFOMING;
                    emit cmdCalAfcGetDataPartReady(part_num_rec);
                    if (part_num_rec == 511) {
                        emit cmdCalAfcDataReady(calData);
                        status = ST_READING_DATA_DONE;
                    }
                    else {
                        cmdCalAfcGetData(part_num_rec + 1);
                    }
                }
                else {
                    status = ST_READING_DATA_ERROR;
                    qDebug() << "Error of reading SRAM part number" << part_num_rec;
                }
                break;

            case PANEL_LAST_LOG_GET:
                lastLog = (EthLogErrorStructTypeDef*)&data_rec[2];
                logErr = lastLog->log.error;
                if (logErr) {
                    status = ST_LAST_LOG_ERR;
                } else {
                    status = ST_LAST_LOG_SUCC;
                }
                qDebug("Last Error is: 0x%02X", logErr);
                break;
            }

            emit panelStatus(status);
        }
        else if (cmd_rec_status == PANEL_ERROR) {
            qDebug() << "Error" << datagramRec.toHex();
        }

    }
}
