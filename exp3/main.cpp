#include "G.h"

#include <vector>
#include <cstdlib>
#include <assert.h>
#include <ctime>
#include <iostream>
#include <cstdio>
#include <cstring>
#include <cmath>
#include <sys/time.h>

using namespace std;

#define TIME_TICK_START gettimeofday( &ttv, NULL ); tts = ttv.tv_sec + double(ttv.tv_usec) / 1000000;
#define TIME_TICK_END gettimeofday( &ttv, NULL ); tte = ttv.tv_sec + double(ttv.tv_usec) / 1000000;
#define TIME_TICK_PRINT(T) printf("%s RESULT: %.2lf S\r\n", (#T), tte - tts );
#define TIME_TICK (tte-tts)
struct timeval ttv;
double tts, tte;

template <typename T>
class Mat
{
private:
    T* content;
    T** ptr;
public:
    Mat() {
        content = NULL;
        ptr = NULL;
    }
    ~Mat() {
        if (content) delete[] content;
        if (ptr) delete[] ptr;
    }
    void init(int n, int m) {
        content = new T[n*m];
        ptr = new T*[n];
        for(int i = 0; i < n; i ++) ptr[i] = content+i*m;
    }
    T* operator [](int x) {
        return ptr[x];
    }
    const T* operator [](int x) const {
        return ptr[x];
    }
};
struct Task
{
    int S, T;
};
struct Point
{
    double x, y;
};
struct Result
{
    double timeused;
    double rst;
};

const double threshold = 0.8;
const double EPS = 1e-4;
const int MAXN = int(1e4)+10;

vector<Point> points;
vector<Task> riders;
vector<Task> drivers;
int match[MAXN];

double Distance(const int p1, const int p2);

void loadTask(); // 载入数据

double KM(int N, const Mat<double>& dis);

Result solveNormal(); // 常规做法

Result solveQuick(); // 优化

Result solveEuclid(); // 欧几里德做法

int main(int argc, char* argv[])
{
    loadTask();

    // solveNormal();
    // Result euclid_rst = solveEuclid();
    // cout << euclid_rst.rst << endl;

    loadFiles();
    // Result normal_rst = solveNormal();
    // cout << normal_rst.rst << endl;

    Result quick_rst = solveQuick();
    cout << quick_rst.rst << endl;

    return 0;
}

double Distance(const int p1, const int p2)
{
    return sqrt((points[p1].x-points[p2].x)*(points[p1].x-points[p2].x)+(points[p1].y-points[p2].y)*(points[p1].y-points[p2].y));
}

void loadTask()
{
    points.clear();
    {
        FILE *fd = fopen("driver_dataset/BJ.node", "r");
        int id;
        double x, y;
        while(fscanf(fd, "%d%lf%lf", &id, &x, &y) == 3)
        {
            points.push_back((Point){x, y});
        }
        fclose(fd);
    }

    riders.clear();
    {
        FILE *fd = fopen("rider.txt", "r");
        int S, T;
        while(fscanf(fd, "%d%d", &S, &T) == 2)
        {
            riders.push_back((Task){S, T});
        }
        fclose(fd);
    }

    drivers.clear();
    {
        FILE *fd = fopen("driver.txt", "r");
        int S, T;
        while(fscanf(fd, "%d%d", &S, &T) == 2)
        {
            drivers.push_back((Task){S, T});
        }
        fclose(fd);
    }

    cout << "points : " << points.size() << " riders : " << riders.size() << " drivers: " << drivers.size() << " " << double(clock())/CLOCKS_PER_SEC << endl;
}

static double lx[MAXN], ly[MAXN];
static int usedx[MAXN], usedy[MAXN];
double delta;

bool dfs(int u, const int N, const Mat<double>& dis)
{
    usedx[u] = true;
    for(int i = 0;i < N; i ++)
        if(dis[u][i]+EPS >= lx[u] + ly[i] && !usedy[i])
        {
            usedy[i] = true;
            if(!match[i] || dfs(match[i], N, dis))
            {
                match[i] = u;
                return true;
            }
        }else if(lx[u]+ly[i]-dis[u][i]>EPS)
            delta=min(delta, lx[u]+ly[i]-dis[u][i]);
    return false;
}
double KM(int N, const Mat<double>& dis)
{
    memset(match, 0, sizeof(match));
    for(int i = 0; i < N; i ++)
    {
        lx[i] = -1e10;
        ly[i] = 0;
        for(int j = 0;j < N; j ++)
            lx[i] = max(lx[i], dis[i][j]);
    }
    for(int i = 0; i < N; i ++)
    {
        while(1)
        {
            memset(usedx, 0, sizeof(usedx));
            memset(usedy, 0, sizeof(usedy));
            delta = 1e10;
            if(dfs(i, N, dis)) break;
            
            delta = 1e10;
            for(int x = 0; x < N; x ++) if (usedx[x])
                for(int y = 0; y < N; y ++) if (!usedy[y])
                    delta = min(lx[x]+ly[y]-dis[x][y], delta);
            for(int j = 0; j < N; j ++)
            {
                if(usedx[j]) lx[j] -= delta;
                if(usedy[j]) ly[j] += delta;
            }
        }
        if (i % 50 == 0)
            cout << "KM : " << i << " " << double(clock())/CLOCKS_PER_SEC << endl;
    }
    double ans=0;
    for(int i = 0; i < N; i ++)
        ans += lx[i] + ly[i];
    return ans;
}

