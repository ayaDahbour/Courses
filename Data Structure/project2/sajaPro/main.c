// SAJA SHAREEF
// 1200901
// SEC: 2
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define lsize 30
#define lineSize 300
typedef struct course *ptr_course;
typedef struct course *Courses;
typedef struct topic *ptr_topic;
typedef struct topic *ptr;
typedef ptr_topic listoftopic;
struct topic {
    char topic1[lsize];
    ptr_topic next;
};
//THE STRUCT OF NODES IN AVL TREE
struct course {
    char courseName[lsize];
    int CreditHours;
    char CourseCode[lsize];
    char Department[lsize];
    ptr_topic topic;
    ptr_course left;
    ptr_course right;
    int height; ////HOW MANY THE MAXIMUM NUMBER OF NODES UNDER THIS NODE
    int counter;
};

Courses MakeEmpty(Courses course);

ptr_course DoubleRotateWithLeft(ptr_course K3);

int Max(int Lhs, int Rhs);

int Height(ptr_course P);

void PrintInOrder(ptr_course t);

listoftopic makeEmpty(ptr_topic L);

void printList(ptr_topic p);

void trim(char topic[lsize]);

ptr_topic InsertTopic(char *token);

void insert(ptr_topic list, ptr_topic ptr, char topic1[lsize]);

ptr_course SingleRotateWithLeft(ptr_course K2);

Courses
Insertcourse(char name[lsize], int hours, char code[20], char department[20], Courses pCourse, ptr_topic listoftopic);

Courses InsertNewCourse(Courses pCourse);

ptr_course DoubleRotateWithRight(ptr_course K1);

void printMenu();

int readCoursesFile(Courses pCourse);

ptr_course MinValueNode(Courses T);
ptr_topic makeEmptyTopic(ptr_topic L);
void topicsAssociatedWithCourse(Courses pCourse);

ptr_course find(char coursecode[30], Courses pCourse);

void coursesSameDepartment(Courses pCourse);

int checkdep(Courses pCourse, char department[30]);

Courses DeleteCourse(Courses pCourse);

ptr_course SingleRotateWithRight(Courses K1);

ptr_course Delete_code(Courses pCourse, char coursecode[30]);

ptr_topic deleteAllCourses_Letter(Courses pCourse);

void delete_letter(Courses pCourse, char character, ptr_topic l);

ptr DeleteAllCourses_Department(Courses pCourse);

int checkdepTo_DeleteCourses(Courses pCourse, char department[30], ptr pTopic);

void inOrderDisplayOnFile(Courses pCourse, FILE *file);

void storeInFile(Courses pCourse);

void updateCourse(ptr_course courseToUpdate);

