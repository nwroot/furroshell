#pragma once

#ifdef DEBUG
#define DEBUG_OUT(fmt, ...) (fprintf(stderr, "%s:%s:%d: " fmt, __FILE__, __func__, __LINE__, ##__VA_ARGS__))
#else
#define DEBUG_OUT(fmt, ...) do {} while(0);
#endif
