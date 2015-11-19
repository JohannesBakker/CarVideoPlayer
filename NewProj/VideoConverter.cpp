
#include "stdafx.h"
#include "NewProj.h"
#include "NewProjDlg.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "ffmpeg/libavformat/avformat.h"
#include "ffmpeg/libswscale/swscale.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define STREAM_FRAME_RATE		25	/* 25 images */
#define STREAM_PIX_FMT			AV_PIX_FMT_YUV420P 	/* default pix_fmt */

static int sws_flags = SWS_BICUBIC;


/**************************************************************/

/* video output */
static AVFrame *frame, *Appo;
static AVPicture src_picture, dst_picture;
static const char*filename;
static AVOutputFormat *fmt;
static AVFormatContext *oc;
static AVStream *video_st;
AVCodec *video_codec;
static double video_time;


/* add a video output stream */
AVStream *add_video_stream(AVFormatContext *oc, AVCodec **code, enum AVCodecID codec_id, int w, int h) 
{
	AVCodecContext *c;
	AVStream *st;

	/* find the encoder */
	*codec = avcodec_find_encoder(codec_id);
	if (!(*codec)) {
		fprintf(stderr, "Could not find encoder for %s\n", avcodec_get_name(codec_id));
		return NULL;
	}

	st = avformat_new_stream(oc *codec);
	if (!st) {
		fprintf(stderr, "Could not allocate stream\n");
		return NULL;
	}

	st->id = oc->nb_streams - 1;
	c = st->codec;

	switch () 
	{
		case AVMEDIA_TYPE_AUDIO:			
			c->sample_fmt = AV_SAMPLE_FMT_FLTP;
			c->bit_rate = 64000;
			c->sample_rate = 44100;
			c->channels = 2;
			break;

		case AVMEDIA_TYPE_VIDEO:
			c->codec_id = codec_id;
			c->bit_rate = 400000;

			// Resolution must be a multiple of two
			c->width = w;
			c->height = h;

			/* timebase: This is the fundamental unit of time (in seconds) in terms
			* of which frame timestamps are represented. For fixed-fps content,
			* timebase should be 1/framerate and timestamp increments should be 
			* identical to 1. */
			c->time_base.den = STREAM_FRAME_RATE;
			c->time_base.num = 1;
			c->gop_size = 12;	// emit one intra frame every twelve frames at most
			c->pix_fmt = STREAM_PIX_FMT;

			if (c->codec_id == AV_CODEC_ID_MPEG2VIDEO) {
				// just for testing, we also add B frames
				c->max_b_frames = 2;
			}

			if (c->codec_id == AV_CODEC_ID_MPEG1VIDEO) {
				/* Needed to avoid using macroblocks in which some coeffs overflow.
				* This does not happen with normal video, it just happens here as
				* the motion of the chroma plane does not match the luma plane. */
				c->mb_decision = 2;
			}
			break;

		default:
			break;
	}

	// Some formats want stream headers to be separate.
	if (oc->oformat->flags & AVFMT_GLOBALHEADER)
		c->flags |= CODEC_FLAG_GLOBAL_HEADER;

	return st;
}

static void open_video(AVFormatContext *oc, AVCodec *codec, AVStream *st) 
{
	int ret;
	AVCodecContext *c = st->codec;

	// open the codec
	ret = avcodec_open2(c,codec, NULL);
	if (ret < 0) 
	{
		fprintf(stderr, "Could not open video codec: %s\n", av_err2str(ret));
		return;
	}

	// Allocate the encoded raw picture.
	ret = avpicture_alloc(&dst_picture, c->pix_fmt, c->width, c->height);
	if (ret < 0) 
	{
		fprintf(stderr, "Could not allocate picture: %s\n", av_err2str(ret));
		return;
	}

	/* If the output format is not YUV420P, then a temporary YUV420P
	* picture is needed too. It is then converted to the required
	* output format. */
	Appo = NULL;
	if (c->pix_fmt != AV_PIX_FMT_YUV420P)
	{
		ret = avpicture_alloc(&src_picture, AV_PIX_FMT_YUV420P, c->width, c->height);
		if (ret < 0) {
			fprintf(stderr, "Could not allocate temporary picture: %s\n", av_err2str(ret));
			return;
		}

		Appo = frame;
	}

	// copy data and linesize picture pointers to frame
	*((AVPicture *)frame) = dst_picture;
}

