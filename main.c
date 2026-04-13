#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <inttypes.h>

#include "graph.h"
#include "test_graph.h"

typedef unsigned long long ull;

int main(int argc, char* argv[])
{
    ull K = 0;
    double D = 0.0;
    char* filename = NULL;
    int opt;

    // parse command line options
    while ((opt = getopt(argc, argv, "k:d:")) != -1) {
        switch (opt) {
            case 'k':
                K = strtoull(optarg, NULL, 10);
                break;
            case 'd':
                D = strtod(optarg, NULL);
                break;
            default: 
                printf("Usage: %s -k <K> -d <D> filename\n", argv[0]); 
                printf("\tK: length of random walk (max val. 2^64 - 1)\n"); 
                printf("\tD: damping ratio - probability of jumping to random node (from 0 to 1)\n"); 
                printf("\tFilename: filename of input graph to simulate walks on\n"); 
                exit(EXIT_FAILURE);
        }
    }

    // after options, the remaining argument should be the filename
    if (optind >= argc) {
        fprintf(stderr, "Expected filename after options\n");
        exit(EXIT_FAILURE);
    }
    filename = argv[optind];

    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    printf("K = %llu, D = %f, filename = %s\n", K, D, filename);

    printf("Building graph.\n");
    Graph* g = init_graph(file);
    printf("Vertex count: %"PRIu64". Edge count: %"PRIu64".\n", g->vertex_count, g->edge_count);
    test_graph(g, file);

    fclose(file);
    return 0;
}