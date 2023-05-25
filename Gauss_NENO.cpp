#include <iostream>
#include<sys/time.h>
#include <arm_neon.h>
using namespace std;
const int N = 16;
float a[N][N];
void init()
{
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            a[i][j] = float(rand()) / 100;
}

int main()
{


    timeval* start = new timeval();
    timeval* stop = new timeval();
    double durationTime = 0.0;
    init();
    gettimeofday(start, NULL);
    for (int tim = 0; tim < 1000; tim++) {
        float32x4_t t0, t1, t2, t3;
        for (int k = 0; k < N; k++)
        {
            t0 = vld1q_dup_f32(a[k] + k);
            int j;

            for (j = k + 1; j + 3 < N; j += 4)
            {
                t1 = vld1q_f32(a[k] + j);
                t2 = vdivq_f32(t1, t0);
                vst1q_f32(a[k] + j, t2);
            }
            for (; j < N; j++)
                a[k][j] /= a[k][k];

            a[k][k] = 1.0;
            for (int i = k + 1; i < N; i++)
            {
                t0 = vld1q_dup_f32(a[i] + k);
                int j;
                for (j = k + 1; j + 3 < N; j += 4)
                {
                    t1 = vld1q_f32(a[k] + j);
                    t2 = vld1q_f32(a[i] + j);
                    t3 = vmulq_f32(t0, t1);
                    t2 = vsubq_f32(t2, t3);
                    vst1q_f32(a[i] + j, t2);
                }
                for (; j < N; j++)
                    a[i][j] -= a[i][k] * a[k][j];
                a[i][k] = 0.0;
            }
        }

    }
    gettimeofday(stop, NULL);
    durationTime = stop->tv_sec * 1000 + double(stop->tv_usec) / 1000 - start->tv_sec * 1000 - double(start->tv_usec) / 1000;
    cout << " ParallelAlgorithm time: " << double(durationTime) << " ms" << endl;
    return 0;
}


//.text.align 4.globl _my_test_my_test : vld1.32 {d0}, [r0, :64]    vst1.32 {d0}, [r1, :64]    bx      lr