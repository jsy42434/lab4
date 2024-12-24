#include <stdio.h>

int main() {
    char buffer[1024];
    fgets(buffer, sizeof(buffer), stdin); // Read POST body from stdin
    printf("Content-Type: text/plain\n\n");
    printf("Received: %s\n", buffer);
    return 0;
}
