#include "common.h"
//错误处理函数#
void handleError(const char* message)
{
    fprintf(stderr, "错误：%s\n", message);
}
//计算链表中题目数量#
int cntQuestions(Question* qHead)
{
    int count = 0;
    Question* curr = qHead;
    while (curr)
	{
        count++;
        curr = curr->next;
    }
    return count;
}
//自动计算答案#
int calAns(const char* content)
{
    //复制内容以便修改
    char exp[100];
    strcpy(exp, content);

    //提取操作数和运算符
    char op;
    int i, num1 = 0, num2 = 0;
    for(i = 0; i < strlen(exp); i++)
    {
        if(exp[i]>='0' && exp[i]<='9')
            num1 = num1 * 10 + exp[i]-'0';
        else
        {
            op = exp[i];
            i++;
            break;
        }
    }
    for(; i < strlen(exp); i++)
    {
        if(exp[i]>='0' && exp[i]<='9')
            num2 = num2 * 10 + exp[i]-'0';
    }
    switch (op)//开始四则运算
    {
        case '+':
            return num1 + num2;
        case '-':
            return num1 - num2;
        case '*':
            return num1 * num2;
        case '/':
            if (num2 == 0)
            {
                handleError("除数不能为零!");
                return -1;
            }
        return num1 / num2;
        default:
            handleError("无效的操作符!");
        return -1;
    }
}
//自动分配题号(即自动排序)#
int setId(Question* head)
{
    int maxId = 0;
    Question* current = head;

    while (current != NULL) {
        if (current->id > maxId)
            maxId = current->id;
        current = current->next;
    }

    return maxId + 1;
}
//保存题目到文件#
void save(Question* head)
{
    FILE* file = fopen("timu.txt", "w");
    if (!file)
    {
        handleError("无法打开题目文件!");
        return;
    }
    Question* temp = head;//避免移动 head 指针

    //写入文件
    while (temp)
    {
        setId(temp);
        fprintf(file,"%d %s %d\n",temp->id,temp->content,temp->answer);
        temp = temp->next;
    }
    fclose(file);
}
//添加题目到链表#
void addQuestion(Question** head, const char* content, int answer)
{
    Question* newQuestion = (Question*)malloc(sizeof(Question));
    if (!newQuestion)
	{
        printf("内存分配失败!");
        return;
    }
    newQuestion->id = setId(*head);       //存id
    strcpy(newQuestion->content, content);//存题目
    newQuestion->content[sizeof(newQuestion->content) - 1] = '\0';//确保字符串以null结尾
    newQuestion->answer = answer;		  //存答案
    newQuestion->next = *head;			  //指向原头节点
    *head = newQuestion;				  //成为新的头节点

    //保存到文件
    FILE* file = fopen("timu.txt", "a");
    if (!file)
    {
        handleError("无法打开题目文件!");
        free(newQuestion);//如果文件打开失败,释放新分配的节点
        return;
    }
    // 写入新题目信息到文件
    if (fprintf(file,"%d %s %d\n",newQuestion->id,newQuestion->content,newQuestion->answer)<0)
    {
        handleError("写入文件时发生错误!");
        fclose(file);
        free(newQuestion);//发生错误时清理资源
        return;
    }
    fclose(file);
}
//从输入中添加题目#
void addQuestionFromInput(Question** head)
{
    char content[100];

    printf("请输入题目内容(如:50+40,不带空格!):");
    scanf(" %s", content);//接收带空格的字符串

    int answer = calAns(content);//自动计算答案
    if (answer == -1)
	{
        handleError("无法添加题目,解析失败!");
        return;
    }

    //添加新题目
    addQuestion(head, content, answer);
    printf("题目已成功添加!\n题目:%s 答案:%d\n",content,answer);
}
//修改题目#
void modifyQuestion(Question* head, int id)
{
    Question* temp = head;
    //查找目标题目
    while (temp && temp->id != id)// temp 存在且其 id 不等于要查找的题目 id
        temp = temp->next;
    if (!temp)//如果没找到
	{
        handleError("未找到指定题目!");
        return;
    }

    //输入新的题目内容
    char newContent[100];
    printf("请输入新的题目内容(如:50+40,不带空格!):");
    scanf(" %[^\n]", newContent);
    int newAnswer = calAns(newContent);//解析新答案
    if (newAnswer == -1)
	{
        handleError("输入的新题目内容无效!");
        return;
    }

    //更新题目
    strcpy(temp->content, newContent);
    temp->answer = newAnswer;
    printf("题号 %d 的题目已成功修改！\n", id);
    //保存到文件
    save(head);
}
//批量添加#
void batchAdd(Question** head, int count)
{
    for (int i = 0; i < count; i++)
	{
        printf("正在进行第 %d/%d 次添加...\n", i + 1, count);
        addQuestionFromInput(head);
    }
}
//批量修改#
void batchModify(Question* head, int count)
{
    for (int i = 0; i < count; i++)
	{
        int id;
        printf("请输入要修改的题号(剩余 %d 个操作):", count - i);
        scanf("%d", &id);
        modifyQuestion(head, id);
    }
}
//保存学生信息到文件#
void saveStudentsToFile(Student* head)
{
    FILE* file = fopen("student.txt", "a");
    if (!file)
    {
        handleError("无法打开学生文件!");
        return;
    }
    Student* temp = head;//避免移动 head 指针

    //写入文件
    while (temp)
    {
        fprintf(file, "%s %s %s %d\n", temp->major, temp->className, temp->name, temp->totalScore);
        temp = temp->next;
    }
    fclose(file);
}
//学生答题#
void ans(Question* qHead, Student** sHead, int cnt)
{
    //计算链表中题目的总数
    int totalQuestions = cntQuestions(qHead);
    //如果请求的题目数量大于总题目数量
    if (cnt > totalQuestions)
    {
        handleError("题目数量不足,无法完成随机抽题!");
        return;
    }
    //将链表中的题目存储到数组中,以便进行随机选择
    Question** arr = (Question**)malloc(totalQuestions * sizeof(Question*));
    if (arr == NULL)
    {
        handleError("内存分配失败!");
        return;
    }
    Question* temp = qHead;
    for (int i = 0; i < totalQuestions; i++)
    {
        arr[i] = temp;
        temp = temp->next;
    }
    //用当前时间作为随机数生成的种子,随机打乱数组
    srand(time(NULL));
    for (int i = totalQuestions - 1; i > 0; i--)
    {
        int j = rand() % (i + 1);
        //交换数组中的元素以实现随机打乱
        Question* swap = arr[i];
        arr[i] = arr[j];
        arr[j] = swap;
    }

    //获取学生信息
    char major[50], className[50], name[50];
    printf("请输入专业,班级,姓名(仅限英文!):\n");
    scanf("%s %s %s", major, className, name);

    //创建新学生结构体并分配内存
    Student* newStudent = (Student*)malloc(sizeof(Student));
    if (!newStudent)
    {
        handleError("内存分配失败!");
        return;
    }
    strcpy(newStudent->major, major);
    strcpy(newStudent->className, className);
    strcpy(newStudent->name, name);

    //初始化总分和答案数组
    newStudent->totalScore = 0;
    newStudent->next = NULL;
    newStudent->answers = (int*)malloc(cnt * sizeof(int));
    if (!newStudent->answers)
    {
        handleError("内存分配失败!");
        free(newStudent);
        return;
    }
    //学生答题
    printf("\n---------- 开 始 答 题 ----------\n");
    int score = 100 / cnt;//计算每题分数
    printf("每道题的分值为:%d\n",score);
    for (int i = 0; i < cnt; i++)
    {
        printf("题号：%d\n题目：%s\n", arr[i]->id, arr[i]->content);
        scanf("%d", &newStudent->answers[i]);
        //计算总分
        if(newStudent->answers[i] == arr[i]->answer)
        {
            newStudent->totalScore += score;
            printf("————!!! Accepted !!!————\n");
        }
        else
        {
            printf("————!!! Wrong Answer !!!————\n");
            printf("————!!! The right answer is %d !!!————\n", arr[i]->answer);
        }
    }

    //将新学生添加到链表末尾
    if (*sHead == NULL)//链表是否为空
        *sHead = newStudent;
    else
    {
        Student* sTemp = *sHead;
        while (sTemp->next)
            sTemp = sTemp->next;
        sTemp->next = newStudent;
    }

    //保存学生信息到文件
    FILE* file = fopen("student.txt", "a");
    if (!file)
    {
        handleError("无法打开文件以保存学生答题信息!");
        return;
    }
    fprintf(file, "%s %s %s %d\n", major, className, name, newStudent->totalScore);
    fclose(file);
    free(arr);
    printf("学生 %s 的总分是:%d\n", name, newStudent->totalScore);
}
//比较函数用于升序排序#
int compareAscending(const void* a, const void* b)
{
    Student* studentA = *(Student**)a;
    Student* studentB = *(Student**)b;
    return studentA->totalScore - studentB->totalScore;
}
//比较函数用于降序排序#
int compareDescending(const void* a, const void* b)
{
    Student* studentA = *(Student**)a;
    Student* studentB = *(Student**)b;
    return studentB->totalScore - studentA->totalScore;
}
//学生成绩排序函数#
void sortStudentsByScore(Student** head, int order)
{
    if (!head || !*head)
    {
        handleError("学生列表为空,无需排序!");
        return;
    }
    //计算链表中学生的总数
    int totalStudents = 0;
    Student* temp = *head;
    while (temp)
    {
        totalStudents++;
        temp = temp->next;
    }
    //将链表中的学生存储到数组中,以便进行排序
    Student** arr = (Student**)malloc(totalStudents * sizeof(Student*));
    if (arr == NULL)
    {
        handleError("内存分配失败!");
        return;
    }
    temp = *head;
    for (int i = 0; i < totalStudents; i++)
    {
        arr[i] = temp;
        temp = temp->next;
    }
    //根据输入决定排序方式
    if (order == 1)
        qsort(arr, totalStudents, sizeof(Student*), compareAscending);
    else if (order == 2)
        qsort(arr, totalStudents, sizeof(Student*), compareDescending);
    else
    {
        handleError("无效的排序选项!");
        free(arr);
        return;
    }
    //重新构建链表
    *head = arr[0];
    temp = *head;
    for (int i = 1; i < totalStudents; i++)
    {
        temp->next = arr[i];
        temp = temp->next;
    }
    temp->next = NULL;
    //释放数组内存
    free(arr);
    //保存排序后的学生信息到文件
    FILE* file = fopen("student.txt", "w");
    if (!file)
    {
        handleError("无法打开学生文件!");
        return;
    }
    temp = *head;//避免移动 head 指针

    //写入文件
    while (temp)
    {
        fprintf(file, "%s %s %s %d\n", temp->major, temp->className, temp->name, temp->totalScore);
        temp = temp->next;
    }
    fclose(file);
    printf("学生成绩已成功按要求排序!\n");
}
//查询学生成绩#
void queryStudentScore(Student* head)
{
    if (!head)
    {
        handleError("学生列表为空,无法查询!");
        return;
    }

    char major[50], className[50], name[50];
    printf("请输入专业,班级,姓名:\n");
    scanf("%s %s %s", major, className, name);

    Student* temp = head;
    while (temp)
    {
        if (strcmp(temp->major, major) == 0 &&
            strcmp(temp->className, className) == 0 &&
            strcmp(temp->name, name) == 0)
        {
            printf("学生 %s 的总分是: %d\n", temp->name, temp->totalScore);
            return;
        }
        temp = temp->next;
    }
    handleError("未找到该学生的信息!");
}
//根据题号查询题目及其答案#
void queryQuestionById(Question* head, int id)
{
    if (!head)
    {
        handleError("题目列表为空,无法查询!");
        return;
    }
    Question* temp = head;
    while (temp)
    {
        if (temp->id == id)
        {
            printf("题号：%d\n题目：%s\n答案：%d\n", temp->id, temp->content, temp->answer);
            return;
        }
        temp = temp->next;
    }
    handleError("未找到指定题号的题目!");
}

