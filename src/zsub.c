/**
 *  zsub -- pipe zmq sub socket to stdout
 *
 *  Usage: zsub [options] host:port
 *    -h               Show this message and exit
 *    -f <filter>      Subscribe to filter
 *    -s <identity>    Set socket identity
 *
 * @author Ori Livneh <ori@wikimedia.org>
 * @license GPL v2 or later; see COPYING
 * @file
 */

#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <getopt.h>
#include "czmq.h"

static struct config {
    char *endpoint;
    char *filter;
    char *sid;
} config;

/* Print usage and exit */
static void usage() {
    fprintf(stderr,
        "zsub -- pipe zmq sub socket to stdout\n"
        "\n"
        "Usage: zsub [options] host:port\n"
        "  -h               Show this message and exit\n"
        "  -f <filter>      Subscribe to filter\n"
        "  -s <identity>    Set socket identity\n"
        "\n");
    exit(EXIT_FAILURE);
}

/* Read line from socket to stdout */
static int s_print(zloop_t *loop, zmq_pollitem_t *poller, void *arg) {
    char *line = zstr_recv(poller->socket);
    if (line) {
        printf("%s", line);
        free(line);
    }
    return 0;
}

/* Parse command-line arguments */
static int argParse(int argc, char *argv[]) {
    int c;

    while ((c = getopt (argc, argv, "hf:s:")) != -1) {
        switch (c) {
        case 'f' :
            config.filter = optarg;
            break;
        case 's' :
            config.sid = optarg;
            break;
        case 'h' :
        default  :
            usage();
        }
    }

    if (optind >= argc) {
        /* Missing non-option arg */
        usage();
    }
    int rc = asprintf(&config.endpoint, "tcp://%s", argv[optind]);
    assert(rc >= 0);

    return 0;
}

int main(int argc, char **argv)
{

    zctx_t *ctx = zctx_new();
    void *sub = zsocket_new(ctx, ZMQ_SUB);

    config.endpoint = NULL;
    config.filter = NULL;
    config.sid = NULL;
    argParse(argc, argv);

    if (config.filter) {
        zsockopt_set_subscribe(sub, config.filter);
    }
    if (config.sid) {
        zsockopt_set_identity(sub, config.sid);
    }
    zsocket_connect(sub, config.endpoint);

    /* Set up reactor for ZMQ_POLLIN event */
    zloop_t *loop = zloop_new();
    zmq_pollitem_t poll_sub = { sub, 0, ZMQ_POLLIN };
    zloop_poller(loop, &poll_sub, s_print, NULL);
    zloop_start(loop);

    /* Clean-up */
    zloop_destroy(&loop);
    zctx_destroy(&ctx);

    exit(EXIT_SUCCESS);
}