int main() {
    Courses courses = NULL;
    courses = MakeEmpty(courses);
    FILE *inputfile;
    char course[lineSize];
    char *token;
    char courseName[lsize];
    int CreditHours;
    char CourseCode[lsize];
    char Department[lsize];
    ptr_topic Listoftopic;
    Listoftopic = (listoftopic) malloc(sizeof(struct topic));
    Listoftopic->next = NULL;
    inputfile = fopen("courses.txt", "r");
    if (inputfile == NULL) {
        printf("cannot found the file\n");
        return -1;
    }
    ptr list_codes;
    ptr list_department;
    list_codes = (listoftopic) malloc(sizeof(struct topic));
    ptr_topic ptrlist;
    int isCoursesFileRead = 0;
    char coursecode[lsize];
    char choice[2];
    do {
        printMenu();
        scanf("%s", choice);
        if (strlen(choice) >
            2)    // if the option more than two character so it will be wrong, because our options are 1,2,3,4,5,7,8,9,10
            strcpy(choice, "0"); //  and this will help us to get rid of the wrong scan like "2d"
        switch (atoi(choice)) {
            case 1: {
                //if (readCoursesFile(courses) != -1) {
                //   isCoursesFileRead = 1;
                while (fgets(course, 250, inputfile)) {
                    token = strtok(course, ":");
                    strcpy(courseName, token);
                    token = strtok(NULL, "#");
                    CreditHours = atoi(token);
                    token = strtok(NULL, "#");
                    strcpy(CourseCode, token);
                    token = strtok(NULL, "/");
                    strcpy(Department, token);
                    ptrlist = InsertTopic(token);
                    courses = Insertcourse(courseName, CreditHours, CourseCode, Department, courses, ptrlist);
                }
                printf("The Courses file has been read successfully\n");
                fclose(inputfile);
                PrintInOrder(courses);
            }
                break;
            case 2 :
                courses = InsertNewCourse(courses);
                break;
            case 3 :

                printf("ENTER CODE OF COURSE YOU WANT TO UPDATE : \n");
                scanf("\n%[^\n]s", coursecode);
                ptr_course ptr;
                ptr = find(coursecode, courses);
                if (ptr != NULL) {
                    updateCourse(ptr);
                    PrintInOrder(courses);
                } else
                    printf("THERE IS NO COURSE THAT HAS THIS CODE !!");
                break;
            case 4 :
                if (courses == NULL) {
                    printf("empty tree !");
                } else
                    PrintInOrder(courses);
                break;
            case 5 :
                topicsAssociatedWithCourse(courses);
                break;
            case 6 :
                coursesSameDepartment(courses);
                break;
            case 7 :
                courses = DeleteCourse(courses);
                PrintInOrder(courses);
                break;
            case 8 :
                list_codes = deleteAllCourses_Letter(courses);
                ptr_topic ptr1 = list_codes->next;
                while (ptr1 != NULL) {
                    Delete_code(courses, ptr1);
                    ptr1 = ptr1->next;
                }
                break;
            case 9 :
                list_department = DeleteAllCourses_Department(courses);
                ptr_topic ptr2 = list_department->next;
                while (ptr2 != NULL) {
                    Delete_code(courses, ptr2);
                    ptr2 = ptr2->next;
                }
                break;
            case 10 :
                storeInFile(courses);
                break;

            case 11 :
                MakeEmpty(courses);
                exit(1);

        }
    } while (atoi(choice) != 9);

    return 0;
}

void printMenu() {
    printf("\n\n-----------------------------------------------------------------------------------------\n"
           "\t\t\t\tChoose an option to do \n");
    printf("1.  Read the file courses.txt file and create the tree.\n"
           "2.  Insert a new course from the user with all its associated data.\n"
           "3.  Find a course and support updating of its information.\n"
           "4.  List courses in lexicographic order with their associated information (credit hours, IDs, and topics).\n"
           "5.  List all topics associated with a given course.\n"
           "6.  List all courses in lexicographic order that belong to the same department.\n"
           "7.  Delete a course.\n"
           "8.  Delete all courses that start with a specific letter.\n"
           "9.  Delete all courses belong to a given department.\n"
           "10. Save all words in file offered_courses.txt\n"
           "11. EXIT.\n\n--->");
}

/////////////////////////////////////////// 1
int Height(ptr_course P) {
    if (P == NULL)
        return -1;
    else
        return P->height;
}

int Max(int Lhs, int Rhs) { // O(1)
    return Lhs > Rhs ? Lhs : Rhs;
}

ptr_course SingleRotateWithLeft(ptr_course K2) {
    ptr_course K1;
    K1 = K2->left;
    K2->left = K1->right;
    K1->right = K2;
    K2->height = Max(Height(K2->left), Height(K2->right)) + 1;
    K1->height = Max(Height(K1->left), K2->height) + 1;
    return K1;
}

ptr_course DoubleRotateWithLeft(ptr_course K3) {
    K3->left = SingleRotateWithRight(K3->left);
    return SingleRotateWithLeft(K3);
}

ptr_course DoubleRotateWithRight(ptr_course K1) {
    /* Rotate between K3 and K2 */
    K1->right = SingleRotateWithLeft(K1->right);
    /* Rotate between K1 and K2 */
    return SingleRotateWithRight(K1);
}
ptr_course MinValueNode(Courses T) {
    ptr_course L = T;
    /* loop down to find the leftmost leaf */
    while (L->left != NULL)
        L = L->left;
    return L;
}

