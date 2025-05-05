#include "common.h"
int main()
{
    system("chcp 65001");//设置控制台编码为 UTF-8 以防止乱码
    //初始化指针
    Question* qHead = NULL;
    Student* sHead = NULL;
    //加载题目和学生信息
    if (loadQuestionsFromFile(&qHead) != 0)
        handleError("加载题目信息失败!");
    if (loadStudentsFromFile(&sHead) != 0)
        handleError("加载学生信息失败!");

    printf("\n  ----#### 欢迎来到数学口算系统 ####----  \n");
    ERROR:
    printf("如果你是老师,请输入0\n如果你是学生,请输入1\n");
    int tOrS;
    scanf("%d",&tOrS);
    if (tOrS != 1 && tOrS != 0)
    {
        handleError("错误，请重新输入!");
        goto ERROR;
    }
    if (tOrS == 0) t(qHead,sHead);
    if (tOrS == 1) s(qHead,sHead);
    printf("\n  ----#### 感谢您的使用 ####----  \n");
    getchar();
    return 0;
}