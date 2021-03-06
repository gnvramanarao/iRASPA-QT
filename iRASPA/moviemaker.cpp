#include "moviemaker.h"
#include <QDebug>
#include <iostream>

MovieWriter::MovieWriter(const unsigned int width, const unsigned int height, int fps, LogReporting* logReporter, Type type) :
    _logReporter(logReporter), _type(type), _width(width), _height(height), _fps(fps)
{
}

MovieWriter::~MovieWriter()
{

}

int MovieWriter::initialize(const std::string& filename)
{
  _oformat = av_guess_format(nullptr, filename.c_str(), nullptr);
  if (!_oformat)
  {
    _logReporter->logMessage(LogReporting::ErrorLevel::error, "ffmpeg could not guess output format");
    return -1;
  }
  switch(_type)
  {
  case Type::h265:
    _oformat->video_codec = AV_CODEC_ID_HEVC;
    break;
  case Type::h264:
    _oformat->video_codec = AV_CODEC_ID_H264;
    break;
  case Type::vp9:
    _oformat->video_codec = AV_CODEC_ID_VP9;
    break;
  case Type::av1:
    _oformat->video_codec = AV_CODEC_ID_AV1;
    break;
  default:
    _oformat->video_codec = AV_CODEC_ID_H264;
    break;
  }

  int err = avformat_alloc_output_context2(&_ofctx, _oformat, NULL, filename.c_str());
  if (err)
  {
    _logReporter->logMessage(LogReporting::ErrorLevel::error, "failed to create ffmpeg output context");
    return -1;
  }

  _codec = avcodec_find_encoder(_oformat->video_codec);
  if (!_codec)
  {
    avformat_free_context(_ofctx);
    _logReporter->logMessage(LogReporting::ErrorLevel::error, "failed to create ffmpeg codec");
    return -1;
  }

  // Add a new stream to a media file
  _videoStream = avformat_new_stream(_ofctx, _codec);
  if (!_videoStream)
  {
    avformat_free_context(_ofctx);
    _logReporter->logMessage(LogReporting::ErrorLevel::error, "failed to create ffmpeg stream");
    return -1;
  }

  _cctx = avcodec_alloc_context3(_codec);
  if (!_cctx)
  {
    avformat_free_context(_ofctx);
    _logReporter->logMessage(LogReporting::ErrorLevel::error, "failed to create ffmpeg codec context");
    return -1;
  }

  _pkt = av_packet_alloc();
  if (!_pkt)
  {
    avformat_free_context(_ofctx);
    avcodec_free_context(&_cctx);
    _logReporter->logMessage(LogReporting::ErrorLevel::error, "failed to create ffmpeg packet");
    return -1;
  }

  _videoStream->codecpar->codec_id = _oformat->video_codec;
  _videoStream->codecpar->codec_type = AVMEDIA_TYPE_VIDEO;
  _videoStream->codecpar->width = _width;
  _videoStream->codecpar->height = _height;
  _videoStream->codecpar->format = AV_PIX_FMT_YUV420P;

  _videoStream->codecpar->codec_tag = 0;
  switch(_type)
  {
  case Type::h265:
    _videoStream->codecpar->codec_tag = MKTAG('h', 'v', 'c', '1'); // for h265
    _videoStream->codecpar->bit_rate = 5000000;
    break;
  case Type::h264:
    _videoStream->codecpar->bit_rate = 5000000;
    break;
  case Type::vp9:
    _videoStream->codecpar->bit_rate = 5000000;
    break;
  case Type::av1:
    _videoStream->codecpar->bit_rate = 5000000;
    break;
  default:
    _videoStream->codecpar->bit_rate = 5000000;
    break;
  }

  avcodec_parameters_to_context(_cctx, _videoStream->codecpar);

  _cctx->time_base = { 1, _fps };
  _cctx->framerate = {_fps, 1};
  _cctx->max_b_frames = 1;
  _cctx->gop_size = 10;

  if (_videoStream->codecpar->codec_id == AV_CODEC_ID_H264)
  {
    av_opt_set(_cctx, "preset", "slow", 0);
  }
  else if (_videoStream->codecpar->codec_id == AV_CODEC_ID_H265)
  {
    av_opt_set(_cctx, "preset", "slow", 0);
  }


  av_dump_format(_ofctx, 0, filename.c_str(), 1);

  if ((err = avcodec_open2(_cctx, _codec, NULL)) < 0)
  {
    avformat_free_context(_ofctx);
    avcodec_free_context(&_cctx);
    av_packet_free(&_pkt);
    _logReporter->logMessage(LogReporting::ErrorLevel::error, "failed to open codec");
    return -1;
  }
  if (_ofctx->oformat->flags & AVFMT_GLOBALHEADER) 
  {
      _cctx->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;
  }
  avcodec_parameters_from_context(_videoStream->codecpar, _cctx);

  if (!(_oformat->flags & AVFMT_NOFILE))
  {
    if ((err = avio_open(&_ofctx->pb, filename.c_str(), AVIO_FLAG_WRITE)) < 0)
    {
      avformat_free_context(_ofctx);
      avcodec_free_context(&_cctx);
      av_packet_free(&_pkt);
      _logReporter->logMessage(LogReporting::ErrorLevel::error, "failed to open file");
      return -1;
    }
    _logReporter->logMessage(LogReporting::ErrorLevel::verbose, "file opened " + QString::fromStdString(filename));
  }

  if ((err = avformat_write_header(_ofctx, NULL)) < 0)
  {
    avformat_free_context(_ofctx);
    avcodec_free_context(&_cctx);
    av_packet_free(&_pkt);
    _logReporter->logMessage(LogReporting::ErrorLevel::error, "failed to write header " + QString::number(err));
    return -1;
  }

  int ret;
  _rgbpic = av_frame_alloc();
  _rgbpic->format = AV_PIX_FMT_BGR0;
  _rgbpic->width = _width;
  _rgbpic->height = _height;
  ret = av_frame_get_buffer(_rgbpic, 0);
  if (ret < 0)
  {
    avformat_free_context(_ofctx);
    avcodec_free_context(&_cctx);
    av_packet_free(&_pkt);
    _logReporter->logMessage(LogReporting::ErrorLevel::error, "ffmpeg could not allocate the video frame data");
    return -1;
  }

  // Allocating memory for each conversion output YUV frame.
  _frame = av_frame_alloc();
  _frame->format = AV_PIX_FMT_YUV420P;
  _frame->width = _width;
  _frame->height = _height;
  ret = av_frame_get_buffer(_frame, 0);
  if (ret < 0)
  {
    avformat_free_context(_ofctx);
    avcodec_free_context(&_cctx);
    av_packet_free(&_pkt);
    av_frame_free(&_rgbpic);
    _logReporter->logMessage(LogReporting::ErrorLevel::error, "ffmpeg could not allocate the video frame data");
    return -1;
  }

  if(!(_swsCtx = sws_getContext(_width, _height, AV_PIX_FMT_BGR0, _width, _height, AV_PIX_FMT_YUV420P, SWS_FAST_BILINEAR, NULL, NULL, NULL)))
  {
    avformat_free_context(_ofctx);
    avcodec_free_context(&_cctx);
    av_packet_free(&_pkt);
    av_frame_free(&_rgbpic);
    _logReporter->logMessage(LogReporting::ErrorLevel::error, "ffmpeg could not allocate a SwsContext");
    return -1;
  }

  _logReporter->logMessage(LogReporting::ErrorLevel::verbose, "ffmpeg initialized");
  return 0;
}

