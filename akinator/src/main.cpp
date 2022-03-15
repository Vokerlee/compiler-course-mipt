#include "akinator.h"

int main(int argc, char *argv[])
{
    if (argc == 1)
        akinator_menu("akinator_base.txt");
    else
        akinator_menu(argv[1]);

    return 0;
}
