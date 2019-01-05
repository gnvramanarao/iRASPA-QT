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

#include "projectstructure.h"
#include <cfloat>
#include <QSize>

ProjectStructure::ProjectStructure(): _camera(std::make_shared<RKCamera>())
{
  _backgroundImage = QImage(QSize(1024,1024), QImage::Format_ARGB32);
  _backgroundImage.fill(QColor(255,255,255,255));
}

ProjectStructure::ProjectStructure(QString filename, SKColorSets& colorSets, ForceFieldSets& forcefieldSets, LogReporting *log): _camera(std::make_shared<RKCamera>())
{
  QUrl url = QUrl::fromLocalFile(filename);
  if (url.isValid())
  {
    std::shared_ptr<Scene> scene = std::make_shared<Scene>(url, colorSets, forcefieldSets, log);
    _sceneList->appendScene(scene);

    _camera->resetForNewBoundingBox(this->renderBoundingBox());
  }

  _backgroundImage = QImage(QSize(1024,1024), QImage::Format_ARGB32);
  _backgroundImage.fill(QColor(255,255,255,255));

}

ProjectStructure::ProjectStructure(QList<QUrl>  fileURLs, SKColorSets& colorSets, ForceFieldSets& forcefieldSets, LogReporting *log): _camera(std::make_shared<RKCamera>())
{
  foreach (const QUrl &url, fileURLs)
  {
    if (url.isValid())
    {
      std::shared_ptr<Scene> scene = std::make_shared<Scene>(url, colorSets, forcefieldSets, log);
      _sceneList->appendScene(scene);
    }
  }
  _camera->resetForNewBoundingBox(this->renderBoundingBox());

  _backgroundImage = QImage(QSize(1024,1024), QImage::Format_ARGB32);
  _backgroundImage.fill(QColor(255,255,255,255));

}

ProjectStructure::~ProjectStructure()
{

}

std::shared_ptr<SceneList> ProjectStructure::getSceneTreeModel()
{
  return _sceneList;
}

std::shared_ptr<Structure> ProjectStructure::getCellTreeModel()
{
  if(_sceneList->scenes().empty())
  {
    return nullptr;
  }
  std::shared_ptr<Scene> scene = _sceneList->selectedScene();
  if(!scene)
  {
    return nullptr;
  }

  if(scene->movies().empty())
  {
    return nullptr;
  }
  std::shared_ptr<Movie> movie = scene->selectedMovie();
  if(!movie)
  {
    return nullptr;
  }
  if(movie->frames().empty())
  {
    return nullptr;
  }
  std::shared_ptr<iRASPAStructure> iraspaStructure = movie->selectedFrame();
  if(!iraspaStructure)
  {
    return nullptr;
  }
  return iraspaStructure->structure();
}

std::shared_ptr<Movie> ProjectStructure::getFrameListModel()
{
  if(_sceneList->scenes().empty())
  {
    return nullptr;
  }
  std::shared_ptr<Scene> scene = _sceneList->selectedScene();
  if(!scene)
  {
    return nullptr;
  }
  if(scene->movies().empty())
  {
    return nullptr;
  }
  return scene->selectedMovie();
}

std::shared_ptr<SKAtomTreeController> ProjectStructure::getAtomTreeModel()
{
  if(_sceneList->scenes().empty())
  {
    return nullptr;
  }
  std::shared_ptr<Scene> scene = _sceneList->selectedScene();
  if(!scene)
  {
    return nullptr;
  }

  if(scene->movies().empty())
  {
    return nullptr;
  }
  std::shared_ptr<Movie> movie = scene->selectedMovie();
  if(!movie)
  {
    return nullptr;
  }
  if(movie->frames().empty())
  {
    return nullptr;
  }
  std::shared_ptr<iRASPAStructure> iraspaStructure = movie->selectedFrame();
  if(!iraspaStructure)
  {
    return nullptr;
  }
  return iraspaStructure->structure()->atomsTreeController();
}

