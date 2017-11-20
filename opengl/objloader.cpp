#include "ObjLoader.h"
#include <fstream>
#include <iostream>
using namespace std;

ObjLoader::ObjLoader(string filename)
{
    string line;
    fstream f;
    f.open(filename,ios::in);
    vNum = 0;
    fNum = 0;
    if (!f.is_open()){
        qDebug("Something Went Wrong When Opening Objfiles\n" );
    }

    while(!f.eof()){

        getline(f,line);
        //cout << line <<endl;
        vector<string> parameters;

        string tailMark = " ";

        string ans = "";

        line = line.append(tailMark);

        for (int i = 0; i < line.length(); i++) {

            char ch = line[i];
            if (ch != ' ') {
                ans+=ch;
            }
            else {
                //cout << ans << endl;
                parameters.push_back(ans);
                ans = "";
            }
        }

//        if (parameters.size() != 4) {
//            cout << "the size is not correct" << endl;
//        }
        //qDebug("2");
        if(parameters.size() == 4){
               if (parameters[0] == "v") {

                        vector<GLfloat>Point;

                        for (int i = 1; i < 4; i++) {
                            GLfloat xyz = atof(parameters[i].c_str());
                            vSets.push_back(xyz);
                            vNum++;
                        }

//                       vSets.push_back(Point);
//                        vNum++;
                    }

                    else if (parameters[0] == "f") {

                        //vector<GLint>vIndexSets;

                        for (int i = 1; i < 4; i++){
                            string x = parameters[i];
                            string ans = "";
                            for (int j = 0; j < x.length(); j++) {
                                char ch = x[j];
                                if (ch != '/') {
                                    ans += ch;
                                }
                                else {
                                    break;
                                }
                            }
                            //qDebug("1");
                            //cout<<ans <<endl;
                            GLint index = atof(ans.c_str());
                            index--;

                            fSets.push_back(index);
                            fNum++;
                        }

//                       fSets.push_back(vIndexSets);
//                        fNum++;
                    }
                     else if (parameters[0] == "vn") {

                               for (int i = 1; i < 4; i++) {
                                   GLfloat vnxyz = atof(parameters[i].c_str());
                                   vnSets.push_back(vnxyz);
                               }

                           }

                           else if (parameters[0] == "vt") {

                               for (int i = 1; i < 4; i++) {
                                   GLfloat vnxyz = atof(parameters[i].c_str());
                                   vtSets.push_back(vnxyz);
                               }

                   }
        }
        else if(parameters.size() == 5)
        {
            if(parameters[0] == "f")
            {
                for (int i = 1; i < 5; i++){
                    string x = parameters[i];
                    string ans = "";
                    for (int j = 0; j < x.length(); j++) {
                        char ch = x[j];
                        if (ch != '/') {
                            ans += ch;
                        }
                        else {
                            break;
                        }
                    }
                    GLint index = atof(ans.c_str());
                    index--;

                    FSets.push_back(index);
                    FNum++;
                }
            }
        }
    }
    f.close();

}


