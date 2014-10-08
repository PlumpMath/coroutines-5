#include <cstdlib>
#include <iostream>
#include <queue>

#include <boost/bind.hpp>
#include <boost/coroutine/all.hpp>
#include <stdlib.h>     /* srand, rand */

#include <boost/timer/timer.hpp>

class Image {
public:
    Image(int size) :
        x(0), size(size), y(0), miniJobSize(100000)
    {
        std::cout << "Image()" << std::endl;
    };
    ~Image() {
        std::cout << "~Image()" << std::endl;
    };
    int x;
    const int size;
    int y;
    const int miniJobSize;
};

typedef boost::coroutines::symmetric_coroutine< int >  coro_t;
typedef boost::coroutines::symmetric_coroutine< Image& >  coro_Image_t;

void imageOptJobCoroFn( coro_Image_t::yield_type & yield) {
    Image& theJob = yield.get();
    while(theJob.x < theJob.size) {
        int miniJobSize = theJob.size - theJob.x;
        miniJobSize = miniJobSize > theJob.miniJobSize ? theJob.miniJobSize : miniJobSize;
        while(theJob.y < miniJobSize) {
            theJob.x++;
            theJob.y++;
        }
        theJob.y = 0;
        yield();
    }
}
void imageOptJobFn(Image& theJob) {
    while(theJob.x < theJob.size) {
        theJob.x++;
    }
}

class ImageOptJobHolder {
public:
    ImageOptJobHolder(int idx, int imageSize) :
        idx(idx),
        image(imageSize),
        coroImage(imageOptJobCoroFn) {
    }
    int idx;
    Image image;
    coro_Image_t::call_type coroImage;
    bool isDone() {
        return image.x == image.size;
    }
};

void doDispatch()
{
    std::queue<ImageOptJobHolder*> jobQueue;
    uint16_t numJobs = 50;
    ImageOptJobHolder* jobs[numJobs];

    for(int i=0; i<numJobs; i++) {
        /* generate secret number between 1 and 10: */
        int size = rand() % 100000000 + 1;
        ImageOptJobHolder* job = new ImageOptJobHolder(i, size);
        jobs[i] = job;
        jobQueue.push(job);
    }


    {
        boost::timer::auto_cpu_timer timer;

        while(!jobQueue.empty()) {
            ImageOptJobHolder* job = jobQueue.front();
            jobQueue.pop();
            job->coroImage(job->image);
            if(!(job->isDone())) {
                jobQueue.push(job);
            } else {
                std::cout << "Finished " << job->idx << " " << job->image.size << std::endl;
            }
        }
        boost::timer::cpu_times const elapsed_times(timer.elapsed());
        std::cout << "Finished " << (elapsed_times.system + elapsed_times.user) << std::endl;
    }
    {
        for(int i=0; i<numJobs; i++) {
            ImageOptJobHolder* job = jobs[i];
            job->image.x = 0;
            jobQueue.push(job);
        }

        boost::timer::auto_cpu_timer timer;

        while(!jobQueue.empty()) {
            ImageOptJobHolder* job = jobQueue.front();
            jobQueue.pop();
            //std::cout << "Running job " << job << std::endl;
            imageOptJobFn(job->image);
            std::cout << "Finished " << job->idx << " " << job->image.size << std::endl;
        }
        boost::timer::cpu_times const elapsed_times(timer.elapsed());
        std::cout << "Finished " << (elapsed_times.system + elapsed_times.user) << std::endl;
    }
    for(int i=0; i<numJobs; i++) {
        delete jobs[i];
    }

}


int main( int argc, char * argv[])
{
    srand (time(NULL));

    doDispatch();
    std::cout << "Done" << std::endl;

    return EXIT_SUCCESS;
}
