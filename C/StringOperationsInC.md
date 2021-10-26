# String operations in C

## Reference:
 - **CodeVault** https://www.youtube.com/playlist?list=PLfqABt5AS4FmSwyvP5a3mYsaksq6yR3-Z

## String literals and string initializations

```cs
int main(int argc, char** argv){
    
    char str[] = "Hello";
    printf("%s\n", str); // Hello
    str[0] = 'h';
    printf("%s\n", str); // hello

    return 0;
}
```

Working fine.

```cs
int main(int argc, char** argv){
    
    char* str = "Hello";
    printf("%s\n", str); // Hello
    str[0] = 'h';
    printf("%s\n", str); // crash !!!

    return 0;
}
```

Crashing, but why?

```cs
char str[] = "Hello"; // is equivalent to:
char str[] = {'H', 'e', 'l', 'l', 'o', '\0};
```

```cs
const static char noname[] = "Hello!"; // 
char* str = noname; // equivalent to:
// char* str = "Hello!"; // and the problem also breaks
```

- The computer stores the `char* str` as a compile time constant (i think)
- The computer stores the `char[] str` on the function stack.

What's the difference?

`char* str` might be read-only.

So if you want to initialize a string that can be later modified, use this:

```cs
char[] str = "modifiable";
// or
char* str = malloc(50 * sizeof(char));
strcpy(str, "Hello!");
```

Like this:

```cs
int main(int argc, char** argv){
    
    char* str = malloc(50 * sizeof(char));
    strcpy(str, "Hello!");
    str[0] = 'A';
    printf("%s\n", str); // Aello!
 
    return 0;
}
```

<br>
<br>

## How to declare an array of strings in C

At first:

`#include <string.h>`

```cs
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char** argv){
    
    char array[3][50];

    // string copy
    strcpy(array[0], "Hello World!");
    strcpy(array[1], "test");
    strcpy(array[2], "123");
    
    printf("Strings are: \n%s\n%s\n%s\n", array[0], array[1], array[2]);

    return 0;
}
```

Good.

Let's see Another way:

```cs
int main(int argc, char** argv){
    
    char array[3 * 50];

    // string copy
    strcpy(array, "Hello World!");
    strcpy(array + 50, "test");
    strcpy(array + 100, "123");
    
    printf("Strings are: \n%s\n%s\n%s\n", array, 
    array + 50, array + 100);

    return 0;
}
```

It's working too, amazing.

Here's another way:

```cs
int main(int argc, char** argv){
    
    char array[][50] = {
        "Hello World",
        "test",
        "123"
    };

    printf("Strings are: \n%s\n%s\n%s\n", array[0], array[1], array[2]);

    return 0;
}
```

Wonderful.

But.

When using the array initializer you don't need to write 3 there `char array[3][50] = {...}`. Okay?

So: 

```cs
int main(int argc, char** argv){
    
    char array[][50] = { // HERE
        "Hello World",
        "test",
        "123"
    };

    printf("Strings are: \n%s\n%s\n%s\n", array[0], array[1], array[2]);

    return 0;
}
```

The size of the outer array is automatically computed. However you can't leave the `[50]` part.

Also keep in mind that this notation `char** array = {...}` will doesn't really work. It will compile, but will crash.

<br>
<br>

## How to properly copy text in C

Look at the following code:

```cs
int main(int argc, char** argv){
    
    char text1[50];
    char text2[] = "Temp";
    char text3[] = "Hello this world";

    strcpy(text1, text3);

    printf("%s\n%s\n%s\n", text1, // Hello this world
    text2, // Temp
    text3); // Hello this world

    return 0;
}
```

As expected.

Another way:

```cs
int main(int argc, char** argv){
    
    char text1[50];
    char text2[] = "Temp";
    char text3[] = "Hello this world";

    // strcpy(text1, text3);
    strncpy(text1, text3, 17); // We can tell how many chars we want to copy
    // We use 17 instead of 16 bcs strncpy doesn't copy the null character like strcpy
    // To copy the full text:
    // strncpy(text1, text3, strlen(text3) + 1);
    // strlen doesn't include the null character too

    printf("%s\n%s\n%s\n", text1, text2, text3);

    return 0;
}
```

For more information:
    - https://www.youtube.com/watch?v=R5Xk5N9-rno&list=PLfqABt5AS4FmSwyvP5a3mYsaksq6yR3-Z&index=3


<br>
<br>
<br>
<br>
<br>