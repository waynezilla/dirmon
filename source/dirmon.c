/**********************************************************************
*                              dirname                                *
**********************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/inotify.h>
#include <limits.h>
#include <unistd.h>
#define BUF_LEN ( 10 * (sizeof(struct inotify_event) + NAME_MAX + 1) )

static void
displayInotifyEvent(struct inotify_event *i) {


    if (i->mask & IN_OPEN) printf("ACCESSSED ");
    printf("\n");

    if (i->len > 0) printf("\t name = %s\n", i->name);
}

int
main(int argc, char *argv[])
{
    int inotifyFd, wd, j;
    char buf[BUF_LEN] __attribute__((aligned(8)));
    ssize_t numRead;
    char *p;
    struct inotify_event *event;

    inotifyFd = inotify_init();
    wd = inotify_add_watch(inotifyFd, argv[1], IN_ALL_EVENTS);
    printf("Watching %s using wd %d\n", argv[1], wd);

    for (;;) {
        numRead = read(inotifyFd, buf, BUF_LEN);
        if (numRead == 0) {
            printf("read returned 0");
            exit(1);
        }

        printf("Read %ld bytes from inotify fd\n", (long) numRead);

        for ( p =buf; p < buf + numRead; ) {
            event = (struct inotify_event *) p;
            displayInotifyEvent(event);
            p += sizeof(struct inotify_event) + event->len;
        }
    }

    exit(0);
}
