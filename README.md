# rcom
A tool to remove comments and documentation from source code files

It works officially with C, C++ and Java. It probably works too with any language that has the same type of comments.

rcom makes a backup of the original file that has the same name with the character '~' appended.

#### Install

To compile just run make in the src directory.

#### Examples

To see all options type ```rcom --help```

* Remove all types of comments 

```rcom -a c file.c file2.c file3.c```

* Remove javadoc comments of comments from a c file named main.c 

```rcom -j java main.java```

* Remove block comments and replace them with empty lines

```rcom -be c++ file.cpp```




###### Bugs

* Files with trigraphs ??/ and ??' may fail since rcom doesn't replace trigraphs by their equivalent characters.




###### License

GNU General Public License v3

![GPLv3 Logo](http://www.gnu.org/graphics/gplv3-127x51.png "GPLv3 Logo")