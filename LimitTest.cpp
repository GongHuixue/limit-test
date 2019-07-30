/*
 * Copyright (C) 2007 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <pthread.h>
#include <inttypes.h>
#include <sys/resource.h>

#include <algorithm>
#include <functional>
#include <limits>
#include <ostream>

//#include <android/native_window.h>

#include <gui/ISurfaceComposer.h>
#include <gui/LayerState.h>

#include <gui/Surface.h>
#include <gui/SurfaceComposerClient.h>
#include <private/gui/ComposerService.h>

#include <utils/Log.h>
#include <utils/SystemClock.h>
#include <utils/threads.h>
#include <cutils/properties.h>
#include <android-base/properties.h>


#include <ui/DisplayInfo.h>
#include <ui/Rect.h>
#include <utils/String8.h>

#include <math.h>
#include <math/vec3.h>

#include <time.h>

//using namespace android;

static int FREE_MEM_VPT = 80000;
static int const mem_size = 1 * 1024 * 1024;
static int free_mem_size;

struct MEMINFO {
	char cTotalMem[20];
	unsigned long TotalMem;
	char cFreeMem[20];
	unsigned long FreeMem;
	char cBuffers[20];
	unsigned long Buffers;
	char cCached[20];
	unsigned long Cached;
	char cSwapCached[20];
	unsigned long SwapCached;
};

struct CPUINFO {
	char cpu[20];
	unsigned int user;
	unsigned int nice;
	unsigned int system;
	unsigned int idle;
	unsigned int lowait;
	unsigned int irq;
	unsigned int softirq;
};

void get_meminfo(MEMINFO *mem) {
	FILE *fd;
    char buffer[256];
	MEMINFO *temp_mem;
	temp_mem = mem;

	fd = fopen("/proc/meminfo", "r");
	fgets(buffer, sizeof(buffer), fd);
	sscanf(buffer, "%s %lu", temp_mem->cTotalMem, &temp_mem->TotalMem);
	fgets(buffer, sizeof(buffer), fd);
	sscanf(buffer, "%s %lu", temp_mem->cFreeMem, &temp_mem->FreeMem);
	fgets(buffer, sizeof(buffer), fd);
	fgets(buffer, sizeof(buffer), fd);
	sscanf(buffer, "%s %lu", temp_mem->cBuffers, &temp_mem->Buffers);
	fgets(buffer, sizeof(buffer), fd);
	sscanf(buffer, "%s %lu", temp_mem->cCached, &temp_mem->Cached);
	fgets(buffer, sizeof(buffer), fd);
	sscanf(buffer, "%s %lu", temp_mem->cSwapCached, &temp_mem->SwapCached);
    fclose(fd);
}

void get_cpuinfo(CPUINFO *cpu) {
	FILE *fd;
	char buffer[256];
	CPUINFO *cpu_info;
	cpu_info = cpu;
	
	fd = fopen("/proc/stat", "r");
	fgets(buffer, sizeof(buffer), fd);
	sscanf(buffer, "%s %u %u %u %u %u %u %u", cpu_info->cpu, &cpu_info->user, &cpu_info->nice, &cpu_info->system, &cpu_info->idle, &cpu_info->lowait, &cpu_info->irq, &cpu_info->softirq);
	fclose(fd);
}

void * one_thread(void* arg) {
	printf("one thread\n");
	MEMINFO mem_info;
	while(1) {
	    get_meminfo(&mem_info);
 
        printf("---->total mem = %lu, free mem = %lu, free buffer = %lu, free cached = %lu \n", mem_info.TotalMem, mem_info.FreeMem, mem_info.Buffers, mem_info.Cached);
		
		if(mem_info.FreeMem >= FREE_MEM_VPT) {
            int *p = (int *)malloc(sizeof(int) * mem_size);
            if(p != NULL) {
	            memset(p, 0, sizeof(int) * mem_size);
                printf("alloc size = %d, p = %p\n", mem_size, p);
            } else {
                printf("malloc failed\n");
                break;
            }
		}else {
            printf("---------->No Memory Left<----------\n");
	        sleep(1);
		}
	}
	pthread_exit(NULL);
	printf("one thread exit\n");
}

void * two_thread(void* arg) {
	printf("two thread\n");
	MEMINFO mem_info;
	while(1) {
	    get_meminfo(&mem_info);
 
        printf("---->total mem = %lu, free mem = %lu, free buffer = %lu, free cached = %lu \n", mem_info.TotalMem, mem_info.FreeMem, mem_info.Buffers, mem_info.Cached);
		
		if(mem_info.FreeMem >= FREE_MEM_VPT) {
            int *p = (int *)malloc(sizeof(int) * mem_size);
            if(p != NULL) {
	            memset(p, 0, sizeof(int) * mem_size);
                printf("alloc size = %d, p = %p\n", mem_size, p);
            } else {
                printf("malloc failed\n");
                break;
            }
		}else {
            printf("---------->No Memory Left<----------\n");
	        sleep(1);
		}
	}
	pthread_exit(NULL);
	printf("two thread exit\n");
}

void * third_thread(void* arg) {
	printf("third thread\n");
	MEMINFO mem_info;
	while(1) {
	    get_meminfo(&mem_info);
 
        printf("---->total mem = %lu, free mem = %lu, free buffer = %lu, free cached = %lu \n", mem_info.TotalMem, mem_info.FreeMem, mem_info.Buffers, mem_info.Cached);
		
		if(mem_info.FreeMem >= FREE_MEM_VPT) {
            int *p = (int *)malloc(sizeof(int) * mem_size);
            if(p != NULL) {
	            memset(p, 0, sizeof(int) * mem_size);
                printf("alloc size = %d, p = %p\n", mem_size, p);
            } else {
                printf("malloc failed\n");
                break;
            }
		}else {
            printf("---------->No Memory Left<----------\n");
	        sleep(1);
		}
	}
	pthread_exit(NULL);
	printf("third thread exit\n");
}

int main(int argc, char* argv[])
{
	MEMINFO mem_info;
	int rate;
	int ret;
	pthread_t thread_one;
    pthread_t thread_two;
    pthread_t thread_third;
	if(argc < 2) {
        printf("usage: limittest <limit mem size *M> ./limittest 80\n");
        return 1;
	}

    FREE_MEM_VPT = (int)(atoi(argv[1]) * 1000);

	get_meminfo(&mem_info);
    printf("---------->Start Full Test<----------\n");

	printf("limit memory size = %d\n", FREE_MEM_VPT);
	rate = ((100 * (mem_info.FreeMem + mem_info.Buffers + mem_info.Cached))/mem_info.TotalMem);
	printf("---->total mem = %lu, free mem = %lu, free buffer = %lu, free cached = %lu, rate = %d\n", mem_info.TotalMem, mem_info.FreeMem, mem_info.Buffers, mem_info.Cached, rate);
	
    ret = pthread_create(&thread_one, NULL, one_thread, NULL);
    if(ret != 0) {
	    printf("Create Thread One Failed\n");
	    return 0;
    }
    
    ret = pthread_create(&thread_two, NULL, two_thread, NULL);
    if(ret != 0) {
	    printf("Create Thread two Failed\n");
	    return 0;
    }
	
	
    ret = pthread_create(&thread_third, NULL, third_thread, NULL);
    if(ret != 0) {
	    printf("Create Thread Thrid Failed\n");
	    return 0;
    }
	
	//system("pause");
	//pthread_join(thread_one, NULL);
	getchar();
    printf("---------->Stop Full Test<----------\n");
	
}