std::shared_ptr<SKBondSetController> ProjectStructure::getBondListModel()
{
  if(_sceneList->scenes().empty())
  {
    return nullptr;
  }
  std::shared_ptr<Scene> scene = _sceneList->selectedScene();
  if(!scene)
  {
    return nullptr;
  }

  if(scene->movies().empty())
  {
    return nullptr;
  }
  std::shared_ptr<Movie> movie = scene->selectedMovie();
  if(!movie)
  {
    return nullptr;
  }
  if(movie->frames().empty())
  {
    return nullptr;
  }
  std::shared_ptr<iRASPAStructure> iraspaStructure = movie->selectedFrame();
  if(!iraspaStructure)
  {
    return nullptr;
  }
  return iraspaStructure->structure()->bondSetController();
}

std::shared_ptr<Structure> ProjectStructure::selectedStructure()
{
  if(_sceneList->scenes().empty())
  {
    return nullptr;
  }
  std::shared_ptr<Scene> scene = _sceneList->selectedScene();
  if(!scene)
  {
    return nullptr;
  }

  if(scene->movies().empty())
  {
    return nullptr;
  }
  std::shared_ptr<Movie> movie = scene->selectedMovie();
  if(!movie)
  {
    return nullptr;
  }
  if(movie->frames().empty())
  {
    return nullptr;
  }
  std::shared_ptr<iRASPAStructure> iraspaStructure = movie->selectedFrame();
  if(!iraspaStructure)
  {
    return nullptr;
  }
  return iraspaStructure->structure();
}






std::vector<int> ProjectStructure::numberOfScenes() const
{
  std::vector<int> v = std::vector<int>(_sceneList->scenes().size());

  for(size_t i=0;i<_sceneList->scenes().size();i++)
  {
    v[i] = renderStructuresForScene(i).size();
  }

  return v;
}

int ProjectStructure::numberOfMovies(int sceneIndex) const
{
  return 0;
}

std::vector<std::shared_ptr<RKRenderStructure>> ProjectStructure::renderStructuresForScene(size_t i) const
{
  std::vector<std::shared_ptr<RKRenderStructure>> structures = std::vector<std::shared_ptr<RKRenderStructure>>();

  std::shared_ptr<Scene> scene = _sceneList->scenes()[i];
  for(std::shared_ptr<Movie> movie: scene->movies())
  {
    std::shared_ptr<iRASPAStructure> selectedFrame = movie->selectedFrame();
    if(selectedFrame)
    {
      if(std::shared_ptr<Structure> structure = selectedFrame->structure())
      {
        if(std::shared_ptr<RKRenderStructure> structure2 = std::dynamic_pointer_cast<RKRenderStructure>(structure))
        {
          structures.push_back(structure2);
        }
      }
    }
  }
  return structures;
}

std::vector<std::vector<std::shared_ptr<Structure>>> ProjectStructure::structures() const
{
  std::vector<std::vector<std::shared_ptr<Structure>>> sceneStructures = std::vector<std::vector<std::shared_ptr<Structure>>>();


  for(std::shared_ptr<Scene> scene : _sceneList->scenes())
  {
    std::vector<std::shared_ptr<Structure>> structures =std::vector<std::shared_ptr<Structure>>();
    for(std::shared_ptr<Movie> movie: scene->movies())
    {
      std::shared_ptr<iRASPAStructure> selectedFrame = movie->selectedFrame();
      if(selectedFrame)
      {
        if(std::shared_ptr<Structure> structure = selectedFrame->structure())
        {
          structures.push_back(structure);
        }
      }
    }
    sceneStructures.push_back(structures);
  }
  return sceneStructures;
}

