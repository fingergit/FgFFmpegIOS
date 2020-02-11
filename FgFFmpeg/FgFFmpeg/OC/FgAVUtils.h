//
//  FgAVUtils.h
//  FgFFmpeg
//
//  Created by redos on 2020/2/11.
//  Copyright © 2020 redos. All rights reserved.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@protocol FgFFmpegProgressCallback <NSObject>
-(void)OnProgress: (long) taskId status:(int) status frames: (long) frames totalFrames: (long)totalFrames;
@end


@interface FgAVUtils : NSObject

+(id<FgFFmpegProgressCallback>) delegate;

+(NSString*)getFileInfo: (NSString*)filePath;
+(int)ffmpegMain: (long) taskId argv: (NSArray*) argv callback: (id<FgFFmpegProgressCallback>) callback;
@end

NS_ASSUME_NONNULL_END
