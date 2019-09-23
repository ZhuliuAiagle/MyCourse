#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <string.h>
#include <iostream>
#define MAX 1000

using namespace std;


// 向北行驶的车 1 需要象限 a 和 b
// 向西行驶的车 2 需要象限 b 和 c
// 向南行驶的车 3 需要象限 c 和 d
// 向东行驶的车 4 需要象限 d 和 a

enum dir_t {NORTH, WEST, SOUTH, EAST};

/*
car_t 类型结构变量，用于表征车辆的基本信息，
包括车辆在全局队列中的id，方向（从何处来）以及在本方向队列中的
序号（用于在判断条件变量的过程中决定某一辆处于等待状态的车辆是
否应当进入就绪态）
*/

struct car_t
{
    int id;
    dir_t dir;
    int order;
};
/*
Queue类，用于队列的实现。具有max_count（该队列曾经达到的最长长度，用于判断某一方向的车辆是否完全通过路口），now_order（
当前该方向处于或应当处于就绪或活跃状态的车辆的队内编号，用于判断某一方向的车辆是否完全通过路口），thread[MAX]
（用于存储该方向所有车辆线程的栈），thp（thread的栈指针），pi[MAX] （队列数组），front和end（队列的头指针和尾指针）
这几个成员变量以及构造函数、入队出队方法和count()函数（计算当前队列中成员的个数）。
Queue类用于实现各个方向的队列。与之互作的还有存储所有车辆对象的数组和存储所有车辆线程的数组。
Ptr是车辆对象数组和车辆线程数组共用的下标指针。

*/
class queue
{
    public:
        int max_count; // 该队列曾经达到的最长长度，用于判断某一方向的车辆是否完全通过路口
        int now_order = 0; // 当前该方向处于或应当处于就绪或活跃状态的车辆的队内编号，用于判断某一方向的车辆是否完全通过路口
        pthread_t thread[MAX]; // 用于存储该方向所有车辆线程的栈
        int thp = 0; // 栈指针
        car_t qi[MAX];
        int front = -1;
        int end = 0;
        //pi[MAX] （队列数组），front和end（队列的头指针和尾指针）
        //这几个成员变量以及构造函数、入队出队方法和count()函数（计算当前队列中成员的个数）。
        queue()
        {
            front = -1;
            end = 0;
            thp = 0;
            now_order = 0;

        }
        // 入队函数
        void enqueue(car_t car){
            qi[++front] = car;
        }
        // 出队函数
        car_t dequeue(){
            car_t err = {-1, NORTH, -1};
            return front >= end ? qi[end++] : err;
        }
        // 计数函数
        int count()
        {
            return front - end + 1;
        }
};

// 为路口四个资源加锁

int resource = 4;
pthread_mutex_t mutex_resource;
pthread_mutex_t mutex_a;
pthread_mutex_t mutex_b;
pthread_mutex_t mutex_c;
pthread_mutex_t mutex_d;


// 四个方向的队列
queue qe;
queue qw;
queue qs;
queue qn;

// 存储所有车辆信息的数组
car_t q[MAX];
pthread_t car_thread[MAX];
int ptr = 0;

// mutex_e-mutex_n四个变量用与给四个方向以及四个队列加锁
pthread_mutex_t mutex_e;
pthread_mutex_t mutex_w;
pthread_mutex_t mutex_s;
pthread_mutex_t mutex_n;

// cond_s和cond_n是负责阻塞和唤醒四个方向车辆线程的条件变量
pthread_cond_t cond_e; 
pthread_cond_t cond_w;
pthread_cond_t cond_s;
pthread_cond_t cond_n;

pthread_mutex_t mutex_ce;
pthread_mutex_t mutex_cw;
pthread_mutex_t mutex_cs;
pthread_mutex_t mutex_cn;


// firstNorth-firstEast，则是为了让左车低优先级车辆在右侧车辆让行的情况下跳过阻塞判断直接通行的标志变量
bool firstNorth = false;
bool firstSouth = false;
bool firstWest = false;
bool firstEast = false;
// c_firstNorth-c_firstWest是用于阻塞和唤醒因让行（包括右侧车和左侧车）而等待的线程的条件变量
pthread_cond_t c_firstNorth;
pthread_cond_t c_firstSouth;
pthread_cond_t c_firstEast;
pthread_cond_t c_firstWest;


// mutex_firstNorth-mutex_firstEast则是配合其使用的条件变量。
pthread_mutex_t mutex_firstNorth;
pthread_mutex_t mutex_firstSouth;
pthread_mutex_t mutex_firstEast;
pthread_mutex_t mutex_firstWest;

