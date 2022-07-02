#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>

#define S_POINT 1 // �J�n�s�s��
#define Kosuu 1 // ���s��
#define MAX 9999 // �ő�l
#define M_ARGC 1 // ����
#define getrandom(MAX, MIN) (rand() % (int)(MAX - MIN + 1) + MIN)//�w��͈̗͂����쐬
#define Max 25 // �����̍ő�l
#define Min 5 // �����̍ŏ��l
int Node; // �s�s�̐�
int* ex; // �s�����s�s�̗L��
int** dis; // �����s��
int* d_min; // �ύX�O�C�U�p
int* r_min; // �ʂ�s�s�̊i�[
int d_p; // ������
int level; // �ċA�̐[��
int* array; // �����i�[
int tan1, tan2; //�ŒZ�H�ꎞ�i�[�ꏊ�A������
FILE* fp; // �t�@�C���|�C���^
FILE* fp2; // �t�@�C���|�C���^

clock_t t1;
clock_t t2;
void b_main(int, int);
void kyori();
void hyouji();
void hyouji2();

int arrayInit() {
    int i;
    ex = (int*)malloc(sizeof(int) * (Node + 1));    //  memory�m��
    d_min = (int*)malloc(sizeof(int) * (Node + 1));
    r_min = (int*)malloc(sizeof(int) * (Node + 1));
    dis = (int**)malloc(sizeof(int*) * (Node + 1));
    for (i = 0; i <= Node; i++)
        dis[i] = (int*)malloc(sizeof(int) * (Node + 1));
    if (ex == NULL || d_min == NULL || r_min == NULL || dis == NULL) {
        printf("Error memory can't allocate!\n");  //�m�ۂł��Ȃ������ꍇ
        exit(1);
    }
    return 0;
}

void arrayFree() {
    free(ex);
    free(d_min);
    free(r_min);
    free(dis);
}

void makeRandom() {
    int i, count = 0;
    int indata1, indata2, bufsize;
    indata1 = Node;
    indata2 = Kosuu;
    bufsize = indata1 * (indata1 - 1) / 2 * indata2;
    // printf("bufsize=%d\n", bufsize);
    array = (int*)malloc(sizeof(int) * (bufsize + 1)); //memory�m��
    //randomize();
    for (i = 0; i < bufsize; i++) {
        array[i] = getrandom(Max, Min), ++count;
    }
}

void makeMap() {       //�s�s�Ԃ̋����쐬
    int i, j, h, k, w;
    int count = 1;
    printf("�s�s�Ԃ̋���\n");
    fputs("City Distance\n", fp);
    fputs("City Distance\n", fp2);
    for (h = 0; h < Kosuu; h++) {
        k = 0, w = 1;
        for (j = 1; j <= Node; j++) {
            for (i = w; i <= Node; i++) {
                if (j == i)
                    dis[j][i] = 99;     //�ړ����Ȃ�������
                else {
                    dis[j][i] = array[k];
                    dis[i][j] = array[k];
                    k++;
                }
            }
            w++;
        }

        for (j = 1; j <= Node; j++) {
            for (i = 1; i <= Node; i++) {
                printf("%2d, ", dis[j][i]);
                fprintf(fp, "%d ,", dis[i][j]);
                fprintf(fp2, "%d ,", dis[i][j]);
            }
            printf("  �s�s%d", j);
            fprintf(fp, "City %d\n", j);
            fprintf(fp2, "City %d\n", j);
            puts("");
        }
        puts("");
    }
    free(array);
}

