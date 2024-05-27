//
// Created by Nicola Ricciardi.
//

#include "cli.h"


/**
 * Validate number of parameters. It must be exactly n.
 *
 * @param n number of params required
 * @param argc actual parameters number
 */
void require_n_params_or_fail(const int n, const int argc) {

    if(argc - 1 < n) {
        printf("ERROR: Too few params\n");
        exit(1);
    }

    if(argc - 1 > n) {
        printf("ERROR: Too many params\n");
        exit(1);
    }
}
