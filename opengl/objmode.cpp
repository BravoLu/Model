#include "objmode.h"
#include <iostream>
using namespace std;
ObjMode::ObjMode()
{

}
ObjMode::~ObjMode(){

}
bool ObjMode::loadObjModel(QString nFileStr,QVector<VertexData>&nVertexData, QVector<unsigned int> &index, QVector<VertexData>& nVertexData1 , QVector<unsigned int> & index1)
{
    QFile nObjFile(nFileStr);
    if(!nObjFile.exists())
            return false;

    if(!nObjFile.open(QFile::ReadOnly))
            return false;

    nVertexData.clear();
    index.clear();

    m_TextureInfo.clear();
    m_NormalInfo.clear();
    m_VertexInfo.clear();
    m_VertexIndex.clear();
    m_FaceIndex.clear();
    r_VertexIndex.clear();
    r_FaceIndex.clear();

    QTextStream nTextStream(&nObjFile);
    for(;!nTextStream.atEnd();)
    {
        QString nLineString = nTextStream.readLine();  //读第一行数据
        QByteArray nData = nLineString.toLocal8Bit();
        if(nData.length()<=2)
            continue;
        if(nData.at(0) == 'v')
        {
            QStringList nStrList = nLineString.split(" ");
            if(nData[1] == 't')
            {
                if(nStrList.count()<=0||nStrList.at(0)!="vt")
                    continue;
                VertexTex nTexture;
                for(int i = 1; i < nStrList.count(); i++)
                    nTexture.coord[i-1] = nStrList.at(i).toFloat();
                m_TextureInfo << nTexture;
            }
            else if(nData[1] == 'n')
            {
                if(nStrList.count() <= 0||nStrList.at(0)!= "vn")
                    continue;
                VertexNor nNormal;
                for(int i=1;i<nStrList.count(); i++)
                    nNormal.normal[i-1] = nStrList.at(i).toFloat();
                m_NormalInfo << nNormal;
            }
            else
            {
                if(nStrList.count() <=0||nStrList.at(0)!="v")
                    continue;
                VertexPos nPos;
                for(int i = 1; i < nStrList.count(); i++)
                    nPos.position[i-1] = nStrList.at(i).toFloat();
                m_VertexInfo <<nPos;
            }
        }
        else if(nData[0] == 'f')
        {
            QStringList nStrList = nLineString.split(" ");
            if(nStrList.count() <= 0||nStrList.at(0)!= "f")
                continue;
            if(nStrList.count() == 4)
            {
            for(int i = 1 ; i < nStrList.count(); i++)
            {
                VertexIndex nIndex;
                QString nFaceIndexStr = nStrList.at(i);
                QStringList nFaceList = nFaceIndexStr.split("/");
                nIndex.posIndex = nFaceList.at(0).toUInt() - 1;
                nIndex.coordIndex = nFaceList.at(1).toUInt() - 1;
                nIndex.normalIndex = nFaceList.at(2).toUInt() - 1;
                bool isFinded = false;
                for(int j = 0; j < m_VertexIndex.count() ; j++)
                {
                    if(nIndex.posIndex == m_VertexIndex.at(j).posIndex && nIndex.coordIndex==m_VertexIndex.at(j).coordIndex && nIndex.normalIndex==m_VertexIndex.at(j).normalIndex) //************
                    {
                        isFinded = true;
                        m_FaceIndex << j;
                        break;
                    }
                }
                if(!isFinded)
                {
                    m_VertexIndex << nIndex;
                    m_FaceIndex << m_VertexIndex.count() - 1;
                }
            }
            }
            else if(nStrList.count() == 5)
            {
                for(int i = 1 ; i < nStrList.count(); i++)
                {
                    VertexIndex nIndex;
                    QString nFaceIndexStr = nStrList.at(i);
                    QStringList nFaceList = nFaceIndexStr.split("/");
                    nIndex.posIndex = nFaceList.at(0).toUInt() - 1;
                    nIndex.coordIndex = nFaceList.at(1).toUInt() - 1;
                    nIndex.normalIndex = nFaceList.at(2).toUInt() - 1;
                    bool isFinded = false;
                    for(int j = 0; j < r_VertexIndex.count() ; j++)
                    {
                        if(nIndex.posIndex == r_VertexIndex.at(j).posIndex && nIndex.coordIndex==r_VertexIndex.at(j).coordIndex && nIndex.normalIndex==r_VertexIndex.at(j).normalIndex) //************
                        {
                            isFinded = true;
                            r_FaceIndex << j;
                            break;
                        }
                    }
                    if(!isFinded)
                    {
                        r_VertexIndex << nIndex;
                        r_FaceIndex << r_VertexIndex.count() - 1;
                    }
                }
            }
        }

    }
    nObjFile.close();
    //将m_VertexInfo拷贝到nVertexData;
    for(int i = 0 ; i < m_VertexIndex.count() ; ++i)
    {
        VertexData nPerVertextData;
        int posIndex = m_VertexIndex.at(i).posIndex;
        int textureIndex = m_VertexIndex.at(i).coordIndex;
        int normalIndex = m_VertexIndex.at(i).normalIndex;
        memcpy(nPerVertextData.position,m_VertexInfo.at(posIndex).position,sizeof(VertexPos));
        memcpy(nPerVertextData.texcoord,m_TextureInfo.at(textureIndex).coord,sizeof(VertexTex));
        memcpy(nPerVertextData.normal,m_NormalInfo.at(normalIndex).normal,sizeof(VertexNor));
        nVertexData.push_back(nPerVertextData);
    }
    for(int i = 0 ; i < r_VertexIndex.count() ; ++i)
    {
        VertexData nPerVertextData;
        int posIndex = r_VertexIndex.at(i).posIndex;
        int textureIndex = r_VertexIndex.at(i).coordIndex;
        int normalIndex = r_VertexIndex.at(i).normalIndex;
        memcpy(nPerVertextData.position,m_VertexInfo.at(posIndex).position,sizeof(VertexPos));
        memcpy(nPerVertextData.texcoord,m_TextureInfo.at(textureIndex).coord,sizeof(VertexTex));
        memcpy(nPerVertextData.normal,m_NormalInfo.at(normalIndex).normal,sizeof(VertexNor));
        nVertexData1.push_back(nPerVertextData);
    }
    for(int i = 0 ; i < r_FaceIndex.count(); i++)
    {
        index1.push_back(r_FaceIndex.at(i));
    }
    for(int i = 0 ; i < m_FaceIndex.count(); i++)
    {
        index.push_back(m_FaceIndex.at(i));
    }
    return true;
}

