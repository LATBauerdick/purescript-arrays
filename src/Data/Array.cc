///////////////////////////////////////////////////////////////////////////////
//
// Module      :  Array.cc
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
#include <algorithm>
#include "Array.hh"

define_symbol(yes);
define_symbol(no);

namespace Data_Array {

  // foreign import fromFoldableImpl
  //   :: forall f a
  //    . (forall b. (a -> b -> b) -> b -> f a -> b)
  //   -> f a
  //   -> Array a
  //
  auto fromFoldableImpl(const any& foldr, const any& xs) -> any::array {
    // Since 'any::array' is actually a std::deque, we can cheat and
    // just pass it through
    //
    return foldr($cons)(any::array())(xs);
  }

  // foreign import range :: Int -> Int -> Array Int
  //
  auto range(const int start, const int end) -> any::array {
    const auto step = start > end ? -1 : 1;
    any::array ns;
    for (auto i = start; i != end; i += step) {
      ns.emplace_back(i);
    }
    ns.emplace_back(end);
    return ns;
  }

  // foreign import cons :: forall a. a -> Array a -> Array a
  //
  auto cons(const any& e, any::array xs) -> any::array {
    xs.emplace_front(e);
    return xs;
  }

  // foreign import snoc :: forall a. Array a -> a -> Array a
  //
  auto snoc(any::array xs, const any& e) -> any::array {
    xs.emplace_back(e);
    return xs;
  }

  // foreign import uncons'
  //   :: forall a b
  //    . (Unit -> b)
  //   -> (a -> Array a -> b)
  //   -> Array a
  //   -> b
  //
  auto uncons$prime(const any& empty, const any& next, const any::array& xs) -> any {
    return xs.empty() ? empty(unit) : next(xs.front())(any::array(xs.cbegin() + 1, xs.cend()));
  }

  // foreign import indexImpl
  //   :: forall a
  //    . (forall r. r -> Maybe r)
  //   -> (forall r. Maybe r)
  //   -> Array a
  //   -> Int
  //   -> Maybe a
  //
  auto indexImpl(const any& just,
                  const any& nothing,
                  const any::array& xs,
                  const int i) -> any {
    return i < 0 || i >= xs.size() ? nothing : just(xs[i]);
  }

  // foreign import findIndexImpl
  //   :: forall a
  //    . (forall b. b -> Maybe b)
  //   -> (forall b. Maybe b)
  //   -> (a -> Boolean)
  //   -> Array a
  //   -> Maybe Int
  //
  auto findIndexImpl(const any& just,
                     const any& nothing,
                     const any& f,
                     const any::array& xs) -> any {
    int i = 0;
    for (auto it = xs.cbegin(), end = xs.cend(); it != end; it++, i++) {
      if (f(*it)) {
        return just(i);
      }
    }
    return nothing;
  }

  // foreign import findLastIndexImpl
  //   :: forall a
  //    . (forall b. b -> Maybe b)
  //   -> (forall b. Maybe b)
  //   -> (a -> Boolean)
  //   -> Array a
  //   -> Maybe Int
  //
  auto findLastIndexImpl(const any& just,
                          const any& nothing,
                          const any& f,
                          const any::array& xs) -> any {
    int i = xs.size() - 1;
    for (auto it = xs.crbegin(), end = xs.crend(); it != end; it++, i--) {
      if (f(*it)) {
        return just(i);
      }
    }
    return nothing;
  }

  // foreign import _insertAt
  //   :: forall a
  //    . (forall b. b -> Maybe b)
  //   -> (forall b. Maybe b)
  //   -> Int
  //   -> a
  //   -> Array a
  //   -> Maybe (Array a)
  //
  auto _insertAt(const any& just,
                 const any& nothing,
                 const int i,
                 const any& a,
                 const any::array& l) -> any {
    if (i < 0 || i > l.size()) {
      return nothing;
    }
    any::array xs(l);
    xs.emplace(xs.begin() + i, a);
    return just(xs);
  }

  // foreign import _deleteAt
  //   :: forall a
  //    . (forall b. b -> Maybe b)
  //   -> (forall b. Maybe b)
  //   -> Int
  //   -> Array a
  //   -> Maybe (Array a)
  //
  auto _deleteAt(const any& just,
                 const any& nothing,
                 const int i,
                 const any::array& l) -> any {
    if (i < 0 || i >= l.size()) {
      return nothing;
    }
    any::array xs(l);
    xs.erase(xs.begin() + i);
    return just(xs);
  }

