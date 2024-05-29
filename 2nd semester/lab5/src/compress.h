#pragma once

#include "printing_in_file.h"
#include "huffman_tree.h"

// Compresses data from in.txt and prints in out.txt.
void Compress(FILE* in, FILE* out);
