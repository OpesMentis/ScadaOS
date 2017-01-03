#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>

#include <handler.h>
#include <decoder.h>
#include <util.h>


int main(int argc, char *argv [])
{
    // parse args
    struct OPTS opts;
    if (parse_args(argc, argv, &opts) == -1)
        exit(EXIT_FAILURE);

    // open handlers
    if (hndopen(opts, &hnd) == -1)
        exit(EXIT_FAILURE);

    // signals handler
    struct sigaction action;
    action.sa_handler = signals_handler;
    sigemptyset(& (action.sa_mask));
    action.sa_flags = 0;
    sigaction(SIGINT, & action, NULL);

    // read port
    char buff[50];
    fd_set fdset;

    while(1)
    {
	
        bzero(buff, sizeof(buff));

        FD_ZERO(&fdset);
        FD_SET(hnd.gpsfd, &fdset);

        select(hnd.gpsfd+1, &fdset, NULL, NULL, NULL);

        if (FD_ISSET(hnd.gpsfd, &fdset))
        {
            int bytes = read(hnd.gpsfd, buff, sizeof(buff));

            if (bytes > 0)
                decode_frame(buff);
        }
    }

    // close handlers
    hndclose(&hnd);

    exit(EXIT_SUCCESS);
}
