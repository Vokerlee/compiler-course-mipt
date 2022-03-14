#include "sort_header.h"

int main(int argc, char* argv[])
{
    FILE* onegin = fopen(argv[1], "rb");
    FILE* res = fopen(argv[2], "wb");

    //FILE* onegin = fopen("source.txt",  "rb");
    //FILE* res    = fopen("results.txt", "wb");

    assert(onegin);
    assert(res);

    //sortlines(onegin, res, "rhyme", "bred+");
    sortlines(onegin, res, argv[3], argv[4]);

    fclose(onegin);
    fclose(res);

    //sortlines_test();

    return 0;
}

