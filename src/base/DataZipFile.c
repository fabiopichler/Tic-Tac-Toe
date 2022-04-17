//-------------------------------------------------------------------------------
// Copyright (c) 2020-2022 Fábio Pichler
/*-------------------------------------------------------------------------------

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

-------------------------------------------------------------------------------*/

#include "DataZipFile.h"

#ifdef USE_DATA_ZIP

#include <stdio.h>
#include <stdlib.h>

#include <physfs.h>

bool DataZipFile_Init()
{
    if (!PHYSFS_init(NULL))
    {
        printf("Failure. reason: %s.\n", PHYSFS_getLastError());

        return false;
    }

    if (!PHYSFS_mount("data.zip", NULL, 1))
    {
        PHYSFS_deinit();
        printf("Failure. reason: %s.\n", PHYSFS_getLastError());

        return false;
    }

    return true;
}

void DataZipFile_Close()
{
    PHYSFS_deinit();
}

int DataZipFile_Read(const char *filename, char **buffer)
{
    PHYSFS_File *file = PHYSFS_openRead(filename);

    if (file)
    {
        PHYSFS_sint64 size = PHYSFS_fileLength(file);

        if (size > 0)
        {
            *buffer = malloc(size);
            PHYSFS_sint64 readed = PHYSFS_readBytes(file, *buffer, size);
            PHYSFS_close(file);

            if (readed == size)
                return readed;

            free(*buffer);
        }
        else
        {
            PHYSFS_close(file);
        }
    }

    printf("failed to open. Reason: [%s].\n", PHYSFS_getLastError());

    *buffer = NULL;

    return 0;
}

SDL_RWops *DataZipFile_Load_RW(const char *filename)
{
    char *buffer;
    int size = DataZipFile_Read(filename, &buffer);

    if (size > 0)
        return SDL_RWFromConstMem(buffer, size);

    return NULL;
}

#endif // USE_DATA_ZIP
