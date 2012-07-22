/**
 *  zpub -- pipe lines from stdin to zmq pub socket
 *
 *  Usage: zpub [options]
 *    -p <port>        Server port (default: 8006)
 *    -i <interface>   Interface to bind (default: "*")
 *    -m <count>       ZeroMQ high-water mark (default: 1000)
 *    -h               Show this message and exit
 *
 * @author Ori Livneh <ori@wikimedia.org>
 * @file
 */

#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <getopt.h>
#include "czmq.h"

#define Z_IFACE "*";
#define Z_PORT 8006;
#define Z_HWM 1000;

static struct config {
    char *iface;  // interface
    int port;     // port
    int hwm;      // high-water mark
} config;


/* Print usage and exit */
static void usage() {
    fprintf(stderr,
        "zpub -- pipe lines from stdin to zmq pub socket\n"
        "\n"
        "Usage: zpub [options]\n"
        "  -p <port>        Server port (default: %d)\n"
        "  -i <interface>   Interface to bind (default: %s)\n"
        "  -m <count>       ZeroMQ high-water mark (default: %d)\n"
        "  -h               Show this message and exit\n"
        "\n", config.port, config.iface, config.hwm );
    exit(EXIT_FAILURE);
}

/* Parse command-line arguments */
static int argParse(int argc, char *argv[]) {
    int c;

    while ((c = getopt (argc, argv, "hi:p:")) != -1) {
        switch (c) {
        case 'p' :
            config.port = atoi(optarg);
            break;
        case 'i' :
            config.iface = optarg;
            break;
        case 'm' :
            config.hwm = atoi(optarg);
            break;
        case 'h' :
        default  :
            usage();
        }
    }

    return 0;
}

int main(int argc, char **argv)
{
    zctx_t *ctx = zctx_new();
    void *pub = zsocket_new(ctx, ZMQ_PUB);
    char *line = NULL;
    size_t len = 0;

    config.hwm = Z_HWM;
    config.iface = Z_IFACE;
    config.port = Z_PORT;
    argParse(argc, argv);

    /* Flush pending messages on close */
    zctx_set_linger(ctx, 0);

    zsockopt_set_hwm(pub, config.hwm);
    zsocket_bind(pub, "tcp://%s:%d", config.iface, config.port);

    /* Read line and write to zmq socket until EOF */
    while (getline(&line, &len, stdin) != -1) {
        zstr_send(pub, line);
    }

    /* Clean-up */
    zctx_destroy(&ctx);
    free(line);

    exit(EXIT_SUCCESS);
}
