/*************************************************************************************************************
 Copyright (c) 2006-2019 David Dubbeldam, Sofia Calero, Thijs J.H. Vlugt.

    D.Dubbeldam@uva.nl            http://www.acmm.nl/molsim/users/dubbeldam/index.html
    scaldia@upo.es                http://www.upo.es/raspa/sofiacalero.php
    t.j.h.vlugt@tudelft.nl        http://homepage.tudelft.nl/v9k6y

 Permission is hereby granted, free of charge, to any person
 obtaining a copy of this software and associated documentation
 files (the "Software"), to deal in the Software without
 restriction, including without limitation the rights to use,
 copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the
 Software is furnished to do so, subject to the following
 conditions:

 The above copyright notice and this permission notice shall be
 included in all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 OTHER DEALINGS IN THE SOFTWARE.
 *************************************************************************************************************/

#pragma once

#include <stdint.h>
#include <string>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include "logreporting.h"

// based on: https://stackoverflow.com/questions/34511312/how-to-encode-a-video-from-several-images-generated-in-a-c-program-without-wri
//      and: https://github.com/apc-llc/moviemaker-cpp

extern "C"
{
  #include <x264.h>
  #include <libswscale/swscale.h>
  #include <libavcodec/avcodec.h>
  #include <libavutil/mathematics.h>
  #include <libavformat/avformat.h>
  #include <libavutil/opt.h>
}

class MovieWriter
{
public :
  MovieWriter(const std::string& filename, const unsigned int width, const unsigned int height, int fps, LogReporting *logReporting);
   ~MovieWriter();
  void addFrame(const uint8_t* pixels, int iframe);
private:
  LogReporting *_logReporter = nullptr;
  const unsigned int _width, _height;
  int _fps;
  SwsContext* _swsCtx;
  AVCodec *_codec;
  AVOutputFormat* _oformat;
  AVStream* _videoStream;
  AVFormatContext* _ofctx;
  AVCodecContext* _cctx;
  AVPacket *_pkt;

  AVFrame *_rgbpic;
  AVFrame *_frame;

  std::vector<uint8_t> _pixels;
};