ptr_course SingleRotateWithRight(Courses K1) {
    ptr_course K2;

    K2 = K1->right;
    K1->right = K2->left;
    K2->left = K1;

    K1->height = Max(Height(K1->left), Height(K1->right)) + 1;
    K2->height = Max(Height(K2->right), K1->height) + 1;

    return K2;  /* New root */
}
Courses Insertcourse(char name[lsize], int hours, char code[lsize], char department[lsize], Courses pCourse, ptr_topic listoftopic) { //INSERT TREE
    if (pCourse == NULL) {
        pCourse = malloc(sizeof(struct course));
        if (pCourse == NULL)
            printf("Out of space!!!");
        else {
            pCourse->CreditHours = hours;
            strcpy(pCourse->Department, department);
            strcpy(pCourse->CourseCode, code);
            strcpy(pCourse->courseName, name);
            pCourse->topic = listoftopic;
            pCourse->height = 0;
            pCourse->counter = 0;
            pCourse->left = pCourse->right = NULL;
        }
    } else if (strcmp(code, pCourse->CourseCode) == -1) {//if the new node is less than the parent
        pCourse->left = Insertcourse(name, hours, code, department, pCourse->left, listoftopic);
        if (Height(pCourse->left) - Height(pCourse->right) == 2) {

            if (strcmp(code, pCourse->left->CourseCode) == -1)
                pCourse = SingleRotateWithLeft(pCourse);
            else
                pCourse = DoubleRotateWithLeft(pCourse);
        }
    } else if (strcmp(code, pCourse->CourseCode) == 1) {//if the new node is more than the parent
        pCourse->right = Insertcourse(name, hours, code, department, pCourse->right, listoftopic);
        if (Height(pCourse->right) - Height(pCourse->left) == 2) {
            if (strcmp(code, pCourse->right->CourseCode) == 1)
                pCourse = SingleRotateWithRight(pCourse);
            else
                pCourse = DoubleRotateWithRight(pCourse);
        }
    } else {
        pCourse->counter++;
    }
    //update the height of the  nodes
    pCourse->height = Max(Height(pCourse->left), Height(pCourse->right)) + 1;
    return pCourse;
}
int readCoursesFile(Courses courses) {// TO Read the file courses.txt file and create the tree
    FILE *inputfile;
    char course[lineSize];
    char *token;
    char courseName[lsize];
    int CreditHours;
    char CourseCode[lsize];
    char Department[lsize];
    ptr_topic Listoftopic;
    Listoftopic = (listoftopic) malloc(sizeof(struct topic));
    Listoftopic->next = NULL;
    inputfile = fopen("courses.txt", "r");
    if (inputfile == NULL) {
        printf("cannot found the file\n");
        return -1;
    }
    ptr_topic ptrlist;
    while (fgets(course, 250, inputfile)) {
        token = strtok(course, ":");
        strcpy(courseName, token);
        token = strtok(NULL, "#");
        CreditHours = atoi(token);
        token = strtok(NULL, "#");
        strcpy(CourseCode, token);
        token = strtok(NULL, "/");
        strcpy(Department, token);
        ptrlist = InsertTopic(token);
        courses = Insertcourse(courseName, CreditHours, CourseCode, Department, courses, ptrlist);
    }
    printf("The Courses file has been read successfully\n");
    fclose(inputfile);
    return 0;
}
////////////////////////////////////////////// 2
Courses InsertNewCourse(Courses pCourse) {// Insert a new course from the user with all its associated data
    int num;
    int i = 0;
    ptr_topic ptr;
    ptr_topic list;
    list = (listoftopic) malloc(sizeof(struct topic));
    list->next = NULL;
    ptr = list;
    ptr_course new_course;
    char topic[lsize];
    new_course = (ptr_course) malloc(sizeof(struct course));
    printf("ENTER THE COURSE NAME: \n");
    gets(new_course->courseName);
    gets(new_course->courseName);
    printf("ENTER THE COURSE CODE : \n");
    scanf("%s", (new_course->CourseCode));
    printf("ENTER CREDIT HOURS OF COURSE : \n");
    scanf("%d", &new_course->CreditHours);
    printf("ENTER THE DEPARTMENT :\n");
    gets(new_course->Department);
    gets(new_course->Department);
    printf("ENTER NUMBER OF TOPIC : \n");
    scanf("%d", &num);
    for (i = 0; i < num; i++) {
        printf("enter one topic : \n");
        scanf("%s", topic);
        insert(list, ptr, topic);
        ptr = ptr->next;
    }
    pCourse = Insertcourse(new_course->courseName, new_course->CreditHours, new_course->CourseCode,new_course->Department, pCourse, list);
    PrintInOrder(pCourse);//TO PRINT THE TREE IN ORDER
    return pCourse;
}

