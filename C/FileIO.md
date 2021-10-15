# File Input Output operations in C

## Reference:
 - **CodeVault** https://youtube.com/playlist?list=PLfqABt5AS4Fmv8DszqJjIjmCjvTtTVslT


## How to read a line of text in C from console

```cpp
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    char text[100];
    printf("Type a line of text here: ");
    // scanf("%[^\n]s", text);
    // getchar(); not recommended

    fgets(text, 100, stdin);
    text[strlen(text) - 1] = 0; // '\0'
    int i;
    for (i = 0; i <= strlen(text); i++) {
        printf("%d ", text[i]);
    }

    printf("\n");
    printf("You typed the line: %s", text);

    return 0;
}
```

<br>

## How to print in binary

To-Do

<br>

## Writing to text files

```cpp
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Point Point;
struct Point {
    int x;
    int y;
};

int main() {
    Point p1 = {
        .x = 12,
        .y = -9
    };

    FILE* out;

    fopen_s(&out, "out.txt", "w");
    if (out == NULL) {
        printf("ERROR: FILE* out\n");
        return 1;
    }

    char buffer_out[256];

    sprintf_s(buffer_out, 256, "%d, %d\n", p1.x, p1.y);
    size_t bytes_wrote = fwrite(buffer_out, sizeof(char), strlen(buffer_out), out);
    fclose(out);

    if (bytes_wrote == strlen(buffer_out)) {
        printf("Successful writing.\n");
    }

    return 0;
}
```

## Reading from text files

```cpp
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Point Point;
struct Point {
    int x;
    int y;
};

int main() {
    FILE* in;

    char buffer_in[256];

    // Reading File
    fopen_s(&in, "out.txt", "r");
    if (in == NULL) {
        return 1;
    }

    if (fgets(buffer_in, 256, in) == NULL) {
        fclose(in);
        return 1;
    }
    fclose(in);

    Point p2;
    sscanf_s(buffer_in, "%d, %d\n", &p2.x, &p2.y);
    printf("Read from file the point: %d %d\n", p2.x, p2.y);
    
    return 0;
}
```

<br>

## Improvements to reading and writing to text files in C

```cpp
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Point Point;
struct Point {
    int x;
    int y;
};

int main() {
    Point p1 = {
        .x = 12,
        .y = -9
    };

    Point p2;

    FILE* out;
    FILE* in;

    // -- Writing File --

    fopen_s(&out, "out.txt", "w");
    if (out == NULL) {
        printf("ERROR: FILE* out\n");
        return 1;
    }

    int byte_wrote = fprintf_s(out, "%d, %d\n", p1.x, p1.y);

    fclose(out);

    if (byte_wrote <= 0) {
        return 2;
    }

    
    // -- Reading File --

    fopen_s(&in, "out.txt", "r");
    if (in == NULL) {
        return 3;
    }

    int elements_read = fscanf_s(in, "%d, %d\n", &p2.x, &p2.y);
    fclose(in);
    if (elements_read < 2) {
        return 4;
    }
    
    printf("Read from file the point: %d %d\n", p2.x, p2.y);
    return 0;
}
```

### Final optimization:

```cpp
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Point Point;
struct Point {
    int x;
    int y;
};

int main() {
    Point p1 = {
        .x = 12,
        .y = -9
    };
    Point p2;
    FILE* file;

    // -- Writing File --
    fopen_s(&file, "out.txt", "w+");
    if (file == NULL) {
        printf("ERROR: FILE* out\n");
        fclose(file);
        return 1;
    }

    int byte_wrote = fprintf_s(file, "%d, %d\n", p1.x, p1.y);
    if (byte_wrote <= 0) {
        fclose(file);
        return 2;
    }

    fseek(file, 0, SEEK_SET); // Important

    // -- Reading File --
    int elements_read = fscanf_s(file, "%d, %d\n", &p2.x, &p2.y);
    fclose(file);
    if (elements_read < 2) {
        return 4;
    }
    
    printf("Read from file the point: %d %d\n", p2.x, p2.y);
    return 0;
}
```

<br>

## Reading/Writing structs to files (aka Serialization)

```cpp
#include <stdio.h>
#include <stdlib.h>

const char* PERSON_FORMAT_IN = "(%[^,], %d, %c)";
const char* PERSON_FORMAT_OUT = "(%s, %d, %c)\n";

typedef struct Person {
    char name[20];
    int age;
    char gender;
} Person;

int main() {
    Person p1 = {
        .name = "John",
        .age = 22,
        .gender = 'M'
    };
    Person p2;

    FILE* file;
    fopen_s(&file, "people.txt", "w+");
    if (file == NULL) {
        return 1;
    }

    fprintf_s(file, PERSON_FORMAT_OUT, p1.name, p1.age, p1.gender);
    fseek(file, 0, SEEK_SET);

    // -- Read --
    fscanf_s(file, PERSON_FORMAT_IN, p2.name, 20, &p2.age, &p2.gender);

    fclose(file);
    return 0;
}
```

<br>

## Read an array of structs in C

`structs.txt` : <br>
```cpp
0 1 15
2 2 30
1 0 1

```

```cpp
#include <stdio.h>
#include <stdlib.h>

typedef struct Point {
    int x, y;
    int val;
} Point;

int main() {
    FILE* file = fopen("structs.txt", "r");
    if (file == NULL) {
        return 1;
    }
 
    char buffer[200];
    fgets(buffer, 200, file);
    
    Point points[100];
    int i = 0;
    while (!feof(file)) {
        Point* p = points + i;
        sscanf(buffer, "%d %d %d", &p->x, &p->y, &p->val);
        fgets(buffer, 200, file);
        i++;
    }
    fclose(file);

    int n = i;
    for (i = 0; i < n; i++) {
        printf("Read point: %d %d %d\n", points[i].x, points[i].y, points[i].val);
    }

    return 0;
}
```

<br>
<br>
<br>
<br>