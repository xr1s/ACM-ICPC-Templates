template <
  typename Key, typename Mapped,
  typename Hash_Fn, typename Eq_Fn, typename Comb_Hash_Fn,
  typename Resize_Policy, bool Store_Hash,
  typename _Alloc
> class cc_hash_table;

template <
  typename Key, typename Mapped,
  typename Hash_Fn, typename Eq_Fn, typename Comb_Probe_Fn,
  typename Probe_Fn, typename Resize_Policy, bool Store_Hash,
  typename _Alloc
> class gp_hash_table;
