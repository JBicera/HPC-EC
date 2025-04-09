# Extra Credit Lab: Cache-Oblivious Sorting

In this lab, you will be implementing the a cache-oblivious sorting algorithm, 
the k-funnel sort. As the primary metric, we will be looking at cache 
misses and try to beat the C standard library *qsort* as implemented on 
PACE.

## Getting Started
Begin by obtaining the starter code from the github repository.

```
    git clone https://github.gatech.edu/omscse6220/ECsp25.git
```

Note that this is the [GT github server](https://github.gatech.edu), so you will 
need to authenticate with your GT credentials.

Optionally, you may choose use a git hosting service for your code.  As always, 
please **do not make your repository public** on Github or another git hosting 
service. Anyone will be able to see it. If you feel the need to use a hosting 
service for your project, please keep your repository private.  Your GT account 
allows you to create free private repositories on [the GT github server](https://github.gatech.edu).  


For more details on the funnel sort algorithm, you can refer to these 
resources:

* [Cache-Oblivious Algorithms and Data Structures [PDF]](http://erikdemaine.org/papers/BRICS2002/paper.pdf)
* [Cache-Oblivious Algorithms and Data Structures [Video]](https://ocw.mit.edu/courses/electrical-engineering-and-computer-science/6-046j-design-and-analysis-of-algorithms-spring-2015/lecture-videos/lecture-24-cache-oblivious-algorithms-searching-sorting/)
* [Walkthrough by Srinivas Eswar [PDF]](documentation/Funnel_Sort.pdf)

## Programming
You need to fill in the implementation of funnel sort in *funnel.c* that is stored in the **student** directory.

Some utility functions are provided in *utils/sort_utils.c* with their simple 
explanations in *utils/sort_utils.h* and timing functions in *utils/timer.c* and *utils/timer.h*.

## Testing Your Code and Measuring Performance
You can test your code for correctness by doing the following:

```sh
   $ make clean
   $ make sort
   $ ./sort -c -n [SIZE]
```

To check the performance of your implementation against quick sort, you can 
run the python script *metrics.py*. You need to pass an argument 
*SIZE* to the script to set up different array sizes for sorting. For eg.
```python2 ./metrics.py 8388608 ``` to run with an array size of 8388608.

The following slurm jobs are provided for your testing on the cluster:

* timing.sub (batch - exclusive)
* pace_interactive.sh (you will need to source *pace_env.sh* like prior labs)

Funnel sort will probably be slower than quick sort, but should have a lot less 
data cache misses if the test case exceeds the cache size.

> Note that you may observe larger cache misses with Funnel Sort when the size is not large enough.

## Submission

Lab submissions for this class will be through Canvas under the appropriate assignment.

The file you need to submit for this lab is:

```sh
   funnel.c
```
This file should exist in the **student** directory of your lab folder.

Do **not** submit or modify any other files.

If you include any additional headers or libraries, please make sure they compile as-is on the VM and PACE.

## Grading

Your grade on this lab will consist of the following:

* Performance score

This lab is scored a little bit differently than prior labs so please review the forum announcement post.