pthread_cond_t area_e;
pthread_cond_t area_s;
pthread_cond_t area_w;
pthread_cond_t area_n;

// Ready系列布尔变量用于表征对应方向是否有等待、就绪的车辆。
// 理论上说，除非该方向所有车全部通过路口，ready函数调用后对应方向的ready变量应当始终是true
// 在队列中有处于就绪状态的车辆
bool west_ready = false;
bool east_ready = false;
bool south_ready = false;
bool north_ready = false;

// 在路口处于已经发车的车辆
// Active系列布尔变量用于表征该方向当前是否有活跃车辆（正在过路口）
bool west_active = false;
bool north_active = false;
bool south_active = false;
bool east_active = false;
// now系列car_t变量用于表征当前各方向处于活跃状态的车辆信息
car_t now_north = {-1, NORTH, -1};
car_t now_east = {-1, EAST, -1};
car_t now_west = {-1, WEST, -1};
car_t now_south = {-1, SOUTH, -1};
// ready系列car_t变量用于表征当前已经进入就绪状态但还未占用路口的车辆信息。
car_t ready_north = {-1, NORTH, -1};
car_t ready_east = {-1, EAST, -1};
car_t ready_west = {-1, WEST, -1};
car_t ready_south = {-1, SOUTH, -1};


/*
Ddl_graph变量用于检测潜在死锁。它表征线程之间互相等待的关系，行是等待者，列是被等待者，
每当一个进程开始等待另一个进程，他就要将对应的项置为true，当等待结束，原进程重新将对应的项置为false。
它们的对应次序如代码注释所示。考虑本题的情境，因为本题的底图始终是一个环（无向环，因为每个线程最多只等待两个其他的线程）
所以原则上说，当ddl_graph中出现环，潜在死锁也就产生了。
之所以称这种死锁方式为“潜在死锁”，是因为它是由进程间互相等待造成的，并不是由资源占用造成的，四个方向的车并没有开进路口。
*/

bool ddl_graph[4][4] = 
{
//(waitee)    e     s      w       n         
            {false, false, false, false}, // e
            {false, false, false, false}, // n
            {false, false, false, false}, // w
            {false, false, false, false}  // s
                                          //(waitor)              
};
/*
它通过遍历ddl_graph矩阵检测当前进程中是否存在循环等待的情况，一旦出现循环等待，他将返回一个大于零的值来宣告潜在死锁的产生，
以便死锁处理线程进行调度。本题情境中有两种可能的循环等待：“优先环”，即每一辆车都等待其右边的车；以及“饥饿环”，即每一辆车都
试图谦让其左边的车。
*/
int test_loop()
{
    // 优先环
    if(ddl_graph[0][3] && ddl_graph[1][2] && ddl_graph[2][1] && ddl_graph[3][0])
        return 1;
    // 饥饿环
    if(ddl_graph[0][1] && ddl_graph[1][0] && ddl_graph[2][3] && ddl_graph[3][2])
        return 2;
    return 0;
}

