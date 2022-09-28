# Syntacore test
## Getting Started
1. [Installation](#installation)
2. [Testing](#testing)
3. [Usage](#usage)
4. [Example](#example)

## Installation
This describes the installation process using cmake. As pre-requisites, you'll need git and cmake installed.
```
# Clone repository
$ git clone https://github.com/al-romanov/syntacore-task.git
# Go to the project root directory
$ cd syntacore-task
# Generate build files
$ cmake -S . -B build
# Build the project
$ cmake --build build
```
This builds the syntacore-task executable and syntacore-task-lib library and tests. On a unix system, the build directory should now look something like this:
```
\syntacore-task
  \build
    \test
      test
      ...
    libsyntacore_task_lib.a
    libsyntacore_task
    ...
  ...
```
## Testing
For testing solution you'll need to type next following commands.
```
# Go to the build/test directory
$ cd build/test
# Launch tests
$ ctest
```
## Usage
In order to launch the application run the following command.
```
$ ./build/syntacore-task
```
The application supports 3 commands
- k *v* -- adds value to storage
- m *i* -- finds i-th smallest element in the storage
- n *j* -- return number of values in storage which are smaller than given *j*
## Example
There is example of application usage.
```
# Launch the application
$ ./build/syntacore-task
# Input:
k 8 k 2 k -1 m 1 m 2 n 3
# Output:
-1 2 2
```
