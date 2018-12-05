constexpr int maxv = 256;           // 最大输入节点数
vector<size_t> adj[maxv];           // 原图邻接表
size_t dfn[maxv], low[maxv];        // Tarjan 算法: 搜索编号
stack<size_t, vector<size_t>> stk;  // Tarjan 算法: 搜索栈
size_t dfn_count, scc_count;        // Tarjan 算法: 全局计数器
vector<size_t> scc[maxv];           // Tarjan 算法输出: 连通块中节点编号
size_t belong[maxv];                // Tarjan 算法输出: 节点所属连通块编号
vector<size_t> tree[maxv];          // 缩点后重构的森林的邻接表
size_t loop[maxv];                  // 缩点后每个节点的自环数量*2
// Clear dfn_count, scc_count, dfn, loop, adj, scc, tree for each test case.

void tarjan(size_t u, size_t p) {
  dfn[u] = low[u] = ++dfn_count; stk.push(u);
  for (size_t v: adj[u]) if (v != p)
    if (dfn[v]) low[u] = min(low[u], dfn[v]);
    else tarjan(v, u), low[u] = min(low[u], low[v]);
  if (dfn[u] == low[u]) {
    for (size_t v; (v = stk.top()) != u; stk.pop())
      scc[scc_count].push_back(v), belong[v] = scc_count;
    scc[belong[u] = scc_count++].push_back(u), stk.pop();
  }
}

int sg(size_t node, size_t parent) {
  size_t xor_sum = 0;
  for (size_t child: tree[node])
    if (child != parent) xor_sum ^= sg(child, node) + 1;
  return xor_sum ^ loop[node] >> 1 & 1;
}

void rebuild() {
  for (size_t i = 0; i != v; ++i)
    for (size_t j: adj[i])
      if (belong[i] == belong[j]) ++loop[belong[i]];
      else tree[belong[i]].push_back(belong[j]);
}
