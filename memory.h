//
// Created by Oleslav Boychuk on 13.09.2023.
//

#ifndef MAPENGINE_MEMORY_H
#define MAPENGINE_MEMORY_H

#include <iostream>
#include <mach/vm_statistics.h>
#include <mach/mach_types.h>
#include <mach/mach_init.h>
#include <mach/mach_host.h>
#include <mach/mach.h>

int print_memory_stat()
{
    struct task_basic_info t_info{};
    mach_msg_type_number_t t_info_count = TASK_BASIC_INFO_COUNT;
    vm_size_t page_size;
    mach_port_t mach_port;
    mach_msg_type_number_t count;
    vm_statistics64_data_t vm_stats;
    mach_port = mach_host_self();
    count = sizeof(vm_stats) / sizeof(natural_t);
    if (KERN_SUCCESS == host_page_size(mach_port, &page_size) &&
        KERN_SUCCESS == host_statistics64(mach_port, HOST_VM_INFO,
                                          (host_info64_t)&vm_stats, &count) &&
        KERN_SUCCESS == task_info(mach_task_self(),TASK_BASIC_INFO, (task_info_t)&t_info,
                                  &t_info_count))
    {
        long long free_memory = (int64_t)vm_stats.free_count * (int64_t)page_size;
        long long used_memory = ((int64_t)vm_stats.active_count +
                                 (int64_t)vm_stats.inactive_count +
                                 (int64_t)vm_stats.wire_count) *  (int64_t)page_size;
        printf(
                "--------------------------\n"
                // "suspend count for task: %d\n"
                "virtual memory size: %lu MB\n"
                "resident memory size: %lu MB\n"
                "free memory: %lld MB\n"
                "used memory: %lld MB\n"
                // "total user run time for terminated threads: %d ms\n"
                // "total system run time for terminated threads: %d ms\n"
                // "default policy for new threads: %d\n"
                // t_info.suspend_count,
                "--------------------------\n",
                t_info.virtual_size / 1000000,
                t_info.resident_size / 1000000,
                free_memory / 1000000,
                used_memory / 1000000
                // _info.user_time.microseconds,
                // _info.system_time.microseconds,
                // _info.policy
        );
    }
    return 0;
}

#endif //MAPENGINE_MEMORY_H
