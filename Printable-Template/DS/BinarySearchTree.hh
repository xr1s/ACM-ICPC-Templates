template <typename T>
class BinarySearchTree {
  struct TreeNode { size_t size, pn, ch[2]; T value; };
  struct Iterator {
    Iterator(BinarySearchTree *ptr = 0, size_t idx = 0): ptr{ptr}, idx{idx} { }
    Iterator lc() const { return {this->ptr, this->ptr->mem[this->idx].ch[0]}; }
    Iterator rc() const { return {this->ptr, this->ptr->mem[this->idx].ch[1]}; }
    Iterator pn() const { return {this->ptr, this->ptr->mem[this->idx].pn}; }
    const T &operator*() const { return this->ptr->mem[this->idx].value; }
    Iterator &operator++() {  // ++end will cause endless loop!
      size_t p = exchange(this->idx, this->ptr->mem[this->idx].ch[1]);
      if (this->idx == 0)
        for (swap(this->idx, p); this->ptr->mem[this->idx].ch[1] == p; )
          p = exchange(this->idx, this->ptr->mem[this->idx].pn);
      else while ((p = this->ptr->mem[this->idx].ch[0])) this->idx = p;
      return *this;
    }
    Iterator &operator--() {  // --end will cause endless loop!
      size_t p = exchange(this->idx, this->ptr->mem[this->idx].ch[0]);
      if (this->idx == 0)
        for (swap(this->idx, p); this->ptr->mem[this->idx].ch[0] == p; )
          p = exchange(this->idx, this->ptr->mem[this->idx].pn);
      else while ((p = this->ptr->mem[this->idx].ch[0])) this->idx = p;
      return *this;
    }
    friend bool operator==(const Iterator &lhs, const Iterator &rhs) {
      return lhs.ptr == rhs.ptr && lhs.idx == rhs.idx;
    }
    friend bool operator!=(const Iterator &lhs, const Iterator &rhs) {
      return lhs.ptr != rhs.ptr || lhs.idx != rhs.idx;
    }
   private:
    friend class BinarySearchTree;
    BinarySearchTree *ptr; size_t idx;
  };
  size_t root_node; vector<TreeNode> mem; vector<size_t> rec;
  void push_up(size_t n) {
    this->mem[n].size = 1 + \
        this->mem[this->mem[n].ch[0]].size + this->mem[this->mem[n].ch[1]].size;
  }
  void rotate(size_t n) {
    const size_t p = this->mem[n].pn, q = this->mem[p].pn;
    bool lr = this->mem[p].ch[1] == n;
    if ((this->mem[p].ch[lr] = this->mem[n].ch[!lr]))
      this->mem[this->mem[n].ch[!lr]].pn = p;
    if ((this->mem[n].pn = q))
      this->mem[q].ch[this->mem[q].ch[1] == p] = n;
    this->mem[n].ch[!lr] = p; this->mem[p].pn = n;
    this->push_up(p); this->push_up(n);
  }
 public:  // begin and end are omitted.
  using iterator = Iterator;
  BinarySearchTree(): root_node{0}, mem{{0, 0, {0, 0}, {}}} { };
  Iterator insert(const T &value) {
    size_t node = this->root_node, idx = this->mem.size(); bool lr;
    while (this->mem[node].ch[lr = this->mem[node].value < value])
      ++this->mem[node = this->mem[node].ch[lr]].size;
    if (!rec.empty()) {
      this->mem[idx = this->rec.back()] = {1, node, {0, 0}, value};
      this->rec.pop_back();
    } else this->mem.push_back({1, node, {0, 0}, value});
    if (this->root_node) {
      this->mem[node].ch[lr] = idx;
      ++this->mem[this->root_node].size;
    } else this->root_node = idx;
    return {this, idx};
  }
  void remove(Iterator n) {
    Iterator p = n; size_t q = n.idx;
    if (this->mem[n.idx].ch[1] != 0) {
      q = this->mem[(++p).idx].pn;
      if ((this->mem[q].ch[this->mem[q].ch[1] == p.idx] = p.rc().idx))
        this->mem[p.rc().idx].pn = q;
      memcpy(&this->mem[p.idx].ch, &this->mem[n.idx].ch, sizeof(size_t) * 2);
    } else p = n.lc(), this->mem[n.idx].ch[0] = this->mem[p.idx].ch[0];
    for (; q != 0; q = this->mem[q].pn) --this->mem[q].size;
    if (p.idx) memcpy(&this->mem[p.idx], &this->mem[n.idx], sizeof(size_t) * 2);
    if (n.pn().idx) this->mem[n.pn().idx].ch[n.pn().rc() == n] = p.idx;
    if (this->mem[n.idx].ch[0]) this->mem[this->mem[n.idx].ch[0]].pn = p.idx;
    if (this->mem[n.idx].ch[1]) this->mem[this->mem[n.idx].ch[1]].pn = p.idx;
    if (this->root_node == n.idx) this->root_node = p.idx;
    this->rec.push_back(n.idx);
  }
  const T &operator[](size_t k) { // caller ensures k >= 0 && k < tree.size().
    size_t n = this->root_node;
    while (k != this->mem[this->mem[n].ch[0]].size)
      if (k < this->mem[this->mem[n].ch[0]].size) n = this->mem[n].ch[0];
      else k -= this->mem[this->mem[n].ch[0]].size + 1, n = this->mem[n].ch[1];
    return this->mem[n].value;
  }
  Iterator find(const T &value) {
    size_t n = this->root_node, p = 0;
    for (; n != 0; n = this->mem[n].ch[this->mem[n].value < value])
      if (!(value < this->mem[n].value) && !(this->mem[n].value < value)) p = n;
    return {this, p};
  }
  Iterator lower_bound(const T &value) {
    size_t n = this->root_node, p = 0, q;
    for (; n != 0; n = this->mem[n].ch[this->mem[n].value < value])
      if (!(this->mem[n].value < value)) p = n;
    return {this, p};
  }
  Iterator upper_bound(const T &value) {
    size_t n = this->root_node, p = 0;
    for (; n != 0; n = this->mem[n].ch[!(value < this->mem[n].value)])
      if (value < this->mem[n].value) p = n;
    return {this, p};
  }
  size_t rank(const T &value) {  // if value was inserted, the rank of value.
    size_t n = this->root_node, r = 0;
    while (n != 0)
      if (!(this->mem[n].value < value)) n = this->mem[n].ch[0];
      else r += this->mem[this->mem[n].ch[0]].size + 1, n = this->mem[n].ch[1];
    return r;
  }
};
