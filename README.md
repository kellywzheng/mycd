# mycd
Implementations of the "cd" Unix command in C++ and Python


## To run the code:
Download the zip file of the repository. In the command line, navigate to the zip file's directory.

### Python:
To run the example test cases: \
```
python3 mycd.py
```
To provide your own test case: \
* python3 mycd.py (current directory) (new directory)

### C++:
To run the example test cases: \ 
* g++ -std=c++11 mycd.cpp\
* ./a.out\
To provide your own test case: \
* g++ -std=c++11 mycd.cpp\
* ./a.out (current directory) (new directory)

## Test Results

| Current Directory  | New Directory |  Output Result |
| ------------- | ------------- | ------------- |
| /  | abc | /abc |
| /abc/def  | ghi  | /abc/def/ghi |
| /abc/def | .. | /abc |
| /abc/def | /abc | /abc |
| /abc/def | /abc/klm | /abc/klm |
| /abc/def | ../.. | / |
| /abc/def | ../../.. | / |
| /abc/def | . | /abc/def |
| /abc/def | ..klm | ..klm: No such file or directory |
| /abc/def | ////// | / |
| /abc/def | ...... | ......: No such file or directory |
| /abc/def | ../gh///../klm/. | /abc/klm |
