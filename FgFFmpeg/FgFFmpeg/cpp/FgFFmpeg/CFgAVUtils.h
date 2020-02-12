//
// Created by redos on 2020-02-10.
//

#ifndef MYFFMPEGTEST_FGAVUTILS_H
#define MYFFMPEGTEST_FGAVUTILS_H

#include <string>
#include "FgFFmpegDef.h"

class CFgAVUtils {
public:
    static std::string getFileInfo(std::string filePath);
    static int ffmpegMain(int argc, char **argv, FFmpegProgressCB cbProgress, int64_t taskId);
    static void printCodecs();
};


#endif //MYFFMPEGTEST_FGAVUTILS_H