void data_ini(char* s)
{
    // 数据初始化
    cout << "data ini..."<< endl;
    pthread_mutex_init(&mutex_resource, NULL);
    pthread_mutex_init(&mutex_a, NULL);
    pthread_mutex_init(&mutex_b, NULL);
    pthread_mutex_init(&mutex_c, NULL);
    pthread_mutex_init(&mutex_d, NULL);
    pthread_mutex_init(&mutex_e, NULL);
    pthread_mutex_init(&mutex_w, NULL);
    pthread_mutex_init(&mutex_s, NULL);
    pthread_mutex_init(&mutex_n, NULL);
    pthread_cond_init(&cond_e, NULL);
    pthread_cond_init(&cond_w, NULL);
    pthread_cond_init(&cond_s, NULL);
    pthread_cond_init(&cond_n, NULL);
    pthread_mutex_init(&mutex_ce, NULL);
    pthread_mutex_init(&mutex_cw, NULL);
    pthread_mutex_init(&mutex_cs, NULL);
    pthread_mutex_init(&mutex_cn, NULL);
    pthread_cond_init(&c_firstNorth, NULL);
    pthread_cond_init(&c_firstSouth, NULL);
    pthread_cond_init(&c_firstWest, NULL);
    pthread_cond_init(&c_firstEast, NULL);
    pthread_mutex_init(&mutex_firstEast, NULL);
    pthread_mutex_init(&mutex_firstNorth, NULL);
    pthread_mutex_init(&mutex_firstSouth, NULL);
    pthread_mutex_init(&mutex_firstWest, NULL);
    pthread_cond_init(&area_e, NULL);
    pthread_cond_init(&area_w, NULL);
    pthread_cond_init(&area_s, NULL);
    pthread_cond_init(&area_n, NULL);
    int ww = 0;
    int ss = 0;
    int nn = 0;
    int ee = 0;
    for(int i = 0; i < strlen(s); i++)
    {
        // 西方的车辆入队
        if(s[i] == 'w')
        {
            car_t temp = {i+1, WEST, ww++};
            qw.enqueue(temp);
            q[ptr++] = temp;
        }
        // 南方的车辆入队
        else if(s[i] == 's')
        {
            car_t temp = {i+1, SOUTH, ss++};
            qs.enqueue(temp);
            q[ptr++] = temp;
        }
        // 北方的车辆入队
        if(s[i] == 'n')
        {
            car_t temp = {i+1, NORTH, nn++};
            qn.enqueue(temp);
            q[ptr++] = temp;
        }
        // 东方的车辆入队
        if(s[i] == 'e')
        {
            car_t temp = {i+1, EAST, ee++};
            qe.enqueue(temp);
            q[ptr++] = temp;
        }
    }
    // 计算max_count
    qw.max_count = qw.count();
    qe.max_count = qe.count();
    qs.max_count = qs.count();
    qn.max_count = qn.count();
    cout << "data ini...finish"<< endl;
}
// ready函数。将所有存在车辆的方向ready变量全部置为true，表示这个方向还有车
void ready()
{
    cout << "ready..."<< endl;
    if(qs.count())
    {
        // 南方ready置为true
        south_ready = true;
    }
    if(qw.count())
    {
        // 西方ready置为true
        west_ready = true;
    }
    if(qn.count())
    {
        // 北方ready置为true
        north_ready = true;
    }
    if(qe.count())
    {
        // 东方ready置为true
        east_ready = true;
    }
    cout << "ready...finish"<< endl;
}
// start函数，通过向所有方向的小车发送启动信号来启动系统
void start()
{
    cout << "start..."<< endl;
    pthread_cond_signal(&cond_s);
    pthread_cond_signal(&cond_w);
    pthread_cond_signal(&cond_n);
    pthread_cond_signal(&cond_e);
    cout << "start...finish"<< endl;
}

// 死锁检测函数
void* check_ddl(void* para)
{
    int flag;
    while(1)
    {
        // 如果test_loop为真
        if(flag = test_loop()) // 这里确实是赋值！！！
        {
            cout << " protential deadlock occurs, signal north to go " << endl;
        }
        // 首先将所有数据强行归位，让北边先过
        firstEast = firstNorth = firstSouth = firstWest = false;
        if(flag == 1) // 优先环
        {
            pthread_cond_signal(&c_firstWest);
        }
        else if(flag == 2) // 饥饿环
        {
            pthread_cond_signal(&c_firstEast);
        }
        sleep(1);
        // 如果四个方向都没有车，程序结束
        if(north_ready == false && south_ready == false && west_ready == false && east_ready == false)
        {
            cout << "All car successfully passed the cross"<< endl;
            exit(0);
        }
    }
}

