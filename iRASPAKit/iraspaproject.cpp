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

#include "iraspaproject.h"
#include "project.h"
#include <iostream>
#include <lzma.h>
#include <QtDebug>


iRASPAProject::iRASPAProject(): _projectType(ProjectType::none),
  _fileNameUUID(QUuid::createUuid().toString().mid(1,36).toUpper()),
  _project(std::make_shared<Project>()),
  _nodeType(NodeType::leaf),
  _storageType(StorageType::local),
  _lazyStatus(LazyStatus::loaded),
  _undoStack()
{

}

iRASPAProject::iRASPAProject(std::shared_ptr<Project> project):
  _projectType(ProjectType::generic),
  _fileNameUUID(QUuid::createUuid().toString().mid(1,36).toUpper()),
  _project(project), _nodeType(NodeType::leaf),
  _storageType(StorageType::local),
  _lazyStatus(LazyStatus::loaded),
  _undoStack()
{

}

iRASPAProject::iRASPAProject(std::shared_ptr<ProjectStructure> project):
  _projectType(ProjectType::structure),
  _fileNameUUID(QUuid::createUuid().toString().mid(1,36).toUpper()),
  _project(project), _nodeType(NodeType::leaf),
  _storageType(StorageType::local),
  _lazyStatus(LazyStatus::loaded),
  _undoStack()
{

}

iRASPAProject::iRASPAProject(std::shared_ptr<ProjectGroup> project):
  _projectType(ProjectType::group),
  _fileNameUUID(QUuid::createUuid().toString().mid(1,36).toUpper()),
  _project(project), _nodeType(NodeType::group),
  _storageType(StorageType::local),
  _lazyStatus(LazyStatus::loaded),
  _undoStack()
{

}

void iRASPAProject::readData(ZipReader& reader)
{
  _data = reader.fileData(QString("nl.darkwing.iRASPA_Project_") + _fileNameUUID);
}


void iRASPAProject::unwrapIfNeeded()
{
  if(_lazyStatus == LazyStatus::lazy)
  {
    QByteArray uncompressedData = ZipReader::xzUncompress(_data);
    QDataStream stream(&uncompressedData, QIODevice::ReadOnly);

    try
    {
      switch(_projectType)
      {
      case ProjectType::none:
        break;
      case ProjectType::generic:
        stream >> _project;
        break;
      case ProjectType::group:
      {
        std::shared_ptr<ProjectGroup> groupNode = std::make_shared<ProjectGroup>();
        stream >> groupNode;
        _project = groupNode;
        stream >> _project;
        break;
      }
      case ProjectType::structure:
      {
        std::shared_ptr<ProjectStructure> projectNode = std::make_shared<ProjectStructure>();
        stream >> projectNode;
        _project = projectNode;
        stream >> _project;
        break;
      }
      case ProjectType::VASP:
        break;
      case ProjectType::RASPA:
        break;
      case ProjectType::GULP:
        break;
      case ProjectType::CP2K:
        break;
      }

      _lazyStatus = LazyStatus::loaded;
    }
    catch (InvalidArchiveVersionException ex)
    {
      std::cout << "Error: " << ex.message().toStdString() << std::endl;
      std::cout << ex.what() << ex.get_file() << std::endl;
      std::cout << "Function: " << ex.get_func() << std::endl;
    }
    catch(InconsistentArchiveException ex)
    {
      std::cout << "Error: " << ex.message().toStdString() << std::endl;
      std::cout << ex.what() << ex.get_file() << std::endl;
      std::cout << "Function: " << ex.get_func() << std::endl;
    }
    catch(std::exception e)
    {
      std::cout << "Error: " << e.what() << std::endl;
    }
  }
}

void iRASPAProject::saveData(ZipWriter& writer)
{
  // lazy projects: write compressed data directly (stored in _data)
  if(_lazyStatus == LazyStatus::lazy)
  {
    writer.addFile(QString("nl.darkwing.iRASPA_Project_") + _fileNameUUID, _data);
    return;
  }

  QByteArray byteArray = QByteArray();
  QDataStream stream(&byteArray, QIODevice::WriteOnly);

  switch(_projectType)
  {
  case ProjectType::none:
    break;
  case ProjectType::generic:
    stream << _project;
    break;
  case ProjectType::group:
    stream << std::dynamic_pointer_cast<ProjectGroup>(_project);
    stream << _project;
    break;
  case ProjectType::structure:
    stream << std::dynamic_pointer_cast<ProjectStructure>(_project);
    stream << _project;
    break;
  case ProjectType::VASP:
    break;
  case ProjectType::RASPA:
    break;
  case ProjectType::GULP:
    break;
  case ProjectType::CP2K:
    break;
  }

  QByteArray compressedData = ZipWriter::xzCompress(byteArray);

  writer.addFile(QString("nl.darkwing.iRASPA_Project_") + _fileNameUUID, compressedData);
}

QDataStream &operator<<(QDataStream& stream, const std::shared_ptr<iRASPAProject>& node)
{
  stream << node->_versionNumber;
  stream << static_cast<typename std::underlying_type<iRASPAProject::ProjectType>::type>(node->_projectType);
  stream << node->_fileNameUUID;
  stream << static_cast<typename std::underlying_type<iRASPAProject::NodeType>::type>(node->_nodeType);
  stream << static_cast<typename std::underlying_type<iRASPAProject::StorageType>::type>(node->_storageType);
  stream << static_cast<typename std::underlying_type<iRASPAProject::LazyStatus>::type>(iRASPAProject::LazyStatus::lazy);

  return stream;
}

QDataStream &operator>>(QDataStream& stream, std::shared_ptr<iRASPAProject>& node)
{
  qint64 versionNumber;
  stream >> versionNumber;
  if(versionNumber > node->_versionNumber)
  {
    throw InvalidArchiveVersionException(__FILE__, __LINE__, "iRASPAProject");
  }
  qint64 projectType;
  stream >> projectType;
  node->_projectType = iRASPAProject::ProjectType(projectType);
  stream >> node->_fileNameUUID;

  qint64 nodeType;
  stream >> nodeType;
  node->_nodeType = iRASPAProject::NodeType(nodeType);
  qint64 storageType;
  stream >> storageType;
  node->_storageType = iRASPAProject::StorageType(storageType);
  qint64 lazyStatus;
  stream >> lazyStatus;
  node->_lazyStatus = iRASPAProject::LazyStatus(lazyStatus);

  return stream;
}