Result solveNormal()
{
    const int N = riders.size();
    assert(riders.size() == N);
    assert(drivers.size() == N);

    Mat<double> dis;
    Mat<int> A, B, C;
    dis.init(N, N);
    A.init(N, N);
    B.init(N, N);
    C.init(N, N);

    TIME_TICK_START
    for(int j = 0; j < N; j ++)
    {
        for(int i = 0; i < N; i ++)
        {
            A[i][j] = search(drivers[j].S, riders[i].S);
        }
        if (j % 50 == 0) cout << "dis1 : " << j << " " << double(clock())/CLOCKS_PER_SEC << endl;
    }
    for(int i = 0; i < N; i ++)
    {
        for(int j = 0; j < N; j ++)
        {
            B[i][j] = search(riders[i].S, riders[i].T);
        }
        if (i % 50 == 0) cout << "dis2 : " << i << " " << double(clock())/CLOCKS_PER_SEC << endl;
    }
    for(int i = 0; i < N; i ++)
    {
        for(int j = 0; j < N; j ++)
        {
            C[i][j] = search(riders[i].T, drivers[j].T);
        }
        if (i % 50 == 0) cout << "dis3 : " << i << " " << double(clock())/CLOCKS_PER_SEC << endl;
    }
    for(int i = 0; i < N; i ++)
        for(int j = 0; j < N; j ++)
        {
            if (B[i][j] >= (A[i][j]+B[i][j]+C[i][j])*threshold)
                dis[i][j] = double(B[i][j])/(A[i][j]+B[i][j]+C[i][j]);
            else dis[i][j] = 0;
        }
    double rst = KM(N, dis);

    TIME_TICK_END
    TIME_TICK_PRINT(solveNormal)
    return (Result){TIME_TICK, rst};
}

Result solveQuick() // 优化
{
    const int N = riders.size();
    assert(riders.size() == N);
    assert(drivers.size() == N);

    Mat<double> dis;
    Mat<int> A, B, C;
    Mat<bool> possible;
    dis.init(N, N);
    A.init(N, N);
    B.init(N, N);
    C.init(N, N);
    possible.init(N, N);

    TIME_TICK_START
    const double MUL = 2.0/sqrt(2.0);
    for(int i = 0; i < N; i ++)
        for(int j = 0; j < N; j ++)
        {
            double A = Distance(drivers[j].S, riders[i].S);
            double B = Distance(riders[i].S, riders[i].T);
            double C = Distance(riders[i].T, drivers[j].T);
            possible[i][j] = MUL*B/(A+B+C)>=threshold;
        }
    for(int j = 0; j < N; j ++)
    {
        for(int i = 0; i < N; i ++)
        {
            if (possible[i][j]) A[i][j] = search(drivers[j].S, riders[i].S);
        }
        if (j % 50 == 0) cout << "dis1 : " << j << " " << double(clock())/CLOCKS_PER_SEC << endl;
    }
    for(int i = 0; i < N; i ++)
    {
        for(int j = 0; j < N; j ++)
        {
            if (possible[i][j]) B[i][j] = search(riders[i].S, riders[i].T);
        }
        if (i % 50 == 0) cout << "dis2 : " << i << " " << double(clock())/CLOCKS_PER_SEC << endl;
    }
    for(int i = 0; i < N; i ++)
    {
        for(int j = 0; j < N; j ++)
        {
            if (possible[i][j]) C[i][j] = search(riders[i].T, drivers[j].T);
        }
        if (i % 50 == 0) cout << "dis3 : " << i << " " << double(clock())/CLOCKS_PER_SEC << endl;
    }
    for(int i = 0; i < N; i ++)
        for(int j = 0; j < N; j ++)
        {
            if (possible[i][j] && B[i][j] >= (A[i][j]+B[i][j]+C[i][j])*threshold)
                dis[i][j] = double(B[i][j])/(A[i][j]+B[i][j]+C[i][j]);
            else dis[i][j] = 0;
        }
    double rst = KM(N, dis);

    TIME_TICK_END
    TIME_TICK_PRINT(solveQuick)
    return (Result){TIME_TICK, rst};
}

Result solveEuclid() // 欧几里德做法
{
    const int N = riders.size();
    assert(riders.size() == N);
    assert(drivers.size() == N);

    TIME_TICK_START
    Mat<double> dis;
    dis.init(N, N);
    for(int i = 0; i < N; i ++)
        for(int j = 0; j < N; j ++)
        {
            double A = Distance(drivers[j].S, riders[i].S);
            double B = Distance(riders[i].S, riders[i].T);
            double C = Distance(riders[i].T, drivers[j].T);
            if (B/(A+B+C)>=threshold)
                dis[i][j] = B/(A+B+C);
            else dis[i][j] = 0;
        }
    double rst = KM(N, dis);

    TIME_TICK_END
    TIME_TICK_PRINT(solveEuclid)
    return (Result){TIME_TICK, rst};
}