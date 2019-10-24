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
}

/*
 * Отправка кода команды (cmd) с сопутствующими параметрами (data)
 */
void PanelConnect::panelSendCmd(quint8 cmd, QByteArray data)
{
    cmd_send = cmd;
    data.prepend(QByteArray::fromRawData((char*)&cmd, 1));
    udpConnect->writeDatagram(data, dissIPAddr, UDP_PORT_TELEM_MODE_OUT);
}

/*
 * Команда 0xA0 - запрос версий ПО, CRC
 */
void PanelConnect::cmdGetVersion()
{
    panelSendCmd(PANEL_BUILD_MK);
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
void PanelConnect::cmdStartCalAfc(int cycles)
{
    QByteArray data;
    data.append(QByteArray::fromRawData((char*)&cycles, sizeof(int)));
    data.append(QByteArray::fromHex("01"));
    panelSendCmd(PANEL_AFCCAL_SET_CONTROL, data);
}

/*
 * Команда 0xD1 - запрос состяния калибровки поправок АЧХ
 */
void PanelConnect::cmdCalAfcGetStatus()
{
    panelSendCmd(PANEL_AFCCAL_GET_STATUS);
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
                break;
            case PANEL_TECH_MODE_SET:
                qDebug() << "Main mode params is setted...";
                cmdMainModeStart();
                break;
            case PANEL_MAIN_MODE_SET:
                qDebug() << "Main mode is started...";
                break;
            case PANEL_AFCCAL_SET_CONTROL:
                qDebug() << "Calibr process is started...";
                break;
            case PANEL_AFCCAL_GET_STATUS:
                calibrate_afc_cnt = *((int*)&data_rec[2]);
                cal_done = data_rec[18];
                emit cmdCalAfcStatusReady(calibrate_afc_cnt, cal_done);
                break;

            case PANEL_AFCCAL_GET_DATA:
                part_num_rec = *((quint16*)&data_rec[66]);
                if (part_num_rec == part_num_send) {
                    if (part_num_rec == 0) {
                        calData.resize(0);
                    }
                    calData.append(QByteArray::fromRawData(
                                       (char*)&data_rec[2], 64));
                    qDebug() << "Read part" << part_num_rec + 1 << "of" << 1024;
                    if (part_num_rec == 1023) {
                        emit cmdCalAfcDataReady(calData);
                    }
                    else {
                        cmdCalAfcGetData(part_num_rec + 1);
                    }
                }
                else {
                    qDebug() << "Error of reading SRAM rart number";
                }
                break;

            case PANEL_LAST_LOG_GET:
                lastLog = (EthLogErrorStructTypeDef*)&data_rec[2];
                qDebug("Last Error is: 0x%02X", lastLog->log.error);
                break;

            }
        }
        else if (cmd_rec_status == PANEL_ERROR)
            qDebug() << "Error";

    }
}