ptr_topic InsertTopic(char *token) {// TO INSERT THE TOPICS IN LINKED LIST
    char topic[lsize];
    ptr_topic ptr;
    ptr_topic list;
    list = (listoftopic) malloc(sizeof(struct topic));
    list->next = NULL;
    ptr = list;
    token = strtok(NULL, ",");
    while (token != NULL) {
        strcpy(topic, token);
        trim(topic);
        insert(list, ptr, topic);
        ptr = ptr->next;
        token = strtok(NULL, ",");
    }
    return list;
}

void insert(ptr_topic list, ptr_topic ptr, char topic1[lsize]) {//MAKE LINKED LIST OF TOPICS
    ptr_topic temp;
    temp = (listoftopic) malloc(sizeof(struct topic));
    if (temp != NULL) {
        strcpy(temp->topic1, topic1);
        temp->next = ptr->next;
        ptr->next = temp;
    }
}
///////////////////////////////////////////////////3
void updateCourse(ptr_course courseToUpdate) {// TO Find a course and support updating of its information
    char option;
    printf("ENTER THE NEW COURSE NAME: \n");
    scanf("%s", courseToUpdate->courseName);
    printf("ENTER NEW CREDIT HOURS OF COURSE : \n");
    scanf("%d", &courseToUpdate->CreditHours);
    printf("ENTER THE NEW DEPARTMENT :\n");
    scanf("%s", courseToUpdate->Department);
    printf("Do you want to update Topics? (y/n)\n");
    scanf("%s", &option);
    if (option == 'y' || option == 'Y') {
        ptr_topic list;
        list = (listoftopic) malloc(sizeof(struct topic));
        list->next = NULL;
        ptr_topic ptr = list;
        int num;
        char topic[lsize];
        printf("ENTER NUMBER OF TOPIC : \n");
        scanf("%d", &num);
        for (int i = 0; i < num; i++) {
            printf("enter one topic : \n");
            scanf("%s", topic);
            //trim(topic);
            insert(list, ptr, topic);
            ptr = ptr->next;
        }
        courseToUpdate->topic = list;
    }
}
////////////////////////////////////////////// 4
void PrintInOrder(ptr_course t) { // List courses in lexicographic order with their associated information (credit hours, IDs, and topics).
    if (t != NULL) {
        PrintInOrder(t->left);
        printf("________________________________________________________________________________________________________________________\n");
        printf("Name: %s\nCredit Hours: %d\t\tCode: %s\t\tDepartment: %s", t->courseName, t->CreditHours, t->CourseCode,
               t->Department);
        printf("\nTopics: ");
        printList(t->topic);//TO LIST THE TOPICS OF COURSE
        PrintInOrder(t->right);
    }
}
////////////////////////////////////////5
void topicsAssociatedWithCourse(Courses pCourse) { //List all topics associated with a given course
    char coursecode[lsize];
    printf("ENTER CODE OF COURSE TO LIST ALL TOPICS ASSOCIATED  WITH THIS COURSE :\n");
    scanf("%s", coursecode);
    ptr_course ptr;
    ptr = find(coursecode, pCourse); // FIND THE COURSE TO LIST THE TOPICS
    if (ptr != NULL) {
        printList(ptr->topic);
    } else
        printf("THERE IS NO COURSE THAT HAS THIS CODE !!");
}

ptr_course find(char coursecode[lsize], Courses pCourse) { // FIND THE COURSE THAT HAS A GIVER CODE
    if (pCourse == NULL) {
        return NULL;
    } else if (strcmp(coursecode, pCourse->CourseCode) < 0)
        return find(coursecode, pCourse->left);
    else if (strcmp(coursecode, pCourse->CourseCode) > 0)
        return find(coursecode, pCourse->right);
    else
        return pCourse;
}

/////////////////////////////////////////////////// 6
void coursesSameDepartment(Courses pCourse) { // TO LIST ALL COURSES IN  LEXICOGRAGHIC  ORDER THAT BELONG TO THE SAME DEPARTMENT
    char department[lsize];
    printf("ENTER THE DEPARTMENT :\n");
    scanf("\n%[^\n]s", department);
    int check = 0; // TO CHECK IF THERE ARE COURSES IN A GIVEN DEPARTMENT
    check = checkdep(pCourse, department);
    if (check == 0)
        printf("THERE ARE NO COURSES IN THIS DEPARTMENT\n");
}

