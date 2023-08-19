#include <fstream>
#include <iostream>
#include <cmath>
#include "ObjLoader.h"
using namespace std;

ObjLoader::ObjLoader(string filename)
{
    string line;
    fstream f;
    f.open(filename, ios::in);
    if (!f.is_open()) {
        cout << "failed to open the file " << endl;
    }
    while (!f.eof()) {
        getline(f, line);
        //cout << line.c_str() << endl;
        //cout << line.length() << endl;
        vector<string> parameters;
        string ans;
        line = line.append(" ");
        //cout << line.c_str() << endl;
        //cout << line.length() << endl;
        for (int i = 0; i < line.length(); i++) {
            char ch = line[i];
            if (ch != ' ') {
                ans += ch;
                //cout << ans << endl;
            }
            else {
                parameters.push_back(ans);
                ans = "";
            }
        }
        //cout << parameters[0] << endl;
        //cout << parameters.size() << endl;
        if (parameters.size() != 4) {
            //cout << "the obj file format is invalid" << endl;
            //if (!startflag) {
            //    flagindex++;
            //}
            if (parameters[0] == "cstype") {
                //cout << "cstype" << endl;
                flagindex++;
            }
        }
        else {
            if (parameters[0] == "v") { //顶点
                flags.push_back(flagindex);
                startflag = 1;
                vector<GLfloat> Point;
                for (int i = 1; i < 4; i++) {
                    GLfloat xyz = atof(parameters[i].c_str());//atof是ascII to float的缩写, 它将ascII字符串转换为相应的单精度浮点数
                    Point.push_back(xyz);
                }
                vSets.push_back(Point);
                if (totalNum < winsize) {
                    GLfloat tmpr = sqrt(Point[0] * Point[0] + Point[2] * Point[2]);
                    init_size += tmpr;
                    if (tmpr > maxR) {
                        maxR = tmpr;
                    }
                    goodvSets.push_back(Point);
                }
                totalNum++;
            }
        }
    }
    crtvalue = init_size;
    for (int i = winsize; i < totalNum; i++) {
        if (ifOutlier(i)) {
            goodvSets.push_back(vSets[i]);
            float r = sqrt(vSets[i][0] * vSets[i][0] + vSets[i][1] * vSets[i][1] + vSets[i][2] * vSets[i][2]);
            float phi = asin(sqrt(vSets[i][0] * vSets[i][0] + vSets[i][2] * vSets[i][2]) / r);
            if (phi > maxp) {
                maxp = phi;
            }
            if (phi < minp) {
                minp = phi;
            }
        }
        else {
            badvSets.push_back(vSets[i]);
        }
    }
    //cout << "init_size: " << init_size << endl;
    cout << "goodNum: " << goodvSets.size() << endl;
    cout << "badNum: " << badvSets.size() << endl;
    cout << "totalNum: " << totalNum << endl;
    cout << "max radius: " << maxR << endl;
    f.close();
}

bool ObjLoader::ifOutlier(int index) {
    GLfloat oldvalue = crtvalue;
    GLfloat tmp1 = sqrt(vSets[index - winsize][0] * vSets[index - winsize][0] + vSets[index - winsize][2] * vSets[index - winsize][2]);
    GLfloat tmp2 = sqrt(vSets[index][0] * vSets[index][0] + vSets[index][2] * vSets[index][2]);
    if (tmp2 > maxR) {
        maxR = tmp2;
    }
    crtvalue -= tmp1;
    crtvalue += tmp2;
    GLfloat delta = abs(crtvalue - oldvalue) / oldvalue;
    if (delta > 0.1) {
        //cout << "delta: " << delta << endl;
        return false;
    }
    return true;
}

void ObjLoader::Draw() {
    glDisable(GL_LIGHTING);
    glPointSize(3.0f);
    glBegin(GL_POINTS);
    for (int i = 0; i < totalNum; i++) {
        glColor3f(0.3, 0.3, 0.3);
        float x = 0.1 * vSets[i][0], y = 0.1 * vSets[i][1], z = 0.1 * vSets[i][2];
        glVertex3f(x, y, z);
    }
    glEnd();
    glEnable(GL_LIGHTING);
}

void ObjLoader::checkOutlier() {
    glDisable(GL_LIGHTING);
    glPointSize(3.0f);
    glBegin(GL_POINTS);
    for (int i = 0; i < goodvSets.size(); i++) {
        glColor3f(0.3, 0.3, 0.3);
        float x = 0.1 * goodvSets[i][0], y = 0.1 * goodvSets[i][1], z = 0.1 * goodvSets[i][2];
        glVertex3f(x, y, z);
    }
    glEnd();
    glColor3f(1.0, 0.0, 0.0);
    glPointSize(10.0f);
    glBegin(GL_POINTS);
    for (int i = 0; i < badvSets.size(); i++) {
        float x = 0.1 * badvSets[i][0], y = 0.1 * badvSets[i][1], z = 0.1 * badvSets[i][2];
        glVertex3f(x, y, z);
    }
    glEnd();
    glEnable(GL_LIGHTING);
}

