#include <QCoreApplication>
#include <QDebug>
#include <QString>
#include <QByteArray>
#include <QTextCodec>
#include <QDataStream>
#include "repmod.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    CRep *pqtRep = new CRep();
    QString sqtName = "b2b.rep";
    pqtRep->CreateReport(sqtName, 2);

    QString sData = "TESTTEST";
    QString sStrOtl = "Отладка";




    // Создаем кодек для Windows-1251
    QTextCodec *codec = QTextCodec::codecForName("Windows-1251");
  //  QTextCodec *codec1 = QTextCodec::codecForName("UTF-8");

    QByteArray windows = sStrOtl.toLocal8Bit().data();

    qDebug() <<"windows "<< windows;



    // Преобразование QString в QByteArray с использованием кодека
    QByteArray windows1251ByteArray = codec->fromUnicode(sStrOtl);
//    QByteArray windows1251ByteArray2 = codec1->fromUnicode(sStrOtl);

    //pqtRep->AddExtData("Sou2", "Sou2", "Sou2", 1, sData.toUtf8());

    pqtRep->AddToReport("Sou2", "Eve2", "Job2", 1000, windows);

//    // pqtRep->AddToReport("SEMS", "ENGI", "JCOM", 98, windows1251ByteArray2);
//    pqtRep->AddToReport("SEMS", "ENGI", "JCOM", 1, "ТЯ");
//    pqtRep->AddToReport("SEMS", "ENGI", "JCOM", 1, "Ков");
//    pqtRep->AddToReport("PIPA","BiBA","LIPA", 2, "999967asd21345");
//    pqtRep->AddToReport("SEMS", "ENGI", "JCOM", 1, "Власенко");

    pqtRep->DestroyReport();
    delete pqtRep;
    qDebug() << "doiila suda;";
    //return a.exec();
}


