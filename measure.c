#include <fcntl.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#define FIB_DEV "/dev/fibonacci"


int main()
{
    char write_buf[] = "testing writing";
    int offset = 100;
    int iterations = 5000;
    int fd = open(FIB_DEV, O_RDWR);
    if (fd < 0) {
        perror("Failed to open character device");
        exit(1);
    }
    for (int i = 0; i <= offset; i++) {
        lseek(fd, i, SEEK_SET);
        double kernel_time[iterations];
        double user_time[iterations];
        double mean_k = 0.0, sd_k = 0.0, sd_u = 0.0, mean_u = 0.0, res_k = 0.0,
               res_u = 0.0, sys_cal = 0.0;
        int count_k = 0, count_u = 0;
        for (int j = 0; j < iterations; j++) {
            long long sz, fz;
            struct timespec t_start, t_end;
            clock_gettime(CLOCK_REALTIME, &t_start);
            sz = write(fd, write_buf, 0);
            fz = write(fd, write_buf, 1);
            clock_gettime(CLOCK_REALTIME, &t_end);
            long long dif = t_end.tv_nsec - t_start.tv_nsec;
            kernel_time[j] = sz;
            user_time[j] = dif;
            mean_k += sz;
            mean_u += dif;
        }
        // get mean
        mean_k /= iterations;
        mean_u /= iterations;

        // get standar deviation
        for (int j = 0; j < iterations; j++) {
            sd_k += (kernel_time[j] - mean_k) * (kernel_time[j] - mean_k);
            sd_u += (user_time[j] - mean_u) * (user_time[j] - mean_u);
        }
        sd_k = sqrt(sd_k / iterations);
        sd_u = sqrt(sd_u / iterations);

        // remove outliers
        for (int j = 0; j < iterations; j++) {
            if (kernel_time[j] <= (mean_k + 1 * sd_k) &&
                kernel_time[j] >= (mean_k - 1 * sd_k)) {
                res_k += kernel_time[j];
                count_k += 1;
            }
            if (user_time[j] <= (mean_u + 1 * sd_u) &&
                user_time[j] >= (mean_u - 1 * sd_u)) {
                res_u += user_time[j];
                count_u += 1;
            }
        }
        res_k /= count_k;
        res_u /= count_u;
        sys_cal = res_u - res_k;
        printf("%d %f %f %f \n", i, res_k, res_u, sys_cal);
    }
}