void ObjLoader::removeOutlier() {
    glDisable(GL_LIGHTING);
    glPointSize(3.0f);
    glBegin(GL_POINTS);
    for (int i = 0; i < goodvSets.size(); i++) {
        glColor3f(0.3, 0.3, 0.3);
        float x = 0.1 * goodvSets[i][0], y = 0.1 * goodvSets[i][1], z = 0.1 * goodvSets[i][2];
        glVertex3f(x, y, z);
        GLfloat newtmpr = sqrt(goodvSets[i][0] * goodvSets[i][0] + goodvSets[i][2] * goodvSets[i][2]);
        if (newmaxR < newtmpr) {
            newmaxR = newtmpr;
        }
    }
    maxR = newmaxR;
    if (!removed) {
        cout << "new max radius: " << maxR << endl;
    }
    removed = 1;
    glEnd();
    glEnable(GL_LIGHTING);
}

void ObjLoader::pointReshape() {
    glDisable(GL_LIGHTING);
    glPointSize(3.0f);
    glBegin(GL_POINTS);
    if (minp < 0.0000001) {
        minp = 0.0000001;
    }
    float q = pow(maxp / minp, 1 / (vSets.size() - 1.0));
    //if (key == 1) {
    //    cout << "maxphi: " << maxp << ", minphi: " << minp << ", size: " << vSets.size() << endl;
    //    cout << "q: " << q << endl;
    //    key = 0;
    //}
    for (int i = 0; i < goodvSets.size(); i++) {
        glColor3f(0.3, 0.3, 0.3);
        float r = sqrt(goodvSets[i][0] * goodvSets[i][0] + goodvSets[i][1] * goodvSets[i][1] + goodvSets[i][2] * goodvSets[i][2]);
        float phi = asin(sqrt(goodvSets[i][0] * goodvSets[i][0] + goodvSets[i][2] * goodvSets[i][2]) / r);
        float tmpn = ((phi - minp) / (maxp - minp)) * vSets.size();
        phi = minp * pow(q, tmpn - 1);
        float tmpr = 2 * phi * r / 3.1415926f;
        float theta = acos(abs(goodvSets[i][0]) / sqrt(goodvSets[i][0] * goodvSets[i][0] + goodvSets[i][2] * goodvSets[i][2]));
        if (goodvSets[i][0] < 0 && goodvSets[i][2] >= 0) {
            theta = 3.1415926f - theta;
        }
        else if (goodvSets[i][0] < 0 && goodvSets[i][2] < 0) {
            theta = 3.1415926f + theta;
        }
        else if (goodvSets[i][0] >= 0 && goodvSets[i][2] < 0) {
            theta = 2.0 * 3.1415926f - theta;
        }
        float x = 0.1 * tmpr * cos(theta), z = 0.1 * tmpr * sin(theta);
        glVertex3f(x, 0, z);
    }
    /*cout << "over" << endl;*/
    glEnd();
    glEnable(GL_LIGHTING);
}

void ObjLoader::Map() {
    glDisable(GL_LIGHTING);
    glPointSize(3.0f);
    glBegin(GL_POINTS);
    if (minp < 0.0000001) {
        minp = 0.0000001;
    }
    float q = pow(maxp / minp, 1 / (vSets.size() - 1.0));
    for (int i = 0; i < goodvSets.size(); i++) {
        glColor3f(0.3, 0.3, 0.3);
        float x0 = 0.1 * goodvSets[i][0], y0 = 0.1 * goodvSets[i][1], z0 = 0.1 * goodvSets[i][2];
        glVertex3f(x0, y0, z0);
        glColor3f(1.0, 0.0, 0.0);
        float r = sqrt(goodvSets[i][0] * goodvSets[i][0] + goodvSets[i][1] * goodvSets[i][1] + goodvSets[i][2] * goodvSets[i][2]);
        float phi = asin(sqrt(goodvSets[i][0] * goodvSets[i][0] + goodvSets[i][2] * goodvSets[i][2]) / r);
        float tmpn = ((phi - minp) / (maxp - minp)) * vSets.size();
        phi = minp * pow(q, tmpn - 1);
        float tmpr = 2 * phi * r / 3.1415926f;
        float theta = acos(abs(goodvSets[i][0]) / sqrt(goodvSets[i][0] * goodvSets[i][0] + goodvSets[i][2] * goodvSets[i][2]));
        if (goodvSets[i][0] < 0 && goodvSets[i][2] >= 0) {
            theta = 3.1415926f - theta;
        }
        else if (goodvSets[i][0] < 0 && goodvSets[i][2] < 0) {
            theta = 3.1415926f + theta;
        }
        else if (goodvSets[i][0] >= 0 && goodvSets[i][2] < 0) {
            theta = 2.0 * 3.1415926f - theta;
        }
        float x = 0.1 * tmpr * cos(theta), z = 0.1 * tmpr * sin(theta);
        glVertex3f(x, 0, z);
        glEnd();
        glBegin(GL_LINES);
        glColor3f(1.0, 1.0, 1.0);
        glVertex3f(x0, y0, z0);
        glVertex3f(x, 0, z);
        glEnd();
        glBegin(GL_POINTS);
    }
    /*cout << "over" << endl;*/
    glEnd();
    glEnable(GL_LIGHTING);
}

