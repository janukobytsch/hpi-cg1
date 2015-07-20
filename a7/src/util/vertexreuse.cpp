
#include <map>

#include "vertexreuse.h"

namespace 
{

    struct VertexData
    {
        VertexData(
            QVector3D & vertex)
        //,   QVector3D & normal
        //,   QVector2D & texc)
        :   vertex(vertex)
        //,   normal(normal)
        //,   texc(texc)
        {
        }

        QVector3D vertex;
        //QVector3D normal;
        //QVector2D texc;

        static const size_t numHashElements = 3;

        // foo hash -> better use c++11 with functional std::hash...
        static quint64 hash(float value)
        {
            return *reinterpret_cast<quint64*>(&value); 
        }

        quint64 hash() const 
        {
            quint64 hashValues[numHashElements] = 
            {
                hash(vertex.x() * 1000.f)
            ,   hash(vertex.y() * 1000.f)
            ,   hash(vertex.z() * 1000.f)
            //,   std::hash<float>()(normal.x())
            //,   std::hash<float>()(normal.y())
            //,   std::hash<float>()(normal.z())
            //,   std::hash<float>()(texc.x()  )
            //,   std::hash<float>()(texc.y()  )
            };

            quint64 resultHash = 0;

            for (int i = 0; i < numHashElements; ++i)
                resultHash ^= (hashValues[i] << i);

            return resultHash;
        }
    };

    typedef std::map<size_t, int> t_indicesByHash;
    typedef std::map<int, int> t_indicesByIndex;

    typedef std::vector<VertexData> t_vertexData;
}

void VertexReuse::optimize(
    t_vec3s & vertices
,   t_vec3s & normals
,   t_vec2s & texcs
,   t_uints & indices)
{
    const int numVertices = vertices.size();

    //bool normalsUsed = normals.size() == numVertices;
    //bool texcsUsed = texcs.size() == numVertices;

    //if(!normalsUsed) 
    //    normals.resize(numVertices);
    //if(!texcsUsed) 
    //    texcs.resize(numVertices);

    t_indicesByHash indicesByHash;
    t_indicesByIndex idReMap;
    
    t_vertexData data;
    int index(0);

    for(int i = 0; i < numVertices; ++i)
    {
        VertexData vd(vertices[i]);; //, normals[i], texcs[i]);
        const size_t hash = vd.hash();

        t_indicesByHash::iterator f = indicesByHash.find(hash);
        if(f != indicesByHash.end())
            idReMap[i] = f->second;
        else
        {
            indicesByHash[hash] = index;
            idReMap[i] = index;

            data.push_back(vd);
            ++index;
        }
    }

    vertices.resize(index);
    for(int i = 0; i < index; ++i)
        vertices[i] = data[i].vertex;

    //if(normalsUsed)
    //{
    //    normals.resize(index);
    //    for(int i = 0; i < index; ++i)
    //        normals[i] = data[i].normal;
    //}
    //if(texcsUsed)
    //{
    //    texcs.resize(index);
    //    for(int i = 0; i < index; ++i)
    //        texcs[i] = data[i].texc;
    //}

    const int size = indices.size();
    for (int i = 0; i < size; ++i)
        indices[i] = idReMap[i];
}
