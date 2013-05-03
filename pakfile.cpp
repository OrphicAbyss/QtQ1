#include <string.h>
#include "pakfile.h"

bool PakFile::isValid() {
    return 0 == strncmp("PACK",&this->id[0], 4);
}

int PakFile::entityCount() {
    return entrySize / sizeof(PakEntry);
}

PakEntry *PakFile::getPackEntries() {
    return (PakEntry *)(((char *)this) + entryOffset);
}

void *PakEntry::getFileData(void *pakHeader) {
    return (char *)pakHeader + filePosition;
}
