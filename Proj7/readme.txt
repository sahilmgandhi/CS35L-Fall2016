Sahil Gandhi
11/9/2016
TA: Thuy Vu
Lab: Monday/Wednesday 4-6 PM

This is merely a brief report on my homework, detailing issues that I ran into as well as how well my changes 
actually make SRT perform.

I only neeeded to modify main.c and the makefile as this is what the instructions told me to do, and the following 
were the primary changes that I made:

1) I added in the make file, the flag that allows me to compile it to run with multiple threads (-lpthread).

2) I also tried to parallelize the loops ... since this is supposed to be embarassingly parallel, I knew for sure that 
I could parallelize the outer loop without a problem. I also tried to parallelize the inner loops, but this ended up 
taking more time than I expected (and a LOT of extra work) ... which I took to be a sign that I was going on the wrong 
path.

Problems I encountered:

1) The pthread_create takes a function as an argument. That meant I had to take out some of the code from the main 
method into a different function, and that also meant that I needed to make some variables shared. Thus I had to make 
some variables global so they would be initialized in main, and then used in the new function that I made. 

2) When I was passing the starting position of where the parallelization should happen, I kept passing in "(void *) i" 
which gave me segmentation faults. Even though the TAs example had this working, it took me a while to realize that 
this was happening due to that variable, i, being shared across the threads. A race condition was happening, and that 
needed to go away asap. I fixed this by creating an array of ints with the size of nthreads, and then assigning each 
position in that array with the correct offset that I wanted. I then passed that particular cell only to the 
functiooon, and the segmentation fault went away.

3)Another problem that I had was with properly printing out the pixel values to the .ppm file. Even though I took CS33 
and had experience with parallelization, I didn't realize for a VERY long time that if I try to parallelize the 
printing, at some points, that would be in the wrong order depending on which thread got to that point first. Thus the 
solution was to only do that part after I joined all the threads together.

--------------------------------------------------

These were the outputs of "make clean check" for the timing part. It generally seems that adding more threads did 
indeed bring down the overall real time of the program (meaning it executed faster). As expected, the CPU (user) time 
was the same because even though the work was split across many threads, the user time is calculated by summing up the 
total time spent across all the threads. The sys time is roughly the same for of the different thread scenarios.

Between 1 and 2 threads, the time was reduced by 50%, and with each subsequent doubling of threads, the time reducted 
by 50%. That's why the 8 thread version is 8x faster than the 1 thread version, and I believe that my implementation 
of multithreading to SRT greatly improved its performance!

1 thread
real    0m46.131s
user    0m46.120s
sys     0m0.001s

2 threads
real    0m22.290s
user    0m44.466s
sys     0m0.005s

4 threads
real    0m11.234s
user    0m44.626s
sys     0m0.004s

8 threads
real    0m5.833s
user    0m45.280s
sys     0m0.002s
