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

#include "scene.h"
#include "iraspastructure.h"
#include <QFile>
#include <QTextStream>
#include <iostream>

Scene::Scene()
{

}

Scene::Scene(QString displayName): _displayName(displayName)
{
}

// private constructor, parent of movie needs to be set
Scene::Scene(std::shared_ptr<Movie> movie): _movies{movie}
{
}

std::shared_ptr<Scene> Scene::create(std::shared_ptr<Movie> movie)
{
  // std::make_shared can not call private constructor
  std::shared_ptr<Scene> scene =  std::shared_ptr<Scene>( new Scene( std::forward<Scene>(movie)));
  movie->setParent(scene);
  return scene;
}

std::shared_ptr<Movie> Scene::selectedMovie()
{
  if(_selectedMovie)
    return _selectedMovie;
  return nullptr;
}

void Scene::setSelectedFrameIndices(int frameIndex)
{
  for(std::shared_ptr<Movie> movie : _movies)
  {
    movie->setSelectedFrameIndex(frameIndex);
  }
}

void Scene::setSelectedMovie(std::shared_ptr<Movie> movie)
{
  _selectedMovie = movie;
  if(_selectedMovie)
  {
    _selectedMovies.insert(_selectedMovie);
  }
}

void Scene::setSelectedMovies(std::set<std::shared_ptr<Movie>> movies)
{
  _selectedMovies = movies;
}


QString Scene::displayName() const
{
 return _displayName;
}

Scene::Scene(QUrl url, const SKColorSets& colorSets, ForceFieldSets& forcefieldSets, LogReporting *log, bool asSeparateProject, bool onlyAsymmetricUnit, bool asMolecule)
{
	QFile file(url.toLocalFile());
	QFileInfo info(file);
	if (file.open(QIODevice::ReadOnly))
	{
    QTextStream in(&file);

    _displayName = info.baseName();

    QString fileContent = in.readAll();

    std::shared_ptr<SKParser> parser;

    if (info.suffix() == "cif")
    {
      parser = std::make_shared<SKCIFParser>(fileContent, onlyAsymmetricUnit, asMolecule, CharacterSet::whitespaceAndNewlineCharacterSet());
    }
    else if (info.suffix() == "pdb")
    {
      parser = std::make_shared<SKPDBParser>(fileContent, onlyAsymmetricUnit, asMolecule, CharacterSet::whitespaceAndNewlineCharacterSet());
    }
    parser->startParsing();

    std::vector<std::vector<std::shared_ptr<SKStructure>>> movies = parser->movies();

    for (std::vector<std::shared_ptr<SKStructure>> movieFrames : movies)
    {
      std::shared_ptr<Movie> movie = std::make_shared<Movie>(info.baseName());
      for (std::shared_ptr<SKStructure> frame : movieFrames)
      {
        //std::shared_ptr<Structure> structure;
        std::shared_ptr<iRASPAStructure> iraspastructure;

        switch(frame->kind)
        {
        case SKStructure::Kind::crystal:
          iraspastructure = std::make_shared<iRASPAStructure>(std::make_shared<Crystal>(frame));
          break;
        case SKStructure::Kind::molecularCrystal:
          iraspastructure = std::make_shared<iRASPAStructure>(std::make_shared<MolecularCrystal>(frame));
          break;
        case SKStructure::Kind::molecule:
          iraspastructure = std::make_shared<iRASPAStructure>(std::make_shared<Molecule>(frame));
          break;
        case SKStructure::Kind::protein:
          iraspastructure = std::make_shared<iRASPAStructure>(std::make_shared<Protein>(frame));
          break;
        case SKStructure::Kind::proteinCrystal:
          iraspastructure = std::make_shared<iRASPAStructure>(std::make_shared<ProteinCrystal>(frame));
          break;
        default:
          if (log)
          {
            log->logMessage(LogReporting::ErrorLevel::info, "Unknown structure format");
          }
          return;
        }

        iraspastructure->structure()->setRepresentationStyle(Structure::RepresentationStyle::defaultStyle, colorSets);
        iraspastructure->structure()->setAtomForceFieldIdentifier("Default", forcefieldSets);

        iraspastructure->structure()->computeBonds();

        iraspastructure->structure()->reComputeBoundingBox();
        iraspastructure->structure()->recomputeDensityProperties();


        if (log)
        {
          size_t numberOfAtoms = iraspastructure->structure()->atomsTreeController()->flattenedLeafNodes().size();
          log->logMessage(LogReporting::ErrorLevel::info, "Read " + QString::number(numberOfAtoms) + " atoms");
        }

        movie->append(iraspastructure);
      }
      _movies.push_back(movie);
    }
  }
	else
	{
		if (log)
		{
			log->logMessage(LogReporting::ErrorLevel::error, "File not found!");
		}
	}
}


std::optional<int> Scene::selectMovieIndex()
{
  std::vector<std::shared_ptr<Movie>>::const_iterator itr = std::find(_movies.begin(), _movies.end(), selectedMovie());
  if (itr != _movies.end())
  {
    int row = itr-_movies.begin();
    return row;
  }

  return std::nullopt;
}


std::optional<int> Scene::findChildIndex(std::shared_ptr<Movie> movie)
{
  std::vector<std::shared_ptr<Movie>>::const_iterator itr = std::find(_movies.begin(), _movies.end(), movie);
  if (itr != _movies.end())
  {
    int row = itr-_movies.begin();
    return row;
  }

  return std::nullopt;
}

bool Scene::removeChild(size_t row)
{
  if (row < 0 || row >= _movies.size())
     return false;

  _movies.erase(_movies.begin() + row);
  return true;
}

bool Scene::removeChildren(size_t position, size_t count)
{
  if (position < 0 || position + count > _movies.size())
    return false;
  std::vector<std::shared_ptr<Movie>>::iterator start = _movies.begin() + position;
  std::vector<std::shared_ptr<Movie>>::iterator end = _movies.begin() + position + count;
  _movies.erase(start, end);

  return true;
}

bool Scene::insertChild(size_t row, std::shared_ptr<Movie> child)
{
  if (row < 0 || row > _movies.size())
    return false;

  _movies.insert(_movies.begin() + row, child);
  child->setParent(this->shared_from_this());
  return true;
}

QDataStream &operator<<(QDataStream &stream, const std::shared_ptr<Scene> &scene)
{
  stream << scene->_versionNumber;
  stream << scene->_displayName;
  stream << scene->_movies;
  return stream;
}

QDataStream &operator>>(QDataStream &stream, std::shared_ptr<Scene> &scene)
{
  qint64 versionNumber;
  stream >> versionNumber;

  if(versionNumber > scene->_versionNumber)
  {
    throw InvalidArchiveVersionException(__FILE__, __LINE__, "Scene");
  }
  stream >> scene->_displayName;
  stream >> scene->_movies;

  for(std::shared_ptr<Movie> movie : scene->_movies)
  {
    movie->setParent(scene);
  }

  return stream;
}
