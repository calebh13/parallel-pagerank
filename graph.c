#include "graph.h"
#include <ctype.h>
#include <stdio.h>
#include <string.h>

// Returns 1 if line contained a valid edge, 0 otherwise.
// On success, writes the two endpoints into *in_node and *out_node.
int parse_edge(const char* line, uint64_t* in_node, uint64_t* out_node)
{
    // Skip comments
    if (line[0] == '#' || line[0] == '/') return 0;

    // Advance to next number
    int i = 0;
    for (; line[i] != '\0' && !isdigit(line[i]); i++);
    if (line[i] == '\0') return 0;

    char* endptr;
    *in_node = strtoull(line + i, &endptr, 10);

    // Advance to next number again
    for (; *endptr != '\0' && !isdigit(*endptr); endptr++);
    if (*endptr == '\0') return 0;

    *out_node = strtoull(endptr, NULL, 10);
    return 1;
}

static void set_edge_and_vertex_counts(Graph* g, FILE* file)
{
    char* line = NULL;
    size_t n = 1;
    uint64_t max = 0, edges = 0, in_node, out_node;
    while (getline(&line, &n, file) != -1) {
        if (!parse_edge(line, &in_node, &out_node)) continue;
        edges++;
        if (in_node > max) max = in_node;
        if (out_node > max) max = out_node;
    }
    g->edge_count = edges;
    g->vertex_count = max + 1;
    free(line);
}

// For now, this is assuming no dictionary is needed.
static void build_edges_and_offsets(Graph* g, FILE* file)
{
    char* line = NULL;
    size_t n = 1;
    uint64_t max = 0, edges = 0, in_node, out_node, prev_node = -1;
    while (getline(&line, &n, file) != -1) {
        if (!parse_edge(line, &in_node, &out_node)) continue;
        /* CSR logic: we make a list of all outgoing edges, and if we are starting a new node, 
         * record in `offsets` the index of where itd in bs neighbors begin in `edges`. */
        g->edges[edges] = out_node;
        if (in_node != prev_node) {
            /* Since the list is in sorted order, everything we didn't see in between
             * `in_node` and `prev_node` has the same offset as `in_node`; they have no edges. 
             * Also note that unsigned overflow is defined behavior in C, so this is fine. */
            for (uint64_t i = prev_node + 1; i < in_node; i++) {
                g->offsets[i] = edges;
            }
            g->offsets[in_node] = edges;
        }
        prev_node = in_node;
        edges++;
    }
    // Fill any trailing nodes that had no outgoing edges
    for (uint64_t i = prev_node + 1; i < g->vertex_count; i++) {
        g->offsets[i] = g->edge_count;
    }
    // Sentinel to allow `g->offsets[in_node + 1]` to always work
    g->offsets[g->vertex_count] = g->edge_count;
    free(line);
}

Graph* init_graph(FILE* file) 
{
    Graph* g = calloc(1, sizeof(Graph));
    if (!g) exit(EXIT_FAILURE);
    set_edge_and_vertex_counts(g, file);
    rewind(file);
    g->edges = calloc(g->edge_count, sizeof(uint64_t));
    g->offsets = calloc(g->vertex_count + 1, sizeof(uint64_t)); // includes sentinel at end
    if (!g->edges || !g->offsets) exit(EXIT_FAILURE);
    build_edges_and_offsets(g, file);
    rewind(file);
    return g;
}

void free_graph(Graph* g)
{
    free(g->edges);
    free(g->offsets);
    free(g);
}