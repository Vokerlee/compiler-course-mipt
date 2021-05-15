# Mini text library

The usage of library is simple. 

You have the folowwing structure of text:

```C++
struct text_t
{
    char *buffer  = nullptr;
    line_t *lines = nullptr;

    size_t n_symbols    = 0;    // the number of symbols in text
    size_t n_real_lines = 0;    // the amount of all the lines in text
    size_t n_lines      = 0;    // the amount lines without empty lines and comment-lines

    int line_counter = 0;
    char *counter    = nullptr;
};
```

First of all you should create text-structure and use constructor for it:
```C++
text_t text = {};
construct_text(&text);
```

Then `fill_text` function is used:

```C++
fill_text(code, &text, NO_COMMENTS);
// void fill_text (FILE *source, text_t *text, char read_regime = DEFAULT_REGIME);
```
Here `code` is a `FILE*` type, there are two regimes: `NO_COMMENTS` and `DEFAULT_REGIME` (you can to ignore it in usage, fill this field empty).

After that the structure of text is filled. To see the results of the program you can use `print_text_lines`:

```C++
void print_text_lines (FILE *res, text_t *text);
```
All in all this `text_t` structure is very convenient to use it in other projects.

Thanks for attention.