//去除字符串首尾空白字符的函数#
char* strtrim(char* str)
{
    if (str == NULL)
        return NULL;

    //找到非空白字符的起始位置
    char *start = str;
    while (isspace((unsigned char)*start))
        start++;

    //如果全是空白字符,返回空字符串
    if (*start == '\0')
    {
        static char empty[] = "";
        return empty;
    }

    //找到非空白字符的结束位置
    char *end = start + strlen(start) - 1;
    while (end > start && isspace((unsigned char)*end))
        end--;

    //写入新的终止符
    *(end + 1) = '\0';

    return start;
}
//检查是否为空白行或注释行#
int isBlankOrCommentLine(const char *line)
{
    if (line == NULL ||
        line[0] == '\0' ||
        line[0] == '#')
        return 1;
    return 0;
}
//读取题目信息并建立链表#
int loadQuestionsFromFile(Question** head)
{
    FILE *file = fopen("timu.txt", "r");
    if (!file)
    {
        handleError("无法打开题目文件!");
        return -1;
    }

    *head = NULL;
    Question **tail = head;

    char line[512];
    int lineNumber = 0;
    while (fgets(line, sizeof(line), file))
    {
        lineNumber++;

        //跳过空白行和注释行
        char *trimmedLine = strtrim(line);
        if (isBlankOrCommentLine(trimmedLine))
            continue;

        //创建新节点
        Question *newQuestion = (Question*)malloc(sizeof(Question));
        if (newQuestion == NULL)
        {
            handleError("内存分配失败!");
            fclose(file);
            return -1;
        }

        //初始化成员变量
        newQuestion->id = 0;
        strcpy(newQuestion->content, "");
        newQuestion->answer = 0;
        newQuestion->next = NULL;

        //解析一行数据,格式为：id content answer
        char *token;
        token = strtok(trimmedLine, " ");
        if (token != NULL)
            sscanf(token, "%d", &newQuestion->id);
        else
        {
            handleError("文件数据格式错误或解析失败!");
            free(newQuestion);
            fclose(file);
            return -1;
        }

        token = strtok(NULL, " ");
        if (token != NULL)
        {
            strncpy(newQuestion->content, token, sizeof(newQuestion->content) - 1);
            newQuestion->content[sizeof(newQuestion->content) - 1] = '\0';//确保字符串以null结尾
        }
        else
        {
            handleError("文件数据格式错误或解析失败!");
            free(newQuestion);
            fclose(file);
            return -1;
        }

        token = strtok(NULL, "\n");//这里添加了 \n 以处理行尾换行符
        if (token != NULL)
            sscanf(token, "%d", &newQuestion->answer);
        else
        {
            handleError("文件数据格式错误或解析失败!");
            free(newQuestion);
            fclose(file);
            return -1;
        }

        //添加到链表末尾
        *tail = newQuestion;
        tail = &(newQuestion->next);

        //打印调试信息
        printf("Loaded question with ID: %d, Content: \"%s\", Answer: %d\n",
               newQuestion->id, newQuestion->content, newQuestion->answer);
    }

    fclose(file);
    return 0; // 成功返回0
}
//加载学生信息并建立链表#
int loadStudentsFromFile(Student** head)
{
    FILE *file = fopen("student.txt", "r");
    if (!file)
    {
        handleError("无法打开学生文件!");
        return -1;
    }
    *head = NULL;
    Student **tail = head;

    char line[256];
    while (fgets(line, sizeof(line), file))
    {
        //跳过空白行和注释行
        char *trimmedLine = strtrim(strdup(line));//strdup需要在程序结束前free
        if (isBlankOrCommentLine(trimmedLine))
        {
            free(trimmedLine);
            continue;
        }
        //创建新节点
        Student *newStudent = (Student*)malloc(sizeof(Student));
        if (newStudent == NULL)
        {
            handleError("内存分配失败!");
            free(trimmedLine);
            fclose(file);
            return -1;
        }

        //动态分配并初始化答案数组为全 0,假设每个学生最多有100个答案
        const int answerCount = 100;
        newStudent->answers = (int*)calloc(answerCount, sizeof(int));
        if (newStudent->answers == NULL)
        {
            handleError("内存分配失败!");
            free(newStudent);
            fclose(file);
            return -1;
        }
        // 初始化成员变量
        strcpy(newStudent->major, "");
        strcpy(newStudent->className, "");
        strcpy(newStudent->name, "");
        newStudent->totalScore = 0;
        newStudent->next = NULL;

        //解析一行数据,假设格式为：major className name totalScore
        char *token;
        token = strtok(line, " ");
        if (token != NULL)
        {
            strncpy(newStudent->major, token, sizeof(newStudent->major) - 1);
            newStudent->major[sizeof(newStudent->major) - 1] = '\0'; //确保字符串以null结尾
        }
        else
        {
            handleError("文件数据格式错误或解析失败,无法读取专业!");
            free(newStudent);
            free(trimmedLine);
            fclose(file);
            return -1;
        }

        token = strtok(NULL, " ");
        if (token != NULL)
        {
            strncpy(newStudent->className, token, sizeof(newStudent->className) - 1);
            newStudent->className[sizeof(newStudent->className) - 1] = '\0'; //确保字符串以null结尾
        }
        else
        {
            handleError("文件数据格式错误或解析失败,无法读取班级!");
            free(newStudent);
            free(trimmedLine);
            fclose(file);
            return -1;
        }

        token = strtok(NULL, " ");
        if (token != NULL)
        {
            strncpy(newStudent->name, token, sizeof(newStudent->name) - 1);
            newStudent->name[sizeof(newStudent->name) - 1] = '\0'; //确保字符串以null结尾
        }
        else
        {
            handleError("文件数据格式错误或解析失败,无法读取姓名!");
            free(newStudent);
            free(trimmedLine);
            fclose(file);
            return -1;
        }

        token = strtok(NULL, ",\n"); //注意这里添加了 \n 以处理行尾换行符
        if (token != NULL)
            sscanf(token, "%d", &newStudent->totalScore);
        else
        {
            handleError("文件数据格式错误或解析失败,无法读取分数!");
            free(newStudent);
            free(trimmedLine);
            fclose(file);
            return -1;
        }

        //添加到链表末尾
        *tail = newStudent;
        tail = &(newStudent->next);
        newStudent->next = NULL;
    }

    fclose(file);
    return 0;//成功返回 0
}

