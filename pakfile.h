#ifndef PAKFILE_H
#define PAKFILE_H

struct PakEntry;

struct PakFile
{
    /** Magic ID should always be the values 'PACK' in a valid pack file. */
    char id[4];
    /** Position of the list of pack entities from the start of the file. */
    int entryOffset;
    /** Size of the entity lump in the file. */
    int entrySize;

    /**
     * Return true if the pack header/file is valid.
     */
    bool isValid();
    /**
     * Return the number of entities in this pack.
     */
    int entityCount();
    /**
     * Return the pointer to the pack entry lump.
     */
    PakEntry *getPackEntries();
};

struct PakEntry
{
    /** Filename including path and extension. */
    char filename[56];
    /** Position of entry in the pack. */
    int filePosition;
    /** Size of the entry in the pack. */
    int fileLength;

    /** Get a pointer to the entry data, given a pointer to the pack header. */
    void *getFileData(void *pakHeader);
};
#endif // PAKFILE_H
