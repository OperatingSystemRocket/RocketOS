#pragma once

#define BYTES_IN_WORD 4
#define MIN_BLOCK_SIZE 8 //4 words for bookkeeping and 4 word payload 
#define WORDS_IN_PAGE (PAGE_SIZE/BYTES_IN_WORD)
