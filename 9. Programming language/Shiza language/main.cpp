#include <Windows.h>

#include "lang_parsing.h"
#include <unistd.h>

int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    printf("Ажедаю имя вашигЪо фаила:\n");

    char *file_name = (char *) calloc(100, sizeof(char));

    scanf("%s", file_name);

    FILE *language = fopen(file_name, "rb");
    assert(language);

    lang_asm(language);

    fclose(language);
    free(file_name);

    printf("Хорошечок. Всё ис ОК (и заебОК)\n");

    system("pause");

    return 0;
}
