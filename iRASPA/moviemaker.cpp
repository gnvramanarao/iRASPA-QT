#include "moviemaker.h"
#include <QDebug>
#include <iostream>

MovieWriter::MovieWriter(const std::string& filename, const unsigned int width, const unsigned int height, int fps): _width(width), _height(height),  _fps(fps), _pixels(4 * width * height)
{
  _oformat = av_guess_format(nullptr, filename.c_str(), nullptr);
  if (!_oformat)
  {
    std::cout << "can't create output format" << std::endl;
    return;
  }
  _oformat->video_codec = AV_CODEC_ID_H264;

  int err = avformat_alloc_output_context2(&_ofctx, _oformat, NULL, filename.c_str());
  if (err)
  {
    std::cout << "can't create output context" << std::endl;
    return;
  }

  _codec = avcodec_find_encoder(_oformat->video_codec);
  if (!_codec)
  {
    std::cout << "can't create codec" << std::endl;
    return;
  }

  _videoStream = avformat_new_stream(_ofctx, _codec);
  if (!_videoStream)
  {
    std::cout << "can't find format" << std::endl;
    return;
  }

  _cctx = avcodec_alloc_context3(_codec);
  if (!_cctx)
  {
    std::cout << "can't create codec context" << std::endl;
    return;
  }

  _pkt = av_packet_alloc();
  if (!_pkt)
  {
    std::cout << "can't create pkt" << std::endl;
    return;
  }

  _videoStream->codecpar->codec_id = _oformat->video_codec;
  _videoStream->codecpar->codec_type = AVMEDIA_TYPE_VIDEO;
  _videoStream->codecpar->width = _width;
  _videoStream->codecpar->height = _height;
  _videoStream->codecpar->format = AV_PIX_FMT_YUV420P;
  _videoStream->codecpar->bit_rate = 2000000;
  avcodec_parameters_to_context(_cctx, _videoStream->codecpar);
  _cctx->time_base = (AVRational){ 1, _fps };
  _cctx->framerate = (AVRational){_fps, 1};
  _cctx->max_b_frames = 1;
  _cctx->gop_size = 10;


  if (_videoStream->codecpar->codec_id == AV_CODEC_ID_H264)
  {
    av_opt_set(_cctx, "preset", "ultrafast", 0);
  }
  else if (_videoStream->codecpar->codec_id == AV_CODEC_ID_H265)
  {
    av_opt_set(_cctx, "preset", "ultrafast", 0);
  }

  av_dump_format(_ofctx, 0, filename.c_str(), 1);

  if ((err = avcodec_open2(_cctx, _codec, NULL)) < 0)
  {
    std::cout << "Failed to open codec" << err << std::endl;
    return;
  }

  //avcodec_parameters_from_context(videoStream->codecpar, cctx);

  if (!(_oformat->flags & AVFMT_NOFILE))
  {
    if ((err = avio_open(&_ofctx->pb, filename.c_str(), AVIO_FLAG_WRITE)) < 0)
    {
      std::cout << "Failed to open file" << err << std::endl;
      return;
    }
  }

  if ((err = avformat_write_header(_ofctx, NULL)) < 0)
  {
    std::cout << "Failed to write header" << err << std::endl;
    return;
  }


  // Preparing to convert my generated RGB images to YUV frames.
  _swsCtx = sws_getContext(_width, _height, AV_PIX_FMT_RGB24, _width, _height, AV_PIX_FMT_YUV420P, SWS_FAST_BILINEAR, NULL, NULL, NULL);


  // Preparing the containers of the frame data:
  // Allocating memory for each RGB frame, which will be lately converted to YUV.
  int ret;
  _rgbpic = av_frame_alloc();
  _rgbpic->format = AV_PIX_FMT_RGB24;
  _rgbpic->width = _width;
  _rgbpic->height = _height;
  ret = av_frame_get_buffer(_rgbpic, 1);
  if (ret < 0)
  {
    std::cout << "Could not allocate the video frame data" << ret << std::endl;
    return;
  }

  // Allocating memory for each conversion output YUV frame.
  _frame = av_frame_alloc();
  _frame->format = AV_PIX_FMT_YUV420P;
  _frame->width = _width;
  _frame->height = _height;
  ret = av_frame_get_buffer(_frame, 1);
  if (ret < 0)
  {
    std::cout << "Could not allocate the video frame data" << ret << std::endl;
    return;
  }
}

void MovieWriter::addFrame(const uint8_t* pixels, int iframe)
{
  int ret;

  ret = av_frame_make_writable(_frame);
  if (ret < 0)
  {
    std::cout << "Frame not writable" << ret << std::endl;
    return;
  }

  // The AVFrame data will be stored as RGBRGBRGB... row-wise,
  // from left to right and from top to bottom.
  for (unsigned int y = 0; y < _height; y++)
  {
    for (unsigned int x = 0; x < _width; x++)
    {
      // rgbpic->linesize[0] is equal to width.
      _rgbpic->data[0][y * _rgbpic->linesize[0] + 3 * x + 0] = pixels[y * 4 * _width + 4 * x + 2];
      _rgbpic->data[0][y * _rgbpic->linesize[0] + 3 * x + 1] = pixels[y * 4 * _width + 4 * x + 1];
      _rgbpic->data[0][y * _rgbpic->linesize[0] + 3 * x + 2] = pixels[y * 4 * _width + 4 * x + 0];
    }
  }

  // Not actually scaling anything, but just converting
  // the RGB data to YUV and store it in yuvpic.
  sws_scale(_swsCtx, _rgbpic->data, _rgbpic->linesize, 0, _height, _frame->data, _frame->linesize);

  // The PTS of the frame are just in a reference unit,
  // unrelated to the format we are using. We set them,
  // for instance, as the corresponding frame number.
  _frame->pts = iframe;

  if (ret = avcodec_send_frame(_cctx, _frame) < 0)
  {
    std::cout << "Failed to send frame" << ret <<std::endl;
    return;
  }

  while (ret >= 0)
  {
    ret = avcodec_receive_packet(_cctx, _pkt);
    if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
      return;
    else if (ret < 0)
    {
      fprintf(stderr, "Error during encoding\n");
      exit(1);
    }

    av_packet_rescale_ts(_pkt, (AVRational){1, _fps}, _videoStream->time_base);
    _pkt->stream_index = _videoStream->index;

    std::cout << "Write packet " << _pkt->size << std::endl;
    av_write_frame(_ofctx, _pkt);
    av_packet_unref(_pkt);
  }
}

MovieWriter::~MovieWriter()
{
  for (;;)
  {
    avcodec_send_frame(_cctx, NULL);
    if (avcodec_receive_packet(_cctx, _pkt) == 0)
    {
      av_packet_rescale_ts(_pkt, (AVRational){1, _fps}, _videoStream->time_base);
      _pkt->stream_index = _videoStream->index;

      av_write_frame(_ofctx, _pkt);
      av_packet_unref(_pkt);
    }
    else
    {
      break;
    }
  }

  av_write_trailer(_ofctx);


  if (!(_oformat->flags & AVFMT_NOFILE))
  {
    int err = avio_close(_ofctx->pb);
    if (err < 0)
    {
      std::cout << "Failed to close file" << err <<std::endl;
    }
  }

  av_packet_free(&_pkt);

  av_frame_free(&_rgbpic);
  av_frame_free(&_frame);

  avcodec_free_context(&_cctx);
  avformat_free_context(_ofctx);

  sws_freeContext(_swsCtx);
}
