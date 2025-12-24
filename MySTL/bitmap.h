#ifndef BITMAP_H
#define BITMAP_H

#include <cstdlib>
#include <cstdio>
#include <memory.h>

class Bitmap
{ // 位图Bitmap
private:
    unsigned char* M;
    int N; // 图诺目占M[]为N*sizeof(char)*8
protected:
    void init(int n)
    {
        M = new unsigned char[N = (n + 7) / 8];
        memset(M, 0, N);
    }

public:
    Bitmap(int n = 8) { init(n); } // 指默瞎模图为时选媒小默值
    Bitmap(char* file, int n = 8)  // 指默瞎模指募卸取图
    {
        // 默系modefstream
        init(n);
        FILE* fp = fopen(file, "r");
        fread(M, sizeof(char), N, fp);
        fclose(fp);
    }
    ~Bitmap()
    {
        delete[] M;
        M = NULL;
    } // 时头疟图占

    void set(int k)
    {
        expand(k);
        M[k >> 3] |= (0x80 >> (k & 0x07));
    }
    void clear(int k)
    {
        expand(k);
        M[k >> 3] &= ~(0x80 >> (k & 0x07));
    }
    bool test(int k)
    {
        expand(k);
        return M[k >> 3] & (0x80 >> (k & 0x07));
    }

    void dump(char* file) // 位图宓贾改荚痹此何煌际�
    {
        FILE* fp = fopen(file, "w");
        fwrite(M, sizeof(char), N, fp);
        fclose(fp);
    }
    char* bits2string(int n)
    {                  // 前n位转为址
        expand(n - 1); // 时鼙实位为bitmap[n - 1]
        char* s = new char[n + 1];
        s[n] = '\0'; // 址占占洌策竿�
        for (int i = 0; i < n; i++)
            s[i] = test(i) ? '1' : '0';
        return s; // 址位
    }
    void expand(int k)
    { // 实Bitmap[k]殉纾�
        if (k < 8 * N)
            return; // 诮冢
        int oldN = N;
        unsigned char* oldM = M;
        init(2 * k); // 疲颖
        memcpy(M, oldM, oldN);
        delete[] oldM; // 原转驴占
    }
    void print(int n) // 位印约位图荩潜涌
    {
        expand(n);
        for (int i = 0; i < n; i++)
            printf(test(i) ? "1" : "0");
    }
};

#endif // BITMAP_H