// -*- C++ -*-
// $Id: qdp_outersubtype.h,v 1.3 2005-01-20 03:02:38 edwards Exp $

/*! \file
 * \brief Outer grid classes after a subset
 */


QDP_BEGIN_NAMESPACE(QDP);

//! OScalar class narrowed to a subset
/*! 
 * Only used for lvalues
 */
template<class T, class Sub>
class OSubScalar: public QDPSubType<T, OScalar<T>, Sub>
{
  typedef OScalar<T> C;

public:
  OSubScalar(const OScalar<T>& a, const Sub& ss): F(a), s(ss) {}
  OSubScalar(const OSubScalar& a): F(a.F), s(a.s) {}
  ~OSubScalar() {}

  //---------------------------------------------------------
  // Operators
  // NOTE: all assignment-like operators except operator= are
  // inherited from QDPSubType

  inline
  void operator=(const typename WordType<T>::Type_t& rhs)
    {
      this->assign(rhs);
    }

  inline
  void operator=(const Zero& rhs)
    {
      this->assign(rhs);
    }

  template<class T1,class C1>
  inline
  void operator=(const QDPType<T1,C1>& rhs)
    {
      this->assign(rhs);
    }

  template<class T1,class C1>
  inline
  void operator=(const QDPExpr<T1,C1>& rhs)
    {
      this->assign(rhs);
    }


  inline
  void operator=(const OSubScalar& rhs)
    {
      this->assign(rhs);
    }


private:
  // Hide default constructor
  OSubScalar() {}

public:
  C& field() {return const_cast<C&>(F);}
  const Sub& subset() const {return s;}

private:
  const C&  F;
  const Sub& s;
};



//-------------------------------------------------------------------------------------
//! OLattice class narrowed to a subset
/*! 
 * Only used for lvalues
 */
template<class T, class Sub> 
class OSubLattice: public QDPSubType<T, OLattice<T>, Sub>
{
  typedef OLattice<T> C;

public:
  OSubLattice(const OLattice<T>& a, const Sub& ss): F(a), s(ss) {}
  OSubLattice(const OSubLattice& a): F(a.F), s(a.s) {}
  ~OSubLattice() {}

  //---------------------------------------------------------
  // Operators
  // NOTE: all assignment-like operators except operator= are
  // inherited from QDPType

  inline
  void operator=(const typename WordType<T>::Type_t& rhs)
    {
      this->assign(rhs);
    }

  inline
  void operator=(const Zero& rhs)
    {
      this->assign(rhs);
    }

  template<class T1,class C1>
  inline
  void operator=(const QDPType<T1,C1>& rhs)
    {
      this->assign(rhs);
    }

  template<class T1,class C1>
  inline
  void operator=(const QDPExpr<T1,C1>& rhs)
    {
      this->assign(rhs);
    }


  inline
  void operator=(const OSubLattice& rhs)
    {
      this->assign(rhs);
    }


private:
  // Hide default constructor
  OSubLattice() {}

public:
  C& field() {return const_cast<C&>(F);}
  const Sub& subset() const {return s;}

private:
  const C&  F;
  const Sub& s;
};


//-----------------------------------------------------------------------------
// Traits class for returning the subset-ted class name of a outer grid class
//-----------------------------------------------------------------------------

template<class T>
struct QDPSubTypeTrait<OScalar<T>, Subset> 
{
  typedef OSubScalar<T,Subset>  Type_t;
};

template<class T>
struct QDPSubTypeTrait<OScalar<T>, UnorderedSubset> 
{
  typedef OSubScalar<T,UnorderedSubset>  Type_t;
};

template<class T>
struct QDPSubTypeTrait<OScalar<T>, OrderedSubset> 
{
  typedef OSubScalar<T,OrderedSubset>  Type_t;
};

template<class T>
struct QDPSubTypeTrait<OLattice<T>, Subset> 
{
  typedef OSubLattice<T,Subset>  Type_t;
};

template<class T>
struct QDPSubTypeTrait<OLattice<T>, UnorderedSubset> 
{
  typedef OSubLattice<T,UnorderedSubset>  Type_t;
};

template<class T>
struct QDPSubTypeTrait<OLattice<T>, OrderedSubset> 
{
  typedef OSubLattice<T,OrderedSubset>  Type_t;
};


//-----------------------------------------------------------------------------
// Traits classes to support operations of simple scalars (floating constants, 
// etc.) on QDPTypes
//-----------------------------------------------------------------------------

template<class T>
struct WordType<OSubScalar<T,Subset> > 
{
  typedef typename WordType<T>::Type_t  Type_t;
};

template<class T>
struct WordType<OSubScalar<T,UnorderedSubset> > 
{
  typedef typename WordType<T>::Type_t  Type_t;
};

template<class T>
struct WordType<OSubScalar<T,OrderedSubset> > 
{
  typedef typename WordType<T>::Type_t  Type_t;
};

template<class T>
struct WordType<OSubLattice<T,Subset> > 
{
  typedef typename WordType<T>::Type_t  Type_t;
};

template<class T>
struct WordType<OSubLattice<T,UnorderedSubset> > 
{
  typedef typename WordType<T>::Type_t  Type_t;
};

template<class T>
struct WordType<OSubLattice<T,OrderedSubset> > 
{
  typedef typename WordType<T>::Type_t  Type_t;
};


//-----------------------------------------------------------------------------
// Scalar Operations
//-----------------------------------------------------------------------------

//! dest = 0
template<class T> 
void zero_rep(OScalar<T>& dest, const Subset& s) 
{
  zero_rep(dest.field().elem());
}

//! dest = 0
template<class T, class S>
void zero_rep(OSubScalar<T,S> dest) 
{
  zero_rep(dest.field().elem());
}

//! dest = (mask) ? s1 : dest
template<class T1, class T2, class S> 
void copymask(OSubScalar<T2,S> dest, const OScalar<T1>& mask, 
	      const OScalar<T2>& s1) 
{
  copymask(dest.field().elem(), mask.elem(), s1.elem());
}


//-----------------------------------------------------------------------------
// Random numbers
//! dest  = random  
/*! Implementation is in the specific files */
template<class T>
void random(OSubScalar<T,UnorderedSubset> d);

//! dest  = random  
/*! Implementation is in the specific files */
template<class T>
void random(OSubScalar<T,OrderedSubset> d);


//! dest  = gaussian
template<class T, class S>
void gaussian(OSubScalar<T,S> dd)
{
  OLattice<T>& d = dd.field();
  const S& s = dd.subset();

  OScalar<T>  r1, r2;

  random(r1(s));
  random(r2(s));

  fill_gaussian(d.elem(), r1.elem(), r2.elem());
}


QDP_END_NAMESPACE();
