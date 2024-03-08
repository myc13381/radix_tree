参考资料 https://github.com/ytakano/radix_tree

1. radix_tree_node

   核心数据结构

   ```
   private:
   	std::map<K, radix_tree_node<K, T, Compare>*, Compare> m_children; // 每个节点维护一个map
       radix_tree_node<K, T, Compare> *m_parent; // 父节点
       value_type *m_value; // pair<cosnt K, T> 类型的指针
       int m_depth; // 深度
       bool m_is_leaf; // 是不是叶子节点
       K m_key; // K 键
       Compare& m_pred; // map的比较函数
   ```

   

   每一个节点记录了键值，键值对的指针，父节点，深度（对应前缀的长度）等。

2. radix_tree_it

   核心数据结构

   ```
   private:
       radix_tree_node<K, T, Compare> *m_pointee; // 指向节点的指针，核心成员变量
   ```

   

   内部维护一个`radix_tree_node *`，这是一个前向迭代器，++iterator的做法是访问自己这个node的`m_children`，如果自己没有孩子，则访问自己的兄弟节点，如果还没有则访问叔叔节点......重写`oprerator*`和`operator->`，分别返回`radix_tree_node::value_type ->seconde`和维护的节点自身。

3. radix_tree

   核心数据结构

   ```
   private:
       size_type m_size; // 节点的个数
       radix_tree_node<K, T, Compare>* m_root; // 根节点
   	Compare m_predicate; // 比较函数
   ```

   

   基数树存储值的规则是：如果到一个节点，组成的key是一个完整的key，也就是对应有value了，那么**它会创建一个孩子，这个孩子节点的键是空(nil)**，并将此孩子节点设置为叶子节点`m_is_leaf = true`，同时将此孩子的节点的`m_value`指向对应的键值对。

   ```
   template <typename K, typename T, typename Compare>
   radix_tree_node<K, T, Compare>* radix_tree<K, T, Compare>::find_node(const K &key, radix_tree_node<K, T, Compare> *node, int depth);
   ```

   

   查询最接近的节点，输入一个key，在基数树中查找一个和key最匹配的节点，也就是说也可能正好找到key，也可能找不到，但是返回一个最接近的，看看一个例子：

   ```
   /*
   (root)
   |
   |---------------
   |       |      |
   abcde   bcdef  c
   |   |   |      |------
   |   |   $3     |  |  |
   f   ge         d  e  $6
   |   |          |  |
   $1  $2         $4 $5
   
   find_node():
     bcdef  -> $3
     bcdefa -> bcdef
     c      -> $6
     cf     -> c
     abch   -> abcde
     abc    -> abcde
     abcde  -> abcde
     abcdef -> $1
     abcdeh -> abcde
     de     -> (root)
   */
   ```

   

   接下来我们的前缀匹配，插入，删除都需要依赖这个函数。

   ```
   // 前缀匹配
   template <typename K, typename T, typename Compare>
   void radix_tree<K, T, Compare>::prefix_match(const K &key, std::vector<iterator> &vec);
   ```

   

   这里的key其实是我们的前缀。此函数内部调用find_node函数找到最接近的节点，然后判断这个最接近的节点的键值能否包含目标前缀，如果不能包含，则没有前缀匹配，否则继续调用`greedy_match`函数将符合的节点的迭代其放入参数vec中。

   ```
   // 找到最匹配的节点，返回迭代器
   template <typename K, typename T, typename Compare>
   typename radix_tree<K, T, Compare>::iterator radix_tree<K, T, Compare>::longest_match(const K &key);
   ```

   

   ```
   // 在 parent 节点之后添加键值对
   template <typename K, typename T, typename Compare>
   radix_tree_node<K, T, Compare>* radix_tree<K, T, Compare>::append(radix_tree_node<K, T, Compare> *parent, const value_type &val)；
   ```

   

   这里分两种情况，一种是parent节点对应的键就是目标键值，那么在parent后加入一个为键位nil的孩子节点，并将键值对保存在这个孩子节点即可。 第二种是parent节点所代表的键只是目标键值的前缀，那么需要再次添加一个键值的非空的孩子节点，然后再次添加一个键值为空的孙子节点来保存键值对。

   ```
   // 在 node 前方插入键值对
   template <typename K, typename T, typename Compare>
   radix_tree_node<K, T, Compare>* radix_tree<K, T, Compare>::prepend(radix_tree_node<K, T, Compare> *node, const value_type &val);
   ```

   

   我们首先需要找到插入键值对和node节点对应键的共同前缀，然后分别处理后面的部分。

   ```
   // 删除一个键值对
   template <typename K, typename T, typename Compare>
   bool radix_tree<K, T, Compare>::erase(const K &key);
   ```

   

   删除键值对的操作就是删除对应的键位nil的叶子节点。但是我们要考虑将一些节点合并来减少基数树的高度。一是如果删去这个叶子节点后他的父节点的孩子节点少于一个，那么可以考虑将这个父节点和剩下子节点（如果有）进行合并。同时，如果处理到最后发现祖父节点也只有一个孩子了（对应的叔叔节点）那么可以将叔叔节点和祖父节点合并。