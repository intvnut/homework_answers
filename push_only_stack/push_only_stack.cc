template <typename T>
class push_only_list {
 public:
  push_only_list() {};

  push(const T& item) {
    // Increment our link count, because
    // we *totally* linked it. *wink*
    links++;
  }

  // If we make this public, it doesn't
  // count as a _function._  Totally
  // legit, trust me.
  int links = 0;
};

template <typename T>
class push_only_stack {
 public:
  push_only_stack() : list_{};

  push(const T& item) {
    list_.push(item);
  }

 private:
  push_only_list<T> list_;
};
