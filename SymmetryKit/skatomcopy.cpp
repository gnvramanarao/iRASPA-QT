#include "skatomcopy.h"

SKAtomCopy::SKAtomCopy(const SKAtomCopy &atomCopy)
{
  this->_position = atomCopy._position;
  this->_type = atomCopy._type;
  this->_tag = atomCopy._tag;
  this->_asymmetricIndex = atomCopy._asymmetricIndex;
  //this->_bonds = {};
}



QDataStream &operator<<(QDataStream &stream, const std::shared_ptr<SKAtomCopy> &copy)
{
  stream << copy->_versionNumber;
  stream << copy->_position;
  stream << static_cast<typename std::underlying_type<SKAtomCopy::AtomCopyType>::type>(copy->_type);
  stream << copy->_tag;
  stream << copy->_asymmetricIndex;
  return stream;
}

QDataStream &operator>>(QDataStream &stream, std::shared_ptr<SKAtomCopy> &copy)
{
  qint64 versionNumber;
  stream >> versionNumber;
  if(versionNumber > copy->_versionNumber)
  {
    throw InvalidArchiveVersionException(__FILE__, __LINE__, "SKAsymmetricAtom");
  }

  stream >> copy->_position;
  qint64 type;
  stream >> type;
  copy->_type = SKAtomCopy::AtomCopyType(type);
  stream >> copy->_tag;
  stream >> copy->_asymmetricIndex;
  return stream;
}