void* west_thread(void *para)
{
    sleep(1);
    car_t temp_ready;
    // 选择下一个就绪车辆
    if((temp_ready = qw.dequeue()).id != -1)
    {
        west_ready = true;
    }
    // 进入就绪态，队列上锁
    //if(west_ready == false) {cout << "No car from west" << endl; return NULL;}
    pthread_mutex_t tplock;
    pthread_mutex_init(&tplock, NULL);
    pthread_mutex_lock(&tplock);
    west_ready = true;
    // 等待发车信号启动，不属于自己的order不发车
    do{
        pthread_cond_wait(&cond_w, &tplock);
    }while(qw.now_order != temp_ready.order);
    pthread_mutex_unlock(&tplock);
    pthread_mutex_lock(&mutex_w);
    ready_west = temp_ready;
    // 如果之前赋予了左车先决的条件，要等n左车走完
    pthread_mutex_lock(&mutex_firstNorth);
    if(north_ready == true && firstNorth == true)
    {
        ddl_graph[2][3] = true;
        pthread_cond_wait(&c_firstNorth, &mutex_firstNorth);
        pthread_mutex_unlock(&mutex_firstNorth);
        ddl_graph[2][3] = false;
    }
    else
    {
        pthread_mutex_unlock(&mutex_firstNorth);
    }
    // 然后判断右边有没有车在就绪或者行驶,注意要在自己没有被优先授权的情况下
    pthread_mutex_lock(&mutex_firstWest);
    if(south_ready == true && !firstWest)
    {
        pthread_mutex_unlock(&mutex_firstWest);
        //如果右边有车在等待让他们先走,等走完了解锁
        ddl_graph[2][1] = true;
        // pthread_cond_signal(&cond_s);
        pthread_mutex_lock(&mutex_firstSouth);
        pthread_cond_wait(&c_firstSouth, &mutex_firstSouth);
        pthread_mutex_unlock(&mutex_firstSouth);
        ddl_graph[2][1] = false;
    }
    else{
        pthread_mutex_unlock(&mutex_firstWest);
    }
    // 优先模式
    // 进入运行态，进入路口
    pthread_mutex_lock(&mutex_d);
    west_active = true;
    now_west = ready_west;
    // 如果没有就绪车辆

    cout << "car " << now_west.id << " from west enter" << endl;
    sleep(1);
    // 尝试进入下一个区域
    pthread_mutex_lock(&mutex_a);
    // 成功进入下一区域后立刻离开
    cout << "car " << now_west.id << " from west leave" << endl;
    pthread_mutex_unlock(&mutex_d);
    sleep(1);
    pthread_mutex_unlock(&mutex_a);
    // 防止饥饿
    if(north_ready == true)
    {
        // 设置北方为优先
        firstNorth = true;
        pthread_cond_signal(&cond_n);
        sleep(1);
    }
    // 退出运行态
    west_active = false;
    pthread_mutex_unlock(&mutex_w);
    // 向下一辆车发送
    qw.now_order++;
    pthread_cond_broadcast(&cond_w);
    pthread_mutex_lock(&mutex_firstWest);
    if(firstWest) firstWest = false;
    pthread_mutex_unlock(&mutex_firstWest);
    // 所有线程全部结束
    if(qw.max_count == temp_ready.order + 1)
    {
        west_ready = false;
    }
    // 解锁所有等待本车的线程
    pthread_cond_signal(&c_firstWest);
    return NULL;
}

void* east_thread(void *para)
{
    car_t temp_ready;
    if((temp_ready = qe.dequeue()).id != -1)
    {
        east_ready = true;
    }
    // 进入就绪态，队列上锁
    //if(east_ready == false) {cout << "No car from east" << endl; return NULL;}
    pthread_mutex_t tplock;
    pthread_mutex_init(&tplock, NULL);
    east_ready = true;
    // 等待发信号启动
    pthread_cond_wait(&cond_e, &tplock);
    pthread_mutex_unlock(&tplock);
    // 等待发车信号启动，不属于自己的order不发车
    do{
    pthread_mutex_lock(&mutex_e);
    }while(qe.now_order != temp_ready.order);
    ready_east = temp_ready;
    // 如果之前赋予了左边先决的条件，要等左边走完
    pthread_mutex_lock(&mutex_firstSouth);
    if(south_ready == true && firstSouth == true)
    {
        ddl_graph[0][1] = true;
        pthread_cond_wait(&c_firstSouth, &mutex_firstSouth);
        pthread_mutex_unlock(&mutex_firstSouth);
        ddl_graph[0][1] = false;
    }
    else
    {
        pthread_mutex_unlock(&mutex_firstSouth);
    }
    // 然后判断右边有没有车在就绪或者行驶,在自己没有被优先授权的情况下
    pthread_mutex_lock(&mutex_firstEast);
    if(north_ready == true && !firstEast)
    {
        pthread_mutex_unlock(&mutex_firstEast);
        ddl_graph[0][3] = true;
        //如果右边有车在等待让他们先走,等走完了解锁
        // pthread_cond_signal(&cond_n);
        pthread_mutex_lock(&mutex_firstNorth);
        pthread_cond_wait(&c_firstNorth, &mutex_firstNorth);
        pthread_mutex_unlock(&mutex_firstNorth);
        ddl_graph[0][3] = false;
    }
    else{
        pthread_mutex_unlock(&mutex_firstEast);
    }
    // 优先模式
    // 进入运行态，进入路口
    pthread_mutex_lock(&mutex_b);
    east_active = true;
    now_east = ready_east;
    // 选择下一个就绪车辆
    // 如果没有就绪车辆
    cout << "car " << now_east.id << " from east enter" << endl;
    // 尝试进入下一个区域
    pthread_mutex_lock(&mutex_c);
    sleep(1);
    // 成功进入下一区域后立刻离开
    cout << "car " << now_east.id << " from east leave" << endl;
    pthread_mutex_unlock(&mutex_b);
    sleep(1);
    pthread_mutex_unlock(&mutex_c);
    // 防止饥饿
    if(south_ready == true)
    {
        // 设置北方为优先
        firstSouth = true;
        pthread_cond_signal(&cond_s);
        sleep(1);
    }
    // 退出运行态
    east_active = false;
    pthread_mutex_unlock(&mutex_e);
    // 向下一辆车发送信号
    qe.now_order++;
    pthread_cond_broadcast(&cond_e);
    pthread_mutex_lock(&mutex_firstEast);
    if(firstEast) firstEast = false;
    pthread_mutex_unlock(&mutex_firstEast);
    // 同方向所有线程全部结束
    if(qe.max_count == temp_ready.order + 1)
    {
        east_ready = false;
    }
    // 解锁所有等待本车的线程
    pthread_cond_signal(&c_firstEast);
    return NULL;
}

