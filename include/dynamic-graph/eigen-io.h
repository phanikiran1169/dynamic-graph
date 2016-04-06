//
// Copyright 2016 CNRS
//
// Author: Rohan Budhiraja
//
// This file is part of dynamic-graph.
// dynamic-graph is free software: you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License
// as published by the Free Software Foundation, either version 3 of
// the License, or (at your option) any later version.
// dynamic-graph is distributed in the hope that it will be
// useful, but WITHOUT ANY WARRANTY; without even the implied warranty
// of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.  You should
// have received a copy of the GNU Lesser General Public License along
// with dynamic-graph.  If not, see <http://www.gnu.org/licenses/>.

#ifndef DYNAMIC_GRAPH_EIGEN_IO_H
#define DYNAMIC_GRAPH_EIGEN_IO_H

#include <iostream>
#include <boost/format.hpp>
#include <boost/numeric/conversion/cast.hpp>

#include <dynamic-graph/exception-signal.h>
#include <dynamic-graph/linear-algebra.h>
#include <Eigen/Geometry>

using dynamicgraph::ExceptionSignal;

//TODO: Eigen 3.3 onwards has a global Eigen::Index definition.
//If Eigen version is updated, use Eigen::Index instead of this macro.


  /* \brief Eigen Vector input from istream
   *
   * Input Vector format: [N](val1,val2,val3,...,valN)
   * e.g. [5](1,23,32.2,12.12,32)
   */
namespace Eigen {
  typedef EIGEN_DEFAULT_DENSE_INDEX_TYPE eigen_index;
  inline std::istringstream& operator >> (std::istringstream &iss, 
					  dynamicgraph::Vector &inst) {
    unsigned int _size;
    double _dbl_val;
    char _ch;
    boost::format fmt ("Failed to enter %s as vector. Reenter as [N](val1,val2,val3,...,valN)");
    fmt %iss.str();
    if(iss>> _ch && _ch != '['){
      throw ExceptionSignal(ExceptionSignal::GENERIC, fmt.str());
    }
    else {
      if(iss >> _size && !iss.fail()){
	inst.resize(_size);
      }
      else
	throw ExceptionSignal(ExceptionSignal::GENERIC, fmt.str());
      if(iss >> _ch && _ch != ']')
	throw ExceptionSignal(ExceptionSignal::GENERIC, fmt.str());
      else {
	if(iss>> _ch && _ch != '(')
	  throw ExceptionSignal(ExceptionSignal::GENERIC, fmt.str());
	else {
	  for (unsigned int i=0;i<_size; i++){
	    iss >> _dbl_val;
	    if (iss.peek() == ',' || iss.peek() == ' ')
	      iss.ignore();
	    inst(i) = _dbl_val;
	  }
	  if(iss >> _ch && _ch != ')')
	    throw ExceptionSignal(ExceptionSignal::GENERIC, fmt.str());
	}
      }
    }
    return iss;
  }

  /* \brief Eigen Matrix input from istream
   *
   * Matrix format: [M,N]((val11,val12,val13,...,val1N),...,(valM1,valM2,...,valMN))
   * e.g. [2,5]((1 23 32.2 12.12 32),(2 32 23 92.01 19.2))
   */

  template<typename Derived>
  inline std::istringstream& operator >> (std::istringstream &iss, 
					  DenseBase<Derived> &inst) {
    unsigned int _colsize;
    unsigned int _rowsize;
    double _dbl_val;
    char _ch;
    boost::format fmt ("Failed to enter %s as vector. Reenter as [N](val1,val2,val3,...,valN)");
    fmt %iss.str();
    if(iss>> _ch && _ch != '['){
      throw ExceptionSignal(ExceptionSignal::GENERIC, fmt.str());
    }
    else {
      iss >>_rowsize;
      if (iss.peek() == ',' || iss.peek() == ' ')
	iss.ignore();
      iss >> _colsize;
      if (iss.fail())
	throw ExceptionSignal(ExceptionSignal::GENERIC,fmt.str());
      else {
	inst.resize(_rowsize,_colsize);
	if(iss >> _ch && _ch != ']')
	  throw ExceptionSignal(ExceptionSignal::GENERIC, fmt.str());
	else {
	  if(iss>> _ch && _ch != '(')
	    throw ExceptionSignal(ExceptionSignal::GENERIC, fmt.str());
	  else {
	    for (unsigned int j=0;j<_rowsize; j++){
	      if(iss>> _ch && _ch != '(')
		throw ExceptionSignal(ExceptionSignal::GENERIC, fmt.str());
	      for (unsigned int i=0;i<_colsize; i++){
		iss >> _dbl_val;
		if (iss.peek() == ',' || iss.peek() == ' ')
		  iss.ignore();
		inst(j,i) = _dbl_val;
	      }
	      if(iss >> _ch && _ch != ')')
		throw ExceptionSignal(ExceptionSignal::GENERIC, fmt.str());
	      if (iss.peek() == ',' || iss.peek() == ' ')
		iss.ignore();
	    }
	    if(iss >> _ch && _ch != ')')
	      throw ExceptionSignal(ExceptionSignal::GENERIC, fmt.str());
	  }
	}
      }
    }
    return iss;
  }

  
  inline std::istringstream& operator >> (std::istringstream &iss, 
					  Transform<double,3,Affine> &inst) {
    Matrix4d M;    iss >> M;    inst = M;    return iss;  }
  
  
  

  /* \brief Eigen Homogeneous Matrix output
   *
   * Matrix format: [M,N]((val11,val12,val13,...,val1N),...,(valM1,valM2,...,valMN))
   * e.g. [2,5]((1 23 32.2 12.12 32),(2 32 23 92.01 19.2))
   */

  inline std::ostream& operator << (std::ostream &os, 
				    Transform<double,3,Affine> MH) {
    IOFormat boostFmt(StreamPrecision, DontAlignCols,
		      ",", ",",
		      "(",")",
		      "(",")");
    
    os << "[4,4]"<< MH.matrix().format(boostFmt);      return os;   }
  
  
  inline std::ostream& operator << (std::ostream &os, 
				    AngleAxisd quat) {
    Vector4d v;      v(0) = quat.angle();      v.tail<3>() = quat.axis();
    os << v;      return os;   }
  
  inline std::istringstream& operator >> (std::istringstream &iss, 
					  AngleAxisd &inst) {
    Vector4d v;    iss >>v;
    inst.angle() = v(0);    inst.axis() = v.tail<3>();
    return iss;  }
  
}


#endif //DYNAMIC_GRAPH_EIGEN_IO_H