#include "repmod.h"
#include <QDebug>
#include <QFile>
#include <QDataStream>
#include <QDateTime>
#include <cstring>

CRep::CRep() {
    std::memcpy(m_fhFileHead.cConst, "REPORT  ", 8);
    std::memcpy(m_fhFileHead.cVers, "0000", 4);
    m_fhFileHead.bKey[0] = 4; m_fhFileHead.bKey[1] = 3;
    m_fhFileHead.bKey[2] = 2; m_fhFileHead.bKey[3] = 1;
}

CRep::~CRep() {
    CloseFileReport();
}

// Открытие файла:	0-		Чтение
//					1,3 -	Создание нового
//					2-		Добавление записей
// Ответ:			-1 -	Нет такого файла
//					-2 -	Файл не REPORT
//					-3 -	Версия не та
//					-4 -	Длина прочитанного НЕ норм

int CRep::CreateReport(const QString &fileName, int iModeOpen) {
    bool bExistFile = TestFileExist(fileName);

    int result = -1;
    if (iModeOpen == 0) {
        m_file.setFileName(fileName);
        if (m_file.open(QIODevice::ReadOnly)) {
            result = ReadHeadFile();
        }
    } else if (iModeOpen == 1 || iModeOpen == 3 || (iModeOpen == 2 && !bExistFile)) {
        m_file.setFileName(fileName);
        if (m_file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
            WriteHeadFile();
            result = 1;
        }
    } else if (iModeOpen == 2) {
        m_file.setFileName(fileName);
        if (m_file.open(QIODevice::ReadWrite)) {
            result = ReadHeadFile();
        }
    }

    if (result == -2 || result == -3 || result == -4) {
        m_file.close();
    }

    return result;
}

void CRep::WriteHeadFile() {
    std::memcpy(m_fhFileHead.cVers, "0001", 4);

    QByteArray buffer;
    QDataStream stream(&buffer, QIODevice::WriteOnly);
    stream.setByteOrder(QDataStream::LittleEndian);
    stream.writeRawData(reinterpret_cast<char*>(&m_fhFileHead), sizeof(m_fhFileHead));
    m_file.write(buffer);
}

int CRep::ReadHeadFile() {
    int rez = -4;
    FILEHEADER dfh;
    QByteArray buffer = m_file.read(sizeof(dfh));
    if (buffer.size() == sizeof(dfh)) {
        QDataStream stream(buffer);
        stream.setByteOrder(QDataStream::LittleEndian);
        stream.readRawData(reinterpret_cast<char*>(&dfh), sizeof(dfh));

        if ((rez = TestHeadFile(&dfh)) > 0) {
            std::memcpy(&m_fhFileHead, &dfh, sizeof(dfh));
        }
    }
    return rez;

}

int CRep::TestHeadFile(pFILEHEADER pHead) {
    if (std::memcmp(pHead->cConst, "REPORT  ", 8) != 0)
        return -2;
    if (std::memcmp(pHead->cVers, "0001", 4) != 0 && std::memcmp(pHead->cVers, "0002", 4) != 0)
        return -3;
    return 1;
}

bool CRep::TestFileExist(const QString &fileName) {
    QFile file(fileName);
    return file.exists();
}

void CRep::AddExtData(const QString &sourceMnem, const QString &eventMnem, const QString &jobMnem,
                      unsigned int eventClass, const QString &data) {


    qDebug() <<"данные "<< data;
    REPHEADER rh;
    SetFileTime(&rh);
    SetUserPart(&rh, sourceMnem, eventMnem, jobMnem, eventClass);

    QByteArray encryptedHeader;
    EncryptData(QByteArray(reinterpret_cast<char*>(&rh), sizeof(rh)), encryptedHeader);

    int n_data = data.size();
    if (n_data > LEN_DATA_FOR_REP)
        n_data = LEN_DATA_FOR_REP;

    QByteArray lengthData;

    QDataStream s(&lengthData, QIODevice::WriteOnly);
    s.setByteOrder(QDataStream::LittleEndian);
    s << n_data;


    qDebug() <<"данные "<<n_data <<"    " << data;

    QByteArray encryptedData;

    EncryptData(data.toLocal8Bit().right(n_data), encryptedData);

    m_file.seek(m_file.size());

    m_file.write(encryptedHeader + lengthData + encryptedData);
}


void CRep::QDateTimeToByteArray(const QDateTime& qdt, unsigned char byteArray[8]) {
    // Преобразование QDateTime в TDateTime (Double)
    // В Delphi TDateTime считает дни с 30.12.1899
    QDateTime baseDate = QDateTime(QDate(1899, 12, 30), QTime(0, 0, 0));
    double days = baseDate.daysTo(qdt);
    double seconds = baseDate.time().msecsTo(qdt.time()) / 1000.0;
    double tDateTime = days + seconds / 86400.0;

    // Запись double в массив unsigned char
    std::memcpy(byteArray, &tDateTime, 8);
}





void CRep::SetFileTime(pREPHEADER pRH) {

    QDateTime qdt = QDateTime::currentDateTime();
    QDateTimeToByteArray(qdt, pRH->bRecordingTime);
}
void CRep::SetUserPart(pREPHEADER pRH, const QString &sourceMnem, const QString &eventMnem,
                       const QString &jobMnem, unsigned int eventClass) {


    std::memcpy(pRH->cSourceMnem, sourceMnem.toUtf8().constData(), sizeof(pRH->cSourceMnem));
    std::memcpy(pRH->cEventMnem, eventMnem.toUtf8().constData(), sizeof(pRH->cEventMnem));
    std::memcpy(pRH->cJobMnem, jobMnem.toUtf8().constData(), sizeof(pRH->cJobMnem));
    pRH->uEventClass = eventClass;
}

int CRep::EncryptData(const QByteArray &dataIn, QByteArray &dataOut) {
    dataOut.resize(dataIn.size());
    unsigned char keyByte = m_fhFileHead.bKey[0];
    for (int i = 0; i < dataIn.size(); ++i) {
        dataOut[i] = dataIn[i] ^ keyByte;
    }
    return dataIn.size();
}

void CRep::AddToReport(const QString &sourceMnem, const QString &eventMnem, const QString &jobMnem,
                       unsigned int eventClass, const QString &data) {
    AddExtData(sourceMnem, eventMnem, jobMnem, eventClass, data.toUtf8());
}

void CRep::CloseFileReport() {
    if (std::memcmp(m_fhFileHead.cVers, "0000", 4) != 0) {
        m_file.close();
        std::memcpy(m_fhFileHead.cVers, "0000", 4);
    }
}

void CRep::DestroyReport() {
    CloseFileReport();
}
