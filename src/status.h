#ifndef STATUS_H
#define STATUS_H

// Standard system return codes
#define KERNIX_ALL_OK 0
#define EIO 1          // General I/O error
#define EINVARG 2      // Invalid argument
#define ENOMEM 3       // Out of memory
#define EBADPATH 4     // Bad file path
#define EFSNOTUS 5     // Not our filesystem
#define ERDONLY 6      // Read-only filesystem
#define EUNIMP 7       // Unimplemented feature
#define EISTKN 8       // Is taken (resource in use)
#define EINFORMAT 9    // Invalid format (e.g., bad executable)

#endif