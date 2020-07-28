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

#include "scenelist.h"
#include <iostream>
#include <optional>
#include <algorithm>

SceneList::SceneList()
{

}

QString SceneList::displayName() const
{
 return _displayName;
}

std::shared_ptr<Scene> SceneList::selectedScene()
{
  if(_selectedScene)
    return _selectedScene;
  return nullptr;
}

std::optional<int> SceneList::findChildIndex(std::shared_ptr<Scene> scene)
{
  std::vector<std::shared_ptr<Scene>>::const_iterator itr = std::find(_scenes.begin(), _scenes.end(), scene);
  if (itr != _scenes.end())
  {
    int row = itr-_scenes.begin();
    return row;
  }

  return std::nullopt;
}


bool SceneList::removeChild(size_t row)
{
  if (row < 0 || row >= _scenes.size())
     return false;

  _scenes.erase(_scenes.begin() + row);
  return true;
}

bool SceneList::removeChildren(size_t position, size_t count)
{
  if (position < 0 || position + count > _scenes.size())
    return false;
  std::vector<std::shared_ptr<Scene>>::iterator start = _scenes.begin() + position;
  std::vector<std::shared_ptr<Scene>>::iterator end = _scenes.begin() + position + count;
  _scenes.erase(start, end);

  return true;
}

bool SceneList::insertChild(size_t row, std::shared_ptr<Scene> child)
{
  if (row < 0 || row > _scenes.size())
    return false;

  _scenes.insert(_scenes.begin() + row, child);
  return true;
}


void SceneList::setSelectedFrameIndices(int frameIndex)
{
  for(std::shared_ptr<Scene> scene : _scenes)
  {
    scene->setSelectedFrameIndices(frameIndex);
  }
}

void SceneList::setSelectedScene(std::shared_ptr<Scene> scene)
{
  _selectedScene = scene;
  if(_selectedScene)
  {
    _selectedScenes.insert(_selectedScene);
  }
}

std::optional<int> SceneList::selectedSceneIndex()
{
  std::vector<std::shared_ptr<Scene>>::const_iterator itr = std::find(_scenes.begin(), _scenes.end(), selectedScene());
  if (itr != _scenes.end())
  {
    int row = itr-_scenes.begin();
    return row;
  }
  return std::nullopt;
}


void SceneList::clearSelection()
{
  for(std::shared_ptr<Scene> &scene: _scenes)
  {
    scene->selectedMovies().clear();
    for(std::shared_ptr<Movie> movie:scene->movies())
    {
      // FIX
      //movie->selectedFramesSet().clear();
    }
  }
}

std::vector<std::shared_ptr<iRASPAStructure>> SceneList::selectedMoviesiRASPAStructures()
{
  std::vector<std::shared_ptr<iRASPAStructure>> structures{};
  for(std::shared_ptr<Scene> scene: scenes())
  {
    for(std::shared_ptr<Movie> movie: scene->selectedMovies())
    {
      for(std::shared_ptr<iRASPAStructure> frame: movie->frames())
      {
        structures.push_back(frame);
      }
    }
  }
  return structures;
}


std::vector<std::vector<std::shared_ptr<iRASPAStructure>>> SceneList::selectediRASPARenderStructures() const
{
  std::vector<std::vector<std::shared_ptr<iRASPAStructure>>> sceneStructures = std::vector<std::vector<std::shared_ptr<iRASPAStructure>>>();

  for(std::shared_ptr<Scene> scene : _scenes)
  {
    std::vector<std::shared_ptr<iRASPAStructure>> structures = std::vector<std::shared_ptr<iRASPAStructure>>();
    for(std::shared_ptr<Movie> movie: scene->movies())
    {
      for(std::shared_ptr<iRASPAStructure> selectedFrame: movie->selectedFramesSet())
      {
        structures.push_back(selectedFrame);
      }
    }
    sceneStructures.push_back(structures);
  }
  return sceneStructures;
}

std::vector<std::shared_ptr<RKRenderStructure>> SceneList::flattenedSelectediRASPARenderStructures() const
{
  std::vector<std::shared_ptr<RKRenderStructure>> allStructures{};

  for(std::shared_ptr<Scene> scene : _scenes)
  {
    std::vector<std::shared_ptr<iRASPAStructure>> structures = std::vector<std::shared_ptr<iRASPAStructure>>();
    for(std::shared_ptr<Movie> movie: scene->movies())
    {
      for(std::shared_ptr<iRASPAStructure> selectedFrame: movie->selectedFramesSet())
      {
        allStructures.push_back(selectedFrame->structure());
      }
    }
  }
  return allStructures;
}

SceneListSelection SceneList::allSelection()
{
  std::map<std::shared_ptr<Scene>, std::shared_ptr<Movie>> s{};

  for(std::shared_ptr<Scene> scene : scenes())
  {
    s.insert({scene, scene->selectedMovie()});
  }

  std::map<std::shared_ptr<Scene>, std::unordered_set<std::shared_ptr<Movie>>> t{};

  for(std::shared_ptr<Scene> scene : scenes())
  {
    t.insert({scene, scene->selectedMovies()});
  }

  return SceneListSelection(_selectedScene, _selectedScenes, s, t);
}

void SceneList::setSelection(SceneListSelection selection)
{
  _selectedScene = std::get<0>(selection);
  _selectedScenes = std::get<1>(selection);

  for(auto const& [selectedScene, selectedMovie] : std::get<2>(selection))
  {
    if(selectedScene)
    {
       selectedScene->setSelectedMovie(selectedMovie);
    }
  }

  for(auto const& [selectedScene, selectedMovies] : std::get<3>(selection))
  {
    if(selectedScene)
    {
      selectedScene->setSelectedMovies(selectedMovies);
    }
  }
}


QDataStream &operator<<(QDataStream &stream, const std::shared_ptr<SceneList> &sceneList)
{
  stream << sceneList->_versionNumber;
  stream << sceneList->_displayName;
  stream << sceneList->_scenes;

  return stream;
}

QDataStream &operator>>(QDataStream &stream, std::shared_ptr<SceneList> &sceneList)
{
  qint64 versionNumber;
  stream >> versionNumber;

  if(versionNumber > sceneList->_versionNumber)
  {
    throw InvalidArchiveVersionException(__FILE__, __LINE__, "SceneList");
  }
  stream >> sceneList->_displayName;
  stream >> sceneList->_scenes;

  return stream;
}
