#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int bookId;
    char title[100];
    char author[50];
    int year;
    int status; // 0 = available, 1 = borrowed
} Book;

// Kitap bilgilerini kullanıcıdan alır
Book collectBookInfo() {
    Book book;
    int c;

    printf("Enter book Id: ");
    scanf("%d", &book.bookId);

    while ((c = getchar()) != '\n' && c != EOF); // buffer temizliği

    printf("Enter title: ");
    fgets(book.title, sizeof(book.title), stdin);
    book.title[strcspn(book.title, "\n")] = 0; // satır sonu karakterini sil

    printf("Enter author name: ");
    fgets(book.author, sizeof(book.author), stdin);
    book.author[strcspn(book.author, "\n")] = 0;

    printf("Enter publication year: ");
    scanf("%d", &book.year);

    book.status = 0; // kitap başlangıçta müsait
    while ((c = getchar()) != '\n' && c != EOF);

    return book;
}

// Kitap bilgilerini yazdırır
void printBook(Book book) {
    printf("ID: %d | Title: %s | Author: %s | Year: %d | Status: %s\n",
           book.bookId, book.title, book.author, book.year,
           (book.status == 0) ? "Available" : "Borrowed");
}

// Tüm kitapları listeler
void listBooks(Book books[], int n) {
    if (n == 0) {
        printf("No books in library.\n");
        return;
    }
    for (int i = 0; i < n; i++) {
        printBook(books[i]);
    }
}

// Kitap arama (başlık veya yazara göre)
void searchBook(Book books[], int n) {
    char keyword[100];
    int found = 0;

    printf("Enter title or author to search: ");
    fgets(keyword, sizeof(keyword), stdin);
    keyword[strcspn(keyword, "\n")] = 0;

    for (int i = 0; i < n; i++) {
        if (strstr(books[i].title, keyword) != NULL ||
            strstr(books[i].author, keyword) != NULL) {
            printBook(books[i]);
            found = 1;
        }
    }

    if (!found) {
        printf("No books found matching \"%s\"\n", keyword);
    }
}

// Kitap bilgilerini güncelle
void updateBook(Book books[], int n) {
    int id, i, c;
    printf("Enter Book ID to update: ");
    scanf("%d", &id);
    while ((c = getchar()) != '\n' && c != EOF);

    for (i = 0; i < n; i++) {
        if (books[i].bookId == id) {
            printf("Enter new title: ");
            fgets(books[i].title, sizeof(books[i].title), stdin);
            books[i].title[strcspn(books[i].title, "\n")] = 0;

            printf("Enter new author: ");
            fgets(books[i].author, sizeof(books[i].author), stdin);
            books[i].author[strcspn(books[i].author, "\n")] = 0;

            printf("Enter new publication year: ");
            scanf("%d", &books[i].year);
            while ((c = getchar()) != '\n' && c != EOF);

            printf("Status (0=available, 1=borrowed): ");
            scanf("%d", &books[i].status);
            while ((c = getchar()) != '\n' && c != EOF);

            printf("Book updated successfully.\n");
            return;
        }
    }
    printf("Book with ID %d not found.\n", id);
}

// Kitap silme
int deleteBook(Book books[], int *n) {
    int id, i, j;
    int c;
    printf("Enter Book ID to delete: ");
    scanf("%d", &id);
    while ((c = getchar()) != '\n' && c != EOF);

    for (i = 0; i < *n; i++) {
        if (books[i].bookId == id) {
            for (j = i; j < *n - 1; j++) {
                books[j] = books[j + 1];
            }
            (*n)--;
            printf("Book deleted successfully.\n");
            return 1;
        }
    }
    printf("Book with ID %d not found.\n", id);
    return 0;
}

// Ödünç alma ve iade
void borrowReturnBook(Book books[], int n) {
    int id, choice, c;
    printf("Enter Book ID: ");
    scanf("%d", &id);
    while ((c = getchar()) != '\n' && c != EOF);

    for (int i = 0; i < n; i++) {
        if (books[i].bookId == id) {
            printf("1. Borrow\n2. Return\nYour choice: ");
            scanf("%d", &choice);
            while ((c = getchar()) != '\n' && c != EOF);

            if (choice == 1 && books[i].status == 0) {
                books[i].status = 1;
                printf("Book borrowed successfully.\n");
            } else if (choice == 2 && books[i].status == 1) {
                books[i].status = 0;
                printf("Book returned successfully.\n");
            } else {
                printf("Invalid operation.\n");
            }
            return;
        }
    }
    printf("Book not found.\n");
}

// Dosyaya kaydetme
void saveToFile(Book books[], int n, const char *filename) {
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        printf("Failed to open file for writing.\n");
        return;
    }
    fwrite(books, sizeof(Book), n, fp);
    fclose(fp);
    printf("Library saved to file.\n");
}

// Dosyadan yükleme
int loadFromFile(Book books[], const char *filename) {
    FILE *fp = fopen(filename, "rb");
    int count = 0;
    if (!fp) {
        printf("No saved data found, starting fresh.\n");
        return 0;
    }
    while (fread(&books[count], sizeof(Book), 1, fp) == 1) {
        count++;
    }
    fclose(fp);
    printf("%d books loaded from file.\n", count);
    return count;
}

// Menü ve ana program
int main() {
    Book books[100];
    int n = 0;
    int choice;
    const char *filename = "library.dat";

    n = loadFromFile(books, filename);

    do {
        printf("\n--- Library Management System ---\n");
        printf("1. Add Book\n2. List All Books\n3. Search Book\n");
        printf("4. Update Book\n5. Delete Book\n6. Borrow/Return Book\n");
        printf("7. Save & Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        while (getchar() != '\n'); // buffer temizliği

        switch (choice) {
            case 1:
                if (n < 100) {
                    books[n++] = collectBookInfo();
                } else {
                    printf("Library is full!\n");
                }
                break;
            case 2:
                listBooks(books, n);
                break;
            case 3:
                searchBook(books, n);
                break;
            case 4:
                updateBook(books, n);
                break;
            case 5:
                deleteBook(books, &n);
                break;
            case 6:
                borrowReturnBook(books, n);
                break;
            case 7:
                saveToFile(books, n, filename);
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid choice!\n");
        }
    } while (choice != 7);

    return 0;
}
