#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <vector>
#include <atomic>
#include <functional>

using namespace std;
vector <thread> threads_v;
vector<double> time_v;
double avg_time;
static mutex m;
int a = 0,threads_num = 10;
atomic<int> b;

void global_function(const char* txt){
    thread::id id_1 = this_thread::get_id();
    cout<<"thread 1"<<" "<<"number:"<< id_1<<" "<<txt<<"\n";
}

class function_object {
public:
    void operator()(const char* txt) const {
        thread::id id_2 = this_thread::get_id();
        cout << "thread 2"<<" "<<"number:"<< id_2<<" "<<txt<<"\n";
    }
};

class th{
public:
    void ex(const char* txt){
        thread::id id_3 = this_thread::get_id();
        cout<<"thread 3"<<" "<<"number:"<<id_3<<" "<<txt<<"\n";
    }
};

void task_1(){
    thread t_1(global_function,"Created by using global function");
    t_1.join();
    function_object ob;
    thread t_2(ob,"Created by using function object");
    t_2.join();
    th * thr= new th();
    std::thread t_3(&th::ex,thr,"Created by using class method");
    t_3.join();
    thread t_4([] (const char* txt="Created by using lambda function"){thread::id id_4= this_thread::get_id(); cout << "thread 4"<<" "<<"number:"<<id_4<<" "<<txt<<"\n";});
    t_4.join();
    delete thr;
}

void task2_fun(const char* txt){
    for (int i=1; i<=50; i++){
        m.lock();
        cout<<txt<<"  "<<"\n";
        m.unlock();
    }
}
void task_2(){
    for (int i=1;i<=20;i++){
        thread th(task2_fun,"This is from the thread");
        th.join();
    }
}

//task 3
void task3_fun(){
    while (a<10000000){
        a++;
    }
    cout<<a<<endl;
}
void task3_fun2(){
    a=0;
    while(a<10000000){
        m.lock();
        a++;
        m.unlock();
    }
    cout<<a<<endl;
}
void task3_fun3(){
    b=0;
    while(b<10000000){
        b++;
    }
    cout<<b<<endl;
}

//double avg_calc (vector<double>& v){
//    double sum = 0.0,n = v.size();
//    for ( int i=0; i < n; i++)
//    {
//        sum += v[i];
//    }
//    return (sum /n);
//}

void ExTime(const function<void()>& x,const char* txt1){
    threads_v.reserve(threads_num);
    time_v.reserve(threads_num);
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 1; i <= threads_num; ++i) {
        thread thread_3(x);
        thread_3.join();
        threads_v.push_back(move(thread_3));
    }
    auto stop = std::chrono::high_resolution_clock::now();
    auto ex_time = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count();
    //time_v.push_back(ex_time);
    //avg_time = avg_calc(time_v);
    cout << txt1 << ex_time << "\n";
}
void task_3(){
    ExTime(task3_fun,"Operation time for the unsynchronized variant:");
    ExTime(task3_fun2,"Operation time for the multithreaded variant (mutex):");
    ExTime(task3_fun3,"Operation time for the multithreaded variant (atomic):");
    auto start = std::chrono::high_resolution_clock::now();
    task3_fun();
    auto stop = std::chrono::high_resolution_clock::now();
    auto ex_time = std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count();
    cout << "Operation time for the single thread variant:" << ex_time << "\n";
}

int main() {
    task_1();
    task_2();
    task_3();
    return 0;
}



