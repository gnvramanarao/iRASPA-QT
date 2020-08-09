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

#include <QStandardPaths>
#include "savepicturedialog.h"

SavePictureDialog::SavePictureDialog(QWidget *parent) : QFileDialog(parent)
{
  setWindowTitle("Save picture");
  setOption(QFileDialog::DontUseNativeDialog);
  setFileMode(QFileDialog::AnyFile);
  setAcceptMode(QFileDialog::AcceptSave);

  setNameFilters(QStringList({"PNG files (*.png)","TIFF files (*.tiff)", "Images (*.png *.xpm *.jpg *.tiff)"}));
  selectNameFilter(QString("PNG files (*.png)"));

  QDir desktopDir(QStandardPaths::writableLocation(QStandardPaths::DesktopLocation));
  setDirectory(desktopDir);
  selectFile("picture.png");

  QGridLayout* mainLayout = dynamic_cast <QGridLayout*>(this->layout());

  _textBrowser = new QTextBrowser(this);
  _textBrowser->setOpenExternalLinks(true);
  _textBrowser->setHtml("For use in scientific publication, please cite:<br> "
                        "D.Dubbeldam, S. Calero, and T.j.H. Vlugt,<br> "
                        "<a href=\"http://dx.doi.org/10.1080/08927022.2018.1426855\">\"iRASPA, GPU-Accelerated Visualization Software for Materials Scientists\"</a><br> "
                        "Mol. Simulat., DOI: 10.1080/08927022.2018.1426855, 2018.");

  int num_rows = mainLayout->rowCount();
  mainLayout->addWidget(_textBrowser,num_rows,0,1,-1);
}
