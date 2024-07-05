#ifndef REP_H
#define REP_H

#include <QFile>
#include <QByteArray>
#include <QDataStream>
#include <QDateTime>

#define LEN_DATA_FOR_REP 4096

typedef struct {
    char cConst[8];
    char cVers[4];
    unsigned char bKey[4];
} FILEHEADER, *pFILEHEADER;

typedef struct {
    unsigned char bRecordingTime[8];
    char cSourceMnem[4];
    char cEventMnem[4];
    char cJobMnem[4];
    unsigned uEventClass;
    unsigned char bRezerve[12];
} REPHEADER, *pREPHEADER;

class CRep {
public:
    CRep();
    ~CRep();

    int CreateReport(const QString &fileName, int iModeOpen);
    void AddExtData(const QString &sourceMnem, const QString &eventMnem, const QString &jobMnem,
                    unsigned int eventClass, const QString &data);
    void AddToReport(const QString &sourceMnem, const QString &eventMnem, const QString &jobMnem,
                     unsigned int eventClass, const QString &data);
    void DestroyReport();

private:

    void CloseFileReport();
    int EncryptData(const QByteArray &dataIn, QByteArray &dataOut);
    int DecryptData(const QByteArray &dataIn, QByteArray &dataOut);
    void SetUserPart(pREPHEADER pRH, const QString &sourceMnem, const QString &eventMnem,
                     const QString &jobMnem, unsigned int eventClass);

    void SetFileTime(pREPHEADER pRH);
    void QDateTimeToByteArray(const QDateTime& qdt, unsigned char byteArray[8]);

    bool TestFileExist(const QString &fileName);
    void WriteHeadFile();
    int ReadHeadFile();
    int TestHeadFile(pFILEHEADER pHead);



    QFile m_file;
    FILEHEADER m_fhFileHead;
    QByteArray m_buffer;
};

#endif // REP_H