void ObjLoader::Link() {
    glDisable(GL_LIGHTING);
    glPointSize(3.0f);
    glBegin(GL_POINTS);
    if (!removed) {
        //cout << "new max radius: " << maxR << endl;
        for (int i = 0; i < vSets.size(); i++) {
            glColor3f(0.0, 0.0, 0.0);
            float x0 = 0.1 * vSets[i][0], y0 = 0.1 * vSets[i][1], z0 = 0.1 * vSets[i][2];
            glVertex3f(x0, y0, z0);
        }
        glEnd();
        int tmpflag = 0;
        glBegin(GL_LINE_STRIP);
        float oldy = 0.0;
        for (int i = 0; i < vSets.size(); i++) {
            if (flags[i] == tmpflag) {
                glColor3f(0.3, 0.3, 0.3);
                float x = 0.1 * vSets[i][0], y = 0.1 * vSets[i][1], z = 0.1 * vSets[i][2];
                bool ifZero = ((abs(oldy - 0.0) < 0.01) && (abs(y - 0.0) < 0.01));
                if (!ifZero) {
                    glVertex3f(x, y, z);
                }
                oldy = y;
            }
            else {
                glEnd();
                tmpflag++;
                glBegin(GL_LINE_STRIP);
            }
        }
    }
    else {
        for (int i = 0; i < goodvSets.size(); i++) {
            glColor3f(0.0, 0.0, 0.0);
            float x0 = 0.1 * goodvSets[i][0], y0 = 0.1 * goodvSets[i][1], z0 = 0.1 * goodvSets[i][2];
            glVertex3f(x0, y0, z0);
        }
        glEnd();
        int tmpflag = 0;
        glBegin(GL_LINE_STRIP);
        float oldy = 0.0;
        //int count = 0;
        for (int i = 0; i < goodvSets.size(); i++) {
            if (flags[i] == tmpflag) {
                //count++;
                glColor3f(0.3, 0.3, 0.3);
                float x = 0.1 * goodvSets[i][0], y = 0.1 * goodvSets[i][1], z = 0.1 * goodvSets[i][2];
                bool ifZero = ((abs(oldy - 0.0) < 0.01) && (abs(y - 0.0) < 0.01));
                if (!ifZero) {
                    glVertex3f(x, y, z);
                }
                oldy = y;
            }
            else {
                //cout << "count: " << count << endl;
                //count = 0;
                glEnd();
                tmpflag++;
                glBegin(GL_LINE_STRIP);
            }
        }
    }
    glEnd();
    glEnable(GL_LIGHTING);
}

