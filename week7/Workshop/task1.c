#include <stdio.h>
#include <unistd.h>
#include <omp.h>

omp_lock_t lock;
int booksAvailable = 3;

void borrower(int id)
{
    for (int i = 0; i < 10; i++) // Reduced iterations for demonstration
    {
        // Check and borrow a book
        int bookBorrowed = 0;
        omp_set_lock(&lock);
        if (booksAvailable > 0)
        {
            printf("Borrower %d wants to borrow a book.\n", id);
            printf("Books available = %d\n", booksAvailable);
            booksAvailable--;
            bookBorrowed = 1;
            printf("Borrower %d gets a book.\n", id);
            printf("Books available = %d\n", booksAvailable);
        }
        else
        {
            printf("Borrower %d wants to borrow a book but none are available.\n", id);
        }
        omp_unset_lock(&lock);

        if (bookBorrowed)
        {
            // Simulate reading a book
            printf("Borrower %d is reading a book.\n", id);
            usleep(10000);

            // Return the book
            omp_set_lock(&lock);
            printf("Borrower %d is returning a book.\n", id);
            booksAvailable++;
            printf("Books available = %d\n", booksAvailable);
            omp_unset_lock(&lock);
        }

        usleep(10000); // Simulate some delay before next attempt
    }
}

int main()
{
    omp_init_lock(&lock);

    #pragma omp parallel num_threads(5) // 5 borrowers
    {
        int id = omp_get_thread_num();
        borrower(id);
    }

    omp_destroy_lock(&lock);
    return 0;
}