std::vector<std::vector<std::shared_ptr<RKRenderStructure>>> ProjectStructure::renderStructures() const
{
  std::vector<std::vector<std::shared_ptr<RKRenderStructure>>> sceneStructures = std::vector<std::vector<std::shared_ptr<RKRenderStructure>>>();


  for(std::shared_ptr<Scene> scene : _sceneList->scenes())
  {
    std::vector<std::shared_ptr<RKRenderStructure>> structures =std::vector<std::shared_ptr<RKRenderStructure>>();
    for(std::shared_ptr<Movie> movie: scene->movies())
    {
      std::shared_ptr<iRASPAStructure> selectedFrame = movie->selectedFrame();
      if(selectedFrame)
      {
        if(std::shared_ptr<Structure> structure = selectedFrame->structure())
        {
          if(std::shared_ptr<RKRenderStructure> structure2 = std::dynamic_pointer_cast<RKRenderStructure>(structure))
          {
            structures.push_back(structure2);
          }
        }
      }
    }
    sceneStructures.push_back(structures);
  }
  return sceneStructures;
}


std::vector<std::shared_ptr<RKRenderStructure>> ProjectStructure::flattenedRenderStructures() const
{
  std::vector<std::shared_ptr<RKRenderStructure>> structures = std::vector<std::shared_ptr<RKRenderStructure>>();

  for(std::shared_ptr<Scene> scene : _sceneList->scenes())
  {
    for(std::shared_ptr<Movie> movie: scene->movies())
    {
        std::shared_ptr<iRASPAStructure> selectedFrame = movie->selectedFrame();
        if(selectedFrame)
        {
      //for(std::shared_ptr<iRASPAStructure> iraspaStructure : movie->frames())
     // {
        if(std::shared_ptr<Structure> structure = selectedFrame->structure())
        {
          if(std::shared_ptr<RKRenderStructure> structure2 = std::dynamic_pointer_cast<RKRenderStructure>(structure))
          {
            structures.push_back(structure2);
          }
        }
      }
    }
  }
  return structures;
}

std::vector<std::shared_ptr<Structure>> ProjectStructure::flattenedStructures() const
{
  std::vector<std::shared_ptr<Structure>> structures = std::vector<std::shared_ptr<Structure>>();

  for(std::shared_ptr<Scene> scene : _sceneList->scenes())
  {
    for(std::shared_ptr<Movie> movie: scene->movies())
    {
      for(std::shared_ptr<iRASPAStructure> frame: movie->frames())
      {
        if(std::shared_ptr<Structure> structure = frame->structure())
        {
          structures.push_back(structure);
        }
      }
    }
  }
  return structures;
}
/*
std::vector<std::shared_ptr<RKRenderStructure>> ProjectStructure::renderStructures() const
{
  std::vector<std::shared_ptr<RKRenderStructure>> structures = std::vector<std::shared_ptr<RKRenderStructure>>();

  for(std::shared_ptr<Scene> scene : _sceneList.scenes())
  {
    for(std::shared_ptr<Movie> movie: scene->movies())
    {
      for(std::shared_ptr<iRASPAStructure> iraspaStructure : movie->frames())
      {
        if(std::shared_ptr<Structure> structure = iraspaStructure->structure())
        {
          if(std::shared_ptr<RKRenderStructure> structure2 = std::dynamic_pointer_cast<RKRenderStructure>(structure))
          {
            structures.push_back(structure2);
          }
        }
      }
    }
  }

  return structures;
}*/


std::vector<RKInPerInstanceAttributesAtoms> ProjectStructure::renderMeasurementPoints() const
{
  return std::vector<RKInPerInstanceAttributesAtoms>();
}

std::vector<RKRenderStructure> ProjectStructure::renderMeasurementStructure() const
{
  return std::vector<RKRenderStructure>();
}

SKBoundingBox ProjectStructure::renderBoundingBox() const
{
   std::vector<std::shared_ptr<RKRenderStructure>> structures = flattenedRenderStructures();

   if(structures.empty())
   {
     return SKBoundingBox();
   }

   double3 minimum = double3(DBL_MAX, DBL_MAX, DBL_MAX);
   double3 maximum = double3(-DBL_MAX, -DBL_MAX, -DBL_MAX);

   for(std::shared_ptr<RKRenderStructure> frame: structures)
   {
     // for rendering the bounding-box is in the global coordinate space (adding the frame origin)
     SKBoundingBox currentBoundingBox  = frame->transformedBoundingBox() + frame->origin();

     SKBoundingBox transformedBoundingBox = currentBoundingBox;

     minimum.x = std::min(minimum.x, transformedBoundingBox.minimum().x);
     minimum.y = std::min(minimum.y, transformedBoundingBox.minimum().y);
     minimum.z = std::min(minimum.z, transformedBoundingBox.minimum().z);
     maximum.x = std::max(maximum.x, transformedBoundingBox.maximum().x);
     maximum.y = std::max(maximum.y, transformedBoundingBox.maximum().y);
     maximum.z = std::max(maximum.z, transformedBoundingBox.maximum().z);
   }

   return SKBoundingBox(minimum, maximum);
}

