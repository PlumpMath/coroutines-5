coroutines
==========

Example using boost co-routines for a simple job dispatcher

Using boost 1.56

g++ -g -O2  -I ./boost_1_56_0 -o ./jobdispatcher -Wall ./JobDispatcher.cpp -L ./boost_1_56_0/stage/lib -lboost_coroutine -lboost_thread -lboost_context -lboost_chrono -lboost_system -lboost_timer


LD_LIBRARY_PATH=./boost_1_56_0/stage/lib ./jobdispatcher

A "job" is a simple class that increments a count to a total.

First loop runs the jobs using co-routines. They are completed in order of size:

Finished 9 4537129
Finished 35 7578698
Finished 47 11103480
Finished 3 15348233
Finished 27 15960828
Finished 42 19148236
Finished 7 20329774
Finished 19 22421176
Finished 48 23134235
Finished 28 25673220
Finished 10 26405856
Finished 39 29557663
Finished 32 33428080
Finished 15 36339384
Finished 24 36747332
Finished 11 37074222
Finished 1 37336352
Finished 49 37858795
Finished 13 38363985
...


Second loop uses standard sub-routines. The jobs are completed in order of submitting to queue:


Finished 0 45181148
Finished 1 37336352
Finished 2 78108765
Finished 3 15348233
Finished 4 74150618
Finished 5 64104518
Finished 6 50789037
Finished 7 20329774
Finished 8 68705229
Finished 9 4537129
Finished 10 26405856
Finished 11 37074222
Finished 12 53106040
Finished 13 38363985
Finished 14 41142711
Finished 15 36339384
Finished 16 60876115
Finished 17 62843289
Finished 18 96819201
