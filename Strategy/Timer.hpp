#ifndef TIMER_HH
#define TIMER_HH

#ifdef WIN32
#include <windows.h>
#else
#include <sys/time.h>
#endif

class Timer {
public:
  Timer() {
#ifdef WIN32
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&start);
#else
    start.tv_sec = start.tv_usec = 0;
    gettimeofday(&start, NULL);
#endif
  }

  double getElapsedMicroseconds() {
#ifdef WIN32
    QueryPerformanceCounter(&end);
    return (end.QuadPart - start.QuadPart) * 1e6 / freq.QuadPart;
#else
    gettimeofday(&end, NULL);
    return (end.tv_sec - start.tv_sec) * 1e6 + end.tv_usec - start.tv_usec;
#endif
  }

protected:
#ifdef WIN32
  LARGE_INTEGER freq, start, end;
#else
  timeval start, end;
#endif
};

#endif
