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

std::shared_ptr<Movie> Scene::selectedMovie()
{
  if(!_selectedMovie)
  {
    _selectedMovie = _movies.front();
  }
  return _selectedMovie;
}

void Scene::setSelectedFrameIndices(int frameIndex)
{
  for(std::shared_ptr<Movie> movie : _movies)
  {
    movie->setSelectedFrameIndex(frameIndex);
  }
}


QString Scene::displayName() const
{
 return _displayName;
}

Scene::Scene(QUrl url, const SKColorSets& colorSets, ForceFieldSets& forcefieldSets, LogReporting *log)
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
		parser = std::make_shared<SKCIFParser>(fileContent, false, CharacterSet::whitespaceAndNewlineCharacterSet());
	}
	else if (info.suffix() == "pdb")
	{
		parser = std::make_shared<SKPDBParser>(fileContent, false, CharacterSet::whitespaceAndNewlineCharacterSet());
	}
	parser->startParsing();

	std::vector<std::vector<std::tuple<std::shared_ptr<SKAtomTreeController>, std::shared_ptr<SKCell>, int>>> movies = parser->movies();

	for (std::vector<std::tuple<std::shared_ptr<SKAtomTreeController>, std::shared_ptr<SKCell>, int>> movieFrames : movies)
	{
		std::shared_ptr<Movie> movie = std::make_shared<Movie>(info.baseName());
		for (std::tuple<std::shared_ptr<SKAtomTreeController>, std::shared_ptr<SKCell>, int> frame : movieFrames)
		{
			std::shared_ptr<Crystal> crystal = std::make_shared<Crystal>(std::get<0>(frame));

			crystal->setRepresentationStyle(RepresentationStyle::defaultStyle, colorSets);
			crystal->setAtomForceFieldIdentifier("Default", forcefieldSets);
			//crystal->recheckRepresentationStyle();

			crystal->setCell(std::get<1>(frame));

			crystal->setSpaceGroupHallNumber(std::get<2>(frame));

			/*
			crystal->expandSymmetry();
			crystal->setTags();



			crystal->computeBonds();
*/
			crystal->reComputeBoundingBox();

			std::shared_ptr<iRASPAStructure> iraspastructure = std::make_shared<iRASPAStructure>(crystal);

			crystal->recomputeDensityProperties();

			if (log)
			{
				int numberOfAtoms = crystal->atomsTreeController()->flattenedLeafNodes().size();
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


stdx::optional<int> Scene::selectMovieIndex()
{
  std::vector<std::shared_ptr<Movie>>::const_iterator itr = std::find(_movies.begin(), _movies.end(), selectedMovie());
  if (itr != _movies.end())
  {
    int row = itr-_movies.begin();
    return row;
  }

  return stdx::nullopt;
}


stdx::optional<int> Scene::findChildIndex(std::shared_ptr<Movie> movie)
{
  std::vector<std::shared_ptr<Movie>>::const_iterator itr = std::find(_movies.begin(), _movies.end(), movie);
  if (itr != _movies.end())
  {
    int row = itr-_movies.begin();
    return row;
  }

  return stdx::nullopt;
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
  return stream;
}