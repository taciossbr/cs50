#include <cs50.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char * argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage ./recover filename\n");
        return 1;
    }

    FILE *file = fopen(argv[1], "r");

    if (file == NULL)
    {
        fprintf(stderr, "Can't open the %s\n", argv[1]);
        return 2;
    }

    FILE *img;
    bool isOpen = false;
    int count = 0;
    char *buffer = (char*)malloc(512), filename[8];
    while (fread(buffer, 512, 1, file) > 0)
    {

        if (buffer[0] == (char)0xff &&
            buffer[1] == (char)0xd8 &&
            buffer[2] == (char)0xff &&
            (buffer[3] & 0xf0) == 0xe0)
        {
            if (isOpen)
            {
                fclose(img);
                isOpen = false;
            }

            sprintf(filename, "%03d.jpg", count++);
            img = fopen(filename, "w");
            isOpen = true;
            fwrite(buffer, 512, 1, img);
        }
        else if (isOpen)
        {
            fwrite(buffer, 512, 1, img);
        }
    }

    fclose(img);
    free(buffer);
    fclose(file);

    return 0;

}