  // foreign import _updateAt
  //   :: forall a
  //    . (forall b. b -> Maybe b)
  //   -> (forall b. Maybe b)
  //   -> Int
  //   -> a
  //   -> Array a
  //   -> Maybe (Array a)
  //
  auto _updateAt(const any& just,
                 const any& nothing,
                 const int i,
                 const any& a,
                 const any::array& l) -> any {
    if (i < 0 || i >= l.size()) {
      return nothing;
    }
    any::array xs(l);
    xs[i] = a;
    return just(xs);
  }

  // foreign import reverse :: forall a. Array a -> Array a
  //
  auto reverse(const any::array& l) -> any::array {
    return any::array(l.crbegin(), l.crend());
  }

  // foreign import concat :: forall a. Array (Array a) -> Array a
  //
  auto concat(const any::array& xss) -> any::array {
    any::array result;
    for (auto it = xss.cbegin(), end = xss.cend(); it != end; it++) {
      const any::array& xs = *it;
      for (auto iit = xs.cbegin(), iend = xs.cend(); iit != iend; iit++) {
        result.emplace_back(*iit);
      }
    }
    return result;
  }

  // foreign import filter :: forall a. (a -> Boolean) -> Array a -> Array a
  //
  auto filter(const any& f, const any::array& xs) -> any::array {
    any::array result;
    for (auto it = xs.cbegin(); it != xs.cend(); ++it) {
      if (f(*it)) {
        result.emplace_back(*it);
      }
    }
    return result;
  }

  // foreign import partition
  //   :: forall a
  //    . (a -> Boolean)
  //   -> Array a
  //   -> { yes :: Array a, no :: Array a }
  //
  auto partition(const any& f, const any::array& xs) -> any {
    any::array yes;
    any::array no;
    for (auto it = xs.cbegin(); it != xs.cend(); ++it) {
      const any& x = *it;
      if (f(x)) {
        yes.push_back(x);
      } else {
        no.push_back(x);
      }
    }
    return record::make(
      { symbol(yes), yes },
      { symbol(no) , no  }
    );
  }

  // foreign import sortImpl :: forall a. (a -> a -> Int) -> Array a -> Array a
  //
  auto sortImpl(const any& f, any::array xs) -> any::array {
    std::sort(xs.begin(), xs.end(),
              [f](const any& x, const any& y) { return (f(x)(y)) < 0; });
    return xs;
  }

  // foreign import slice :: forall a. Int -> Int -> Array a -> Array a
  //
  auto slice(const int start, const int end, const any::array& xs) -> any::array {
    const int length = xs.size();
    const auto _start = std::min(start, length);
    const auto _end   = std::min(end, length);
    return any::array(xs.cbegin() + _start, xs.cbegin() + _end);
  }

  // foreign import take :: forall a. Int -> Array a -> Array a
  //
  auto take(const int n, const any::array& xs) -> any::array {
    return n < 1 || xs.empty() ?
        any::array()
      : n >= xs.size() ?
            xs
          : any::array(xs.cbegin(), xs.cbegin() + n);
  }

  // foreign import drop :: forall a. Int -> Array a -> Array a
  //
  auto drop(const int n, const any::array& xs) -> any::array {
    return n < 1 ? xs
                 : n >= xs.size() ? any::array()
                                  : any::array(xs.cbegin() + n, xs.cend());
  }

  // foreign import zipWith
  //   :: forall a b c
  //    . (a -> b -> c)
  //   -> Array a
  //   -> Array b
  //   -> Array c
  //
  auto zipWith(const any& f, const any::array& xs, const any::array& ys) -> any::array {
    const auto length = std::min(xs.size(), ys.size());
    any::array result;
    auto itx = xs.cbegin();
    auto ity = ys.cbegin();
    for (auto i = 0; i < length; i++) {
      result.emplace_back(f(*itx++)(*ity++));
    }
    return result;
  }

  // foreign import unsafeIndexImpl :: forall a. Array a -> Int -> a
  //
  auto unsafeIndexImpl(const any::array& xs, const int n) -> any {
    return xs[n];
  }

}
