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

#include <QObject>
#include <QMainWindow>
#include <QAbstractItemModel>
#include <QTreeView>
#include <QModelIndex>
#include <optional>
#include <iraspakit.h>

class SceneTreeViewModel: public QAbstractItemModel
{
  Q_OBJECT

public:
  SceneTreeViewModel();

  void setProject(std::shared_ptr<ProjectTreeNode> projectTreeNode);
  bool isMainSelectedItem(std::shared_ptr<Movie> movie);
  QModelIndex indexOfMainSelected() const;
  QModelIndex indexForItem(std::shared_ptr<Movie> movie);
  QModelIndex indexForItem(std::shared_ptr<Scene> scene);
  DisplayableProtocol *getItem(const QModelIndex &index) const;

  //QT
  QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override final;
  QModelIndex parent(const QModelIndex &index) const override final;
  int rowCount(const QModelIndex &parent = QModelIndex()) const override final;
  int columnCount(const QModelIndex &parent = QModelIndex()) const override final;
  QVariant data(const QModelIndex &index, int role) const override final;
  bool setData(const QModelIndex &index, const QVariant &value, int role) override final;
  Qt::ItemFlags flags(const QModelIndex &index) const override final;
  bool hasChildren(const QModelIndex &parent) const override final;

  bool removeRows(int positions, int count, const QModelIndex &parent = QModelIndex()) override final;
  bool insertRows(int position, int rows, const QModelIndex &parent) override final;
  bool insertRow(int position, std::shared_ptr<Scene> scene, std::shared_ptr<Movie> movie);
  bool insertRow(int position, std::shared_ptr<Scene> scene);
  bool removeRow(int position);
  bool removeRow(int position, std::shared_ptr<Scene> scene, std::shared_ptr<Movie> movie);

  QStringList mimeTypes() const override final;
  Qt::DropActions supportedDropActions() const override final;
  Qt::DropActions supportedDragActions() const override final;
  QMimeData* mimeData(const QModelIndexList &indexes) const override final;
  bool canDropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent) const override final;
  bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent) override final;
  std::shared_ptr<Scene> parentForMovie(const std::shared_ptr<Movie> movie) const;
private:
  std::shared_ptr<ProjectTreeNode> _projectTreeNode;
  std::shared_ptr<SceneList> _sceneList;
signals:
  void rendererReloadData();
  void updateSelection();
  void invalidateCachedAmbientOcclusionTexture(std::vector<std::vector<std::shared_ptr<iRASPAStructure>>> structures);
};