static void write_video_frame(AVFormatContext *oc, AVStream *st)
{
	int ret;
	static struct SwsContext *sws_ctx;
	AVCodecContext *c = st->codec;

	if (c->pix_fmt != AV_PIX_FMT_YUV420P) {
		/* as we only generate a YUV420P picture, we must convert it 
		* to the codec pixel format if needed */
		if (!sws_ctx)
		{
			sws_ctx = sws_getContext(c->width, c->height, AV_PIX_FMT_YUV420P,
				c->width, c->height, c->pix_fmt, 
				sws_flags, NULL, NULL, NULL);
			if (!sws_ctx)
			{
				fprintf(stderr, "Could not initialize the conversion context\n");
				return;
			}
		}

		sws_scale(sws_ctx,
			(const uint8_t * const)src_picture.data, src_picture.linesize,
			0, c->height, dst_picture.data, dst_picture.linesize);
	}

	if (oc->oformat->flags & AVFMT_RAWPICTURE)
	{
		// Raw video case - directly store the picture in the packet
		AVPacket pkt;
		
		av_init_packet(&pkt);

		pkt.flags |= AV_PKT_FLAG_KEY;
		pkt.stream_index = st->index;
		pkt.data = dst_picture.data[0];
		pkt.size = sizeof(AVPicture);

		ret = av_interleaved_write_frame(oc, &pkt);		
	}
	else 
	{
		AVPacket pkt = { 0 };
		int got_packet;

		av_init_packet(&pkt);

		// encode the image
		ret = avcodec_encode_video2(c, &pkt, frame, &got_packet);
		if (ret < 0)
		{
			fprintf(stderr, "Error encoding video frame: %s\n", av_err2str(ret));
			return;
		}

		// If size is zero, it means the image was buffered.
		if (!ret && got_packet && pkt.size)
		{
			pkt.stream_index = st->index;

			// Write the compressed frame to the media file.
			ret = av_interleaved_write_frame(oc, &pkt);
		}
		else
		{
			ret = 0;
		}
	}

	if (ret != 0)
	{
		fprintf(stderr, "Error while writing video frame: %s\n", av_err2str(ret));
		return;
	}
}

void close_video(AVFormatContext *oc, AVStream *st)
{
	avcodec_close(st->codec);
	av_free(src_picture.data[0]);
	av_free(dst_picture.data[0]);
	av_free(Appo);
	av_free(frame);
}

static int movie_open(int w, int h)
{
	int ret = 0;

	if (fmt->video_codec != AV_CODEC_ID_NONE)
	{
		video_st = add_video_stream(oc, &video_codec, fmt->video_codec, w, h);
	}
	else 
	{
		return 1;
	}

	/* Now that all the parameters are set, we can open the audio and
	* video codecs and allocate the necessary encode buffers. */
	if (video_st)
	{
		open_video(oc, video_codec, video_st);
	}

	av_dump_format(oc, 0, filename, 1);

	// open the output file, if needed
	if (!(fmt->flags & AVFMT_NOFILE))
	{
		ret = avio_open(&oc->pb, filename, AVIO_FLAG_WRITE);
		if (ret < 0)
		{
			fprintf(stderr, "Could not open %s: %s\n", filename, av_err2str(ret));
			return 1;
		}
	}

	// Write the stream header, if any.
	ret = avformat_write_header(oc, NULL);
	if (ret < 0)
	{
		fprintf(stderr, "Error occurred when opening output file: %s\n", av_err2str(ret));
		return 1;		
	}

	return 0;
}






