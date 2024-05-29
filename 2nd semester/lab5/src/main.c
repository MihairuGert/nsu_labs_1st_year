#include "compress.h"
#include "decompress.h"

int main(void) {
    FILE* in = fopen("in.txt", "rb");
    FILE* out = fopen("out.txt", "wb");
    char setting;
    if (!fscanf(in, "%c", &setting)) {
        fclose(in);
        fclose(out);
        return 0;
    }
    if (setting == 'c') {
        Compress(in, out);
    } else if (setting == 'd') {
        Decompress(in, out);
    }
    fclose(in);
    fclose(out);
    return EXIT_SUCCESS;
}
