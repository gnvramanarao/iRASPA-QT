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

#include "pythonviewcontroller.h"
#include <iostream>
#include <string>
#include <QString>

PythonViewController* PythonViewController::_currentInstance;

PyObject* PythonViewController::log_CaptureStdout(PyObject* self, PyObject* pArgs)
{
   //std::cout << "log_CaptureStdout" << std::endl;
 char* LogStr = nullptr;
 if (!PyArg_ParseTuple(pArgs, "s", &LogStr)) return nullptr;

 std::cout << "CHECK: " << LogStr << " END" << std::endl;
 //printf("%s", LogStr);
 //PythonViewController::_currentInstance->appendPlainText(LogStr);
 // Simply using printf to do the real work.
 // You could also write it to a .log file or whatever...
 // MessageBox(NULL, LogStr...
 // WriteFile(hFile, LogStr...

 Py_INCREF(Py_None);
 return Py_None;
}

PyObject* PythonViewController::log_CaptureStderr(PyObject* self, PyObject* pArgs)
{
 char* LogStr = nullptr;
 if (!PyArg_ParseTuple(pArgs, "s", &LogStr)) return nullptr;

 //std::cout << "CHECK2 : " << LogStr << " END2" << std::endl;

 //PythonViewController::_currentInstance->appendPlainText(QString(LogStr));

 Py_INCREF(Py_None);
 return Py_None;
}

static PyObject*
redirection_stdoutredirect(PyObject *self, PyObject *args)
{
    const char *string;
    if(!PyArg_ParseTuple(args, "s", &string))
        return NULL;
    //pass string onto somewhere
    Py_INCREF(Py_None);
    return Py_None;
}

static PyMethodDef RedirectionMethods[] = {
    {"stdoutredirect", redirection_stdoutredirect, METH_VARARGS, "stdout redirection helper"},
    {nullptr, nullptr, 0, nullptr}
};

PythonViewController::PythonViewController(QWidget *parent): LineNumberPlainTextEdit(parent)
{
  appendPlainText("Python console ready");

  PythonViewController::_currentInstance = this;
  int cursorWidth = fontMetrics().boundingRect(QLatin1Char('9')).width();
  setCursorWidth(cursorWidth);

  char captureStdoutName[] = "CaptureStdout";
  char captureStderr[] = "CaptureStderr";

  PyMethodDef logMethods[] = {
      {captureStdoutName, log_CaptureStdout, METH_VARARGS, nullptr},
      //{captureStderr, log_CaptureStderr, METH_VARARGS, nullptr},
      {nullptr, nullptr, 0, nullptr}};


  static char name[] = "iRASPA";
  static char moduleName[] = "log";
  Py_SetProgramName(name);

  //static const char* pythonHomeDir = QDir(QCoreApplication::applicationDirPath() + "/" + QString("python2")).absolutePath().toStdString().c_str();
  //Py_SetPythonHome(const_cast<char*>(pythonHomeDir));
  Py_NoSiteFlag = 1;
  Py_Initialize();

  // have a new interpreter per document
  _state = Py_NewInterpreter();

  PyThreadState_Swap(_state);

  // Ending up with calling, so users can use it and import MyFirstModule.
  //Py_InitModule4_64(moduleName, logMethods, nullptr, nullptr, 1013);

  static const std::string string =
"import sys\n\
import math\n\
class CatchOutErr:\n\
    def __init__(self):\n\
        self.value = ''\n\
    def write(self, txt):\n\
        self.value += txt\n\
CatchOutErr = CatchOutErr()\n\
sys.stdout = CatchOutErr\n\
sys.stderr = CatchOutErr\n\
"; //this is python code to redirect stdouts/stderr


  PyObject *pModule = PyImport_AddModule("__main__"); //create main module
  PyRun_SimpleString(string.c_str()); //invoke code to redirect

  _catcher = PyObject_GetAttrString(pModule,"CatchOutErr"); //get our catchOutErr created above
  PyErr_Print(); //make python print any errors

  PyObject *output = PyObject_GetAttrString(_catcher,"value"); //get the stdout and stderr from our catchOutErr object

  char* outputString = PyString_AsString(output);
  appendPlainText(QString(outputString));
}

void PythonViewController::keyPressEvent(QKeyEvent *event)
{
  if (event->key() == Qt::Key_Return)
  {
    std::cout << "TEXT: " << textCursor().block().text().toStdString() << std::endl;
    ExecutePythonCommand(textCursor().block().text());
  }
  else
  {
    LineNumberPlainTextEdit::keyPressEvent(event);
  }
}

void PythonViewController::ExecutePythonCommand(QString command)
{
  PyObject *out = PyString_FromString("");
  PyObject_SetAttrString(_catcher, "value", out);
  PyRun_SimpleString(command.toStdString().c_str()); //this is ok stdout
  //PyRun_SimpleString("1+a"); //this creates an error
  PyErr_Print(); //make python print any errors

  PyObject *output = PyObject_GetAttrString(_catcher,"value"); //get the stdout and stderr from our catchOutErr object

  char* outputString = PyString_AsString(output);
  appendPlainText(QString(outputString));

  Py_DECREF(output);
  Py_DECREF(out);
}

PythonViewController::~PythonViewController()
{
  PyThreadState_Swap(_state);
  Py_EndInterpreter(_state);
  PyThreadState_Swap(nullptr);
}
