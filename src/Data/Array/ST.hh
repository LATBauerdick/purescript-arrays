///////////////////////////////////////////////////////////////////////////////
//
// Module      :  ST.hh
// Copyright   :  (c) Andy Arvanitis 2016
// License     :  MIT
//
// Maintainer  :  Andy Arvanitis <andy.arvanitis@gmail.com>
// Stability   :  experimental
// Portability :
//
// Array ST FFI functions
//
///////////////////////////////////////////////////////////////////////////////
//
#ifndef Data_Array_ST_FFI_HH
#define Data_Array_ST_FFI_HH

#include "PureScript/PureScript.hh"

namespace Data_Array_ST {
  using namespace PureScript;

  // foreign import runSTArray
  //   :: forall a r
  //    . (forall h. Eff (st :: ST h | r) (STArray h a))
  //   -> Eff r (Array a)
  //
  inline auto runSTArray(const any& f) -> any {
    return f;
  }

  // foreign import emptySTArray :: forall a h r. Eff (st :: ST h | r) (STArray h a)
  //
  inline auto emptySTArray() -> any {
    return any::array();
  }

  // foreign import copyImpl :: forall a b h r. a -> Eff (st :: ST h | r) b
  //
  auto copyImpl(const any&) -> any;

  // foreign import peekSTArrayImpl
  //   :: forall a h e r
  //    . (a -> r)
  //   -> r
  //   -> (STArray h a)
  //   -> Int
  //   -> (Eff (st :: ST h | e) r)
  //
  auto peekSTArrayImpl(const any&, const any&, const any&, const int) -> any;

  // foreign import pokeSTArray
  //   :: forall a h r
  //    . STArray h a -> Int -> a -> Eff (st :: ST h | r) Boolean
  //
  auto pokeSTArray(const any&, const int, const any&) -> any;

  // foreign import pushAllSTArray
  //   :: forall a h r
  //    . STArray h a
  //   -> Array a
  //   -> Eff (st :: ST h | r) Int
  //
  auto pushAllSTArray(const any&, const any::array&) -> any;

  // foreign import spliceSTArray
  //   :: forall a h r
  //    . STArray h a
  //   -> Int
  //   -> Int
  //   -> Array a
  //   -> Eff (st :: ST h | r) (Array a)
  //
  auto spliceSTArray(const any&, const int, const int, const any::array&) -> any;

  // foreign import toAssocArray
  //   :: forall a h r
  //    . STArray h a
  //   -> Eff (st :: ST h | r) (Array (Assoc a))
  //
  auto toAssocArray(const any&) -> any;

}


#endif // Data_Array_ST_FFI_HH
