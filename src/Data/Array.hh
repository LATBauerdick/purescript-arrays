///////////////////////////////////////////////////////////////////////////////
//
// Module      :  Array.hh
// Copyright   :  (c) Andy Arvanitis 2016
// License     :  MIT
//
// Maintainer  :  Andy Arvanitis <andy.arvanitis@gmail.com>
// Stability   :  experimental
// Portability :
//
// Array FFI functions
//
///////////////////////////////////////////////////////////////////////////////
//
#ifndef Data_Array_FFI_HH
#define Data_Array_FFI_HH

#include "PureScript/PureScript.hh"

namespace Data_Array {
  using namespace PureScript;

  // foreign import fromFoldableImpl
  //   :: forall f a
  //    . (forall b. (a -> b -> b) -> b -> f a -> b)
  //   -> f a
  //   -> Array a
  //
  auto fromFoldableImpl(const any&, const any&) -> any::array;

  // foreign import range :: Int -> Int -> Array Int
  //
  auto range(const int, const int) -> any::array;

  // foreign import replicate :: forall a. Int -> a -> Array a
  //
  inline auto replicate(const int count, const any& value) -> any::array {
    return any::array(count < 0 ? 0 : count, value);
  }

  // foreign import length :: forall a. Array a -> Int
  //
  inline auto length(const any::array& xs) -> int {
    const auto len = xs.size();
    assert(len <= std::numeric_limits<int>::max());
    return static_cast<int>(xs.size());
  }

  // foreign import cons :: forall a. a -> Array a -> Array a
  //
  auto cons(const any&, const any::array&) -> any::array;

  // foreign import snoc :: forall a. Array a -> a -> Array a
  //
  auto snoc(const any::array&, const any&) -> any::array;

  // foreign import uncons'
  //   :: forall a b
  //    . (Unit -> b)
  //   -> (a -> Array a -> b)
  //   -> Array a
  //   -> b
  //
  auto uncons$prime(const any&, const any&, const any::array&) -> any;

  // foreign import indexImpl
  //   :: forall a
  //    . (forall r. r -> Maybe r)
  //   -> (forall r. Maybe r)
  //   -> Array a
  //   -> Int
  //   -> Maybe a
  //
  auto indexImpl(const any&, const any&, const any::array&, const int) -> any;

  // foreign import findIndexImpl
  //   :: forall a
  //    . (forall b. b -> Maybe b)
  //   -> (forall b. Maybe b)
  //   -> (a -> Boolean)
  //   -> Array a
  //   -> Maybe Int
  //
  auto findIndexImpl(const any&, const any&, const any&, const any::array&) -> any;

  // foreign import findLastIndexImpl
  //   :: forall a
  //    . (forall b. b -> Maybe b)
  //   -> (forall b. Maybe b)
  //   -> (a -> Boolean)
  //   -> Array a
  //   -> Maybe Int
  //
  auto findLastIndexImpl(const any&, const any&, const any&, const any::array&) -> any;

  // foreign import _insertAt
  //   :: forall a
  //    . (forall b. b -> Maybe b)
  //   -> (forall b. Maybe b)
  //   -> Int
  //   -> a
  //   -> Array a
  //   -> Maybe (Array a)
  //
  auto _insertAt(const any&, const any&, const int, const any&, const any::array&) -> any;

  // foreign import _deleteAt
  //   :: forall a
  //    . (forall b. b -> Maybe b)
  //   -> (forall b. Maybe b)
  //   -> Int
  //   -> Array a
  //   -> Maybe (Array a)
  //
  auto _deleteAt(const any&, const any&, const int, const any::array&) -> any;

  // foreign import _updateAt
  //   :: forall a
  //    . (forall b. b -> Maybe b)
  //   -> (forall b. Maybe b)
  //   -> Int
  //   -> a
  //   -> Array a
  //   -> Maybe (Array a)
  //
  auto _updateAt(const any&, const any&, const int, const any&, const any::array&) -> any;

  // foreign import reverse :: forall a. Array a -> Array a
  //
  auto reverse(const any::array&) -> any::array;

  // foreign import concat :: forall a. Array (Array a) -> Array a
  //
  auto concat(const any::array&) -> any::array;

  // foreign import filter :: forall a. (a -> Boolean) -> Array a -> Array a
  //
  auto filter(const any&, const any::array&) -> any::array;

  // foreign import partition
  //   :: forall a
  //    . (a -> Boolean)
  //   -> Array a
  //   -> { yes :: Array a, no :: Array a }
  //
  auto partition(const any&, const any::array&) -> any;

  // foreign import sortImpl :: forall a. (a -> a -> Int) -> Array a -> Array a
  //
  auto sortImpl(const any&, const any::array&) -> any::array;

  // foreign import slice :: forall a. Int -> Int -> Array a -> Array a
  //
  auto slice(const int, const int, const any::array&) -> any::array;

  // foreign import take :: forall a. Int -> Array a -> Array a
  //
  auto take(const int, const any::array&) -> any::array;

  // foreign import drop :: forall a. Int -> Array a -> Array a
  //
  auto drop(const int, const any::array&) -> any::array;

  // foreign import zipWith
  //   :: forall a b c
  //    . (a -> b -> c)
  //   -> Array a
  //   -> Array b
  //   -> Array c
  //
  auto zipWith(const any&, const any::array&, const any::array&) -> any::array;

  // foreign import unsafeIndexImpl :: forall a. Array a -> Int -> a
  //
  auto unsafeIndexImpl(const any::array&, const int) -> any;

}

#endif // Data_Array_FFI_HH
