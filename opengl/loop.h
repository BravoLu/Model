#include <iostream>
#include <cmath>
#include <fstream>
using namespace std;
float vertices[] = {
-0.203125,-0.203125,-0.203125,
0.203125,0.203125,-0.203125,
-0.203125,0.203125,0.203125,
0.203125,-0.203125,0.203125,
0,0.34375,0,
0,0,0.34375,
0.34375,0,0,
0,-0.34375,0,
-0.34375,0,0,
0,0,-0.34375,
0.15625,0.28125,-0.15625,
0.21875,0.21875,0.03125,
0.28125,0.15625,-0.15625,
-0.15625,0.28125,0.15625,
-0.15625,0.15625,0.28125,
0.03125,0.21875,0.21875,
0.21875,0.03125,0.21875,
0.15625,-0.15625,0.28125,
0.28125,-0.15625,0.15625,
-0.15625,-0.28125,-0.15625,
-0.21875,-0.21875,0.03125,
-0.28125,-0.15625,-0.15625,
0.15625,-0.28125,0.15625,
-0.03125,-0.21875,0.21875,
-0.21875,-0.03125,0.21875,
-0.28125,0.15625,0.15625,
-0.21875,0.03125,-0.21875,
-0.15625,-0.15625,-0.28125,
-0.21875,0.21875,-0.03125,
-0.03125,0.21875,-0.21875,
0.15625,0.15625,-0.28125,
0.03125,-0.21875,-0.21875,
0.21875,-0.03125,-0.21875,
0.21875,-0.21875,-0.03125,
};
int indices[] = {
2,11,13,
11,5,12,
13,12,7,
13,11,12,
5,14,16,
14,3,15,
16,15,6,
16,14,15,
7,17,19,
17,6,18,
19,18,4,
19,17,18,
7,12,17,
12,5,16,
17,16,6,
17,12,16,
1,20,22,
20,8,21,
22,21,9,
22,20,21,
8,23,24,
23,4,18,
24,18,6,
24,23,18,
9,25,26,
25,6,15,
26,15,3,
26,25,15,
9,21,25,
21,8,24,
25,24,6,
25,21,24,
1,22,28,
22,9,27,
28,27,10,
28,22,27,
9,26,29,
26,3,14,
29,14,5,
29,26,14,
10,30,31,
30,5,11,
31,11,2,
31,30,11,
10,27,30,
27,9,29,
30,29,5,
30,27,29,
1,28,20,
28,10,32,
20,32,8,
20,28,32,
10,31,33,
31,2,13,
33,13,7,
33,31,13,
8,34,23,
34,7,19,
23,19,4,
23,34,19,
8,32,34,
32,10,33,
34,33,7,
34,32,33,
};
const double pi = 3.14159265;
int initVertice = 34;
int numVertice = 35;
int numface = 64;
const int MAX = 1000;
int edgeVertice[MAX][MAX][3] ;
int newIndex[MAX];
float newVertice[MAX];

int addEdgeVertice(int v1Index,int v2Index, int v3Index)
{
    if(v1Index > v2Index)
    {
        int vTmp;
        vTmp = v1Index;
        v1Index = v2Index;
        v2Index = vTmp;
    }
    if(edgeVertice[v1Index-1][v2Index-1][0]== 0)
    {
        int newIndexOfVertice = numVertice;
        numVertice++;
        edgeVertice[v1Index-1][v2Index-1][0] = newIndexOfVertice;
        edgeVertice[v1Index-1][v2Index-1][1] = v3Index;
    }
    else
        edgeVertice[v1Index-1][v2Index-1][2] = v3Index;

    return edgeVertice[v1Index-1][v2Index-1][0];
}

float Sum(float *vertice, int len , int* adjVertice, int col)
{
    float ans = 0;
    for(int i = 0 ; i < len ; i++){
        int index = adjVertice[i];
        ans += vertice[(index-1)*3+col];
    }
    return ans;
}

