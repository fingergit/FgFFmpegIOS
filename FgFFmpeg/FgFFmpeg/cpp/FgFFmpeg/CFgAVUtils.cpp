//
// Created by redos on 2020-02-10.
//
extern "C"{
#include "libavcodec/jni.h"
#include <libavformat/avformat.h>
#include <libavutil/display.h>
#include "ffmpeg.h"
}
#include <sstream>
#include <FgJniUtils.h>

#include "CFgAVUtils.h"


double _get_rotation(AVStream *st)
{
    uint8_t* displaymatrix = av_stream_get_side_data(st,
                                                     AV_PKT_DATA_DISPLAYMATRIX, NULL);
    double theta = 0;
    if (displaymatrix)
        theta = -av_display_rotation_get((int32_t*) displaymatrix);

    theta -= 360*floor(theta/360 + 0.9/360);

    if (fabs(theta - 90*round(theta/90)) > 2)
        av_log(NULL, AV_LOG_WARNING, "Odd rotation angle.\n"
                                     "If you want to help, upload a sample "
                                     "of this file to ftp://upload.ffmpeg.org/incoming/ "
                                     "and contact the ffmpeg-devel mailing list. (ffmpeg-devel@ffmpeg.org)");

    return theta;
}


std::string CFgAVUtils::getFileInfo(std::string filePath) {
    const char* path= filePath.c_str();
    AVFormatContext* ctx = nullptr;

    av_register_all();
    avcodec_register_all();

    int ret = avformat_open_input(&ctx, path, nullptr, nullptr);
    avformat_find_stream_info(ctx, nullptr);
    if (ret != 0) {
        printf("avformat_open_input() open failed! path:%s, err:%s", path, av_err2str(ret));
        return nullptr;
    }
    int nStreams = ctx->nb_streams;

    AVStream **pStream = ctx->streams;
    AVStream *vStream = nullptr;

    for (int i = 0; i < nStreams; i++) {
        if (pStream[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            vStream = pStream[i];
        }
    }

    int width = 0;
    int height = 0;
    int rotation = 0;
    long fps = 0;
    char *vCodecName = nullptr;
    if(vStream!=nullptr){
        width = vStream->codecpar->width;
        height = vStream->codecpar->height;
        rotation = static_cast<int>(_get_rotation(vStream));
        int num = vStream->avg_frame_rate.num;
        int den = vStream->avg_frame_rate.den;
        if (den > 0) {
            fps = lround(num * 1.0 / den);
        }

        const char *codecName = avcodec_get_name(vStream->codecpar->codec_id);
        vCodecName = const_cast<char *>(codecName);
    }

    long bitrate = ctx->bit_rate;
    long duration = ctx->duration / 1000;//ms

    avformat_close_input(&ctx);
    std::ostringstream buffer;
    buffer << "{\"rotation\":" << rotation
        << ",\"width\":" << width
        << ",\"height\":" << height
        << ",\"duration\":" << duration
        << ",\"bitrate\":" << bitrate
        << ",\"fps\":" << fps
        << ",\"videoCodec\":" << vCodecName
        << "\"}";
    std::string result = buffer.str();

    return result;
}

void OnFFmpegLog(void* ptr, int level, const char* fmt, va_list vl) {
    va_list vl2;
    char line[1024];
    static int print_prefix = 1;

    va_copy(vl2, vl);
    // av_log_default_callback(ptr, level, fmt, vl);
    av_log_format_line(ptr, level, fmt, vl2, line, sizeof(line), &print_prefix);
    va_end(vl2);

    if (level <= AV_LOG_ERROR)
        LOGE("%s", line);
    else if (level <= AV_LOG_WARNING)
        LOGW("%s", line);
    else if (level <= AV_LOG_INFO)
        LOGI("%s", line);
    else if (level <= AV_LOG_VERBOSE)
        LOGV("%s", line);
    else
        LOGD("%s", line);
}

int CFgAVUtils::ffmpegMain(int argc, char **argv, FFmpegProgressCB cbProgress, int64_t taskId) {
    return ffmpeg_main(argc, argv, OnFFmpegLog, cbProgress, taskId);
}

void CFgAVUtils::printCodecs() {
    const AVCodecDescriptor *desc = NULL;
    const AVCodecDescriptor **codecs;
    unsigned nb_codecs = 0, i = 0;

    while ((desc = avcodec_descriptor_next(desc))) {
        nb_codecs++;
        bool isDecoder = (avcodec_find_decoder(desc->id) != NULL);
        bool isEncoder = (avcodec_find_encoder(desc->id) != NULL);
        if (isDecoder && isEncoder) {
            LOGI("[E/D]: %s\n", desc->name);
        }
        else if (isEncoder) {
            LOGI("[E]: %s\n", desc->name);
        }
        else {
//            LOGI("[D]: %s\n", desc->name);
        }
    }

}