void MovieWriter::encodeFrame(AVFrame *frame)
{
  int ret;

  if ((ret = avcodec_send_frame(_cctx, frame)) < 0)
  {
    _logReporter->logMessage(LogReporting::ErrorLevel::error, "ffmpeg failed to send frame");
    return;
  }

  while (ret >= 0)
  {
    ret = avcodec_receive_packet(_cctx, _pkt);
    if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
      return;
    else if (ret < 0)
    {
      avformat_free_context(_ofctx);
      avcodec_free_context(&_cctx);
      av_packet_free(&_pkt);
      av_frame_free(&_rgbpic);
      av_frame_free(&_frame);
      sws_freeContext(_swsCtx);
      _logReporter->logMessage(LogReporting::ErrorLevel::error, "ffmpeg error during encoding");
      return;
    }

    av_packet_rescale_ts(_pkt, {1, _fps}, _videoStream->time_base);
    _pkt->stream_index = _videoStream->index;

    av_write_frame(_ofctx, _pkt);
    av_packet_unref(_pkt);
  }
}


void MovieWriter::addFrame(const uint8_t* pixels, int iframe)
{
  int ret;

  memcpy(_rgbpic->data[0], pixels, _width * _height * 4);

  sws_scale(_swsCtx, _rgbpic->data, _rgbpic->linesize, 0, _height, _frame->data, _frame->linesize);

  _frame->pts = iframe;

  encodeFrame(_frame);
}

int MovieWriter::finalize()
{
  _logReporter->logMessage(LogReporting::ErrorLevel::verbose, "ffmpeg finalizing movie");
  
  encodeFrame(NULL);

  av_write_trailer(_ofctx);

  if (!(_oformat->flags & AVFMT_NOFILE))
  {
    int err = avio_close(_ofctx->pb);
    if (err < 0)
    {
      avformat_free_context(_ofctx);
      avcodec_free_context(&_cctx);
      av_packet_free(&_pkt);
      av_frame_free(&_rgbpic);
      av_frame_free(&_frame);
      sws_freeContext(_swsCtx);
      _logReporter->logMessage(LogReporting::ErrorLevel::error, "ffmpeg failed to close file");
    }
  }

  avformat_free_context(_ofctx);
  avcodec_free_context(&_cctx);
  av_packet_free(&_pkt);
  av_frame_free(&_rgbpic);
  av_frame_free(&_frame);
  sws_freeContext(_swsCtx);
  return 0;
}
