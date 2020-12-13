# Real time

## timing.c
We test about timing accuracy with PREEMPT_RT and preloads page. 
The testbench itself is mostly come from [‘Real Time’ vs. ‘Real Fast’: How to Choose?](https://www.kernel.org/doc/ols/2008/ols2008v2-pages-57-66.pdf) materials, and we assemble them into a runnable c file.

To run it, use the following command:
```sh
gcc timing.c -o timing -lm [-D analyze]
sudo ./timing
```

Notice that the result is mostly matched with [‘Real Time’ vs. ‘Real Fast’: How to Choose?](https://www.kernel.org/doc/ols/2008/ols2008v2-pages-57-66.pdf)'s, which is at most 3 microseconds of error (most of the time being positive), and on average 1.X microseconds. The std dev is rather stable too (which is under 0.5 mostly)

However, it is worth mentioning that sometimes the result ramps into 7~8 microseconds (as average), with a high std dev too (1~2). These weird occasional results resemble much to **timing without preloading page**. However, we are still unsure the reason causing new page fault (if that were the actual reason).
