template <typename T>
class SplayTree {
  // Omitted funtions and structs are as same as BST, except splay operation.
  void rotate(size_t n) {
    const size_t p = this->mem[n].pn, q = this->mem[p].pn;
    bool lr = this->mem[p].ch[1] == n;
    if ((this->mem[p].ch[lr] = this->mem[n].ch[!lr]))
      this->mem[this->mem[n].ch[!lr]].pn = p;
    if ((this->mem[n].pn = q))
      this->mem[q].ch[this->mem[q].ch[1] == p] = n;
    this->mem[n].ch[!lr] = p; this->mem[p].pn = n;
    this->push_up(p);  // Modify 1: this->push_up(n) is not necessary.
  }
 public:  // begin and end are omitted.
  SplayTree(): root_node{0}, mem{{0, 0, {0, 0}, {}}} {
    // Modify 2: prepare 2 minimum and maximum value to hold the range.
    this->mem.push_back({2, 0, {0, 2}, -numeric_limits<T>::max()});
    this->mem.push_back({1, 1, {0, 0}, numeric_limits<T>::max()});
  };
  void remove(Iterator n) {
    // Modify 3: simplified version of remove for splay tree.
    Iterator p = n--; ++p;
    this->splay(n);
    this->splay(p, n);
    this->rec.push_back(p.lc().idx);
    this->mem[p.idx].ch[0] = 0;
    --this->mem[p.idx].size;
    --this->mem[n.idx].size;
  }
  void remove(Iterator s, Iterator t) {
    // Modify 4: add support to range remove.
    this->splay(--s);
    this->splay(t, s);
    // TODO: Recursively push_back subtree to recycle bin `this->rec'
    this->mem[s.idx].ch[0] = 0;
    this->mem[s.idx].size -= this->mem[this->mem[s.idx].ch[0]].size;
    this->mem[t.idx].size -= this->mem[this->mem[s.idx].ch[0]].size;
  }
  void splay(Iterator n, Iterator goal = {0, 0}) {
    for (size_t p; (p = this->mem[n.idx].pn) != goal.idx; ) {
      bool lr = this->mem[this->mem[p].pn].ch[1] == p;
      if (this->mem[p].pn == goal.idx) this->rotate(n.idx);
      else if (this->mem[p].ch[!lr] == n.idx)
        this->rotate(n.idx), this->rotate(n.idx);
      else this->rotate(p), this->rotate(n.idx);
    }
    if (goal.idx == 0) this->root_node = n.idx;
    this->push_up(n.idx);  // Originally push-up'd in rotate.
  }
};
