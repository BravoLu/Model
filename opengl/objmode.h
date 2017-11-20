#ifndef OBJMODE_H
#define OBJMODE_H
#include <QObject>
#include <QString>
#include <QStringList>
#include <QVector>
#include <QFile>
#include <QTextStream>

struct VertexData
{
    float position[3];
    float texcoord[2];
    float normal[3];
};

struct VertexPos
{
    float position[3];
};
struct VertexTex
{
    float coord[2];
};

struct VertexNor
{
    float normal[3];
};

struct VertexIndex
{
    int posIndex;
    int coordIndex;
    int normalIndex;
};

class ObjMode:QObject
{
public:
    ObjMode();
    ~ObjMode();

    bool loadObjModel(QString nFileStr , QVector<VertexData>& nVertexData , QVector<unsigned int>&index , QVector<VertexData>& nVertexData1 , QVector<unsigned int> & index1); //返回nVertexData 和index

private:
    QVector<VertexPos> m_VertexInfo;
    QVector<VertexTex> m_TextureInfo;
    QVector<VertexNor> m_NormalInfo;

    QVector<VertexIndex> r_VertexIndex;
    QVector<unsigned int> r_FaceIndex;
    QVector<VertexIndex> m_VertexIndex;
    QVector<unsigned int> m_FaceIndex;
};

#endif // OBJMODE_H
