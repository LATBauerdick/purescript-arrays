///////////////////////////////////////////////////////////////////////////////
//
// Module      :  ST.cc
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
#include <limits>
#include "ST.hh"

define_symbol(value);
define_symbol(index);

namespace Data_Array_ST {
  using namespace PureScript;

  // foreign import copyImpl :: forall a b h r. a -> Eff (st :: ST h | r) b
  //
  auto copyImpl(const any& xs) -> any {
    return [=]() -> any {
      return any::array(cast<any::array>(xs));
    };
  }

  // foreign import peekSTArrayImpl
  //   :: forall a h e r
  //    . (a -> r)
  //   -> r
  //   -> (STArray h a)
  //   -> Int
  //   -> (Eff (st :: ST h | e) r)
  //
  auto peekSTArrayImpl(const any& just,
                       const any& nothing,
                       const any& xs_,
                       const int i) -> any {
    return [=]() -> any {
      const any::array& xs = xs_;
      return i >= 0 && i < xs.size() ? just(xs[i]) : nothing;
    };
  }

  // foreign import pokeSTArray
  //   :: forall a h r
  //    . STArray h a -> Int -> a -> Eff (st :: ST h | r) Boolean
  //
  auto pokeSTArray(const any& xs_, const int i, const any& a) -> any {
    return [=]() -> any {
      auto& xs = const_cast<any::array&>(cast<any::array>(xs_));
      const bool ret = i >= 0 && i < xs.size();
      if (ret) {
        xs[i] = a;
      }
      return ret;
    };
  }

  // foreign import pushAllSTArray
  //   :: forall a h r
  //    . STArray h a
  //   -> Array a
  //   -> Eff (st :: ST h | r) Int
  //
  auto pushAllSTArray(const any& xs_, const any::array& as) -> any {
    return [=]() -> any {
      auto& xs = const_cast<any::array&>(cast<any::array>(xs_));
      xs.insert(xs.end(), as.cbegin(), as.cend());
      assert(xs.size() <= std::numeric_limits<int>::max());
      return static_cast<int>(xs.size());
    };
  }

  // foreign import spliceSTArray
  //   :: forall a h r
  //    . STArray h a
  //   -> Int
  //   -> Int
  //   -> Array a
  //   -> Eff (st :: ST h | r) (Array a)
  //
  auto spliceSTArray(const any& xs_,
                     const int i,
                     const int howMany,
                     const any::array& bs) -> any {
    return [=]() -> any {
      auto& xs = const_cast<any::array&>(cast<any::array>(xs_));
      if (i < 0) {
        throw runtime_error("Negative index not supported yet");
      }
      any::array removed;
      if (howMany > 0) {
        removed.insert(removed.begin(), xs.begin() + i, xs.begin() + i + howMany);
        xs.erase(xs.begin() + i, xs.begin() + i + howMany);
      }
      xs.insert(xs.begin() + i, bs.cbegin(), bs.cend());
      return removed;
    };
  }

  // foreign import toAssocArray
  //   :: forall a h r
  //    . STArray h a
  //   -> Eff (st :: ST h | r) (Array (Assoc a))
  //
  auto toAssocArray(const any& xs_) -> any {
    return [=]() -> any {
      const any::array& xs = xs_;
      const auto n = xs.size();
      any::array as;
      for (int i = 0; i < n; i++) {
        as.emplace_back(
          record::make(
            { symbol(value), xs[i] },
            { symbol(index), i }
          )
        );
      }
      return as;
    };
  }

}
