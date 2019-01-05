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

#include <ostream>
#include <string>
#include <vector>
#include <mathkit.h>
#include "sksymmetryoperationset.h"
#include "skdefinitions.h"
#include "skpointgroup.h"


class SKSpaceGroupSetting
{
public:
  SKSpaceGroupSetting(qint64 number, qint64 spaceGroupNumber, qint64 order, char ext, QString qualifier, QString HM, QString Hall,
                      bool inversionAtOrigin, int3 inversionCenter, Symmorphicity symmorphicity, bool standard, Centring centring,
                      std::vector<int3> latticeTranslations, qint64 pointGroupNumber, std::string schoenflies, std::string generators,
                      std::string encoding, int3x3 toPrimitiveTransformation);
  SKSymmetryOperationSet fullSeitzMatrices();

  qint64 number() const {return _spaceGroupNumber;}
  qint64 HallNumber() const {return _HallNumber;}
  QString HallString() const {return _HallString;}
  QString HMString() const {return _HMString;}
  qint64 pointGroup() const {return _pointGroupNumber;}
  QString qualifier() const {return _qualifier;}
  Symmorphicity symmorphicity() const {return _symmorphicity;}
  QString symmorphicityString() const;
  QString centringString() const;

  bool inversionAtOrigin() const {return _inversionAtOrigin;}
  int3 inversionCenter() const {return _inversionCenter;}

  std::vector<int3> latticeTranslations() {return _latticeTranslations;}

  friend std::ostream& operator<<(std::ostream& os, const SKSpaceGroupSetting& setting);

