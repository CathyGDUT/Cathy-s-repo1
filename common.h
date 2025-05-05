#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <time.h>
#include <limits.h>
#include <stdbool.h>

//题目结构体
typedef struct Question
{
    int id;                //题号
    char content[100];     //题目内容
    int answer;            //正确答案
    struct Question* next; //指向下一个题目的指针
} Question;

//学生答题结构体
typedef struct Student
{
    char major[50];        //专业
    char className[50];    //班级
    char name[50];         //姓名
    int* answers;          //学生的答案数组
    int totalScore;        //总成绩
    struct Student* next;  //指向下一个学生的指针
} Student;

//错误处理函数
void handleError(const char* message);
int loadQuestionsFromFile(Question** head);
int loadStudentsFromFile(Student** head);
void t(Question* qHead,Student* sHead);
void s(Question* qHead,Student* sHead);

#endif

/*
    "r"： 只读模式。文件必须存在
    "w"： 只写模式。如果文件存在，其内容会被清空。如果文件不存在，会创建新文件
    "a"： 追加模式。如果文件存在，写入的数据会被追加到文件末尾。如果文件不存在，会创建新文件
    "r+"：读写模式。文件必须存在。
    "w+"：读写模式。如果文件存在，其内容会被清空。如果文件不存在，会创建新文件
    "a+"：读写模式。如果文件存在，写入的数据会被追加到文件末尾，且可以读取文件内容。如果文件不存在，会创建新文件
*/