int main(int argc, char* argv[]) {
    int num, count = 0;
    fp = fopen("Dg-TSP.csv", "w");
    fp2 = fopen("Dg-TSP2.csv", "w");
    if (fp == NULL) {
        printf("�t�@�C���I�[�v�����s\n");
        return -1;
    }
    for (num = 100; num < 110; num++) {
        printf("������ = %d\n", num);
        fprintf(fp, "randam, %d\n", num);
        fprintf(fp, "random, %d\n", num);
        srand(num);
        int i, n;
        d_p = 0;
        Node = 20;
        printf("�s�s�� = %d\n",Node);
        fprintf(fp, "Numcity, %d\n", Node);
        fprintf(fp, "Numcity, %d\n", Node);
        arrayInit();
        makeRandom();
        makeMap();
        t1 = clock();
        for (n = 2; n <= Node; n++) {
            for (i = 1; i <= Node; i++)
                ex[i] = 1;
            b_main(S_POINT, n);
            hyouji();
            if (n == 2) {
                tan1 = n;
                tan2 = d_p;
            }
            else if (tan2 > d_p) {
                tan1 = n;
                tan2 = d_p;
            }
        }
        printf("�ŒZ�o�H��\n");
        fputs("Shortest path\n", fp);
        fputs("Shortest path\n", fp2);
        
        for (i = 1; i <= Node; i++)
        ex[i] = 1;
        b_main(S_POINT, tan1);
        hyouji2();
            
        t2 = clock();
        printf("time = %8.3f\n", ((double)t2 - t1) / 1000);
        fprintf(fp, "Time, %8.3f\n", ((double)t2 - t1) / 1000);
        fprintf(fp2, "Time, %8.3f\n", ((double)t2 - t1) / 1000);
        arrayFree();
        puts("");
        fputs("\n", fp);
        fputs("\n", fp2);
    }
    fclose(fp);
    fclose(fp2);
}

void b_main(int end, int num) {
    double min;
    int cost = 0;
    int min_idx, i, j;
    int count = 2;
    ex[end] = 0;
    ex[num] = 0;
    i = num;
    r_min[0] = end;
    r_min[1] = num;
    while (i >= 0) {
        min = MAX;
        min_idx = -1;
        for (j = 1; j <= Node; j++) {
            if (i != j && ex[j] != 0) {
                if (dis[i][j] < min) {
                    min = dis[i][j];
                    min_idx = j;
                }
            }
        }

        if (min_idx < 0) { // �S�Ă̒n�_��K���
            r_min[count] = end;
        }
        else {
            ex[min_idx] = 0;
            r_min[count] = min_idx;
            count++;
        }
        i = min_idx;
    }
}

void kyori() {
    int i;
    d_p = 0;
    i = S_POINT;
    for (i = 0; i < Node; i++) {
        d_p += dis[r_min[i]][r_min[i + 1]];
    }

}

void hyouji() {
    int i;
    kyori();
    puts("Root"); //Tansaku
    fputs("Root\n", fp);
    for (i = 0; i <= Node; i++) {
        if (i == Node) {
            printf("%2d", r_min[i]);
            fprintf(fp,"%2d\n", r_min[i]);
        }
        else {
            printf("%2d��", r_min[i]);
            fprintf(fp, "%2d, ", r_min[i]);
        }
    }
    puts("");
    printf("Total = %5d\n", d_p);
    fprintf(fp, "Total, %5d\n", d_p);
    puts("");
    fputs("\n", fp);
}
void hyouji2() {
    int i;
    kyori();
    puts("Root"); //Tansaku
    fputs("Root\n", fp2);
    for (i = 0; i <= Node; i++) {
        if (i == Node) {
            printf("%2d", r_min[i]);
            fprintf(fp, "%2d\n", r_min[i]);
            fprintf(fp2, "%2d\n", r_min[i]);
        }
        else {
            printf("%2d��", r_min[i]);
            fprintf(fp, "%2d, ", r_min[i]);
            fprintf(fp2, "%2d, ", r_min[i]);
        }
    }
    puts("");
    printf("Total = %5d\n", d_p);
    fprintf(fp, "Total, %5d\n", d_p);
    fprintf(fp2, "Total, %5d\n", d_p);
    puts("");
    fputs("\n", fp);
    fputs("\n", fp2);
}