bool ProjectStructure::hasSelectedObjects() const
{
  return false;
}

RKBackgroundType ProjectStructure::renderBackgroundType() const
{
  return _backgroundType;
}

void ProjectStructure::setBackgroundType(RKBackgroundType type)
{
  _backgroundType = type;
}

QColor ProjectStructure::renderBackgroundColor() const
{
   return _backgroundColor;
}



void ProjectStructure::setBackgroundColor(QColor color)
{
 _backgroundColor = color;
}



const QImage ProjectStructure::renderBackgroundCachedImage()
{
  switch(_backgroundType)
  {
    case RKBackgroundType::color:
    {
      QImage image = QImage(QSize(1024,1024), QImage::Format_ARGB32);
      image.fill(_backgroundColor);
      return image;
    }
    case RKBackgroundType::linearGradient:
    {
      double angle = _backgroundLinearGradientAngle;
      QRectF rect = QRectF(QPointF(0,0),QPointF(1024,1024));
      QPointF startPoint;
      QPointF endPoint;
      double radAngleToEndFromCenter;
      double width = rect.size().width();
      double height = rect.size().height();

      angle = fmod(angle, 360);
      if (angle < 90)
      {
        startPoint = QPointF(0, 0);
        radAngleToEndFromCenter = (angle * 2 - 45) / 180 * M_PI;
      }
      else if (angle < 180)
      {
        startPoint = QPointF(width, 0);
        radAngleToEndFromCenter = ((angle - 90) * 2 + 45) / 180 * M_PI;
      }
      else if (angle < 270)
      {
        startPoint = QPointF(width, height);
        radAngleToEndFromCenter = ((angle - 180) * 2 + 135) / 180 * M_PI;
      }
      else
      {
        startPoint = QPointF(0, height);
        radAngleToEndFromCenter = ((angle - 270) * 2 + 225) / 180 * M_PI;
      }

      endPoint = QPointF( (0.5 + cos(radAngleToEndFromCenter) / sqrt(2.0)) * width,
                          (0.5 + sin(radAngleToEndFromCenter) / sqrt(2.0)) * height);


      QImage image = QImage(QSize(1024,1024), QImage::Format_ARGB32);
      QPainter p(&image);
      QLinearGradient gradient(startPoint,endPoint);
      p.setCompositionMode(QPainter::CompositionMode_Source);

      gradient.setColorAt(0, _backgroundLinearGradientFromColor);
      gradient.setColorAt(1, _backgroundLinearGradientToColor);
      p.fillRect(0, 0, 1024, 1024, gradient);
      return image;
    }
    case RKBackgroundType::radialGradient:
    {
      QImage image = QImage(QSize(1024,1024), QImage::Format_ARGB32);

      double w = image.width()/2;
      double y1 = image.height()/_backgroundRadialGradientRoundness;
      double y2 = -w;

      QPainter p(&image);
      QPointF endCenter = QPointF(image.width()/2.0, y1);
      QPointF startCenter = QPointF(image.width()/2.0, y2);
      double radius = image.width()/2.0;

      QRadialGradient gradient(startCenter, radius, endCenter, radius);
      p.setCompositionMode(QPainter::CompositionMode_Source);
      gradient.setColorAt(0.0, _backgroundRadialGradientFromColor);
      gradient.setColorAt(1.0, _backgroundRadialGradientToColor);
      p.fillRect(0, 0, 1024, 1024, gradient);
      return image;
    }
    case RKBackgroundType::image:
      return _backgroundImage;
  }
}