//教师菜单#
void menuTeacher()
{
    printf("\n | 教务管理系统菜单:\n");
    printf(" | 0.  退出系统\n");
    printf(" | 1.  添加题目\n");
    printf(" | 2.  修改题目\n");
    printf(" | 3.  学生成绩排序\n");
    printf(" | 4.  根据题号查询题目及其答案\n");
    printf(" | 5.  查询学生成绩\n");
}
//学生菜单#
void menuStudent()
{
    printf("\n | 学生答题系统菜单：\n");
    printf(" | 0.  退出系统\n");
    printf(" | 1.  学生答题\n");
    printf(" | 2.  查询学生成绩\n");
    printf(" | 3.  根据题号查询题目及其答案\n");
}
//教师操作#
void t(Question* qHead,Student* sHead)
{
    menuTeacher();
    while (1)
    {
        int choice;//进行哪一个操作
        printf("****请输入操作：");
        scanf("%d", &choice);
        switch (choice)
        {
            case 1: //添加题目
                {
                    int count;
                    printf("请输入要添加的题目数量：");
                    scanf("%d", &count);
                    batchAdd(&qHead, count);
                    break;
                }
            case 2: //修改题目
                {
                    int count;
                    printf("请输入要修改的题目数量：");
                    scanf("%d", &count);
                    if (count>cntQuestions(qHead))
                    {
                        handleError("数量太多,自动为您更改为总题目数量!");
                        count = cntQuestions(qHead);
                    }
                    batchModify(qHead, count);
                    break;
                }
            case 3: //学生成绩排序
                {
                    int order;
                    RE0:
                    printf("升序请输入1,降序请输入2\n");
                    scanf("%d", &order);
                    if (order != 1 && order != 2)
                    {
                        handleError("错误，请重新输入!");
                        goto RE0;
                    }
                    sortStudentsByScore(&sHead,order);
                    break;
                }
            case 4: //根据题号查询题目及其答案
                {
                    int id;
                    printf("请输入题号:");
                    scanf("%d",&id);
                    queryQuestionById(qHead, id);
                    break;
                }
            case 5: //查询学生成绩
                {
                    queryStudentScore(sHead);
                    break;
                }
            case 0:
                {
                    printf("退出系统！\n");
                    return;
                }
            default:
                handleError("无效选择！");
        }
    }
}
//学生操作#
void s(Question* qHead,Student* sHead)
{
    menuStudent();
    while (1)
    {
        int choice;//进行哪一个操作
        printf("****请输入操作：");
        scanf("%d", &choice);
        switch (choice)
        {
            case 1: //学生答题
                {
                    int cnt;
                    //计算链表中题目的总数
                    int totalQuestions = cntQuestions(qHead);
                    printf("题库中共有%d道题\n", totalQuestions);
                    printf("请根据老师的要求输入试卷题目数量:");
                    scanf("%d",&cnt);
                    ans(qHead, &sHead, cnt);
                    break;
                }
            case 2: //查询学生成绩
                {
                    queryStudentScore(sHead);
                    break;
                }
            case 3: //根据题号查询题目及其答案
                {
                    int id;
                    printf("请输入题号:");
                    scanf("%d",&id);
                    queryQuestionById(qHead, id);
                    break;
                }
            case 0:
                {
                    printf("退出系统！\n");
                    return;
                }
            default:
                handleError("无效选择！");
        }
    }
}

