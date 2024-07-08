#include <QCoreApplication>
#include <QDebug>
//#include <QString>
//#include <QByteArray>
#include <QTextCodec>
//#include <QDataStream>
#include "repmod.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    CRep *pqtRep = new CRep();
    QString sqtName = "b2b.rep";
    pqtRep->CreateReport(sqtName, 2);


    QTextCodec::setCodecForLocale(QTextCodec::codecForName("CP866"));

   // QString sData = "TESTTEST";
   QString sStrOtl = "русский или не русский!";


   qDebug() << sStrOtl <<  "BAZA!";



   // qDebug() <<sizeof(sStrOtl) << sStrOtl;


   // Преобразование QString в QByteArray с использованием кодека
  //  QByteArray windows1251ByteArray = codec->fromUnicode(sStrOtl);
//    QByteArray windows1251ByteArray2 = codec1->fromUnicode(sStrOtl);

  //  pqtRep->AddExtData("Sou2", "Sou2", "Sou2", 1, sData);

   // pqtRep->AddToReport("Sou2", "Eve2", "Job2", 2, sStrOtl);

    pqtRep->AddToReport("Sou2", "Eve2", "Job2", 2, "sStrOtl");
    pqtRep->AddToReport("Sou2", "Eve2", "Job2", 3, "data");
    pqtRep->AddToReport("Sou2", "Eve2", "Job2", 4, "Чис");

    pqtRep->AddToReport("SEMS", "ENGI", "JCOM", 1, "ТЯ");
    pqtRep->AddToReport("SEMS", "ENGI", "JCOM", 1, "Ков");
    pqtRep->AddToReport("PIPA","BiBA","LIPA", 2, "999967asd21345");
    pqtRep->AddToReport("SEMS", "ENGI", "JCOM", 1, "давай мясо, давай мясо!Ы");

    pqtRep->DestroyReport();
    delete pqtRep;
    qDebug() << "doiila suda;";
    //return a.exec();
}


