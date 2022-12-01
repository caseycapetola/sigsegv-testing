#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <unistd.h>

void* ptr;
// int PAGE_SIZE;
// void handle_sigsegv(int sig) {
//     printf("SIGSEGV SIGNAL!\n");
//     sleep(1);
//     int val = mprotect(&ptr, 1024, PROT_WRITE | PROT_READ);
//     printf("%d\n", val);
//     if(val==0) {
//         printf("successful mprotect\n");
//     }
// }

void handle_sigsegv(int sig, siginfo_t *info, void *ucontext) {
    printf("si code: %d\n", info->si_code);
    int* temp = (int *) ucontext;
    printf("ucontext: %d\n", *temp);
    printf("bitwise and check: %d\n", *temp & 3);
    int val = mprotect(ptr, 1024, PROT_WRITE | PROT_READ);
    printf("%d\n", val);
    sleep(1);
}

int main(int argc, char* argv[]) {
    struct sigaction sa;
    sa.sa_sigaction = &handle_sigsegv;
    // sa.sa_flags = SA_RESTART;
    sa.sa_flags = SA_SIGINFO;
    // sigaction(SIGTSTP, &sa, NULL);
    sigaction(SIGSEGV, &sa, NULL);

    
    // ptr = (char*)malloc(1024);
    // ptr = malloc(1024);

    int PAGE_SIZE = sysconf(_SC_PAGE_SIZE);
    int vm_size = 16 * PAGE_SIZE;
    if (posix_memalign(&ptr, PAGE_SIZE, vm_size))
    {
        fprintf(stderr, "posix_memalign failed\n");
        return -1;
    }

    char* ptr1 = (char *)ptr;
    // ptr1 = &ptr1[0];
    printf("ptr1: %p, &ptr1[0]: %p\n", ptr1, &ptr1[0]);
    int val = mprotect(ptr1, 1024, PROT_READ);
    printf("main val: %d\n", val);
    // char yuh = ptr1[0];
    printf("yuh\n");
    ptr1[0] = 'a';
    printf("made it\n");
    char* tmp = ptr1;
    char character = ptr1[0];
    printf("made it lower!\n");
    printf("char val: %c\n", character);


    // ptr[1] = 'a';
    // printf("%s", ptr[1]);

    // int x;
    // printf("Input Number: ");
    // scanf("%d", &x);
    // printf("Result %d * 5 = %d\n", x, x * 5);
    return 0;
}