void loop()
{
    int index = 0;
    for(int i = 0 ; i < numface ; i++)
    {
    int vaIndex, vbIndex, vcIndex, vpIndex, vqIndex , vrIndex;
    vaIndex = indices[3*i];
    vbIndex = indices[3*i+1];
    vcIndex = indices[3*i+2];

    vpIndex = addEdgeVertice(vaIndex,vbIndex,vcIndex);
    vqIndex = addEdgeVertice(vbIndex,vcIndex,vaIndex);
    vrIndex = addEdgeVertice(vaIndex,vcIndex,vbIndex);

    newIndex[3*index] = vaIndex;
    newIndex[3*index+1] = vpIndex;
    newIndex[3*index+2] = vrIndex;
    index ++;

    newIndex[3*index] = vpIndex;
    newIndex[3*index+1] = vbIndex;
    newIndex[3*index+2] = vqIndex;
    index ++;

    newIndex[3*index] = vrIndex;
    newIndex[3*index+1] = vqIndex;
    newIndex[3*index+2] = vcIndex;
    index ++;

    newIndex[3*index] = vrIndex;
    newIndex[3*index+1] = vpIndex;
    newIndex[3*index+2] = vqIndex;
    index ++;

    }

    cout << "indices:"<<endl;
    for(int i = 0 ; i < index; i++){
        cout << newIndex[3*i+0]-1 << " " << newIndex[3*i+1]-1 <<" " << newIndex[3*i+2]-1 <<endl;

    }
    cout << endl;
    ofstream outfile;
    outfile.open("dataoutput1.txt",ostream::app);
    outfile << "indices:"<<endl;
    for(int i = 0 ; i < index; i++){
        outfile << newIndex[3*i+0]-1 << "," << newIndex[3*i+1]-1 <<"," << newIndex[3*i+2]-1 <<"," << endl;
    }
    //新的边点位置
    int vNOpposite1Index ,vNOpposite2Index;
    for(int v1 = 1; v1 <= initVertice; v1++)
{
    for(int v2 = v1+1; v2 <= initVertice ;v2++)
    {
        int  vNIndex = edgeVertice[v1-1][v2-1][0];
        if(vNIndex != 0)
        {
            vNOpposite1Index = edgeVertice[v1-1][v2-1][1];
            vNOpposite2Index = edgeVertice[v1-1][v2-1][2];
            if(vNOpposite2Index == 0)
            {
            newVertice[3*vNIndex] = 1/2.0 * (vertices[3*(v1-1)] + vertices[3*(v2-1)]);
            newVertice[3*vNIndex+1] = 1/2.0 * (vertices[3*(v1-1)+1] + vertices[3*(v2-1)+1]);
            newVertice[3*vNIndex+2] = 1/2.0 * (vertices[3*(v1-1)+2] + vertices[3*(v2-1)+2]);
            }
            else
            {
            newVertice[3*vNIndex] = 3/8.0 * (vertices[3*(v1-1)] + vertices[3*(v2-1)]) + 1/8.0*(vertices[3*(vNOpposite1Index-1)] + vertices[3*(vNOpposite2Index-1)]);
            newVertice[3*vNIndex+1] = 3/8.0 * (vertices[3*(v1-1)+1] + vertices[3*(v2-1)+1]) + 1/8.0*(vertices[3*(vNOpposite1Index-1)+1] + vertices[3*(vNOpposite2Index-1)+1]);
            newVertice[3*vNIndex+2] = 3/8.0 * (vertices[3*(v1-1)+2] + vertices[3*(v2-1)+2]) + 1/8.0*(vertices[3*(vNOpposite1Index-1)+2] + vertices[3*(vNOpposite2Index-1)+2]);
            }
        }
    }
}

int adjVertice[MAX][20];
int length[MAX];  //邻接表长度
for(int v =  1; v <= initVertice; v++)
{
    int end = 0 ;
    for(int vTmp = 1 ; vTmp <= initVertice ; vTmp++)
    {
        if((v<vTmp && edgeVertice[v-1][vTmp-1][0] != 0)||(v>vTmp&&edgeVertice[vTmp-1][v-1][0]!= 0))
            adjVertice[v-1][end++] = vTmp;
    }
    length[v-1] = end;
}


for(int  v = 1 ; v <= initVertice ; v++)
{
    int len = length[v-1] ;
    int end = 0;
    int adjBondaryVertices[200];
    for(int i = 0 ; i < len ; i++)
    {
        int vi = adjVertice[v-1][i] ;
        if( ((vi>v)&&(edgeVertice[v-1][vi-1][2]==0)) || ((vi<v)&&(edgeVertice[vi-1][v-1][2]==0)) )
        {
            adjBondaryVertices[end++] = vi;
        }
    }

    if(end == 2)
    {

        newVertice[3*v] = 6.0/8.0*vertices[3*(v-1)] + 1.0/8.0*(vertices[3*(adjBondaryVertices[0]-1)]+vertices[3*(adjBondaryVertices[1]-1)]);
        newVertice[3*v+1] = 6.0/8.0*vertices[3*(v-1)+1] + 1.0/8.0*(vertices[3*(adjBondaryVertices[0]-1)+1]+vertices[3*(adjBondaryVertices[1]-1)+1]);
        newVertice[3*v+2] = 6.0/8.0*vertices[3*(v-1)+2] + 1.0/8.0*(vertices[3*(adjBondaryVertices[0]-1)+2]+vertices[3*(adjBondaryVertices[1]-1)+2]);
    }
    else
    {
        double beta = 1.0/len*(5.0/8.0 - (3.0/8.0 + 1.0/4.0*cos(2*pi/len)) *(3.0/8.0 + 1.0/4.0*cos(2*pi/len)) );
        newVertice[3*v] = (1-len*beta)*vertices[3*(v-1)] + beta*Sum(vertices,len,adjVertice[v-1],0);
        newVertice[3*v+1] = (1-len*beta)*vertices[3*(v-1)+1] + beta*Sum(vertices,len,adjVertice[v-1],1);
        newVertice[3*v+2] = (1-len*beta)*vertices[3*(v-1)+2] + beta*Sum(vertices,len,adjVertice[v-1],2);
    }

}

    cout<<"vertex:"<<endl;
    for(int i = 1 ; i < numVertice ; i++)
    {
        cout << newVertice[3*i] << " " << newVertice[3*i+1] << " " << newVertice[3*i+2] <<endl;
    }

    outfile<<"vertex:"<<endl;
    for(int i = 1 ; i < numVertice ; i++)
    {
        outfile << newVertice[3*i] << "," << newVertice[3*i+1] << "," << newVertice[3*i+2] <<","<<endl;
    }
    outfile.close();
}

