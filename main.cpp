#include <cstdio>
#include <cstring>
#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>
using namespace std;

/* ========================================================
 *                        常量声明                          =
 * =======================================================*/
const int kMaxN = 20;
const int kStateCount = (1 << kMaxN);
const float kINF = 1e9 + 7;
const float kLongitude2Meter = 111319.55; // 一经度对应多少米
const float kLatitude2Meter = 85000.00; // 一纬度对应多少米

/* ========================================================
 *                        数据类型                          =
 * =======================================================*/
class Node{
    float x_position;
    float y_position;

public:
    Node(float x, float  y){// 构造函数
        this->x_position = x;
        this->y_position = y;
    }
    float getDistance(const Node &other_point) const{// 获取两点之间的距离，当前使用曼哈顿距离
        return kLongitude2Meter * abs(this->y_position - other_point.y_position) +
               kLatitude2Meter * abs(this->x_position - other_point.x_position);
        /*return sqrt(abs(this->y_position - other_point.y_position) * abs(this->y_position - other_point.y_position) +
               abs(this->x_position - other_point.x_position) * abs(this->x_position - other_point.x_position));*/
    }
};


/* ========================================================
 *                        数据声明                          =
 * =======================================================*/
// 动态规划数组
float dp[kStateCount][kMaxN];
vector<int> path[kStateCount][kMaxN];

/* ========================================================
 *                        函数声明                          =
 * =======================================================*/
/**
 * 给出文件名file_name，自动读取文件中的数据
 * @param file_name 得到的文件名
 * @param points 在文件中读取到的点
 * @return 返回数据的条目数量
 */
int getData(const char *file_name, vector<Node*> &points){
    freopen(R"(C:\Users\wangsy\Desktop\CQU_MCM_SCHOOL_COMPETITION\dataset\test_data0.txt)", "r", stdin);
    int res = 0;
    float get_x, get_y;
    while(cin >> get_x >> get_y){
        points.push_back(new Node(get_x, get_y));
        ++ res;
    }
    cin.clear();
    freopen("CON", "r", stdin);
    return res;
}

/**
 * 获取地图上点与点之间的距离
 * @param points 点的集合，是一个vector
 * @param distance 写入的数据，是一个双重vector
 */
void getDistance(const vector<Node*> &points, vector<vector<float>> &distance){
    distance.resize(points.size());
    for(int from = 0; from < points.size(); ++ from){
        distance[from].resize(points.size());
        for(int to = 0; to < points.size(); ++ to){
            distance[from][to] = points[from]->getDistance(*points[to]);
        }
    }
}

/**
 * 初始化动态规划数组以及路径数组
 */
void initDp() {
    for(int row = 0; row < kStateCount; ++ row){
        for(int col = 0; col < kMaxN; ++ col){
            dp[row][col] = kINF;
            path[row][col].clear();
        }
    }
}

/**
 * 运行动态规划主体
 * @param point_number 点的数量
 * @param distance 点之间的距离
 * @return 返回解决方案的路径
 */
vector<int>* doDp(int point_number, const vector<vector<float>> &distance) {
    // 初始化所有起点的状态
    for(int point_id = 0; point_id < point_number; ++point_id){
        dp[point_id][0] = 0;
        path[point_id][0].push_back(point_id);
    }
    for (int cur_state = 1; cur_state < (1 << point_number); cur_state++) {
        if((cur_state & ((1 << 11) - 1)) == ((1 << 11) - 1)) cout << cur_state << endl;
        for (int cur_point = 1; cur_point < point_number; cur_point++) {
            if (cur_state & (1 << cur_point))
                continue;
            for (int try_to_point = 0; try_to_point < kMaxN; try_to_point++) {
                if (cur_state & (1 << try_to_point)) {
                    if(dp[cur_state][try_to_point] + distance[try_to_point][cur_point] < dp[(1 << cur_point) | cur_state][cur_point]){
                        dp[(1 << cur_point) | cur_state][cur_point] = dp[cur_state][try_to_point] + distance[try_to_point][cur_point];
                        path[(1 << cur_point) | cur_state][cur_point] = path[cur_state][try_to_point];
                        path[(1 << cur_point) | cur_state][cur_point].push_back(cur_point);
                    }
                    dp[(1 << cur_point) | cur_state][cur_point] = min(dp[(1 << cur_point) | cur_state][cur_point],
                                                      dp[cur_state][try_to_point] + distance[try_to_point][cur_point]);
                }
            }
        }
    }

    auto *res = new vector<int>();
    float ans = 1e9 + 7;
    for (int point_id = 0; point_id < point_number; point_id++){
        if(dp[(1 << point_number) - 1][point_id] + distance[point_id][0]< ans){
            ans = dp[(1 << point_number) - 1][point_id] + distance[point_id][0];
            res->clear();
            for(int push_point_id: path[(1 << point_number) - 1][point_id]){
                res->push_back(push_point_id);
            }
        }
    }
    cout << ans << endl;
    return res;
}

void solve(const char * filename){
    // 定义数据
    int point_number = 0;
    vector<Node*> points;
    vector<vector<float>> distance;
    vector<int>* res;

    point_number = getData("w", points);
    getDistance(points, distance);
    initDp();
    res = doDp(point_number, distance);
    for(auto i: *res){
        cout << i << endl;
    }cout << endl;
}

int main () {
    solve("dataset/test_data0.txt");
    return 0;
}
