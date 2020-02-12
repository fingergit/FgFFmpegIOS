//
//  FgAVUtils.m
//  FgFFmpeg
//
//  Created by redos on 2020/2/11.
//  Copyright © 2020 redos. All rights reserved.
//

#import <string>
#import "CFgAVUtils.h"
#import "FgAVUtils.h"
#import "ffmpeg.h"

id<FgFFmpegProgressCallback> g_callback;
void OnFFmpegProgress(int64_t _id, int status, int64_t frames, int64_t totalFrames) {
    [g_callback OnProgress:1 status:status frames:frames totalFrames:totalFrames];
}

@implementation FgAVUtils

+(NSString*)getFileInfo: (NSString*)filePath {
    std::string info = CFgAVUtils::getFileInfo([filePath UTF8String]);
    NSString *str= [NSString stringWithCString:info.c_str() encoding:[NSString defaultCStringEncoding]];
    return str;
}

+(int)ffmpegMain: (long)taskId argv: (NSArray*)argv callback: (id<FgFFmpegProgressCallback>)callback {
    const char *argCmd[argv.count];
    for (int i = 0; i < argv.count; i++) {
        argCmd[i] = [((NSString*)argv[i]) UTF8String];
    }
    
    g_callback = callback;
    return CFgAVUtils::ffmpegMain(argv.count, (char**)argCmd, OnFFmpegProgress, 1);
}

+(void)printCodecs {
    CFgAVUtils::printCodecs();
}
@end
