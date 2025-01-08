#include <stdio.h>

struct Student
{
    char name[30];
    int age;
    float gpa;
};

int main()
{
    struct Student students[3];

    for (int i = 0; i < 3; i++)
    {
        printf("Enter the details of student %d\n", i + 1);

        printf("Name: ");
        scanf(" %[^\n]", students[i].name);

        printf("Age: ");
        scanf("%d", &students[i].age);

        printf("GPA: ");
        scanf("%f", &students[i].gpa);

        printf("\n");
    }

    printf("Student Details:\n");
    for (int i = 0; i < 3; i++)
    {
        printf("Name: %s, Age: %d, GPA: %.2f\n", students[i].name, students[i].age, students[i].gpa);
    }

    return 0;
}