int checkdep(Courses pCourse, char department[30]) {
    int i = 0;
    if (pCourse != NULL) {
        checkdep(pCourse->left, department);
        if (strcmp(pCourse->Department, department) == 0) {
            printf("%s\t", pCourse->courseName);
            printf("%s\t", pCourse->CourseCode);
            printf("%s\t", pCourse->Department);
            printList(pCourse->topic);
            i = 1;
        }
        checkdep(pCourse->right, department);
    }
    return i;
}
///////////////////////7
Courses DeleteCourse(Courses pCourse) { // TO DELETE THE COURSE
    char coursecode[lsize];
    printf("ENTER CODE OF COURSE YOU WANT TO DELETE : \n");
    scanf("\n%[^\n]s", coursecode);
    pCourse = Delete_code(pCourse, coursecode);
    return pCourse;
}

Courses Delete_code(Courses T, char code[lsize]) {
    Courses TempCell;
    if (T == NULL) {
        printf("No Course with this Code!\n");
        return T;
    }
    if (strcmp(code, T->CourseCode) < 0) {
        T->left = Delete_code(T->left, code);
    } else if (strcmp(code, T->CourseCode) > 0) {
        T->right = Delete_code(T->right, code);
    } else  // we find it
    {
        if (T->right && T->left) {
            TempCell = MinValueNode(T->right);
            strcpy(T->CourseCode, TempCell->CourseCode);
            // Copy the inorder successor's data to this node
            strcpy(T->Department, TempCell->Department);
            strcpy(T->courseName, TempCell->courseName);
            T->CreditHours = TempCell->CreditHours;
            T->topic = TempCell->topic;
            T->right = Delete_code(T->right, T->CourseCode);
        } else {
            TempCell = T;
            if (T->left == NULL) {
                T = T->right;
            } else if (T->right == NULL) {
                T = T->left;
            }
            free(TempCell);
        }
    }
    if (T == NULL)
        return T;
    T->height = 1 + Max(Height(T->left), Height(T->right));
    int balance = Height(T->left) - Height(T->right);
    if (balance > 1 && Height(T->left->left) - Height(T->left->right) >= 0)
        return SingleRotateWithRight(T);
    if (balance > 1 && Height(T->left->left) - Height(T->left->right) < 0) {
        T->left = SingleRotateWithLeft(T->left);
        return SingleRotateWithRight(T);
    }
    if (balance < -1 && Height(T->right->left) - Height(T->right->right) <= 0)
        return SingleRotateWithLeft(T);
    if (balance < -1 && Height(T->right->left) - Height(T->right->right) > 0) {
        T->right = SingleRotateWithRight(T->right);
        return SingleRotateWithLeft(T);
    }
    return T;
}

/////////////////////////////////////////////////////////////////////////////
void trim(char topic[lsize]) {
    int i;
    for (i = 0; i < lsize; i++) {
        if (i == 0 && topic[0] != ' ')
            break;
        if (topic[i] == '\000')
            break;
        topic[i] = topic[i + 1];
    }
    for (i = 0; i < lsize; i++)
        if (topic[i] == '\n') {
            topic[i] = '\0';
            break;
        }
}

void printList(ptr_topic p) { //TO PRINT THE LIST OF TOPICS
    ptr_topic ptr = p->next;
    if (ptr == NULL) {
        printf("\t EMPTY LIST !\n");
        return;
    }
    while (ptr != NULL) {
        if (ptr->next != NULL)
            printf("%s\t", ptr->topic1);
        else
            printf("%s", ptr->topic1);
        ptr = ptr->next;
    }
    printf("\n");
}
/////////////////////////////////////8
ptr deleteAllCourses_Letter(Courses pCourse)  //TO Delete all courses that start with a specific letter
{
    ptr list_codes;
    list_codes = (listoftopic) malloc(sizeof(struct topic)); // make list of codes of all courses that start with this specific letter to delete
    list_codes->next = NULL;
    char character;
    printf("Enter the first letter from course code you want to delete :\n ");
    scanf(" %c", &character);
    delete_letter(pCourse, character, list_codes); //to insert the list
    return list_codes; // return the list of codes
}