void* north_thread(void *para)
{
    car_t temp_ready;
    if((temp_ready = qn.dequeue()).id != -1)
    {
        north_ready = true;
    }
    // 进入就绪态，队列上锁
    //if(north_ready == false) {cout << "No car from north" << endl; return NULL;}
    pthread_mutex_t tplock;
    pthread_mutex_init(&tplock, NULL);
    pthread_mutex_lock(&tplock);
    north_ready = true;
    // 等待发车信号启动，不属于自己的order不发车
    do{
    pthread_cond_wait(&cond_n, &tplock);
    }while(qn.now_order != temp_ready.order);
    pthread_mutex_unlock(&tplock);
    pthread_mutex_lock(&mutex_n);
    ready_north = temp_ready;
    // 如果之前赋予了左方先决的条件，要等左方走完
    pthread_mutex_lock(&mutex_firstEast);
    if(east_ready == true && firstEast == true)
    {
        ddl_graph[1][0] = true;
        pthread_cond_wait(&c_firstEast, &mutex_firstEast);
        pthread_mutex_unlock(&mutex_firstEast);
        ddl_graph[1][0] = false;
    }
    else
    {
        pthread_mutex_unlock(&mutex_firstEast);
    }
    // 然后判断右边有没有车在就绪或者行驶,在自己没有被优先授权的情况下
    pthread_mutex_lock(&mutex_firstNorth);
    if(west_ready == true && !firstNorth)
    {
        pthread_mutex_unlock(&mutex_firstNorth);
        //如果右边有车在等待让他们先走,等走完了解锁
        ddl_graph[1][2] = true;
        // pthread_cond_signal(&cond_w);
        pthread_mutex_lock(&mutex_firstWest);
        pthread_cond_wait(&c_firstWest, &mutex_firstWest);
        pthread_mutex_unlock(&mutex_firstWest);
        ddl_graph[1][2] = false;
    }
    else{
        pthread_mutex_unlock(&mutex_firstNorth);
    }
    // 优先模式
    // 进入运行态，进入路口
    pthread_mutex_lock(&mutex_c);
    north_active = true;
    now_north = ready_north;
    // 选择下一个就绪车辆
    // 如果没有就绪车辆
    cout << "car " << now_north.id << " from north enter" << endl;
    // 尝试进入下一个区域
    sleep(1);
    pthread_mutex_lock(&mutex_d);
    // 成功进入下一区域后立刻离开
    cout << "car " << now_north.id << " from north leave" << endl;
    pthread_mutex_unlock(&mutex_c);
    sleep(1);
    pthread_mutex_unlock(&mutex_d);
    // 防止饥饿
    if(east_ready == true)
    {
        // 设置北方为优先
        firstEast = true;
        pthread_cond_signal(&cond_e);
        sleep(1);
    }
    // 退出运行态
    north_active = false;
    pthread_mutex_unlock(&mutex_n);
    // 向同方向下一辆车发送信号
    qn.now_order++;
    pthread_cond_broadcast(&cond_n);
    pthread_mutex_lock(&mutex_firstNorth);
    if(firstNorth) firstNorth = false;
    pthread_mutex_unlock(&mutex_firstNorth);
    if(qn.max_count == temp_ready.order + 1)
    {
        north_ready = false;
    }
    // 解锁所有等待本车的线程
    pthread_cond_signal(&c_firstNorth);
    return NULL;
}

