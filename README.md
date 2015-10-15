# rcom
A utility to remove comments and documentation from source code files

To learn how to use it type ```rcom --help```

It works officially with C, C++ and Java. It probably works too with any language that has the same type of comments.

To compile just run make in the src directory.

For example, to remove all types of comments from a c file named main.c type: ```
rcom c -a main.c ```

The program is still in Development (beta) because I haven't tested it enough to be sure it doesn't fail in edge cases.

However, rcom makes a backup of the original file by default, so don't be afraid to use it.
