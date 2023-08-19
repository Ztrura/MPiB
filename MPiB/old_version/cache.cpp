//#include <fstream>
//#include <iostream>
//#include <cmath>
//#include "ObjLoader.h"
//using namespace std;
//
//ObjLoader::ObjLoader(string filename)
//{
//    string line;
//    fstream f;
//    f.open(filename, ios::in);
//    if (!f.is_open()) {
//        cout << "failed to open the file " << endl;
//    }
//    while (!f.eof()) {
//        getline(f, line);
//        //cout << line.c_str() << endl;
//        //cout << line.length() << endl;
//        vector<string> parameters;
//        string ans;
//        line = line.append(" ");
//        //cout << line.c_str() << endl;
//        //cout << line.length() << endl;
//        for (int i = 0; i < line.length(); i++) {
//            char ch = line[i];
//            if (ch != ' ') {
//                ans += ch;
//                //cout << ans << endl;
//            }
//            else {
//                parameters.push_back(ans);
//                ans = "";
//            }
//        }
//        //cout << parameters[0] << endl;
//        //cout << parameters.size() << endl;
//        if (parameters.size() != 4) {
//            //cout << "the obj file format is invalid" << endl;
//        }
//        else {
//            if (parameters[0] == "v") { //顶点
//                vector<GLfloat> Point;
//                for (int i = 1; i < 4; i++) {
//                    GLfloat xyz = atof(parameters[i].c_str());//atof是ascII to float的缩写, 它将ascII字符串转换为相应的单精度浮点数
//                    Point.push_back(xyz);
//                }
//                vSets.push_back(Point);
//                if (totalNum < winsize) {
//                    GLfloat tmpr = sqrt(Point[0] * Point[0] + Point[2] * Point[2]);
//                    init_size += tmpr;
//                }
//                totalNum++;
//            }
//            else {
//                vector<GLfloat> gap;
//                vSets.push_back(gap);
//            }
//        }
//    }
//    cout << "init_size: " << init_size << endl;
//    crtvalue = init_size;
//    for (int i = winsize; i < vSets.size(); i++) {
//        if (!ifOutlier(i)) {
//            goodvSets.push_back(vSets[i]);
//        }
//        else {
//            badvSets.push_back(vSets[i]);
//        }
//    }
//    cout << "totalNum: " << totalNum << endl;
//    f.close();
//}
//
//void ObjLoader::Draw() {
//    glDisable(GL_LIGHTING);
//    glPointSize(5.0f);
//    glBegin(GL_POINTS);
//    for (int i = 0; i < vSets.size(); i++) {
//        glColor3f(1.0, 0.0, 0.0);
//        float x = 0.1 * vSets[i][0], y = 0.1 * vSets[i][1], z = 0.1 * vSets[i][2];
//        glVertex3f(x, y, z);
//    }
//    glEnd();
//    glEnable(GL_LIGHTING);
//}
//
//bool ObjLoader::ifOutlier(int index) {
//    GLfloat oldvalue = crtvalue;
//    GLfloat tmp1 = sqrt(vSets[index - 10][0] * vSets[index - 10][0] + vSets[index - 10][2] * vSets[index - 10][2]);
//    GLfloat tmp2 = sqrt(vSets[index][0] * vSets[index][0] + vSets[index][2] * vSets[index][2]);
//    crtvalue -= tmp1;
//    crtvalue += tmp2;
//    GLfloat delta = abs(crtvalue - oldvalue) / oldvalue;
//    if (delta > 0.9) {
//        cout << delta << endl;
//        index++;
//        cout << "index: " << index << endl;
//        return false;
//    }
//    return true;
//}
//
//void ObjLoader::removeOutlier() {
//    for (int i = winsize; i < vSets.size(); i++) {
//        if (!ifOutlier(i)) {
//            goodvSets.push_back(vSets[i]);
//        }
//        else {
//            badvSets.push_back(vSets[i]);
//        }
//    }
//}
