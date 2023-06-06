#ifndef SINGLEPROCESS_H
#define SINGLEPROCESS_H

// LOCAL INCLUDES
#include "../core/status_codes.h"
#include "../utils/args.h"
#include "../core/structs.h"
#include "../core/reader.h"
#include "../core/writer.h"
#include "../core/pager.h"

// METHODS

/*
    Runs the whole program as a single process

    Returns success or error exit code
*/
int run_single_process(Args* args);

#endif