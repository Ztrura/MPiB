#include <vector>
#include <string>
#include <GL/glut.h>
using namespace std;

class ObjLoader {
public:
    ObjLoader() {};
    ObjLoader(string filename);
    void Draw();
    bool ifOutlier(int index);
    void checkOutlier();
    void removeOutlier();
    void pointReshape();
    void Map();
    void Link();
    void lineReshape();
    void lineReshape_WP();
    float maxR = -1000.0;
    float newmaxR = -1000.0;
private:
    int totalNum = 0;
    int winsize = 20;
    int startflag = 0;
    int flagindex = 0;
    int removed = 0;
    float minp = 1000.0, maxp = -1000.0;
    GLfloat init_size = 0;
    GLfloat crtvalue = 0;
    vector<vector<GLfloat>> vSets;
    vector<vector<GLfloat>> goodvSets;
    vector<vector<GLfloat>> badvSets;
    vector<int> flags;
};