void delete_letter(Courses pCourse, char character, ptr_topic list) {
    if (pCourse != NULL) {
        delete_letter(pCourse->left, character, list);
        if (pCourse->CourseCode[0] == character) { // if first letter of code of course is equel this specific letter
            // to insert code this course in the list
            insert(list, list, pCourse->CourseCode);
        }
        delete_letter(pCourse->right, character, list);
    }
}
//////////////////////////////////9
ptr DeleteAllCourses_Department(Courses pCourse) { // TO Delete all courses belong to a given department
    char department[lsize];
    ptr list; // TO MAKE A LIST OF CODES OF COURSES TO DELETE THE COURSES THAT BELONG TO A GIVEN DEPARTMENT
    list = (listoftopic) malloc(sizeof(struct topic));
    list->next = NULL;
    printf("Enter the department you want to delete all courses belong to : \n ");
    scanf("\n%[^\n]s", department);
    int check = 0; // TO CHECK IF THERE ARE COURSES BELONG TO A GIVEN DEPARTMENT
    check = checkdepTo_DeleteCourses(pCourse, department, list);
    if (check == 0)
        printf("THERE ARE NO COURSES TO DELETE IN THIS DEPARTMENT\n");
    return list;//RETURN THIS LIST
}
int checkdepTo_DeleteCourses(Courses pCourse, char department[30], ptr list) {
    int i = 0;
    if (pCourse != NULL) {
        checkdepTo_DeleteCourses(pCourse->left, department, list);
        if (strcmp(pCourse->Department, department) == 0) {//CHECK IF THE GIVEN DEPARTMENT IS THE SAME AS THE DEPARTMENT OF COURSE IN THE TREE
            //TO INSERT CODE OF THIS COURSE IN THE LIST TO DELETEE
            insert(list, list, pCourse->CourseCode);
            i = 1;
        }
        checkdepTo_DeleteCourses(pCourse->right, department, list);
    }
    return i;
}
///////////////////////////////////////// 10
void inOrderDisplayOnFile(Courses pCourse, FILE *file) {
    if (pCourse != NULL) {
        inOrderDisplayOnFile(pCourse->left, file);//FOR PRINT THE NODES INFORMATION AT LEFT SUB TREE
        fprintf(file, "%s:%d#%s#%s/", pCourse->courseName, pCourse->CreditHours, pCourse->CourseCode,
                pCourse->Department);

        ptr_topic ptr = pCourse->topic->next;
        while (ptr != NULL) {
            if (ptr->next != NULL)
                fprintf(file, "%s, ", ptr->topic1);
            else
                fprintf(file, "%s\n", ptr->topic1);
            ptr = ptr->next;
        }

        inOrderDisplayOnFile(pCourse->right, file);//FOR PRINT THE NODES INFORMATION AT RIGHT SUB TREE
    }
}
void storeInFile(Courses pCourse) {//TO Save all words in file offered_courses.txT
    if (pCourse == NULL)
        printf("There is No Course to save in the file!\n");
    else {
        FILE *OUT_FILE = fopen("offered_courses.txt", "w");//FOR OPEN THE OUTPUT FILE
        inOrderDisplayOnFile(pCourse, OUT_FILE);
        fclose(OUT_FILE);//FOR CLOSE THE OUTPUT FILE
        printf("Done!\n");
    }
}
ptr_course MakeEmpty(Courses course) { //TO MAKE THE TREE OF COURSES EMPTY
    if (course != NULL) {
        MakeEmpty(course->left);
        course->topic = makeEmptyTopic(course->topic);
        MakeEmpty(course->right);
        free(course);
    }
    return NULL;
}
ptr_topic makeEmptyTopic(ptr_topic L)//to make the linked list  empty
{
    if (L != NULL)         // if not empty..
    {
        ptr_topic P; // to leads to the list to delete
        ptr_topic temp; // to keep catching the next struct node that will be deleted after P get rid of its list
        P = L->next;
        L->next = NULL;
        while (P != NULL) // while there is a  node to delete, do the next statements
        {
            temp = P->next; // temp catchs the next struct
            free(P); // P get rid of the current struct node,
            P = temp; // and look to the next  node that will be deleted if it is not NULL
        }
    }
    L = (ptr_topic) malloc(sizeof(struct topic)); // give the Linked List(head) a new place with struct node size
    if (L == NULL) // if there is no place for the Linked List, print that
        printf("Out of memory!\n");
    L->next = NULL; // make the pointer in the struct node points on NULL
    return L;
};
