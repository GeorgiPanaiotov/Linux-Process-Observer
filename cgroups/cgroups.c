#include "cgroups.h"

void create_cgroup(pid_t pid)
{
    int dir_status = mkdir("/sys/fs/cgroup/lbx", S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);
    if (dir_status && errno != EEXIST)
    {
        perror("create_cgroup: mkdir");
        exit(EXIT_FAILURE);
    }

    int cgrp_proc_fd = open("/sys/fs/cgroup/lbx/cgroup.procs", O_WRONLY);
    if (cgrp_proc_fd == -1)
    {
        perror("create_cgroup: open");
        exit(EXIT_FAILURE);
    }

    char pid_str[32];
    int pid_len = snprintf(pid_str, sizeof(pid_str), "%d\n", pid);

    if (write(cgrp_proc_fd, pid_str, pid_len) < 0)
    {
        perror("create_cgroup: write");
        close(cgrp_proc_fd);
        exit(EXIT_FAILURE);
    }

    close(cgrp_proc_fd);
}

void cleanup_cgroup(void)
{
    int rmdir_status = rmdir("/sys/fs/cgroup/lbx");
    if (rmdir_status == -1)
    {
        perror("cleanup_cgroup: rmdir");
        exit(EXIT_FAILURE);
    }
}

void enable_cgroup_controllers(void) {
    int fd = open("/sys/fs/cgroup/cgroup.subtree_control", O_WRONLY);
    if (fd >= 0) {
        const char *ctrls = "+cpu +memory +io\n";
        write(fd, ctrls, strlen(ctrls));
        close(fd);
    }
}