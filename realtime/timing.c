#include <sched.h>
#include <time.h>
#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef analyze
#include <math.h>
#endif

#ifdef analyze
void analyzer(double list[], int size) {
	double avg, var, std_dev, sum=0, sum_diff=0;
	for (int j=0; j<size; j++) {
		sum += list[j];
	}
	avg = sum / (double)size;
	for (int k=0; k<size; k++) {
		sum_diff += pow((list[k] - avg), 2);
	}
	var = sum_diff / (double)size;
	std_dev = sqrt(var);
	printf("Avg = %.2f\n", avg);
	printf("Var = %.2f\n", var);
	printf("Dev = %.2f\n", std_dev);
}
#endif

void validation(int iter, long nDelay) {
	#ifdef analyze
	double x[100];
	if (iter > 100) {
		perror("Iter too large");
		exit(-1);
	}
	#endif
	for (int i = 0; i < iter; i++) {
		struct timespec timestart;
		struct timespec timeend;
		struct timespec timewait;
		timewait.tv_sec = nDelay/1000000;
         	timewait.tv_nsec = (nDelay % 1000000) * 1000;
		if (clock_gettime(CLOCK_MONOTONIC, &timestart) != 0) {
			perror("clock_gettime 1");
			exit(-1);
		}
		if (nanosleep(&timewait, NULL) != 0) {
			perror("nanosleep");
			exit(-1);
		}
		if (clock_gettime(CLOCK_MONOTONIC, &timeend) != 0) {
			perror("clock_gettime 2");
			exit(-1);
		}
		double delta = (double)(timeend.tv_sec - timestart.tv_sec)*1000000 +
			(double)(timeend.tv_nsec - timestart.tv_nsec) / 1000.;
		#ifdef analyze
		x[i] = delta - (double)nDelay;
		#endif
		#ifndef analyze
		printf("iter %d delta %g\n", i, delta - (double)nDelay);
		#endif
	}
	#ifdef analyze
	analyzer(x, iter);
	#endif
}

void set_env() {
	struct sched_param sched;
	sched.sched_priority = sched_get_priority_max(SCHED_FIFO);
	if (sched.sched_priority == -1) {
		perror("sched_get_priority_max");
		exit(-1);
	}
	if (sched_setscheduler(0, SCHED_FIFO, &sched) != 0) {
		perror("sched_setscheduler");
		exit(-1);
	}

	if (mlockall(MCL_CURRENT | MCL_FUTURE) != 0) {
		perror("mlockall");
		exit(-1);
	}
}

int main() {
	set_env();
	validation(20, 20);
	return 0;
}
