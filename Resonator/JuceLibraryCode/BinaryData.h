/* =========================================================================================

   This is an auto-generated file: Any edits you make may be overwritten!

*/

#pragma once

namespace BinaryData
{
    extern const char*   github_square_black_png;
    const int            github_square_black_pngSize = 15839;

    extern const char*   instagram_square_black_png;
    const int            instagram_square_black_pngSize = 16551;

    extern const char*   linkedin_square_black_png;
    const int            linkedin_square_black_pngSize = 9048;

    extern const char*   soundcloud_square_black_png;
    const int            soundcloud_square_black_pngSize = 9225;

    // Number of elements in the namedResourceList and originalFileNames arrays.
    const int namedResourceListSize = 4;

    // Points to the start of a list of resource names.
    extern const char* namedResourceList[];

    // Points to the start of a list of resource filenames.
    extern const char* originalFilenames[];

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding data and its size (or a null pointer if the name isn't found).
    const char* getNamedResource (const char* resourceNameUTF8, int& dataSizeInBytes);

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding original, non-mangled filename (or a null pointer if the name isn't found).
    const char* getNamedResourceOriginalFilename (const char* resourceNameUTF8);
}
