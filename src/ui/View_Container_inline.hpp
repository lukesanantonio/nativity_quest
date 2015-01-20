/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
namespace game { namespace ui
{
  template <class T>
  void View_Container<T>::push_child(Shared_View v, T l) noexcept
  {
    children_.push_back({v, l});
  }
  template <class T>
  void View_Container<T>::push_child(child_t c) noexcept
  {
    children_.push_back(std::move(c));
  }
  template <class T>
  void View_Container<T>::insert_child(typename child_vec_t::iterator i,
                                       Shared_View v,
                                       T l) noexcept
  {
    children_.insert(i, {v, l});
  }
  template <class T>
  void View_Container<T>::insert_child(typename child_vec_t::const_iterator i,
                                       Shared_View v,
                                       T l) noexcept
  {
    children_.insert(i, {v, l});
  }
  template <class T>
  void View_Container<T>::insert_child(typename child_vec_t::iterator i,
                                       child_t c) noexcept
  {
    children_.insert(i, std::move(c));
  }
  template <class T>
  void View_Container<T>::insert_child(typename child_vec_t::const_iterator i,
                                       child_t c) noexcept
  {
    children_.insert(i, std::move(c));
  }

  template <class T>
  void View_Container<T>::remove_child(Shared_View v) noexcept
  {
    using std::begin; using std::end;
    auto find_iter = std::find_if(begin(children_), end(children_),
    [&](auto const& c)
    {
      return c.view == v;
    });
    if(find_iter != end(children_))
    {
      children_.erase(find_iter);
    }
  }
  template <class T>
  void View_Container<T>::remove_child(typename child_vec_t::size_type
                                       index) noexcept
  {
    children_.erase(children_.begin() + index);
  }
  template <class T>
  void View_Container<T>::remove_child(typename child_vec_t::iterator
                                       iter) noexcept
  {
    children_.erase(iter);
  }
  template <class T>
  void View_Container<T>::remove_child(typename child_vec_t::const_iterator
                                       i) noexcept
  {
    children_.erase(i);
  }
} }
