# null_checker

This program can be use to check null protection for mallocs in any c program. 
You can use the null_checker script in order to test a certain executable.

## usage

to see all the different flags

```sh
cd null_checker
./null_checker -h
```

to execute with a certain program

```sh
cd null_checker
./null_checker -x /path/to/an/executable --args-of-program
```