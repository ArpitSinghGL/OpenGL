#include <iostream>
#include <stdio.h>

extern "C"
{
    #include <libavcodec/avcodec.h>
    #include <libavformat/avformat.h>
    #include <libavutil/avutil.h>
    #include <libavutil/frame.h>
    #include <libavutil/pixdesc.h>
    #include <libavutil/pixelutils.h>
    #include <libavutil/pixfmt.h>
    #include <libavutil/imgutils.h>
    #include <libswscale/swscale.h>
    #include <inttypes.h>
}

bool load_grayscale_frames(const char* filename , int* width_out , int* height_out , unsigned char** data_out)
{
    int returnValue = 0;

    // Open the file using libavformat

    AVFormatContext* av_format_context = avformat_alloc_context();

    if(!av_format_context)
    {
        printf("Could not create AVFormatContext !\n");
        return false;
    }

    returnValue = avformat_open_input(&av_format_context , filename , NULL , NULL);

    if(returnValue < 0)
    {
        printf("Could not open input file !\n");
        return false;
    }

    returnValue = avformat_find_stream_info(av_format_context , NULL);

    if(returnValue < 0)
    {
        printf("Could not find stream info !\n");
        return false;
    }

    // Finding valid Video Streams from the input file

    unsigned int number_of_streams = av_format_context -> nb_streams;

    int video_stream = -1;

    // printf("The number of streams in the input file: %u\n" , number_of_streams);

    for(int stream = 0 ; stream < number_of_streams ; stream++)
    {
        // printf("stream %d ---> AVMediaType: %d\n" , stream , av_format_context -> streams[stream] -> codecpar -> codec_type);
        if(av_format_context -> streams[stream] -> codecpar -> codec_type == AVMEDIA_TYPE_VIDEO)
        {
            video_stream = stream;
        }
    }

    if(video_stream == -1)
    {
        printf("Could not find video stream\n");
        return false;
    }

    // Get codec parameters and codec context
    AVCodecParameters* video_codec_parameters = av_format_context -> streams[video_stream] -> codecpar;

    const AVCodec* video_codec = avcodec_find_decoder(video_codec_parameters -> codec_id);
    printf("Video Codec ID: %d\n" , video_codec -> id);

    const AVCodec* video_encoder = avcodec_find_encoder(video_codec_parameters -> codec_id);
    const AVCodec* video_decoder = avcodec_find_decoder(video_codec_parameters -> codec_id);

    printf("Name of Video Encoder: %s\n" , video_encoder -> long_name);
    printf("Name of Video Decoder: %s\n" , video_decoder -> long_name);
    printf("Video BitRate: %ld\n" , video_codec_parameters -> bit_rate);
    printf("Video Resolution: %d x %d\n" , video_codec_parameters -> width , video_codec_parameters -> height);

    // Set up a codec context
    AVCodecContext* video_decoder_context = avcodec_alloc_context3(video_decoder);

    if(!video_decoder_context)
    {
        printf("Could not create AVvideo_decoder_context\n");
        return false;
    }

    if(avcodec_parameters_to_context(video_decoder_context , video_codec_parameters) < 0)
    {
        printf("Could not initialize AVvideo_decoder_context\n");
        return false;
    }

    if(avcodec_open2(video_decoder_context , video_decoder , NULL) < 0)
    {
        printf("Could not open codec\n");
        return false;
    }

    // Media Playback Pipeline:
    // Input file -> DEMUXER -> AVPackets(Compressed Data) -> DECODER -> AVFrame(Raw Data)

    // Frame containing frame-data in YUV420p format
    AVFrame* av_frame = av_frame_alloc();

    if(!av_frame)
    {
        printf("Could not allocate input video frame\n");
        return false;
    }

    AVPacket* av_packet = av_packet_alloc();

    if(!av_packet)
    {
        printf("Could not allocate AVPacket\n");
        return false;
    }

    while(av_read_frame(av_format_context , av_packet) >= 0)
    {
        if(av_packet -> stream_index == video_stream)
        {
            // Decode the Video Packet(AVPacket: Compressed Video Data)
            // to get Video Frames (AVFrame: Raw Video Data)

            // Note: In case of Video, the Packet contains only one Frame.

            // Sending the Video Packet to the decoder to get Video Frames
            returnValue = avcodec_send_packet(video_decoder_context , av_packet);
            if(returnValue < 0)
            {
                printf("Error sending packet for decoding\n");
                return false;
            }
            returnValue = avcodec_receive_frame(video_decoder_context , av_frame);
            if(returnValue == AVERROR(EAGAIN))
            {
                printf("More packets needed to produce frame !\n");
                av_packet_unref(av_packet);
                continue;
            }
            else if(returnValue == AVERROR_EOF)
            {
                printf("End of file reached !\n");
                break;
            }
            else if(returnValue < 0)
            {
                printf("Failed to decode packet\n");
                return false;
            }
            else
            {
                // Printing Basic Frame Properties
                printf("Frame Number: %d\n" , video_decoder_context -> frame_number);
                printf("Type: %c\n" , av_get_picture_type_char(av_frame -> pict_type));
                printf("Size: %d bytes\n" , av_frame -> pkt_size);
                printf("pts: %ld\n" , av_frame -> pts);
                printf("key_frame: %d\n" , av_frame -> key_frame);
                printf("Height: %d\n" , video_decoder_context -> height);
                printf("Width: %d\n" , video_decoder_context -> width);
                printf("linesize[0]: %d bytes\n" , (av_frame -> linesize)[0]);
                printf("linesize[1]: %d bytes\n" , (av_frame -> linesize)[1]);
                printf("linesize[2]: %d bytes\n" , (av_frame -> linesize)[2]);

                enum AVPixelFormat pixel_format = video_decoder_context -> pix_fmt;

                const char* pixel_format_name = av_get_pix_fmt_name(pixel_format);

                printf("Pixel Format: %s\n" , pixel_format_name);

                av_packet_unref(av_packet);
                break;
            }
        }
        av_packet_unref(av_packet);
    }

    printf("av_frame -> width: %d\n" , av_frame -> width);
    printf("av_frame -> height: %d\n" , av_frame -> height);

    unsigned char* data = new unsigned char[av_frame -> width * av_frame -> height * 3];

    if(!data)
    {
        printf("Memory allocation for frame data failed !\n");
        return false;
    }

    for(int x = 0 ; x < av_frame -> width ; x++)
    {
        for(int y = 0 ; y < av_frame -> height ; y++)
        {
            data[y * av_frame -> width * 3 + x * 3 + 0] = (av_frame -> data)[0][y * av_frame -> linesize[0] + x];
            data[y * av_frame -> width * 3 + x * 3 + 1] = (av_frame -> data)[0][y * av_frame -> linesize[0] + x];
            data[y * av_frame -> width * 3 + x * 3 + 2] = (av_frame -> data)[0][y * av_frame -> linesize[0] + x];
        }
    }

    *width_out = av_frame -> width;
    printf("*width_out: %d\n" , *width_out);

    *height_out = av_frame -> height;
    printf("*height_out: %d\n" , *height_out);

    *data_out = data;

    // Performing clean-up

    avformat_close_input(&av_format_context);
    printf("av_format_context closed successfully !!!\n");

    avformat_free_context(av_format_context);
    printf("av_format_context freed successfully !!!\n");
    
    // Free the YUV frame
    av_frame_free(&av_frame);
    printf("av_frame deallocated successfully !!!\n");
    
    av_packet_free(&av_packet);
    printf("av_packet deallocated successfully !!!\n");

    avcodec_free_context(&video_decoder_context);
    printf("video_decoder_context freed successfully !!!\n");

    printf("Frame loaded successfully !!!\n");

    return true;
}