void* south_thread(void *para)
{
    car_t temp_ready;
    if((temp_ready = qs.dequeue()).id != -1)
    {
        south_ready = true;
    }
    // 进入就绪态，队列上锁
    //if(south_ready == false) {cout << "No car from south" << endl; return NULL;}
    pthread_mutex_t tplock;
    pthread_mutex_init(&tplock, NULL);
    pthread_mutex_lock(&tplock);
    south_ready = true;
   // 等待发车信号启动，不属于自己的order不发车
    do{
        pthread_cond_wait(&cond_s, &tplock);
    }while(qs.now_order != temp_ready.order);
    pthread_mutex_unlock(&tplock);
    pthread_mutex_lock(&mutex_s);
    ready_south = temp_ready;
    // 如果之前赋予了左先决的条件，要等左走完
    pthread_mutex_lock(&mutex_firstWest);
    if(west_ready == true && firstWest == true)
    {
        ddl_graph[3][2] = true;
        pthread_cond_wait(&c_firstWest, &mutex_firstWest);
        pthread_mutex_unlock(&mutex_firstWest);
        ddl_graph[3][2] = false;
    }
    else
    {
        pthread_mutex_unlock(&mutex_firstWest);
    }
    // 然后判断右边有没有车在就绪或者行驶,在自己没有被优先授权的情况下
    pthread_mutex_lock(&mutex_firstSouth);
    if(east_ready == true && !firstSouth)
    {
        pthread_mutex_unlock(&mutex_firstSouth);
        //如果右边有车在等待让他们先走,等走完了解锁
        ddl_graph[3][0] = true;
        // pthread_cond_signal(&cond_e);
        pthread_mutex_lock(&mutex_firstEast);
        pthread_cond_wait(&c_firstEast, &mutex_firstEast);
        pthread_mutex_unlock(&mutex_firstEast);
        ddl_graph[3][0] = false;
    }
    else{
        pthread_mutex_unlock(&mutex_firstSouth);
    }
    // 优先模式
    // 进入运行态，进入路口
    pthread_mutex_lock(&mutex_a);
    south_active = true;
    now_south = ready_south;
    // 选择下一个就绪车辆
    
    // 如果没有就绪车辆
    cout << "car " << now_south.id << " from south enter" << endl;
    sleep(1);
    // 尝试进入下一个区域
    pthread_mutex_lock(&mutex_b);
    // 成功进入下一区域后立刻离开
    cout << "car " << now_south.id << " from south leave" << endl;
    pthread_mutex_unlock(&mutex_a);
    sleep(1);
    pthread_mutex_unlock(&mutex_b);
    // 防止饥饿
    if(west_ready == true)
    {
        // 设置北方为优先
        firstWest = true;
        pthread_cond_signal(&cond_w);
        sleep(1);
    }
    // 退出运行态
    south_active = false;
    pthread_mutex_unlock(&mutex_s);
    // 向下一辆车发送
    qs.now_order++;
    pthread_cond_broadcast(&cond_s);
    pthread_mutex_lock(&mutex_firstSouth);
    if(firstSouth) firstSouth = false;
    pthread_mutex_unlock(&mutex_firstSouth);
    if(qs.max_count == temp_ready.order + 1)
    {
        south_ready = false;
    }
    pthread_cond_signal(&c_firstSouth);
    return NULL;
}


int main(int argc, char** argv)
{
    data_ini(argv[1]);
    ready();
    for(int i = 0; i < ptr; i++)
    {
        if(q[i].dir == NORTH)
        {
            
            car_thread[i] = qn.thread[qn.thp++];
            pthread_create(&qn.thread[qn.thp++], NULL, north_thread, NULL);
        }
        if(q[i].dir == SOUTH)
        {
            car_thread[i] = qs.thread[qs.thp++];
            pthread_create(&qs.thread[qs.thp++], NULL, south_thread, NULL);
        }
        if(q[i].dir == EAST)
        {
           
            car_thread[i] = qe.thread[qe.thp++];
            pthread_create(&qe.thread[qe.thp++], NULL, east_thread, NULL);
        }
        if(q[i].dir == WEST)
        {
            
            car_thread[i] = qw.thread[qw.thp++];
            pthread_create(&qw.thread[qw.thp++], NULL, west_thread, NULL);
        }
        sleep(1);
    }
    start();
    pthread_create(&car_thread[ptr], NULL, check_ddl, NULL);
    for(int i = 0; i <= ptr; i++)
    {
        // 连check_ddl都join
        pthread_join(car_thread[i], NULL);
    }
}