void ObjLoader::lineReshape() {
    glDisable(GL_LIGHTING);
    glPointSize(3.0f);
    glBegin(GL_POINTS);
    if (minp < 0.0000001) {
        minp = 0.0000001;
    }
    float q = pow(maxp / minp, 1 / (vSets.size() - 1.0));
    for (int i = 0; i < goodvSets.size(); i++) {
        glColor3f(1.0, 1.0, 1.0);
        float r = sqrt(goodvSets[i][0] * goodvSets[i][0] + goodvSets[i][1] * goodvSets[i][1] + goodvSets[i][2] * goodvSets[i][2]);
        float phi = asin(sqrt(goodvSets[i][0] * goodvSets[i][0] + goodvSets[i][2] * goodvSets[i][2]) / r);
        float tmpn = ((phi - minp) / (maxp - minp)) * vSets.size();
        phi = minp * pow(q, tmpn - 1);
        float tmpr = 2 * phi * r / 3.1415926f;
        float theta = acos(abs(goodvSets[i][0]) / sqrt(goodvSets[i][0] * goodvSets[i][0] + goodvSets[i][2] * goodvSets[i][2]));
        if (goodvSets[i][0] < 0 && goodvSets[i][2] >= 0) {
            theta = 3.1415926f - theta;
        }
        else if (goodvSets[i][0] < 0 && goodvSets[i][2] < 0) {
            theta = 3.1415926f + theta;
        }
        else if (goodvSets[i][0] >= 0 && goodvSets[i][2] < 0) {
            theta = 2.0 * 3.1415926f - theta;
        }
        float x = 0.1 * tmpr * cos(theta), z = 0.1 * tmpr * sin(theta);
        glVertex3f(x, 0, z);
    }
    glEnd();
    int tmpflag = 0;
    glLineWidth(2.0f);
    glBegin(GL_LINE_STRIP);
    float oldy = 0.0;
    for (int i = 0; i < goodvSets.size(); i++) {
        if (flags[i] == tmpflag) {
            glColor3f(0.3, 0.3, 0.3);
            float r = sqrt(goodvSets[i][0] * goodvSets[i][0] + goodvSets[i][1] * goodvSets[i][1] + goodvSets[i][2] * goodvSets[i][2]);
            float phi = asin(sqrt(goodvSets[i][0] * goodvSets[i][0] + goodvSets[i][2] * goodvSets[i][2]) / r);
            float tmpn = ((phi - minp) / (maxp - minp)) * vSets.size();
            phi = minp * pow(q, tmpn - 1);
            float tmpr = 2 * phi * r / 3.1415926f;
            float theta = acos(abs(goodvSets[i][0]) / sqrt(goodvSets[i][0] * goodvSets[i][0] + goodvSets[i][2] * goodvSets[i][2]));
            if (goodvSets[i][0] < 0 && goodvSets[i][2] >= 0) {
                theta = 3.1415926f - theta;
            }
            else if (goodvSets[i][0] < 0 && goodvSets[i][2] < 0) {
                theta = 3.1415926f + theta;
            }
            else if (goodvSets[i][0] >= 0 && goodvSets[i][2] < 0) {
                theta = 2.0 * 3.1415926f - theta;
            }
            float x = 0.1 * tmpr * cos(theta), z = 0.1 * tmpr * sin(theta);
            glVertex3f(x, 0, z);
        }
        else {
            glEnd();
            tmpflag++;
            glBegin(GL_LINE_STRIP);
        }
    }
    /*cout << "over" << endl;*/
    glEnd();
    glLineWidth(1.0f);
    glEnable(GL_LIGHTING);
}

void ObjLoader::lineReshape_WP() {
    glDisable(GL_LIGHTING);
    glPointSize(5.0f);
    if (minp < 0.0000001) {
        minp = 0.0000001;
    }
    float q = pow(maxp / minp, 1 / (vSets.size() - 1.0));
    int tmpflag = 0;
    glLineWidth(2.0f);
    glBegin(GL_LINE_STRIP);
    float oldy = 0.0;
    for (int i = 0; i < goodvSets.size(); i++) {
        if (flags[i] == tmpflag) {
            glColor3f(0.3, 0.3, 0.3);
            float r = sqrt(goodvSets[i][0] * goodvSets[i][0] + goodvSets[i][1] * goodvSets[i][1] + goodvSets[i][2] * goodvSets[i][2]);
            float phi = asin(sqrt(goodvSets[i][0] * goodvSets[i][0] + goodvSets[i][2] * goodvSets[i][2]) / r);
            float tmpn = ((phi - minp) / (maxp - minp)) * vSets.size();
            phi = minp * pow(q, tmpn - 1);
            float tmpr = 2 * phi * r / 3.1415926f;
            float theta = acos(abs(goodvSets[i][0]) / sqrt(goodvSets[i][0] * goodvSets[i][0] + goodvSets[i][2] * goodvSets[i][2]));
            if (goodvSets[i][0] < 0 && goodvSets[i][2] >= 0) {
                theta = 3.1415926f - theta;
            }
            else if (goodvSets[i][0] < 0 && goodvSets[i][2] < 0) {
                theta = 3.1415926f + theta;
            }
            else if (goodvSets[i][0] >= 0 && goodvSets[i][2] < 0) {
                theta = 2.0 * 3.1415926f - theta;
            }
            float x = 0.1 * tmpr * cos(theta), z = 0.1 * tmpr * sin(theta);
            glVertex3f(x, 0, z);
        }
        else {
            glEnd();
            tmpflag++;
            glBegin(GL_LINE_STRIP);
        }
    }
    /*cout << "over" << endl;*/
    glEnd();
    glLineWidth(1.0f);
    glEnable(GL_LIGHTING);
}