  static const SKSpaceGroupSetting spaceGroupHall0;
  static const SKSpaceGroupSetting spaceGroupHall1;
  static const SKSpaceGroupSetting spaceGroupHall2;
  static const SKSpaceGroupSetting spaceGroupHall3;
  static const SKSpaceGroupSetting spaceGroupHall4;
  static const SKSpaceGroupSetting spaceGroupHall5;
  static const SKSpaceGroupSetting spaceGroupHall6;
  static const SKSpaceGroupSetting spaceGroupHall7;
  static const SKSpaceGroupSetting spaceGroupHall8;
  static const SKSpaceGroupSetting spaceGroupHall9;
  static const SKSpaceGroupSetting spaceGroupHall10;
  static const SKSpaceGroupSetting spaceGroupHall11;
  static const SKSpaceGroupSetting spaceGroupHall12;
  static const SKSpaceGroupSetting spaceGroupHall13;
  static const SKSpaceGroupSetting spaceGroupHall14;
  static const SKSpaceGroupSetting spaceGroupHall15;
  static const SKSpaceGroupSetting spaceGroupHall16;
  static const SKSpaceGroupSetting spaceGroupHall17;
  static const SKSpaceGroupSetting spaceGroupHall18;
  static const SKSpaceGroupSetting spaceGroupHall19;
  static const SKSpaceGroupSetting spaceGroupHall20;
  static const SKSpaceGroupSetting spaceGroupHall21;
  static const SKSpaceGroupSetting spaceGroupHall22;
  static const SKSpaceGroupSetting spaceGroupHall23;
  static const SKSpaceGroupSetting spaceGroupHall24;
  static const SKSpaceGroupSetting spaceGroupHall25;
  static const SKSpaceGroupSetting spaceGroupHall26;
  static const SKSpaceGroupSetting spaceGroupHall27;
  static const SKSpaceGroupSetting spaceGroupHall28;
  static const SKSpaceGroupSetting spaceGroupHall29;
  static const SKSpaceGroupSetting spaceGroupHall30;
  static const SKSpaceGroupSetting spaceGroupHall31;
  static const SKSpaceGroupSetting spaceGroupHall32;
  static const SKSpaceGroupSetting spaceGroupHall33;
  static const SKSpaceGroupSetting spaceGroupHall34;
  static const SKSpaceGroupSetting spaceGroupHall35;
  static const SKSpaceGroupSetting spaceGroupHall36;
  static const SKSpaceGroupSetting spaceGroupHall37;
  static const SKSpaceGroupSetting spaceGroupHall38;
  static const SKSpaceGroupSetting spaceGroupHall39;
  static const SKSpaceGroupSetting spaceGroupHall40;
  static const SKSpaceGroupSetting spaceGroupHall41;
  static const SKSpaceGroupSetting spaceGroupHall42;
  static const SKSpaceGroupSetting spaceGroupHall43;
  static const SKSpaceGroupSetting spaceGroupHall44;
  static const SKSpaceGroupSetting spaceGroupHall45;
  static const SKSpaceGroupSetting spaceGroupHall46;
  static const SKSpaceGroupSetting spaceGroupHall47;
  static const SKSpaceGroupSetting spaceGroupHall48;
  static const SKSpaceGroupSetting spaceGroupHall49;
  static const SKSpaceGroupSetting spaceGroupHall50;
  static const SKSpaceGroupSetting spaceGroupHall51;
  static const SKSpaceGroupSetting spaceGroupHall52;
  static const SKSpaceGroupSetting spaceGroupHall53;
  static const SKSpaceGroupSetting spaceGroupHall54;
  static const SKSpaceGroupSetting spaceGroupHall55;
  static const SKSpaceGroupSetting spaceGroupHall56;
  static const SKSpaceGroupSetting spaceGroupHall57;
  static const SKSpaceGroupSetting spaceGroupHall58;
  static const SKSpaceGroupSetting spaceGroupHall59;
  static const SKSpaceGroupSetting spaceGroupHall60;
  static const SKSpaceGroupSetting spaceGroupHall61;
  static const SKSpaceGroupSetting spaceGroupHall62;
  static const SKSpaceGroupSetting spaceGroupHall63;
  static const SKSpaceGroupSetting spaceGroupHall64;
  static const SKSpaceGroupSetting spaceGroupHall65;
  static const SKSpaceGroupSetting spaceGroupHall66;
  static const SKSpaceGroupSetting spaceGroupHall67;
  static const SKSpaceGroupSetting spaceGroupHall68;
  static const SKSpaceGroupSetting spaceGroupHall69;
  static const SKSpaceGroupSetting spaceGroupHall70;
  static const SKSpaceGroupSetting spaceGroupHall71;
  static const SKSpaceGroupSetting spaceGroupHall72;
  static const SKSpaceGroupSetting spaceGroupHall73;
  static const SKSpaceGroupSetting spaceGroupHall74;
  static const SKSpaceGroupSetting spaceGroupHall75;
  static const SKSpaceGroupSetting spaceGroupHall76;
  static const SKSpaceGroupSetting spaceGroupHall77;
  static const SKSpaceGroupSetting spaceGroupHall78;
  static const SKSpaceGroupSetting spaceGroupHall79;
  static const SKSpaceGroupSetting spaceGroupHall80;
  static const SKSpaceGroupSetting spaceGroupHall81;
  static const SKSpaceGroupSetting spaceGroupHall82;
  static const SKSpaceGroupSetting spaceGroupHall83;
  static const SKSpaceGroupSetting spaceGroupHall84;
  static const SKSpaceGroupSetting spaceGroupHall85;
  static const SKSpaceGroupSetting spaceGroupHall86;
  static const SKSpaceGroupSetting spaceGroupHall87;
  static const SKSpaceGroupSetting spaceGroupHall88;
  static const SKSpaceGroupSetting spaceGroupHall89;
  static const SKSpaceGroupSetting spaceGroupHall90;
  static const SKSpaceGroupSetting spaceGroupHall91;
  static const SKSpaceGroupSetting spaceGroupHall92;
  static const SKSpaceGroupSetting spaceGroupHall93;
  static const SKSpaceGroupSetting spaceGroupHall94;
  static const SKSpaceGroupSetting spaceGroupHall95;
  static const SKSpaceGroupSetting spaceGroupHall96;
  static const SKSpaceGroupSetting spaceGroupHall97;
  static const SKSpaceGroupSetting spaceGroupHall98;
  static const SKSpaceGroupSetting spaceGroupHall99;
  static const SKSpaceGroupSetting spaceGroupHall100;
  static const SKSpaceGroupSetting spaceGroupHall101;
  static const SKSpaceGroupSetting spaceGroupHall102;
  static const SKSpaceGroupSetting spaceGroupHall103;
  static const SKSpaceGroupSetting spaceGroupHall104;
  static const SKSpaceGroupSetting spaceGroupHall105;
  static const SKSpaceGroupSetting spaceGroupHall106;
  static const SKSpaceGroupSetting spaceGroupHall107;
  static const SKSpaceGroupSetting spaceGroupHall108;
  static const SKSpaceGroupSetting spaceGroupHall109;
  static const SKSpaceGroupSetting spaceGroupHall110;
  static const SKSpaceGroupSetting spaceGroupHall111;
  static const SKSpaceGroupSetting spaceGroupHall112;
  static const SKSpaceGroupSetting spaceGroupHall113;
  static const SKSpaceGroupSetting spaceGroupHall114;
  static const SKSpaceGroupSetting spaceGroupHall115;
  static const SKSpaceGroupSetting spaceGroupHall116;
  static const SKSpaceGroupSetting spaceGroupHall117;
  static const SKSpaceGroupSetting spaceGroupHall118;
  static const SKSpaceGroupSetting spaceGroupHall119;
  static const SKSpaceGroupSetting spaceGroupHall120;
  static const SKSpaceGroupSetting spaceGroupHall121;
  static const SKSpaceGroupSetting spaceGroupHall122;
  static const SKSpaceGroupSetting spaceGroupHall123;
  static const SKSpaceGroupSetting spaceGroupHall124;
  static const SKSpaceGroupSetting spaceGroupHall125;
  static const SKSpaceGroupSetting spaceGroupHall126;
  static const SKSpaceGroupSetting spaceGroupHall127;
  static const SKSpaceGroupSetting spaceGroupHall128;
  static const SKSpaceGroupSetting spaceGroupHall129;
  static const SKSpaceGroupSetting spaceGroupHall130;
  static const SKSpaceGroupSetting spaceGroupHall131;
  static const SKSpaceGroupSetting spaceGroupHall132;
  static const SKSpaceGroupSetting spaceGroupHall133;
  static const SKSpaceGroupSetting spaceGroupHall134;
  static const SKSpaceGroupSetting spaceGroupHall135;
  static const SKSpaceGroupSetting spaceGroupHall136;
  static const SKSpaceGroupSetting spaceGroupHall137;
  static const SKSpaceGroupSetting spaceGroupHall138;
  static const SKSpaceGroupSetting spaceGroupHall139;
  static const SKSpaceGroupSetting spaceGroupHall140;
  static const SKSpaceGroupSetting spaceGroupHall141;
  static const SKSpaceGroupSetting spaceGroupHall142;
  static const SKSpaceGroupSetting spaceGroupHall143;
  static const SKSpaceGroupSetting spaceGroupHall144;
  static const SKSpaceGroupSetting spaceGroupHall145;
  static const SKSpaceGroupSetting spaceGroupHall146;
  static const SKSpaceGroupSetting spaceGroupHall147;
  static const SKSpaceGroupSetting spaceGroupHall148;
  static const SKSpaceGroupSetting spaceGroupHall149;
  static const SKSpaceGroupSetting spaceGroupHall150;
  static const SKSpaceGroupSetting spaceGroupHall151;
  static const SKSpaceGroupSetting spaceGroupHall152;
  static const SKSpaceGroupSetting spaceGroupHall153;
  static const SKSpaceGroupSetting spaceGroupHall154;
  static const SKSpaceGroupSetting spaceGroupHall155;
  static const SKSpaceGroupSetting spaceGroupHall156;
  static const SKSpaceGroupSetting spaceGroupHall157;
  static const SKSpaceGroupSetting spaceGroupHall158;
  static const SKSpaceGroupSetting spaceGroupHall159;
  static const SKSpaceGroupSetting spaceGroupHall160;
  static const SKSpaceGroupSetting spaceGroupHall161;
  static const SKSpaceGroupSetting spaceGroupHall162;
  static const SKSpaceGroupSetting spaceGroupHall163;
  static const SKSpaceGroupSetting spaceGroupHall164;
  static const SKSpaceGroupSetting spaceGroupHall165;
  static const SKSpaceGroupSetting spaceGroupHall166;
  static const SKSpaceGroupSetting spaceGroupHall167;
  static const SKSpaceGroupSetting spaceGroupHall168;
  static const SKSpaceGroupSetting spaceGroupHall169;
  static const SKSpaceGroupSetting spaceGroupHall170;
  static const SKSpaceGroupSetting spaceGroupHall171;
  static const SKSpaceGroupSetting spaceGroupHall172;
  static const SKSpaceGroupSetting spaceGroupHall173;
  static const SKSpaceGroupSetting spaceGroupHall174;
  static const SKSpaceGroupSetting spaceGroupHall175;
  static const SKSpaceGroupSetting spaceGroupHall176;
  static const SKSpaceGroupSetting spaceGroupHall177;
  static const SKSpaceGroupSetting spaceGroupHall178;
  static const SKSpaceGroupSetting spaceGroupHall179;
  static const SKSpaceGroupSetting spaceGroupHall180;
  static const SKSpaceGroupSetting spaceGroupHall181;
  static const SKSpaceGroupSetting spaceGroupHall182;
  static const SKSpaceGroupSetting spaceGroupHall183;
  static const SKSpaceGroupSetting spaceGroupHall184;
  static const SKSpaceGroupSetting spaceGroupHall185;
  static const SKSpaceGroupSetting spaceGroupHall186;
  static const SKSpaceGroupSetting spaceGroupHall187;
  static const SKSpaceGroupSetting spaceGroupHall188;
  static const SKSpaceGroupSetting spaceGroupHall189;
  static const SKSpaceGroupSetting spaceGroupHall190;
  static const SKSpaceGroupSetting spaceGroupHall191;
  static const SKSpaceGroupSetting spaceGroupHall192;
  static const SKSpaceGroupSetting spaceGroupHall193;
  static const SKSpaceGroupSetting spaceGroupHall194;
  static const SKSpaceGroupSetting spaceGroupHall195;
  static const SKSpaceGroupSetting spaceGroupHall196;
  static const SKSpaceGroupSetting spaceGroupHall197;
  static const SKSpaceGroupSetting spaceGroupHall198;
  static const SKSpaceGroupSetting spaceGroupHall199;
  static const SKSpaceGroupSetting spaceGroupHall200;
  static const SKSpaceGroupSetting spaceGroupHall201;
  static const SKSpaceGroupSetting spaceGroupHall202;
  static const SKSpaceGroupSetting spaceGroupHall203;
  static const SKSpaceGroupSetting spaceGroupHall204;
  static const SKSpaceGroupSetting spaceGroupHall205;
  static const SKSpaceGroupSetting spaceGroupHall206;
  static const SKSpaceGroupSetting spaceGroupHall207;
  static const SKSpaceGroupSetting spaceGroupHall208;
  static const SKSpaceGroupSetting spaceGroupHall209;
  static const SKSpaceGroupSetting spaceGroupHall210;
  static const SKSpaceGroupSetting spaceGroupHall211;
  static const SKSpaceGroupSetting spaceGroupHall212;
  static const SKSpaceGroupSetting spaceGroupHall213;
  static const SKSpaceGroupSetting spaceGroupHall214;
  static const SKSpaceGroupSetting spaceGroupHall215;
  static const SKSpaceGroupSetting spaceGroupHall216;
  static const SKSpaceGroupSetting spaceGroupHall217;
  static const SKSpaceGroupSetting spaceGroupHall218;
  static const SKSpaceGroupSetting spaceGroupHall219;
  static const SKSpaceGroupSetting spaceGroupHall220;
  static const SKSpaceGroupSetting spaceGroupHall221;
  static const SKSpaceGroupSetting spaceGroupHall222;
  static const SKSpaceGroupSetting spaceGroupHall223;
  static const SKSpaceGroupSetting spaceGroupHall224;
  static const SKSpaceGroupSetting spaceGroupHall225;
  static const SKSpaceGroupSetting spaceGroupHall226;
  static const SKSpaceGroupSetting spaceGroupHall227;
  static const SKSpaceGroupSetting spaceGroupHall228;
  static const SKSpaceGroupSetting spaceGroupHall229;
  static const SKSpaceGroupSetting spaceGroupHall230;
  static const SKSpaceGroupSetting spaceGroupHall231;
  static const SKSpaceGroupSetting spaceGroupHall232;
  static const SKSpaceGroupSetting spaceGroupHall233;
  static const SKSpaceGroupSetting spaceGroupHall234;
  static const SKSpaceGroupSetting spaceGroupHall235;
  static const SKSpaceGroupSetting spaceGroupHall236;
  static const SKSpaceGroupSetting spaceGroupHall237;
  static const SKSpaceGroupSetting spaceGroupHall238;
  static const SKSpaceGroupSetting spaceGroupHall239;
  static const SKSpaceGroupSetting spaceGroupHall240;
  static const SKSpaceGroupSetting spaceGroupHall241;
  static const SKSpaceGroupSetting spaceGroupHall242;
  static const SKSpaceGroupSetting spaceGroupHall243;
  static const SKSpaceGroupSetting spaceGroupHall244;
  static const SKSpaceGroupSetting spaceGroupHall245;
  static const SKSpaceGroupSetting spaceGroupHall246;
  static const SKSpaceGroupSetting spaceGroupHall247;
  static const SKSpaceGroupSetting spaceGroupHall248;
  static const SKSpaceGroupSetting spaceGroupHall249;
  static const SKSpaceGroupSetting spaceGroupHall250;
  static const SKSpaceGroupSetting spaceGroupHall251;
  static const SKSpaceGroupSetting spaceGroupHall252;
  static const SKSpaceGroupSetting spaceGroupHall253;
  static const SKSpaceGroupSetting spaceGroupHall254;
  static const SKSpaceGroupSetting spaceGroupHall255;
  static const SKSpaceGroupSetting spaceGroupHall256;
  static const SKSpaceGroupSetting spaceGroupHall257;
  static const SKSpaceGroupSetting spaceGroupHall258;
  static const SKSpaceGroupSetting spaceGroupHall259;
  static const SKSpaceGroupSetting spaceGroupHall260;
  static const SKSpaceGroupSetting spaceGroupHall261;
  static const SKSpaceGroupSetting spaceGroupHall262;
  static const SKSpaceGroupSetting spaceGroupHall263;
  static const SKSpaceGroupSetting spaceGroupHall264;
  static const SKSpaceGroupSetting spaceGroupHall265;
  static const SKSpaceGroupSetting spaceGroupHall266;
  static const SKSpaceGroupSetting spaceGroupHall267;
  static const SKSpaceGroupSetting spaceGroupHall268;
  static const SKSpaceGroupSetting spaceGroupHall269;
  static const SKSpaceGroupSetting spaceGroupHall270;
  static const SKSpaceGroupSetting spaceGroupHall271;
  static const SKSpaceGroupSetting spaceGroupHall272;
  static const SKSpaceGroupSetting spaceGroupHall273;
  static const SKSpaceGroupSetting spaceGroupHall274;
  static const SKSpaceGroupSetting spaceGroupHall275;
  static const SKSpaceGroupSetting spaceGroupHall276;
  static const SKSpaceGroupSetting spaceGroupHall277;
  static const SKSpaceGroupSetting spaceGroupHall278;
  static const SKSpaceGroupSetting spaceGroupHall279;
  static const SKSpaceGroupSetting spaceGroupHall280;
  static const SKSpaceGroupSetting spaceGroupHall281;
  static const SKSpaceGroupSetting spaceGroupHall282;
  static const SKSpaceGroupSetting spaceGroupHall283;
  static const SKSpaceGroupSetting spaceGroupHall284;
  static const SKSpaceGroupSetting spaceGroupHall285;
  static const SKSpaceGroupSetting spaceGroupHall286;
  static const SKSpaceGroupSetting spaceGroupHall287;
  static const SKSpaceGroupSetting spaceGroupHall288;
  static const SKSpaceGroupSetting spaceGroupHall289;
  static const SKSpaceGroupSetting spaceGroupHall290;
  static const SKSpaceGroupSetting spaceGroupHall291;
  static const SKSpaceGroupSetting spaceGroupHall292;
  static const SKSpaceGroupSetting spaceGroupHall293;
  static const SKSpaceGroupSetting spaceGroupHall294;
  static const SKSpaceGroupSetting spaceGroupHall295;
  static const SKSpaceGroupSetting spaceGroupHall296;
  static const SKSpaceGroupSetting spaceGroupHall297;
  static const SKSpaceGroupSetting spaceGroupHall298;
  static const SKSpaceGroupSetting spaceGroupHall299;
  static const SKSpaceGroupSetting spaceGroupHall300;
  static const SKSpaceGroupSetting spaceGroupHall301;
  static const SKSpaceGroupSetting spaceGroupHall302;
  static const SKSpaceGroupSetting spaceGroupHall303;
  static const SKSpaceGroupSetting spaceGroupHall304;
  static const SKSpaceGroupSetting spaceGroupHall305;
  static const SKSpaceGroupSetting spaceGroupHall306;
  static const SKSpaceGroupSetting spaceGroupHall307;
  static const SKSpaceGroupSetting spaceGroupHall308;
  static const SKSpaceGroupSetting spaceGroupHall309;
  static const SKSpaceGroupSetting spaceGroupHall310;
  static const SKSpaceGroupSetting spaceGroupHall311;
  static const SKSpaceGroupSetting spaceGroupHall312;
  static const SKSpaceGroupSetting spaceGroupHall313;
  static const SKSpaceGroupSetting spaceGroupHall314;
  static const SKSpaceGroupSetting spaceGroupHall315;
  static const SKSpaceGroupSetting spaceGroupHall316;
  static const SKSpaceGroupSetting spaceGroupHall317;
  static const SKSpaceGroupSetting spaceGroupHall318;
  static const SKSpaceGroupSetting spaceGroupHall319;
  static const SKSpaceGroupSetting spaceGroupHall320;
  static const SKSpaceGroupSetting spaceGroupHall321;
  static const SKSpaceGroupSetting spaceGroupHall322;
  static const SKSpaceGroupSetting spaceGroupHall323;
  static const SKSpaceGroupSetting spaceGroupHall324;
  static const SKSpaceGroupSetting spaceGroupHall325;
  static const SKSpaceGroupSetting spaceGroupHall326;
  static const SKSpaceGroupSetting spaceGroupHall327;
  static const SKSpaceGroupSetting spaceGroupHall328;
  static const SKSpaceGroupSetting spaceGroupHall329;
  static const SKSpaceGroupSetting spaceGroupHall330;
  static const SKSpaceGroupSetting spaceGroupHall331;
  static const SKSpaceGroupSetting spaceGroupHall332;
  static const SKSpaceGroupSetting spaceGroupHall333;
  static const SKSpaceGroupSetting spaceGroupHall334;
  static const SKSpaceGroupSetting spaceGroupHall335;
  static const SKSpaceGroupSetting spaceGroupHall336;
  static const SKSpaceGroupSetting spaceGroupHall337;
  static const SKSpaceGroupSetting spaceGroupHall338;
  static const SKSpaceGroupSetting spaceGroupHall339;
  static const SKSpaceGroupSetting spaceGroupHall340;
  static const SKSpaceGroupSetting spaceGroupHall341;
  static const SKSpaceGroupSetting spaceGroupHall342;
  static const SKSpaceGroupSetting spaceGroupHall343;
  static const SKSpaceGroupSetting spaceGroupHall344;
  static const SKSpaceGroupSetting spaceGroupHall345;
  static const SKSpaceGroupSetting spaceGroupHall346;
  static const SKSpaceGroupSetting spaceGroupHall347;
  static const SKSpaceGroupSetting spaceGroupHall348;
  static const SKSpaceGroupSetting spaceGroupHall349;
  static const SKSpaceGroupSetting spaceGroupHall350;
  static const SKSpaceGroupSetting spaceGroupHall351;
  static const SKSpaceGroupSetting spaceGroupHall352;
  static const SKSpaceGroupSetting spaceGroupHall353;
  static const SKSpaceGroupSetting spaceGroupHall354;
  static const SKSpaceGroupSetting spaceGroupHall355;
  static const SKSpaceGroupSetting spaceGroupHall356;
  static const SKSpaceGroupSetting spaceGroupHall357;
  static const SKSpaceGroupSetting spaceGroupHall358;
  static const SKSpaceGroupSetting spaceGroupHall359;
  static const SKSpaceGroupSetting spaceGroupHall360;
  static const SKSpaceGroupSetting spaceGroupHall361;
  static const SKSpaceGroupSetting spaceGroupHall362;
  static const SKSpaceGroupSetting spaceGroupHall363;
  static const SKSpaceGroupSetting spaceGroupHall364;
  static const SKSpaceGroupSetting spaceGroupHall365;
  static const SKSpaceGroupSetting spaceGroupHall366;
  static const SKSpaceGroupSetting spaceGroupHall367;
  static const SKSpaceGroupSetting spaceGroupHall368;
  static const SKSpaceGroupSetting spaceGroupHall369;
  static const SKSpaceGroupSetting spaceGroupHall370;
  static const SKSpaceGroupSetting spaceGroupHall371;
  static const SKSpaceGroupSetting spaceGroupHall372;
  static const SKSpaceGroupSetting spaceGroupHall373;
  static const SKSpaceGroupSetting spaceGroupHall374;
  static const SKSpaceGroupSetting spaceGroupHall375;
  static const SKSpaceGroupSetting spaceGroupHall376;
  static const SKSpaceGroupSetting spaceGroupHall377;
  static const SKSpaceGroupSetting spaceGroupHall378;
  static const SKSpaceGroupSetting spaceGroupHall379;
  static const SKSpaceGroupSetting spaceGroupHall380;
  static const SKSpaceGroupSetting spaceGroupHall381;
  static const SKSpaceGroupSetting spaceGroupHall382;
  static const SKSpaceGroupSetting spaceGroupHall383;
  static const SKSpaceGroupSetting spaceGroupHall384;
  static const SKSpaceGroupSetting spaceGroupHall385;
  static const SKSpaceGroupSetting spaceGroupHall386;
  static const SKSpaceGroupSetting spaceGroupHall387;
  static const SKSpaceGroupSetting spaceGroupHall388;
  static const SKSpaceGroupSetting spaceGroupHall389;
  static const SKSpaceGroupSetting spaceGroupHall390;
  static const SKSpaceGroupSetting spaceGroupHall391;
  static const SKSpaceGroupSetting spaceGroupHall392;
  static const SKSpaceGroupSetting spaceGroupHall393;
  static const SKSpaceGroupSetting spaceGroupHall394;
  static const SKSpaceGroupSetting spaceGroupHall395;
  static const SKSpaceGroupSetting spaceGroupHall396;
  static const SKSpaceGroupSetting spaceGroupHall397;
  static const SKSpaceGroupSetting spaceGroupHall398;
  static const SKSpaceGroupSetting spaceGroupHall399;
  static const SKSpaceGroupSetting spaceGroupHall400;
  static const SKSpaceGroupSetting spaceGroupHall401;
  static const SKSpaceGroupSetting spaceGroupHall402;
  static const SKSpaceGroupSetting spaceGroupHall403;
  static const SKSpaceGroupSetting spaceGroupHall404;
  static const SKSpaceGroupSetting spaceGroupHall405;
  static const SKSpaceGroupSetting spaceGroupHall406;
  static const SKSpaceGroupSetting spaceGroupHall407;
  static const SKSpaceGroupSetting spaceGroupHall408;
  static const SKSpaceGroupSetting spaceGroupHall409;
  static const SKSpaceGroupSetting spaceGroupHall410;
  static const SKSpaceGroupSetting spaceGroupHall411;
  static const SKSpaceGroupSetting spaceGroupHall412;
  static const SKSpaceGroupSetting spaceGroupHall413;
  static const SKSpaceGroupSetting spaceGroupHall414;
  static const SKSpaceGroupSetting spaceGroupHall415;
  static const SKSpaceGroupSetting spaceGroupHall416;
  static const SKSpaceGroupSetting spaceGroupHall417;
  static const SKSpaceGroupSetting spaceGroupHall418;
  static const SKSpaceGroupSetting spaceGroupHall419;
  static const SKSpaceGroupSetting spaceGroupHall420;
  static const SKSpaceGroupSetting spaceGroupHall421;
  static const SKSpaceGroupSetting spaceGroupHall422;
  static const SKSpaceGroupSetting spaceGroupHall423;
  static const SKSpaceGroupSetting spaceGroupHall424;
  static const SKSpaceGroupSetting spaceGroupHall425;
  static const SKSpaceGroupSetting spaceGroupHall426;
  static const SKSpaceGroupSetting spaceGroupHall427;
  static const SKSpaceGroupSetting spaceGroupHall428;
  static const SKSpaceGroupSetting spaceGroupHall429;
  static const SKSpaceGroupSetting spaceGroupHall430;
  static const SKSpaceGroupSetting spaceGroupHall431;
  static const SKSpaceGroupSetting spaceGroupHall432;
  static const SKSpaceGroupSetting spaceGroupHall433;
  static const SKSpaceGroupSetting spaceGroupHall434;
  static const SKSpaceGroupSetting spaceGroupHall435;
  static const SKSpaceGroupSetting spaceGroupHall436;
  static const SKSpaceGroupSetting spaceGroupHall437;
  static const SKSpaceGroupSetting spaceGroupHall438;
  static const SKSpaceGroupSetting spaceGroupHall439;
  static const SKSpaceGroupSetting spaceGroupHall440;
  static const SKSpaceGroupSetting spaceGroupHall441;
  static const SKSpaceGroupSetting spaceGroupHall442;
  static const SKSpaceGroupSetting spaceGroupHall443;
  static const SKSpaceGroupSetting spaceGroupHall444;
  static const SKSpaceGroupSetting spaceGroupHall445;
  static const SKSpaceGroupSetting spaceGroupHall446;
  static const SKSpaceGroupSetting spaceGroupHall447;
  static const SKSpaceGroupSetting spaceGroupHall448;
  static const SKSpaceGroupSetting spaceGroupHall449;
  static const SKSpaceGroupSetting spaceGroupHall450;
  static const SKSpaceGroupSetting spaceGroupHall451;
  static const SKSpaceGroupSetting spaceGroupHall452;
  static const SKSpaceGroupSetting spaceGroupHall453;
  static const SKSpaceGroupSetting spaceGroupHall454;
  static const SKSpaceGroupSetting spaceGroupHall455;
  static const SKSpaceGroupSetting spaceGroupHall456;
  static const SKSpaceGroupSetting spaceGroupHall457;
  static const SKSpaceGroupSetting spaceGroupHall458;
  static const SKSpaceGroupSetting spaceGroupHall459;
  static const SKSpaceGroupSetting spaceGroupHall460;
  static const SKSpaceGroupSetting spaceGroupHall461;
  static const SKSpaceGroupSetting spaceGroupHall462;
  static const SKSpaceGroupSetting spaceGroupHall463;
  static const SKSpaceGroupSetting spaceGroupHall464;
  static const SKSpaceGroupSetting spaceGroupHall465;
  static const SKSpaceGroupSetting spaceGroupHall466;
  static const SKSpaceGroupSetting spaceGroupHall467;
  static const SKSpaceGroupSetting spaceGroupHall468;
  static const SKSpaceGroupSetting spaceGroupHall469;
  static const SKSpaceGroupSetting spaceGroupHall470;
  static const SKSpaceGroupSetting spaceGroupHall471;
  static const SKSpaceGroupSetting spaceGroupHall472;
  static const SKSpaceGroupSetting spaceGroupHall473;
  static const SKSpaceGroupSetting spaceGroupHall474;
  static const SKSpaceGroupSetting spaceGroupHall475;
  static const SKSpaceGroupSetting spaceGroupHall476;
  static const SKSpaceGroupSetting spaceGroupHall477;
  static const SKSpaceGroupSetting spaceGroupHall478;
  static const SKSpaceGroupSetting spaceGroupHall479;
  static const SKSpaceGroupSetting spaceGroupHall480;
  static const SKSpaceGroupSetting spaceGroupHall481;
  static const SKSpaceGroupSetting spaceGroupHall482;
  static const SKSpaceGroupSetting spaceGroupHall483;
  static const SKSpaceGroupSetting spaceGroupHall484;
  static const SKSpaceGroupSetting spaceGroupHall485;
  static const SKSpaceGroupSetting spaceGroupHall486;
  static const SKSpaceGroupSetting spaceGroupHall487;
  static const SKSpaceGroupSetting spaceGroupHall488;
  static const SKSpaceGroupSetting spaceGroupHall489;
  static const SKSpaceGroupSetting spaceGroupHall490;
  static const SKSpaceGroupSetting spaceGroupHall491;
  static const SKSpaceGroupSetting spaceGroupHall492;
  static const SKSpaceGroupSetting spaceGroupHall493;
  static const SKSpaceGroupSetting spaceGroupHall494;
  static const SKSpaceGroupSetting spaceGroupHall495;
  static const SKSpaceGroupSetting spaceGroupHall496;
  static const SKSpaceGroupSetting spaceGroupHall497;
  static const SKSpaceGroupSetting spaceGroupHall498;
  static const SKSpaceGroupSetting spaceGroupHall499;
  static const SKSpaceGroupSetting spaceGroupHall500;
  static const SKSpaceGroupSetting spaceGroupHall501;
  static const SKSpaceGroupSetting spaceGroupHall502;
  static const SKSpaceGroupSetting spaceGroupHall503;
  static const SKSpaceGroupSetting spaceGroupHall504;
  static const SKSpaceGroupSetting spaceGroupHall505;
  static const SKSpaceGroupSetting spaceGroupHall506;
  static const SKSpaceGroupSetting spaceGroupHall507;
  static const SKSpaceGroupSetting spaceGroupHall508;
  static const SKSpaceGroupSetting spaceGroupHall509;
  static const SKSpaceGroupSetting spaceGroupHall510;
  static const SKSpaceGroupSetting spaceGroupHall511;
  static const SKSpaceGroupSetting spaceGroupHall512;
  static const SKSpaceGroupSetting spaceGroupHall513;
  static const SKSpaceGroupSetting spaceGroupHall514;
  static const SKSpaceGroupSetting spaceGroupHall515;
  static const SKSpaceGroupSetting spaceGroupHall516;
  static const SKSpaceGroupSetting spaceGroupHall517;
  static const SKSpaceGroupSetting spaceGroupHall518;
  static const SKSpaceGroupSetting spaceGroupHall519;
  static const SKSpaceGroupSetting spaceGroupHall520;
  static const SKSpaceGroupSetting spaceGroupHall521;
  static const SKSpaceGroupSetting spaceGroupHall522;
  static const SKSpaceGroupSetting spaceGroupHall523;
  static const SKSpaceGroupSetting spaceGroupHall524;
  static const SKSpaceGroupSetting spaceGroupHall525;
  static const SKSpaceGroupSetting spaceGroupHall526;
  static const SKSpaceGroupSetting spaceGroupHall527;
  static const SKSpaceGroupSetting spaceGroupHall528;
  static const SKSpaceGroupSetting spaceGroupHall529;
  static const SKSpaceGroupSetting spaceGroupHall530;

private:
  qint64 _HallNumber = 1;
  qint64 _spaceGroupNumber = 1;      // space group number (1-230)
  qint64 _order;
  char _ext;                       // '1', '2', 'H', 'R' or '\0'
  QString _qualifier;          // e.g. "-cba" or "b1"
  QString _HMString;                 // H-M symbol; nul-terminated string
  QString _HallString;               // Hall symbol; nul-terminated string
  std::string _encodedGenerators;  // encoded seitz matrix-generators
  std::string _encodedSeitz;       // encoded seitz matrix
  bool _inversionAtOrigin;
  int3 _inversionCenter;
  bool _standard = false;
  Symmorphicity _symmorphicity = Symmorphicity::asymmorphic;
  Centring _centring = Centring::primitive;
  std::vector<int3> _latticeTranslations;
  std::string _schoenflies;
  qint64 _pointGroupNumber;
  int3x3 _toPrimitiveTransformation;
};
