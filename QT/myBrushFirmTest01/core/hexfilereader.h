#ifndef HEXFILEREADER_H
#define HEXFILEREADER_H

#include <QObject>

#include "../file/binaryfile.h"

class HexFileReader : public QObject
{
    Q_OBJECT
public:
    HexFileReader();
    ~HexFileReader();

    bool Open(QString file_path);
    void Close();

    //获得文件总片数计数
    quint16 GetTotalPieceCount();
    //获得一片文件的大小
    quint16 GetPieceDataSize();
    //获得指定片数的数据
    char* GetOnePieceDataAtIndex(quint32 page_index);
    //获得下一片数据
    char* GetNextPieceData();

private:
    //定义一片的长度位64位
    const static quint16 piece_array_len = 64;
    char piece_array[piece_array_len];


    BinaryFile binary_file_;

};

#endif // HEXFILEREADER_H