void ProjectStructure::loadBackgroundImage(QString filename)
{
  QString baseFileName = QFileInfo(filename).fileName();
  _backgroundImageFilename = baseFileName;
  _backgroundImage.load(filename);
}


bool ProjectStructure::showBoundingBox() const
{
  return false;
}

std::vector<RKInPerInstanceAttributesAtoms> ProjectStructure::renderBoundingBoxSpheres() const
{
  return std::vector<RKInPerInstanceAttributesAtoms>();
}

std::vector<RKInPerInstanceAttributesBonds> ProjectStructure::renderBoundingBoxCylinders() const
{
  return std::vector<RKInPerInstanceAttributesBonds>();
}

double ProjectStructure::imageDotsPerInchValue()
{
  switch(imageDPI())
  {
  case RKImageDPI::dpi_72:
    return 72.0;
  case RKImageDPI::dpi_75:
    return 75.0;
  case RKImageDPI::dpi_150:
    return 150.0;
  case RKImageDPI::dpi_300:
    return 300.0;
  case RKImageDPI::dpi_600:
    return 600.0;
  case RKImageDPI::dpi_1200:
    return 1200.0;
  }
}

QDataStream &operator<<(QDataStream& stream, const std::shared_ptr<ProjectStructure>& node)
{
  stream << node->_versionNumber;

  stream << node->_showBoundingBox;

  stream << node->_backgroundType;

  // save picture in PNG format
  QByteArray imageByteArray;
  QBuffer buffer(&imageByteArray);
  buffer.open(QIODevice::WriteOnly);
  node->_backgroundImage.save(&buffer,"PNG");
  stream << imageByteArray;

  stream << node->_backgroundImageFilename;
  stream << node->_backgroundColor;
  stream << node->_backgroundLinearGradientFromColor;
  stream << node->_backgroundLinearGradientToColor;
  stream << node->_backgroundRadialGradientFromColor;
  stream << node->_backgroundRadialGradientToColor;
  stream << node->_backgroundLinearGradientAngle;
  stream << node->_backgroundRadialGradientRoundness;

  stream << node->_renderImagePhysicalSizeInInches;
  stream << node->_renderImageNumberOfPixels;
  stream << node->_aspectRatio;
  stream << node->_imageDPI;
  stream << node->_imageUnits;
  stream << node->_imageDimensions;
  stream << node->_renderImageQuality;
  stream << node->_movieFramesPerSecond;

  stream << node->_camera;
  stream << node->_sceneList;

  return stream;
}

QDataStream &operator>>(QDataStream& stream, std::shared_ptr<ProjectStructure>& node)
{
  qint64 versionNumber;
  stream >> versionNumber;
  if(versionNumber > node->_versionNumber)
  {
    throw InvalidArchiveVersionException(__FILE__, __LINE__, "ProjectStructure");
  }

  stream >> node->_showBoundingBox;
  stream >> node->_backgroundType;

  // read picture in PNG-format
  QByteArray imageByteArray;
  stream >> imageByteArray;
  QBuffer buffer(&imageByteArray);
  buffer.open(QIODevice::ReadOnly);
  node->_backgroundImage.load(&buffer, "PNG");

  stream >> node->_backgroundImageFilename;
  stream >> node->_backgroundColor;
  stream >> node->_backgroundLinearGradientFromColor;
  stream >> node->_backgroundLinearGradientToColor;
  stream >> node->_backgroundRadialGradientFromColor;
  stream >> node->_backgroundRadialGradientToColor;
  stream >> node->_backgroundLinearGradientAngle;
  stream >> node->_backgroundRadialGradientRoundness;

  stream >> node->_renderImagePhysicalSizeInInches;
  stream >> node->_renderImageNumberOfPixels;
  stream >> node->_aspectRatio;
  stream >> node->_imageDPI;
  stream >> node->_imageUnits;
  stream >> node->_imageDimensions;
  stream >> node->_renderImageQuality;

  stream >> node->_movieFramesPerSecond;

  stream >> node->_camera;
  stream >> node->_sceneList;

  return stream;
}

